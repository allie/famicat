1/* TODO: Get memory value of operand */
/* --- CPU instruction functions --- */

/* Add memory to the accumulator with carry */
/* N Z C V */
static void ADC() {
    uint8_t result = cpu.A + cpu.operand + get_flag(FLAG_C);
    calculate_zero(result);
    calculate_carry(result > 0xff);
    calculate_sign(result);
    calculate_overflow(~(cpu.A ^ cpu.operand) & (cpu.A ^ result) & 0x80);
    cpu.A = result;
}

/* AND memory with accumulator */
/* N Z */
static void AND() {
    uint8_t result = cpu.A & cpu.operand;
    calculate_sign(result);
    calculate_zero(result);
    cpu.A = result;
}

/* Shift accumulator left one bit */
/* N Z C */
static void ASL_A() {
    uint8_t result = cpu.A << 1;
    calculate_carry(cpu.A & 0x80);
    calculate_sign(result);
    calculate_zero(result);
    cpu.A = result;
}

/* Shift memory left one bit */
/* N Z C */
static void ASL_M() {
    uint8_t result = cpu.operand << 1;
    calculate_carry(cpu.operand & 0x80);
    calculate_sign(result);
    calculate_zero(result);
    cpu.operand = result;
}

/* Branch on carry clear */
/* No flags changed */
static void BCC() {
    if(!get_flag(FLAG_C))
        cpu.PC += cpu.operand; /* No idea if this is actually what that means */
}

/* Branch on carry set */
/* No flags changed */
static void BCS() {
    if(get_flag(FLAG_C))
        cpu.PC += cpu.operand;
}

/* Branch on result zero (zero set) */
/* No flags changed */
static void BEQ() {
    if(get_flag(FLAG_Z))
        cpu.PC += cpu.operand;
}

/* Test bits in memory with accumulator */
/* N=M7 Z V=M6 */
static void BIT() {
    uint8_t result = cpu.A & cpu.operand;
    calculate_sign(cpu.operand);
    calculate_zero(result);
    calculate_overflow(cpu.operand & FLAG_V);
}

/* Branch on result minus (N set) */
/* No flags changed */
static void BMI() {
    if(get_flag(FLAG_N))
        cpu.PC += cpu.operand;
}

/* Branch on result not zero (zero cleared) */
/* No flags changed */
static void BNE() {
    if(!get_flag(FLAG_Z))
        cpu.PC += cpu.operand;
}

/* Branch on result plus (N cleared) */
/* No flags changed */
static void BPL() {
    if(!get_flag(FLAG_N))
        cpu.PC += cpu.operand;
}

/* Force break */
/* I=1 */
static void BRK() {
    set_flag(FLAG_B);
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
    set_flag(FLAG_C);
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
    set_flag(FLAG_I);
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
    calculate_zero(cpu.X);
    calculate_sign(cpu.X);
}

/* Transfer accumulator to index Y */
/* N Z */
static void TAY() {
    cpu.Y = cpu.A;
    calculate_zero(cpu.Y);
    calculate_sign(cpu.Y);
}

/* Transfer stack pointer to index X */
/* N Z */
static void TSX() {
    cpu.X = cpu.SP;
    calculate_zero(cpu.X);
    calculate_sign(cpu.X);
}

/* Transfer index X to accumulator */
/* N Z */
static void TXA() {
    cpu.A = cpu.X;
    calculate_zero(cpu.A);
    calculate_sign(cpu.A);
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
    calculate_zero(cpu.A);
    calculate_sign(cpu.A);
}
