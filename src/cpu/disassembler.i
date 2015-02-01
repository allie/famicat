#define CALC_OFFSET() \
	((cpu.operand & 0x80) ? cpu.PC - (cpu.operand & 0x7F) : cpu.PC + (cpu.operand & 0x7F))

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
	printf("%04X  %02X ", cpu.PC - 1, cpu.opcode);
}

void disassemble2() {
	if (mode == ABX || mode == ABY || mode == IND)
		printf("%02X %02X  ", cpu.indoperaddr & 0xFF, ((cpu.indoperaddr >> 8) & 0xFF));

	else if (mode == ABS)
		printf("%02X %02X  ", cpu.operaddr & 0xFF, (cpu.operaddr >> 8) & 0xFF);

	else if (mode == IMP || mode == ACC)
		printf("       ");

	else if (mode == ZPG || mode == ZPX || mode == ZPY || mode == IDX || mode == IDY)
		printf("%02X     ", cpu.indoperand);

	else
		printf("%02X     ", cpu.operand);

	printf("%s ", instrsymbols[cpu.opcode]);

	if (mode == IMP)
		printf("                            ");

	else if (mode == ACC)
		printf("A                           ");

	else if (mode == IMM)
			printf("#$%02X                        ", cpu.operand);

	else if (mode == ZPG)
		printf("$%02X = %02X                    ", cpu.indoperand, cpu.operand);

	else if (mode == ZPX)
		printf("$%02X,X @ %02X = %02X             ", cpu.indoperand, cpu.operaddr & 0xFF, cpu.operand);

	else if (mode == ZPY)
		printf("$%02X,Y @ %02X = %02X             ", cpu.indoperand, cpu.operaddr & 0xFF, cpu.operand);

	else if (mode == REL)
		printf("$%04X                       ",
			(cpu.operand & 0x80) ? cpu.PC - (cpu.operand & 0x7F) : cpu.PC + (cpu.operand & 0x7F));

	else if (mode == ABS) {
		if (cpu.opcode == 0x20 || cpu.opcode == 0x4C)
			printf("$%04X                       ", cpu.operaddr);
		else
			printf("$%04X = %02X                  ", cpu.operaddr, cpu.operand);
	}

	else if (mode == ABX)
		printf("$%04X,X @ %04X = %02X         ", cpu.indoperaddr, cpu.operaddr, cpu.operand);

	else if (mode == ABY)
		printf("$%04X,Y @ %04X = %02X         ", cpu.indoperaddr, cpu.operaddr, cpu.operand);

	else if (mode == IND)
		printf("($%04X) = %04X              ", cpu.indoperaddr, cpu.operaddr);

	else if (mode == IDX)
		printf("($%02X,X) @ %02X = %04X = %02X    ",
			cpu.indoperand, cpu.indoperaddr & 0xFF, cpu. operaddr, cpu.operand);

	else if (mode == IDY)
		printf("($%02X),Y = %04X @ %04X = %02X  ",
			cpu.indoperand, cpu.indoperaddr, cpu.operaddr, cpu.operand);

	else
		printf("OOPS");

	printf("A:%02X X:%02X Y:%02X P:%02X SP:%02X\n", cpu.A, cpu.X, cpu.Y, cpu.S, cpu.SP);
}
