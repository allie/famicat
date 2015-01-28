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
