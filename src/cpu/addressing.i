/* Implied */
static void IMP() {
	/* Leave empty */
}

/* Accumulator */
static void ACC() {
	/* Leave empty */
}

/* Immediate */
static void IMM() {
	cpu.operaddr = cpu.PC++;
}

/* Zero-page */
static void ZPG() {
	cpu.operaddr = (WORD)Memory_ReadByte((WORD)cpu.PC++);
}

/* Zero-page, X */
static void ZPX() {

}

/* Zero-page, Y */
static void ZPY() {

}

/* Relative */
static void REL() {
	BYTE num = Memory_ReadByte(cpu.PC++);

}

/* Absolute */
static void ABS() {
	cpu.operaddr = cpu.PC++;
}

/* Absolute, X */
static void ABX() {
	cpu.operaddr = cpu.X + cpu.PC++;
}

/* Absolute, Y */
static void ABY() {
	cpu.operaddr = cpu.Y + cpu.PC++;
}

/* Indirect */
static void IND() {
 	cpu.operaddr = Memory_ReadWord(cpu.PC++);
}

/* (Indirect, X) */
static void IDX() {
	cpu.operaddr = Memory_ReadWord(cpu.X + cpu.PC++);
}

/* (Indirect), Y */
static void IDY() {
	cpu.operaddr = Memory_ReadWord(cpu.PC++);
	cpu.operaddr += cpu.Y;
}
