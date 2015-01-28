/* TODO: Get memory value of operand */
/* --- CPU instruction functions --- */

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
