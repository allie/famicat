#include "cpu.h"
#include "../memory/memory.h"

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

/* --- Addressing mode functions --- */

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
static void IDX() {

}

/* (Indirect), Y */
static void IDY() {

}

/* --- CPU instruction functions --- */

/* Add memory to the accumulator with carry */
/* N Z C V */
static void ADC() {

}

/* AND memory with accumulator */
/* N Z */
static void AND() {

}

/* Shift memory or accumulator left one bit */
/* N Z C */
static void ASL() {

}

/* Branch on carry clear */
/* No flags changed */
static void BCC() {

}

/* Branch on carry set */
/* No flags changed */
static void BCS() {

}

/* Branch on result zero (zero set) */
/* No flags changed */
static void BEQ() {

}

/* Test bits in memory with accumulator */
/* N=M7 Z V=M6 */
static void BIT() {

}

/* Branch on result minus (N set) */
/* No flags changed */
static void BMI() {

}

/* Branch on result not zero (zero cleared) */
/* No flags changed */
static void BNE() {

}

/* Branch on result plus (N cleared) */
/* No flags changed */
static void BPL() {

}

/* Force break */
/* I=1 */
static void BRK() {

}

/* Branch on overflow clear */
/* No flags changed */
static void BVC() {

}

/* Branch on overflow set */
/* No flags changed */
static void BVS() {

}

/* Clear carry flag */
/* C=0 */
static void CLC() {

}

/* Clear decimal mode (NOT USED IN NES MODE) */
/* D=0 */
static void CLD() {
#ifdef NES_MODE

#endif
}

/* Clear interrupt disable flag */
/* I=0 */
static void CLI() {

}

/* Clear overflow flag */
/* V=0 */
static void CLV() {

}

/* Compare memory and accumulator */
/* N Z C */
static void CMP() {

}

/* Compare memory and index X */
/* N Z C */
static void CPX() {

}

/* Compare memory and index Y */
/* N Z C */
static void CPY() {

}

/* Decrement memory by one */
/* N Z */
static void DEC() {

}

/* Decrement index X by one */
/* N Z */
static void DEX() {

}

/* Decrement index Y by one */
/* N Z */
static void DEY() {

}

/* XOR memory with accumulator */
/* N Z */
static void EOR() {

}

/* Increment memory by one */
/* N Z */
static void INC() {

}

/* Increment index X by one */
/* N Z */
static void INX() {

}

/* Increment index Y by one */
/* N Z */
static void INY() {

}

/* Jump to new location */
/* No flags changed */
static void JMP() {

}

/* Jump to new location saving return address */
/* No flags changed */
static void JSR() {

}

/* Load accumulator with memory */
/* N Z */
static void LDA() {

}

/* Load index X with memory */
/* N Z */
static void LDX() {

}

/* Load index Y with memory */
/* N Z */
static void LDY() {

}

/* Shift memory or accumulator right one bit */
/* N=0 Z C */
static void LSR() {

}

/* No operation */
/* No flags changed */
static void NOP() {
	/* Leave empty */
}

/* OR memory with accumulator */
/* N Z */
static void ORA() {

}

/* Push accumulator onto stack */
/* No flags changed */
static void PHA() {

}

/* Push processor status onto stack */
/* No flags changed */
static void PHP() {

}

/* Pull accumulator from stack */
/* No flags changed */
static void PLA() {

}

/* Pull processor status from stack */
/* N Z C I D V = pull from stack */
static void PLP() {

}

/* Rotate memory or accumulator left one bit */
/* N Z C */
static void ROL() {

}

/* Rotate memory or accumulator right one bit */
/* N Z C */
static void ROR() {

}

/* Return from interrupt */
/* N Z C I D V = pull from stack */
static void RTI() {

}

/* Return from subroutine */
/* No flags changed */
static void RTS() {

}

/* Subtract memory from accumulator with borrow */
/* N Z C V */
static void SBC() {

}

/* Set carry flag */
/* C=1 */
static void SEC() {

}

/* Set decimal mode (NOT USED IN NES MODE) */
/* D=1 */
static void SED() {
#ifdef NES_MODE

#endif
}

/* Set interrupt disable flag */
/* I=1 */
static void SEI() {

}

/* Store accumulator in memory */
/* No flags changed */
static void STA() {

}

/* Store index X in memory */
/* No flags changed */
static void STX() {

}

/* Store index Y in memory */
/* No flags changed */
static void STY() {

}

/* Transfer accumulator to index X */
/* N Z */
static void TAX() {

}

/* Transfer accumulator to index Y */
/* N Z */
static void TAY() {

}

/* Transfer stack pointer to index X */
/* N Z */
static void TSX() {

}

/* Transfer index X to accumulator */
/* N Z */
static void TXA() {

}

/* Transfer index X to stack pointer */
/* No flags changed */
static void TXS() {

}

/* Transfer index Y to accumulator */
static void TYA() {

}

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
