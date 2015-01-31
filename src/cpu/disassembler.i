static const char* addrsymbols[256] = {
/*    | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | A    | B    | C    | D    | E    | F    |*/
/* 0 */ "IMP", "IDX", "IMP", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "ACC", "IMM", "ABS", "ABS", "ABS", "ABS",
/* 1 */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPX", "ZPX", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABX", "ABX",
/* 2 */ "ABS", "IDX", "IMP", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "ACC", "IMM", "ABS", "ABS", "ABS", "ABS",
/* 3 */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPX", "ZPX", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABX", "ABX",
/* 4 */ "IMP", "IDX", "IMP", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "ACC", "IMM", "ABS", "ABS", "ABS", "ABS",
/* 5 */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPX", "ZPX", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABX", "ABX",
/* 6 */ "IMP", "IDX", "IMP", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "ACC", "IMM", "IND", "ABS", "ABS", "ABS",
/* 7 */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPX", "ZPX", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABX", "ABX",
/* 8 */ "IMM", "IDX", "IMM", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
/* 9 */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPY", "ZPY", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABY", "ABY",
/* A */ "IMM", "IDX", "IMM", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
/* B */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPY", "ZPY", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABY", "ABY",
/* C */ "IMM", "IDX", "IMM", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
/* D */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPX", "ZPX", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABX", "ABX",
/* E */ "IMM", "IDX", "IMM", "IDX", "ZPG", "ZPG", "ZPG", "ZPG", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
/* F */ "REL", "IDY", "IMP", "IDY", "ZPX", "ZPX", "ZPX", "ZPX", "IMP", "ABY", "IMP", "ABY", "ABX", "ABX", "ABX", "ABX"
};

static const char* instrsymbols[256] = {
/*    | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | A    | B    | C    | D    | E    | F    |*/
/* 0 */ "BRK", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO", "PHP", "ORA", "ASL", "NOP", "NOP", "ORA", "ASL", "SLO",
/* 1 */ "BPL", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO", "CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
/* 2 */ "JSR", "AND", "NOP", "RLA", "BIT", "AND", "ROL", "RLA", "PLP", "AND", "ROL", "NOP", "BIT", "AND", "ROL", "RLA",
/* 3 */ "BMI", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA", "SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
/* 4 */ "RTI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE", "PHA", "EOR", "LSR", "NOP", "JMP", "EOR", "LSR", "SRE",
/* 5 */ "BVC", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE", "CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
/* 6 */ "RTS", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA", "PLA", "ADC", "ROR", "NOP", "JMP", "ADC", "ROR", "RRA",
/* 7 */ "BVS", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA", "SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
/* 8 */ "NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX", "DEY", "NOP", "TXA", "NOP", "STY", "STA", "STX", "SAX",
/* 9 */ "BCC", "STA", "NOP", "NOP", "STY", "STA", "STX", "SAX", "TYA", "STA", "TXS", "NOP", "NOP", "STA", "NOP", "NOP",
/* A */ "LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX", "TAY", "LDA", "TAX", "NOP", "LDY", "LDA", "LDX", "LAX",
/* B */ "BCS", "LDA", "NOP", "LAX", "LDY", "LDA", "LDX", "LAX", "CLV", "LDA", "TSX", "LAX", "LDY", "LDA", "LDX", "LAX",
/* C */ "CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP", "INY", "CMP", "DEX", "NOP", "CPY", "CMP", "DEC", "DCP",
/* D */ "BNE", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP", "CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
/* E */ "CPX", "SBC", "NOP", "ISB", "CPX", "SBC", "INC", "ISB", "INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISB",
/* F */ "BEQ", "SBC", "NOP", "ISB", "NOP", "SBC", "INC", "ISB", "SED", "SBC", "NOP", "ISB", "NOP", "SBC", "INC", "ISB"
};

void disassemble1() {
	printf("$%04X | %02X | %s %s | ",
		cpu.PC - 1, cpu.opcode, instrsymbols[cpu.opcode],
		addrsymbols[cpu.opcode]);
}

void disassemble2() {
	printf("$%04X %02X | A:%02X X:%02X Y:%02X SP:%02X | C:%d Z:%d I:%d D:%d V:%d B:%d N:%d | S: %02X %02X\n",
		cpu.operaddr, cpu.operand, cpu.A, cpu.X, cpu.Y, cpu.SP,
		GET_FLAG(FLAG_C) != 0, GET_FLAG(FLAG_Z) != 0, GET_FLAG(FLAG_I) != 0, GET_FLAG(FLAG_D) != 0,
		GET_FLAG(FLAG_V) != 0, GET_FLAG(FLAG_B) != 0, GET_FLAG(FLAG_N) != 0, Memory_ReadByte(STACK_ADDR | (cpu.SP+1)), Memory_ReadByte(STACK_ADDR | (cpu.SP+2)));
}

void disassemble3() {
	/* TODO: specific stuff for each opcode
	switch (cpu.opcode) {
		case 0x00: printf("\n"); break;
		case 0x01: printf("\n"); break;
		case 0x05: printf("\n"); break;
		case 0x06: printf("\n"); break;
		case 0x07: printf("\n"); break;
		case 0x08: printf("\n"); break;
		case 0x09: printf("\n"); break;
		case 0x0A: printf("\n"); break;
		case 0x0D: printf("\n"); break;
		case 0x0E: printf("\n"); break;
		case 0x10: printf("\n"); break;
		case 0x11: printf("\n"); break;
		case 0x15: printf("\n"); break;
		case 0x16: printf("\n"); break;
		case 0x18: printf("\n"); break;
		case 0x19: printf("\n"); break;
		case 0x1D: printf("\n"); break;
		case 0x1E: printf("\n"); break;
		case 0x20: printf("\n"); break;
		case 0x21: printf("\n"); break;
		case 0x24: printf("\n"); break;
		case 0x25: printf("\n"); break;
		case 0x26: printf("\n"); break;
		case 0x28: printf("\n"); break;
		case 0x29: printf("\n"); break;
		case 0x2A: printf("\n"); break;
		case 0x2C: printf("\n"); break;
		case 0x2D: printf("\n"); break;
		case 0x2E: printf("\n"); break;
		case 0x30: printf("\n"); break;
		case 0x31: printf("\n"); break;
		case 0x35: printf("\n"); break;
		case 0x36: printf("\n"); break;
		case 0x38: printf("\n"); break;
		case 0x39: printf("\n"); break;
		case 0x3D: printf("\n"); break;
		case 0x3E: printf("\n"); break;
		case 0x40: printf("\n"); break;
		case 0x41: printf("\n"); break;
		case 0x45: printf("\n"); break;
		case 0x46: printf("\n"); break;
		case 0x48: printf("\n"); break;
		case 0x49: printf("\n"); break;
		case 0x4A: printf("\n"); break;
		case 0x4C: printf("\n"); break;
		case 0x4D: printf("\n"); break;
		case 0x4E: printf("\n"); break;
		case 0x50: printf("\n"); break;
		case 0x51: printf("\n"); break;
		case 0x55: printf("\n"); break;
		case 0x56: printf("\n"); break;
		case 0x58: printf("\n"); break;
		case 0x59: printf("\n"); break;
		case 0x5D: printf("\n"); break;
		case 0x5E: printf("\n"); break;
		case 0x60: printf("\n"); break;
		case 0x61: printf("\n"); break;
		case 0x65: printf("\n"); break;
		case 0x66: printf("\n"); break;
		case 0x68: printf("\n"); break;
		case 0x69: printf("\n"); break;
		case 0x6A: printf("\n"); break;
		case 0x6C: printf("\n"); break;
		case 0x6D: printf("\n"); break;
		case 0x6E: printf("\n"); break;
		case 0x70: printf("\n"); break;
		case 0x71: printf("\n"); break;
		case 0x75: printf("\n"); break;
		case 0x76: printf("\n"); break;
		case 0x78: printf("\n"); break;
		case 0x79: printf("\n"); break;
		case 0x7D: printf("\n"); break;
		case 0x7E: printf("\n"); break;
		default: printf("\n"); break;
	}
	*/
}
