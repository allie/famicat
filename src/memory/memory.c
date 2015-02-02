#include "memory.h"
#include "../cart/cart.h"

Memory memory;
extern Cart cart;

void Memory_Reset() {
	if (memory.ram != NULL)
		free(memory.ram);

	if (memory.ppureg != NULL)
		free(memory.ppureg);

	if (memory.apureg != NULL)
		free(memory.apureg);

	if (memory.exprom != NULL)
		free(memory.exprom);

	if (memory.sram != NULL)
		free(memory.sram);

	memory.ram = calloc(0x800, sizeof(BYTE));
	memory.ppureg = calloc(8, sizeof(BYTE));
	memory.apureg = calloc(0x20, sizeof(BYTE));
	memory.exprom = calloc(0x1FDF, sizeof(BYTE));
	memory.sram = calloc(0x2000, sizeof(BYTE));
}

static BYTE* decodecpu(WORD addr) {
	// Working RAM + mirrors
	if (addr < 0x2000)
		return memory.ram + (addr % 0x800);

	// PPU control registers + mirrors
	else if (addr >= 0x2000 && addr < 0x4000)
		return memory.ppureg + ((addr - 0x2000) % 8);

	// APU registers
	else if (addr >= 0x4000 && addr < 0x4020)
		return memory.apureg + (addr - 0x4000);

	// Cartridge expansion ROM
	else if (addr >= 0x4020 && addr < 0x6000)
		return memory.exprom + (addr - 0x4020);

	// SRAM
	else if (addr >= 0x6000 && addr < 0x8000)
		return memory.sram + (addr - 0x6000);

	// PRG-ROM 1
	// TODO: MAPPERS!!!
	else if (addr >= 0x8000 && addr < 0xC000)
		return cart.prg + (addr - 0x8000);

	// PRG-ROM 2
	else if (addr >= 0xC000 && addr <= 0xFFFF) {
		if (cart.mapper == 0 && cart.prgromsize == 1)
			return cart.prg + (addr - 0xC000);
		else
			return cart.prg + (addr - 0x8000);
	}

	return 0;
}

BYTE Memory_ReadByte(WORD addr) {
	return *(decodecpu(addr));
}

WORD Memory_ReadWord(WORD addr) {
	return (WORD)Memory_ReadByte(addr) | ((WORD)Memory_ReadByte(addr + 1) << 8);
}

void Memory_WriteByte(WORD addr, BYTE val) {
	// TODO: write protection
	*(decodecpu(addr)) = val;
}

void Memory_WriteWord(WORD addr, WORD val) {
	Memory_WriteByte(addr, (BYTE)(val & 0x00FF));
	Memory_WriteByte(addr + 1, (BYTE)((val & 0xFF00) >> 8));
}

#ifdef DEBUG_MODE
void Memory_Dump() {
	FILE* fp = fopen("memory.log", "w");

	if (!fp) {
		printf("Error opening memory.log for output.\n");
		return;
	}

	// Working RAM + mirrors
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "Working RAM: 0x0000 - 0x1FFF\n");
	fprintf(fp, "--------------------------------------------------------\n");

	for (int i = 0x0000; i < 0x1FFF; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");


	// PPU control registers + mirrors
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "PPU control registers: 0x2000 - 0x3FFF\n");
	fprintf(fp, "--------------------------------------------------------\n");

	for (int i = 0x2000; i < 0x3FFF; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	// APU & I/O registers
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "APU & I/O registers: 0x4000 - 0x401F\n");
	fprintf(fp, "--------------------------------------------------------\n");
	
	for (int i = 0x4000; i < 0x401F; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	// Cartridge expansion ROM
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "Cartridge expansion ROM: 0x4020 - 0x5FFF\n");
	fprintf(fp, "--------------------------------------------------------\n");
	
	for (int i = 0x4020; i < 0x5FFF; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	// SRAM
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "SRAM: 0x6000 - 0x7FFF\n");
	fprintf(fp, "--------------------------------------------------------\n");
	
	for (int i = 0x6000; i < 0x7FFF; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	// PRG ROM 1
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "PRG ROM 1: 0x8000 - 0xBFFF\n");
	fprintf(fp, "--------------------------------------------------------\n");
	
	for (int i = 0x8000; i < 0xBFFF; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	// PRG ROM 1
	fprintf(fp, "--------------------------------------------------------\n");
	fprintf(fp, "PRG ROM 2: 0xC000 - 0xFFFF\n");
	fprintf(fp, "--------------------------------------------------------\n");
	
	for (int i = 0xC000; i < 0xFFFF; i += 16) {
		fprintf(fp, "%04X  ", i);

		for (int j = 0; j < 16; j += 4) {
			for (int k = j; k < (j + 4); k++) {
				BYTE val = Memory_ReadByte((WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
}
#endif
