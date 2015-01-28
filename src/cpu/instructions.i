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

/* DEC memory and CMP result with accumulator (UNOFFICIAL) */
/* ? */
static void DCP() {

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

/* INC memory and SBC result from the accumulator (UNOFFICIAL) */
/* ? */
static void ISB() {

}

/* Jump to new location */
/* No flags changed */
static void JMP() {

}

/* Jump to new location saving return address */
/* No flags changed */
static void JSR() {

}

/* Load accumulator and index X with memory (UNOFFICIAL) */
/* ? */
static void LAX() {

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

/* ROL memory and AND result with accumulator (UNOFFICIAL) */
/* ? */
static void RLA() {
	
}

/* Rotate memory or accumulator left one bit */
/* N Z C */
static void ROL() {

}

/* Rotate memory or accumulator right one bit */
/* N Z C */
static void ROR() {

}

/* ROR memory and ADC result with accumulator (UNOFFICIAL) */
/* ? */
static void RRA() {

}

/* Return from interrupt */
/* N Z C I D V = pull from stack */
static void RTI() {

}

/* Return from subroutine */
/* No flags changed */
static void RTS() {

}

/* AND accumulator with index X and store the result in memory (UNOFFICIAL) */
/* No flags changed */
static void SAX() {

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

/* ASL memory and OR result with accumulator (UNOFFICIAL) */
/* ? */
static void SLO() {

}

/* LSR memory and XOR result with accumulator (UNOFFICIAL) */
/* ? */
static void SRE() {

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
