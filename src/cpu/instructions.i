#define SET_FLAG(f) cpu.S |= f
#define CLEAR_FLAG(f) cpu.S &= ~(f)
#define GET_FLAG(f) (cpu.S & f)

#define CALC_C(c) \
    if ((c)) \
        SET_FLAG(FLAG_C); \
    else \
        CLEAR_FLAG(FLAG_C);

#define CALC_Z(n) \
    if((n) == 0) \
        SET_FLAG(FLAG_Z); \
    else \
        CLEAR_FLAG(FLAG_Z);

#define CALC_I(c) \
    if ((c)) \
        SET_FLAG(FLAG_I); \
    else \
        CLEAR_FLAG(FLAG_I);

#define CALC_B(c) \
    if ((c)) \
        SET_FLAG(FLAG_B); \
    else \
        CLEAR_FLAG(FLAG_B);

#define CALC_V(c) \
    if ((c)) \
        SET_FLAG(FLAG_V); \
    else \
        CLEAR_FLAG(FLAG_V);

#define CALC_N(n) \
    if ((n) & 0x80) \
        SET_FLAG(FLAG_N); \
    else \
        CLEAR_FLAG(FLAG_N);

#define ADD_OFFSET() \
    WORD oldPC = cpu.PC; \
    if(cpu.operand & 0x80) \
        cpu.PC -= (cpu.operand & 0x7f); \
    else \
        cpu.PC += (cpu.operand & 0x7f); \
    if((cpu.PC & 0xff00) != (oldPC & 0xff00)) \
        cpu.cycles += 2; \
    else \
        cpu.cycles++;

#define PAGE_PENALTY() \
    if(((mode == ABX) && (cpu.X > (cpu.operaddr & 0x00ff))) || \
        ((mode == ABY || mode == IDY) && (cpu.Y > (cpu.operaddr & 0x00ff)))) \
            cpu.cycles++;

/* Add memory to the accumulator with carry */
/* N Z C V */
static void ADC() {
    WORD result = (WORD)cpu.A + cpu.operand + GET_FLAG(FLAG_C);
    CALC_Z(result & 0x00FF);
    CALC_C(result & 0xFF00);
    CALC_N(result);
    CALC_V(~(cpu.A ^ cpu.operand) & (cpu.A ^ result) & 0x80);
    cpu.A = (BYTE)(result & 0x00FF);
    PAGE_PENALTY();
}

/* AND memory with accumulator */
/* N Z */
static void AND() {
    BYTE result = cpu.A & cpu.operand;
    CALC_N(result);
    CALC_Z(result);
    cpu.A = result;
    PAGE_PENALTY();
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
    BYTE result = cpu.operand << 1;
    CALC_C(cpu.operand & 0x80);
    CALC_N(result);
    CALC_Z(result);
    Memory_WriteByte(cpu.operaddr, result);
}

/* Branch on carry clear */
/* No flags changed */
static void BCC() {
    if(!GET_FLAG(FLAG_C)){
        ADD_OFFSET();
    }
}

/* Branch on carry set */
/* No flags changed */
static void BCS() {
    if(GET_FLAG(FLAG_C)){
        ADD_OFFSET();
    }
}

/* Branch on result zero (zero set) */
/* No flags changed */
static void BEQ() {
    if(GET_FLAG(FLAG_Z)){
        ADD_OFFSET();
    }
}

/* Test bits in memory with accumulator */
/* N=M7 Z V=M6 */
static void BIT() {
    BYTE result = cpu.A & cpu.operand;
    CALC_N(cpu.operand);
    CALC_Z(result);
    CALC_V(cpu.operand & FLAG_V);
}

/* Branch on result minus (N set) */
/* No flags changed */
static void BMI() {
    if(GET_FLAG(FLAG_N)){
        ADD_OFFSET();
    }
}

/* Branch on result not zero (zero cleared) */
/* No flags changed */
static void BNE() {
    if(!GET_FLAG(FLAG_Z)){
        ADD_OFFSET();
    }
}

/* Branch on result plus (N cleared) */
/* No flags changed */
static void BPL() {
    if(!GET_FLAG(FLAG_N)){
        ADD_OFFSET();
    }
}

/* Force break */
/* I=1 */
static void BRK() {
    SET_FLAG(FLAG_B);
    cpu.PC++;
    pushw(cpu.PC);
    pushb(cpu.S);
    SET_FLAG(FLAG_I);
    cpu.PC = Memory_ReadWord(0xFFFE);
}

/* Branch on overflow clear */
/* No flags changed */
static void BVC() {
    if(!GET_FLAG(FLAG_V)){
        ADD_OFFSET();
    }
}

/* Branch on overflow set */
/* No flags changed */
static void BVS() {
    if(GET_FLAG(FLAG_V)){
        ADD_OFFSET();
    }
}

/* Clear carry flag */
/* C=0 */
static void CLC() {
    CLEAR_FLAG(FLAG_C);
}

/* Clear decimal mode (NOT USED IN NES MODE) */
/* D=0 */
static void CLD() {
    CLEAR_FLAG(FLAG_D);
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
    BYTE result = cpu.A - cpu.operand;
    CALC_C(cpu.A >= cpu.operand);
    CALC_Z(result);
    CALC_N(result);
    PAGE_PENALTY();
}

/* Compare memory and index X */
/* N Z C */
static void CPX() {
    BYTE result = cpu.X - cpu.operand;
    CALC_C(cpu.X >= cpu.operand);
    CALC_Z(result);
    CALC_N(result);
}

/* Compare memory and index Y */
/* N Z C */
static void CPY() {
    BYTE result = cpu.Y - cpu.operand;
    CALC_C(cpu.Y >= cpu.operand);
    CALC_Z(result);
    CALC_N(result);
}

/* Decrement memory by one */
/* N Z */
static void DEC() {
    BYTE result = cpu.operand - 1;
    CALC_Z(result);
    CALC_N(result);
    Memory_WriteByte(cpu.operaddr, result);
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
    BYTE result = cpu.A ^ cpu.operand;
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
    PAGE_PENALTY();
}

/* Increment memory by one */
/* N Z */
static void INC() {
    BYTE result = cpu.operand + 1;
    CALC_Z(result);
    CALC_N(result);
    Memory_WriteByte(cpu.operaddr, result);
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

/* Jump to new location */
/* No flags changed */
static void JMP() {
    cpu.PC = cpu.operaddr;
}

/* Jump to new location saving return address */
/* No flags changed */
static void JSR() {
    pushw(cpu.PC - 1);
    cpu.PC = cpu.operaddr;
}

/* Load accumulator with memory */
/* N Z */
static void LDA() {
    cpu.A = cpu.operand;
    CALC_Z(cpu.A);
    CALC_N(cpu.A);
    PAGE_PENALTY();
}

/* Load index X with memory */
/* N Z */
static void LDX() {
    cpu.X = cpu.operand;
    CALC_Z(cpu.X);
    CALC_N(cpu.X);
    PAGE_PENALTY();
}

/* Load index Y with memory */
/* N Z */
static void LDY() {
    cpu.Y = cpu.operand;
    CALC_Z(cpu.Y);
    CALC_N(cpu.Y);
    PAGE_PENALTY();
}

/* Shift accumulator right one bit */
/* N=0 Z C */
static void LSRA() {
    BYTE result = cpu.A >> 1;
    CALC_C(cpu.A & 1);
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Shift memory right one bit */
/* N=0 Z C */
static void LSR() {
    BYTE result = cpu.operand >> 1;
    CALC_C(cpu.operand & 1);
    CALC_Z(result);
    CALC_N(result);
    Memory_WriteByte(cpu.operaddr, result);
}

/* No operation */
/* No flags changed */
static void NOP() {
    /* Leave empty */
}

/* OR memory with accumulator */
/* N Z */
static void ORA() {
    BYTE result = cpu.A | cpu.operand;
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
    PAGE_PENALTY();
}

/* Push accumulator onto stack */
/* No flags changed */
static void PHA() {
    pushb(cpu.A);
}

/* Push processor status onto stack */
/* No flags changed */
static void PHP() {
    pushb(cpu.S | FLAG_B);
}

/* Pull accumulator from stack */
/* N Z */
static void PLA() {
    cpu.A = pullb();
    CALC_Z(cpu.A);
    CALC_N(cpu.A);
}

/* Pull processor status from stack */
/* N Z C I D V = pull from stack */
static void PLP() {
    cpu.S = (pullb() & ~(FLAG_B)) | 0x20;
}

/* Rotate accumulator left one bit */
/* N Z C */
static void ROLA() {
    BYTE result = cpu.A << 1;
    result |= GET_FLAG(FLAG_C);
    CALC_C(cpu.A & 0x80);
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Rotate memory left one bit */
/* N Z C */
static void ROL() {
    BYTE result = cpu.operand << 1;
    result |= GET_FLAG(FLAG_C);
    CALC_C(cpu.operand & 0x80);
    CALC_Z(result);
    CALC_N(result);
    Memory_WriteByte(cpu.operaddr, result);
}

/* Rotate accumulator right one bit */
/* N Z C */
static void RORA() {
    BYTE result = cpu.A >> 1;
    if(GET_FLAG(FLAG_C))
        result |= 0x80;
    CALC_C(cpu.A & 1);
    CALC_Z(result);
    CALC_N(result);
    cpu.A = result;
}

/* Rotate memory right one bit */
/* N Z C */
static void ROR() {
    BYTE result = cpu.operand >> 1;
    if(GET_FLAG(FLAG_C))
        result |= 0x80;
    CALC_C(cpu.operand & 1);
    CALC_Z(result);
    CALC_N(result);
    Memory_WriteByte(cpu.operaddr, result);
}

/* Return from interrupt */
/* N Z C I D V = pull from stack */
static void RTI() {
    cpu.S = (pullb() & ~(FLAG_B)) | 0x20;
    cpu.PC = pullw();
}

/* Return from subroutine */
/* No flags changed */
static void RTS() {
    cpu.PC = pullw();
    cpu.PC++;
}

/* Subtract memory from accumulator with borrow */
/* N Z C V */
static void SBC() {
    WORD result = (WORD)cpu.A - cpu.operand - (1-GET_FLAG(FLAG_C));
    CALC_Z(result & 0x00FF);
    CALC_N(result);
    CALC_C(!(result & 0xFF00));
    CALC_V((cpu.A ^ cpu.operand) & (cpu.A ^ result) & 0x80);
    cpu.A = (BYTE)(result & 0x00FF);
    PAGE_PENALTY();
}

/* Set carry flag */
/* C=1 */
static void SEC() {
    SET_FLAG(FLAG_C);
}

/* Set decimal mode (NOT USED IN NES MODE) */
/* D=1 */
static void SED() {
    SET_FLAG(FLAG_D);
}

/* Set interrupt disable flag */
/* I=1 */
static void SEI() {
    SET_FLAG(FLAG_I);
}

/* Store accumulator in memory */
/* No flags changed */
static void STA() {
    Memory_WriteByte(cpu.operaddr, cpu.A);
}

/* Store index X in memory */
/* No flags changed */
static void STX() {
    Memory_WriteByte(cpu.operaddr, cpu.X);
}

/* Store index Y in memory */
/* No flags changed */
static void STY() {
    Memory_WriteByte(cpu.operaddr, cpu.Y);
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

#ifdef UNOFFICIAL_MODE

/* DEC memory and CMP result with accumulator (UNOFFICIAL) */
/* ? */
static void DCP() {
    /* TODO */
}

/* INC memory and SBC result from the accumulator (UNOFFICIAL) */
/* ? */
static void ISB() {
    /* TODO */
}

/* Load accumulator and index X with memory (UNOFFICIAL) */
/* ? */
static void LAX() {
    /* TODO */
}

/* ROL memory and AND result with accumulator (UNOFFICIAL) */
/* ? */
static void RLA() {
    /* TODO */
}

/* ROR memory and ADC result with accumulator (UNOFFICIAL) */
/* ? */
static void RRA() {
    /* TODO */
}

/* AND accumulator with index X and store the result in memory (UNOFFICIAL) */
/* No flags changed */
static void SAX() {
    /* TODO */
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

#else

#define DCP NOP
#define ISB NOP
#define LAX NOP
#define RLA NOP
#define RRA NOP
#define SAX NOP
#define SLO NOP
#define SRE NOP

#endif
