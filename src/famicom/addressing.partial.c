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
	cpu.indoperand = Memory_ReadByte(MAP_CPU, cpu.PC++);
	cpu.operaddr = (WORD)cpu.indoperand;
}

/* Zero-page, X */
static void ZPX() {
	cpu.indoperand = Memory_ReadByte(MAP_CPU, cpu.PC++);
	cpu.operaddr = ((WORD)cpu.indoperand + (WORD)cpu.X) & 0xFF;
}

/* Zero-page, Y */
static void ZPY() {
	cpu.indoperand = Memory_ReadByte(MAP_CPU, cpu.PC++);
	cpu.operaddr = ((WORD)cpu.indoperand + (WORD)cpu.Y) & 0xFF;
}

/* Relative */
static void REL() {
	cpu.operaddr = cpu.PC++;
}

/* Absolute */
static void ABS() {
	cpu.operaddr = Memory_ReadWord(MAP_CPU, cpu.PC);
	cpu.PC += 2;
}

/* Absolute, X */
static void ABX() {
	cpu.indoperaddr = Memory_ReadWord(MAP_CPU, cpu.PC);
	cpu.operaddr = cpu.indoperaddr + cpu.X;
	cpu.PC += 2;
}

/* Absolute, Y */
static void ABY() {
	cpu.indoperaddr = Memory_ReadWord(MAP_CPU, cpu.PC);
	cpu.operaddr = cpu.indoperaddr + cpu.Y;
	cpu.PC += 2;
}

/* Indirect */
static void IND() {
	cpu.indoperaddr = Memory_ReadWord(MAP_CPU, cpu.PC);
	WORD tmp = cpu.indoperaddr;
 	WORD tmp2 = (tmp & 0xFF00) | ((tmp + 1) & 0x00FF);
 	cpu.operaddr = (WORD)Memory_ReadByte(MAP_CPU, tmp) | ((WORD)Memory_ReadByte(MAP_CPU, tmp2) << 8);
	cpu.PC += 2;
}

/* (Indirect, X) */
static void IDX() {
	cpu.indoperand = Memory_ReadByte(MAP_CPU, cpu.PC++);
	cpu.indoperaddr = (WORD)cpu.X + cpu.indoperand;
	if (cpu.indoperaddr == 0xFF) {
		cpu.operaddr = (WORD)Memory_ReadByte(MAP_CPU, 0xFF) | ((WORD)Memory_ReadByte(MAP_CPU, 0) << 8);
	}
	else
		cpu.operaddr = Memory_ReadWord(MAP_CPU, ((0x00ff & (cpu.indoperaddr))));
}

/* (Indirect), Y */
static void IDY() {
	cpu.indoperand = Memory_ReadByte(MAP_CPU, cpu.PC++);
	if (cpu.indoperand == 0xFF) {
		cpu.indoperaddr = (WORD)Memory_ReadByte(MAP_CPU, 0xFF) | ((WORD)Memory_ReadByte(MAP_CPU, 0) << 8);
	}
	else
		cpu.indoperaddr = Memory_ReadWord(MAP_CPU, (0x00ff & cpu.indoperand));
	cpu.operaddr = cpu.indoperaddr + cpu.Y;
}
