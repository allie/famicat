#include "cpu.h"
#include "../memory/memory.h"

/* Global CPU */
CPU cpu;

/* --- Stack operations --- */
static void pushb(BYTE val) {
	Memory_WriteByte(MAP_CPU, (STACK_ADDR | cpu.SP--), val);
}

static void pushw(WORD val) {
	pushb((val >> 8) & 0xFF);
	pushb(val & 0xFF);
}

static BYTE pullb() {
	return Memory_ReadByte(MAP_CPU, (STACK_ADDR | ++cpu.SP));
}

static WORD pullw() {
	return (WORD)pullb() | ((WORD)pullb() << 8);
}

static void (*mode)();

/* --- Addressing mode functions --- */
#include "addressing.i"

/* --- CPU instruction functions --- */
#include "instructions.i"

/* --- Disassembler --- */
#ifdef DEBUG_MODE
#include "disassembler.i"
#endif

/* Addressing mode function pointer table for fetching operands; indexed by opcode */
static void (*addr[256])() = {
/*    | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | A  | B  | C  | D  | E  | F  |*/
/* 0 */ IMP, IDX, IMP, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, ABS, ABS, ABS, ABS,
/* 1 */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 2 */ ABS, IDX, IMP, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, ABS, ABS, ABS, ABS,
/* 3 */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 4 */ IMP, IDX, IMP, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, ABS, ABS, ABS, ABS,
/* 5 */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 6 */ IMP, IDX, IMP, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, IND, ABS, ABS, ABS,
/* 7 */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 8 */ IMM, IDX, IMM, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* 9 */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPY, ZPY, IMP, ABY, IMP, ABY, ABX, ABX, ABY, ABY,
/* A */ IMM, IDX, IMM, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* B */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPY, ZPY, IMP, ABY, IMP, ABY, ABX, ABX, ABY, ABY,
/* C */ IMM, IDX, IMM, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* D */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* E */ IMM, IDX, IMM, IDX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* F */ REL, IDY, IMP, IDY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX
};

/* CPU instruction function pointer table; indexed by opcode */
static void (*instr[256])() = {
/*    | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | A   | B  | C  | D  | E  | F  |*/
/* 0 */ BRK, ORA, NOP, SLO, NOP, ORA, ASL, SLO, PHP, ORA, ASLA, NOP, NOP, ORA, ASL, SLO,
/* 1 */ BPL, ORA, NOP, SLO, NOP, ORA, ASL, SLO, CLC, ORA, NOP,  SLO, NOP, ORA, ASL, SLO,
/* 2 */ JSR, AND, NOP, RLA, BIT, AND, ROL, RLA, PLP, AND, ROLA, NOP, BIT, AND, ROL, RLA,
/* 3 */ BMI, AND, NOP, RLA, NOP, AND, ROL, RLA, SEC, AND, NOP,  RLA, NOP, AND, ROL, RLA,
/* 4 */ RTI, EOR, NOP, SRE, NOP, EOR, LSR, SRE, PHA, EOR, LSRA, NOP, JMP, EOR, LSR, SRE,
/* 5 */ BVC, EOR, NOP, SRE, NOP, EOR, LSR, SRE, CLI, EOR, NOP,  SRE, NOP, EOR, LSR, SRE,
/* 6 */ RTS, ADC, NOP, RRA, NOP, ADC, ROR, RRA, PLA, ADC, RORA, NOP, JMP, ADC, ROR, RRA,
/* 7 */ BVS, ADC, NOP, RRA, NOP, ADC, ROR, RRA, SEI, ADC, NOP,  RRA, NOP, ADC, ROR, RRA,
/* 8 */ NOP, STA, NOP, SAX, STY, STA, STX, SAX, DEY, NOP, TXA,  NOP, STY, STA, STX, SAX,
/* 9 */ BCC, STA, NOP, NOP, STY, STA, STX, SAX, TYA, STA, TXS,  NOP, NOP, STA, NOP, NOP,
/* A */ LDY, LDA, LDX, LAX, LDY, LDA, LDX, LAX, TAY, LDA, TAX,  NOP, LDY, LDA, LDX, LAX,
/* B */ BCS, LDA, NOP, LAX, LDY, LDA, LDX, LAX, CLV, LDA, TSX,  LAX, LDY, LDA, LDX, LAX,
/* C */ CPY, CMP, NOP, DCP, CPY, CMP, DEC, DCP, INY, CMP, DEX,  NOP, CPY, CMP, DEC, DCP,
/* D */ BNE, CMP, NOP, DCP, NOP, CMP, DEC, DCP, CLD, CMP, NOP,  DCP, NOP, CMP, DEC, DCP,
/* E */ CPX, SBC, NOP, ISB, CPX, SBC, INC, ISB, INX, SBC, NOP,  SBC, CPX, SBC, INC, ISB,
/* F */ BEQ, SBC, NOP, ISB, NOP, SBC, INC, ISB, SED, SBC, NOP,  ISB, NOP, SBC, INC, ISB
};

/* Clock cycle table; indexed by opcode */
static const DWORD cycles[256] = {
/*    | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | A  | B  | C  | D  | E  | F  |*/
/* 0 */ 7,   6,   2,   8,   3,   3,   5,   5,   3,   2,   2,   2,   4,   4,   6,   6,
/* 1 */ 2,   5,   2,   8,   4,   4,   6,   6,   2,   4,   2,   7,   4,   4,   7,   7,
/* 2 */ 6,   6,   2,   8,   3,   3,   5,   5,   4,   2,   2,   2,   4,   4,   6,   6,
/* 3 */ 2,   5,   2,   8,   4,   4,   6,   6,   2,   4,   2,   7,   4,   4,   7,   7,
/* 4 */ 6,   6,   2,   8,   3,   3,   5,   5,   3,   2,   2,   2,   3,   4,   6,   6,
/* 5 */ 2,   5,   2,   8,   4,   4,   6,   6,   2,   4,   2,   7,   4,   4,   7,   7,
/* 6 */ 6,   6,   2,   8,   3,   3,   5,   5,   4,   2,   2,   2,   5,   4,   6,   6,
/* 7 */ 2,   5,   2,   8,   4,   4,   6,   6,   2,   4,   2,   7,   4,   4,   7,   7,
/* 8 */ 2,   6,   2,   6,   3,   3,   3,   3,   2,   2,   2,   2,   4,   4,   4,   4,
/* 9 */ 2,   6,   2,   6,   4,   4,   4,   4,   2,   5,   2,   5,   5,   5,   5,   5,
/* A */ 2,   6,   2,   6,   3,   3,   3,   3,   2,   2,   2,   2,   4,   4,   4,   4,
/* B */ 2,   5,   2,   5,   4,   4,   4,   4,   2,   4,   2,   4,   4,   4,   4,   4,
/* C */ 2,   6,   2,   8,   3,   3,   5,   5,   2,   2,   2,   2,   4,   4,   6,   6,
/* D */ 2,   5,   2,   8,   4,   4,   6,   6,   2,   4,   2,   7,   4,   4,   7,   7,
/* E */ 2,   6,   2,   8,   3,   3,   5,   5,   2,   2,   2,   2,   4,   4,   6,   6,
/* F */ 2,   5,   2,   8,   4,   4,   6,   6,   2,   4,   2,   7,   4,   4,   7,   7
};

/* Interrupt functions */
/* TODO: Interrupt hijacking */
void CPU_Interrupt_IRQ() {
	CLEAR_FLAG(FLAG_B);
	pushw(cpu.PC);
	pushb(cpu.S);
	SET_FLAG(FLAG_I);
	cpu.PC = Memory_ReadWord(MAP_CPU, 0xFFFE);
}

void CPU_Interrupt_NMI() {
	CLEAR_FLAG(FLAG_B);
	pushw(cpu.PC);
	pushb(cpu.S);
	SET_FLAG(FLAG_I);
	cpu.PC = Memory_ReadWord(MAP_CPU, 0xFFFA);
}

void CPU_Interrupt_RESET() {
	cpu.PC = Memory_ReadWord(MAP_CPU, 0xFFFC);
	cpu.SP = 0xFD;
	cpu.S |= 0x24;
}

/* Re-initialize all CPU registers and variables */
void CPU_Reset() {
	cpu.PC = Memory_ReadWord(MAP_CPU, 0xFFFC);
	cpu.A = 0;
	cpu.X = 0;
	cpu.Y = 0;
	cpu.SP = 0xFD;
	cpu.S = 0x24;
	cpu.opcode = 0;
	cpu.operaddr = 0;
	cpu.indoperaddr = 0;
	cpu.indoperand = 0;
	cpu.operand = 0;
	cpu.cycles = 0;
	cpu.suspended = 0;
}

void CPU_Suspend(DWORD duration) {
	cpu.suspended = duration;
}

/* Execute one CPU instruction */
DWORD CPU_Step() {
	if (cpu.suspended) {
		cpu.suspended--;
		return 1;
	}

	DWORD lastcycles = cpu.cycles;

	/* Check for interrupts */
	switch(cpu.interrupt){
		case IRQ:
			if(!(GET_FLAG(FLAG_I))){
				CPU_Interrupt_IRQ();
				cpu.interrupt = NONE;
			}
			break;
		case NMI:
			CPU_Interrupt_NMI();
			cpu.interrupt = NONE;
			break;
		case RESET:
			CPU_Interrupt_RESET();
			cpu.interrupt = NONE;
			break;
	}

	/* Fetch opcode */
	cpu.opcode = Memory_ReadByte(MAP_CPU, cpu.PC++);

#ifdef DEBUG_MODE
	disassemble1();
#endif

	/* Fetch operand address */
	mode = addr[cpu.opcode];
	(*mode)();

	/* Cache operand value */
	if (mode != ACC && mode != IMP)
		cpu.operand = Memory_ReadByte(MAP_CPU, cpu.operaddr);
	else
		cpu.operand = 0;

#ifdef DEBUG_MODE
	disassemble2();
#endif

	/* Execute CPU instruction */
	(*instr[cpu.opcode])();

	/* Add cycles to the total cycle count */
	cpu.cycles += cycles[cpu.opcode];

	return cpu.cycles - lastcycles;
}
