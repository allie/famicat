#include "cpu.h"
#include "memory.h"

/* Global CPU */
CPU cpu;

/* Re-initialize all CPU registers and variables */
void CPU_Reset() {

}

/* Execute one CPU instruction */
void CPU_Step() {
	/* Fetch opcode */
	cpu.opcode = Memory_ReadByte(cpu.PC++);

	/* Some stuff */

	/* Fetch operand */
	(*addr[cpu.opcode])();

	/* Execute CPU instruction */
	(*instr[cpu.opcode])();

	/* Add cycles to the total cycle count */
	cpu.cycles += cycles[cpu.opcode];
}

/* Addressing mode functions */
/* Immediate */
static void IMM() {

}

/* Zero-page */
static void ZPG() {

}

/* Zero-page, X */
static void ZPX() {

}

/* Zero-page, Y */
static void ZPY() {

}

/* Relative */
static void REL() {

}

/* Absolute */
static void ABS() {

}

/* Absolute, X */
static void ABX() {

}

/* Absolute, Y */
static void ABY() {

}

/* Indirect */
static void IND() {

}

/* (Indirect, X) */
static void INX() {

}

/* (Indirect), Y */
static void INY() {

}

/* Addressing mode function pointer table for fetching operands; indexed by opcode */
static void (*addr[256])() = {
/*    | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | A  | B  | C  | D  | E  | F  |*/
/* 0 */ IMP, INX, IMP, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, ABS, ABS, ABS, ABS,
/* 1 */ REL, INY, IMP, INY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 2 */ ABS, INX, IMP, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, ABS, ABS, ABS, ABS,
/* 3 */ REL, INY, IMP, INY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 4 */ IMP, INX, IMP, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, ABS, ABS, ABS, ABS,
/* 5 */ REL, INY, IMP, INY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 6 */ IMP, INX, IMP, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, ACC, IMM, IND, ABS, ABS, ABS,
/* 7 */ REL, INY, IMP, INY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* 8 */ IMM, INX, IMM, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* 9 */ REL, INY, IMP, INY, ZPX, ZPX, ZPY, ZPY, IMP, ABY, IMP, ABY, ABX, ABX, ABY, ABY,
/* A */ IMM, INX, IMM, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* B */ REL, INY, IMP, INY, ZPX, ZPX, ZPY, ZPY, IMP, ABY, IMP, ABY, ABX, ABX, ABY, ABY,
/* C */ IMM, INX, IMM, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* D */ REL, INY, IMP, INY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX,
/* E */ IMM, INX, IMM, INX, ZPG, ZPG, ZPG, ZPG, IMP, IMM, IMP, IMM, ABS, ABS, ABS, ABS,
/* F */ REL, INY, IMP, INY, ZPX, ZPX, ZPX, ZPX, IMP, ABY, IMP, ABY, ABX, ABX, ABX, ABX
};

/* CPU instruction function pointer table; indexed by opcode */
static void (*instr[256])() = {
/*    | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | A  | B  | C  | D  | E  | F  |*/
/* 0 */ BRK, ORA, NOP, SLO, NOP, ORA, ASL, SLO, PHP, ORA, ASL, NOP, NOP, ORA, ASL, SLO,
/* 1 */ BPL, ORA, NOP, SLO, NOP, ORA, ASL, SLO, CLC, ORA, NOP, SLO, NOP, ORA, ASL, SLO,
/* 2 */ JSR, AND, NOP, RLA, BIT, AND, ROL, RLA, PLP, AND, ROL, NOP, BIT, AND, ROL, RLA,
/* 3 */ BMI, AND, NOP, RLA, NOP, AND, ROL, RLA, SEC, AND, NOP, RLA, NOP, AND, ROL, RLA,
/* 4 */ RTI, EOR, NOP, SRE, NOP, EOR, LSR, SRE, PHA, EOR, LSR, NOP, JMP, EOR, LSR, SRE,
/* 5 */ BVC, EOR, NOP, SRE, NOP, EOR, LSR, SRE, CLI, EOR, NOP, SRE, NOP, EOR, LSR, SRE,
/* 6 */ RTS, ADC, NOP, RRA, NOP, ADC, ROR, RRA, PLA, ADC, ROR, NOP, JMP, ADC, ROR, RRA,
/* 7 */ BVS, ADC, NOP, RRA, NOP, ADC, ROR, RRA, SEI, ADC, NOP, RRA, NOP, ADC, ROR, RRA,
/* 8 */ NOP, STA, NOP, SAX, STY, STA, STX, SAX, DEY, NOP, TXA, NOP, STY, STA, STX, SAX,
/* 9 */ BCC, STA, NOP, NOP, STY, STA, STX, SAX, TYA, STA, TXS, NOP, NOP, STA, NOP, NOP,
/* A */ LDY, LDA, LDX, LAX, LDY, LDA, LDX, LAX, TAY, LDA, TAX, NOP, LDY, LDA, LDX, LAX,
/* B */ BCS, LDA, NOP, LAX, LDY, LDA, LDX, LAX, CLV, LDA, TSX, LAX, LDY, LDA, LDX, LAX,
/* C */ CPY, CMP, NOP, DCP, CPY, CMP, DEC, DCP, INY, CMP, DEX, NOP, CPY, CMP, DEC, DCP,
/* D */ BNE, CMP, NOP, DCP, NOP, CMP, DEC, DCP, CLD, CMP, NOP, DCP, NOP, CMP, DEC, DCP,
/* E */ CPX, SBC, NOP, ISB, CPX, SBC, INC, ISB, INX, SBC, NOP, SBC, CPX, SBC, INC, ISB,
/* F */ BEQ, SBC, NOP, ISB, NOP, SBC, INC, ISB, SED, SBC, NOP, ISB, NOP, SBC, INC, ISB
};

/* Clock cycle table; indexed by opcode */
static const uint32_t cycles[256] = {
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
