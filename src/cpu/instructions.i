/* TODO: Get memory value of operand */

/* Add memory to the accumulator with carry */
/* N Z C V */
static void ADC() {
    BYTE result = cpu.A + cpu.operaddr + GET_FLAG(FLAG_C);
    CALC_Z(result);
    CALC_C(result > 0xff);
    CALC_N(result);
    CALC_V(~(cpu.A ^ cpu.operaddr) & (cpu.A ^ result) & 0x80);
    cpu.A = result;
}

/* AND memory with accumulator */
/* N Z */
static void AND() {
    BYTE result = cpu.A & cpu.operaddr;
    CALC_N(result);
    CALC_Z(result);
    cpu.A = result;
}

/* Shift accumulator left one bit */
/* N Z C */
static void ASLA() {
    BYTE result = cpu.A << 1;
    CALC_C(cpu.A & 0x80);
    CALC_N(result);
    CALC_Z(result);
    cpu.A = result;
}

/* Shift memory left one bit */
/* N Z C */
static void ASL() {
    BYTE result = cpu.operaddr << 1;
    CALC_C(cpu.operaddr & 0x80);
    CALC_N(result);
    CALC_Z(result);
    cpu.operaddr = result;
}

/* Branch on carry clear */
/* No flags changed */
static void BCC() {
    if(!GET_FLAG(FLAG_C))
        cpu.PC += cpu.operaddr; /* No idea if this is actually what that means */
}

/* Branch on carry set */
/* No flags changed */
static void BCS() {
    if(GET_FLAG(FLAG_C))
        cpu.PC += cpu.operaddr;
}

/* Branch on result zero (zero set) */
/* No flags changed */
static void BEQ() {
    if(GET_FLAG(FLAG_Z))
        cpu.PC += cpu.operaddr;
}

/* Test bits in memory with accumulator */
/* N=M7 Z V=M6 */
static void BIT() {
    BYTE result = cpu.A & cpu.operaddr;
    CALC_N(cpu.operaddr);
    CALC_Z(result);
    CALC_V(cpu.operaddr & FLAG_V);
}

/* Branch on result minus (N set) */
/* No flags changed */
static void BMI() {
    if(GET_FLAG(FLAG_N))
        cpu.PC += cpu.operaddr;
}

/* Branch on result not zero (zero cleared) */
/* No flags changed */
static void BNE() {
    if(!GET_FLAG(FLAG_Z))
        cpu.PC += cpu.operaddr;
}

/* Branch on result plus (N cleared) */
/* No flags changed */
static void BPL() {
    if(!GET_FLAG(FLAG_N))
        cpu.PC += cpu.operaddr;
}

/* Force break */
/* I=1 */
static void BRK() {
    SET_FLAG(FLAG_B);
    /* TODO */
}

/* Branch on overflow clear */
/* No flags changed */
static void BVC() {
    if(!GET_FLAG(FLAG_V))
        cpu.PC += cpu.operaddr;
}

/* Branch on overflow set */
/* No flags changed */
static void BVS() {
    if(GET_FLAG(FLAG_V))
        cpu.PC += cpu.operaddr;
}

/* Clear carry flag */
/* C=0 */
static void CLC() {
    CLEAR_FLAG(FLAG_C);
}

/* Clear decimal mode (NOT USED IN NES MODE) */
/* D=0 */
static void CLD() {
    #ifdef NES_MODE
        /* TODO */
    #endif
}

/* Clear interrupt disable flag */
/* I=0 */
static void CLI() {
    CLEAR_FLAG(FLAG_I);
}

/* Clear overflow flag */
/* V=0 */
static void CLV() {
    CLEAR_FLAG(FLAG_V);
}

/* Compare memory and accumulator */
/* N Z C */
static void CMP() {
    BYTE result = cpu.A - cpu.operaddr;
    CALC_C(cpu.A >= cpu.operaddr);
    CALC_Z(result);
    CALC_N(result);
}

/* Compare memory and index X */
/* N Z C */
static void CPX() {
    BYTE result = cpu.X - cpu.operaddr;
    CALC_C(cpu.X >= cpu.operaddr);
    CALC_Z(result);
    CALC_N(result);
}

/* Compare memory and index Y */
/* N Z C */
static void CPY() {
    BYTE result = cpu.Y - cpu.operaddr;
    CALC_C(cpu.Y >= cpu.operaddr);
    CALC_Z(result);
    CALC_N(result);
}

/* DEC memory and CMP result with accumulator (UNOFFICIAL) */
/* ? */
static void DCP() {
    /* TODO */
}

/* Decrement memory by one */
/* N Z */
static void DEC() {
    BYTE result = cpu.operaddr - 1;
    CALC_Z(result);
    CALC_N(result);
    cpu.operaddr = result;
}

/* Decrement index X by one */
/* N Z */
static void DEX() {
    BYTE result = cpu.X - 1;
    CALC_Z(result);
    CALC_N(result);
    cpu.X = result;
}

/* Decrement index Y by one */
/* N Z */
static void DEY() {
    BYTE result = cpu.Y - 1;
    CALC_Z(result);
    CALC_N(result);
    cpu.Y = result;
}

/* XOR memory with accumulator */
/* N Z */
static void EOR() {
    BYTE result = cpu.A ^ cpu.operaddr;
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Increment memory by one */
/* N Z */
static void INC() {
    BYTE result = cpu.operaddr + 1;
    CALC_Z(result);
    CALC_N(result);
    cpu.operaddr = result;
}

/* Increment index X by one */
/* N Z */
static void INX() {
    BYTE result = cpu.X + 1;
    CALC_Z(result);
    CALC_N(result);
    cpu.X = result;
}

/* Increment index Y by one */
/* N Z */
static void INY() {
    BYTE result = cpu.Y + 1;
    CALC_Z(result);
    CALC_N(result);
    cpu.Y = result;
}

/* INC memory and SBC result from the accumulator (UNOFFICIAL) */
/* ? */
static void ISB() {
    /* TODO */
}

/* Jump to new location */
/* No flags changed */
static void JMP() {
    cpu.PC = cpu.operaddr;
}

/* Jump to new location saving return address */
/* No flags changed */
static void JSR() {
    /* TODO */
}

/* Load accumulator and index X with memory (UNOFFICIAL) */
/* ? */
static void LAX() {
    /* TODO */
}

/* Load accumulator with memory */
/* N Z */
static void LDA() {
    cpu.A = cpu.operaddr;
    CALC_Z(cpu.A);
    CALC_N(cpu.A);
}

/* Load index X with memory */
/* N Z */
static void LDX() {
    cpu.X = cpu.operaddr;
    CALC_Z(cpu.X);
    CALC_N(cpu.X);
}

/* Load index Y with memory */
/* N Z */
static void LDY() {
    cpu.Y = cpu.operaddr;
    CALC_Z(cpu.Y);
    CALC_N(cpu.Y);
}

/* Shift accumulator right one bit */
/* N=0 Z C */
static void LSR_A() {
    BYTE result = cpu.A >> 1;
    CALC_C(cpu.A & 1);
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Shift memory right one bit */
/* N=0 Z C */
static void LSR_M() {
    BYTE result = cpu.operaddr >> 1;
    CALC_C(cpu.operaddr & 1);
    CALC_Z(result);
    CALC_N(result);
    cpu.operaddr = result;
}

/* No operation */
/* No flags changed */
static void NOP() {
    /* Leave empty */
}

/* OR memory with accumulator */
/* N Z */
static void ORA() {
    BYTE result = cpu.A | cpu.operaddr;
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Push accumulator onto stack */
/* No flags changed */
static void PHA() {
    /* TODO */
}

/* Push processor status onto stack */
/* No flags changed */
static void PHP() {
    /* TODO */
}

/* Pull accumulator from stack */
/* No flags changed */
static void PLA() {
    /* TODO */
}

/* Pull processor status from stack */
/* N Z C I D V = pull from stack */
static void PLP() {
    /* TODO */
}

/* ROL memory and AND result with accumulator (UNOFFICIAL) */
/* ? */
static void RLA() {
    /* TODO */
}

/* Rotate accumulator left one bit */
/* N Z C */
static void ROL_A() {
    BYTE result = cpu.A << 1;
    result &= GET_FLAG(FLAG_C);
    CALC_C(cpu.A & 0x80);
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Rotate memory left one bit */
/* N Z C */
static void ROL_M() {
    BYTE result = cpu.operaddr << 1;
    result &= GET_FLAG(FLAG_C);
    CALC_C(cpu.operaddr & 0x80);
    CALC_Z(result);
    CALC_N(result);
    cpu.operaddr = result;
}

/* Rotate accumulator right one bit */
/* N Z C */
static void ROR_A() {
    BYTE result = cpu.A >> 1;
    result &= (GET_FLAG(FLAG_C) & 0x80);
    CALC_C(cpu.A & 1);
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Rotate memory right one bit */
/* N Z C */
static void ROR_M() {
    BYTE result = cpu.operaddr >> 1;
    result &= (GET_FLAG(FLAG_C) & 0x80);
    CALC_C(cpu.operaddr & 1);
    CALC_Z(result);
    CALC_N(result);
    cpu.operaddr = result;
}

/* ROR memory and ADC result with accumulator (UNOFFICIAL) */
/* ? */
static void RRA() {
    /* TODO */
}

/* Return from interrupt */
/* N Z C I D V = pull from stack */
static void RTI() {
    /* TODO */
}

/* Return from subroutine */
/* No flags changed */
static void RTS() {
    /* TODO */
}

/* AND accumulator with index X and store the result in memory (UNOFFICIAL) */
/* No flags changed */
static void SAX() {
    /* TODO */
}

/* Subtract memory from accumulator with borrow */
/* N Z C V */
static void SBC() {
    /* TODO */
}

/* Set carry flag */
/* C=1 */
static void SEC() {
    SET_FLAG(FLAG_C);
}

/* Set decimal mode (NOT USED IN NES MODE) */
/* D=1 */
static void SED() {
#ifdef NES_MODE
    /* TODO */
#endif
}

/* Set interrupt disable flag */
/* I=1 */
static void SEI() {
    SET_FLAG(FLAG_I);
}

/* ASL memory and OR result with accumulator (UNOFFICIAL) */
/* ? */
static void SLO() {
    /* TODO */
}

/* LSR memory and XOR result with accumulator (UNOFFICIAL) */
/* ? */
static void SRE() {
    /* TODO */
}

/* Store accumulator in memory */
/* No flags changed */
static void STA() {
    /* TODO */
}

/* Store index X in memory */
/* No flags changed */
static void STX() {
    /* TODO */
}

/* Store index Y in memory */
/* No flags changed */
static void STY() {
    /* TODO */
}

/* Transfer accumulator to index X */
/* N Z */
static void TAX() {
    cpu.X = cpu.A;
    CALC_Z(cpu.X);
    CALC_N(cpu.X);
}

/* Transfer accumulator to index Y */
/* N Z */
static void TAY() {
    cpu.Y = cpu.A;
    CALC_Z(cpu.Y);
    CALC_N(cpu.Y);
}

/* Transfer stack pointer to index X */
/* N Z */
static void TSX() {
    cpu.X = cpu.SP;
    CALC_Z(cpu.X);
    CALC_N(cpu.X);
}

/* Transfer index X to accumulator */
/* N Z */
static void TXA() {
    cpu.A = cpu.X;
    CALC_Z(cpu.A);
    CALC_N(cpu.A);
}

/* Transfer index X to stack pointer */
/* No flags changed */
static void TXS() {
    cpu.SP = cpu.X;
}

/* Transfer index Y to accumulator */
/* N Z */
static void TYA() {
    cpu.A = cpu.Y;
    CALC_Z(cpu.A);
    CALC_N(cpu.A);
}
