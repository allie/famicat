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
	cpu.indoperand = Memory_ReadByte(cpu.PC++);
	cpu.operaddr = (WORD)cpu.indoperand;
}

/* Zero-page, X */
static void ZPX() {
	cpu.indoperand = Memory_ReadByte(cpu.PC++);
	cpu.operaddr = (WORD)cpu.indoperand + cpu.X;
}

/* Zero-page, Y */
static void ZPY() {
	cpu.indoperand = Memory_ReadByte(cpu.PC++);
	cpu.operaddr = (WORD)cpu.indoperand + cpu.Y;
}

/* Relative */
static void REL() {
	cpu.operaddr = cpu.PC++;
}

/* Absolute */
static void ABS() {
	cpu.operaddr = Memory_ReadWord(cpu.PC);
	cpu.PC += 2;
}

/* Absolute, X */
static void ABX() {
	cpu.indoperaddr = Memory_ReadWord(cpu.PC);
	cpu.operaddr = cpu.indoperaddr + cpu.X;
	cpu.PC += 2;
}

/* Absolute, Y */
static void ABY() {
	cpu.indoperaddr = Memory_ReadWord(cpu.PC);
	cpu.operaddr = cpu.indoperaddr + cpu.Y;
	cpu.PC += 2;
}

/* Indirect */
static void IND() {
	cpu.indoperaddr = Memory_ReadWord(cpu.PC);
 	cpu.operaddr = Memory_ReadWord(cpu.indoperaddr);
	cpu.PC += 2;
	/* TODO: Implement bug */
}

/* (Indirect, X) */
static void IDX() {
	cpu.indoperand = Memory_ReadByte(cpu.PC++);
	cpu.indoperaddr = (WORD)cpu.X + cpu.indoperand;
	if (cpu.indoperaddr == 0xFF) {
		cpu.operaddr = (WORD)Memory_ReadByte(0xFF) | ((WORD)Memory_ReadByte(0) << 8);
	}
	else
		cpu.operaddr = Memory_ReadWord(((0x00ff & (cpu.indoperaddr))));
}

/* (Indirect), Y */
static void IDY() {
	cpu.indoperand = Memory_ReadByte(cpu.PC++);
	cpu.indoperaddr = Memory_ReadWord((0x00ff & cpu.indoperand));
	cpu.operaddr = cpu.indoperaddr + cpu.Y;
}
