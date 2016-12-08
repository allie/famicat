#include "memory/memory.h"
#include "cart/cart.h"
#include "apu/apu.h"

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

	if (memory.pattern0 != NULL)
		free(memory.pattern0);

	if (memory.pattern1 != NULL)
		free(memory.pattern1);

	if (memory.nametable0 != NULL)
		free(memory.nametable0);

	if (memory.nametable1 != NULL)
		free(memory.nametable1);

	if (memory.nametable2 != NULL)
		free(memory.nametable2);

	if (memory.nametable3 != NULL)
		free(memory.nametable3);

	if (memory.paletteram != NULL)
		free(memory.paletteram);

	memory.ram = (BYTE*)calloc(0x800, sizeof(BYTE));
	memory.ppureg = (BYTE*)calloc(8, sizeof(BYTE));
	memory.apureg = (BYTE*)calloc(0x20, sizeof(BYTE));
	memory.exprom = (BYTE*)calloc(0x1FDF, sizeof(BYTE));
	memory.sram = (BYTE*)calloc(0x2000, sizeof(BYTE));

	memory.pattern0 = (BYTE*)calloc(0x1000, sizeof(BYTE));
	memory.pattern1 = (BYTE*)calloc(0x1000, sizeof(BYTE));
	memory.nametable0 = (BYTE*)calloc(0x400, sizeof(BYTE));
	memory.nametable1 = (BYTE*)calloc(0x400, sizeof(BYTE));
	memory.nametable2 = (BYTE*)calloc(0x400, sizeof(BYTE));
	memory.nametable3 = (BYTE*)calloc(0x400, sizeof(BYTE));
	memory.paletteram = (BYTE*)calloc(0x20, sizeof(BYTE));
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
		return memory.apureg;

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

static BYTE* decodeppu(WORD addr) {
	// Handle nametable mirrors
	if (addr >= 0x3000 && addr < 0x3F00)
		addr -= 0x1000;

	// Pattern table 0
	if (addr < 0x1000)
		return memory.pattern0 + addr;

	// Pattern table 1
	else if (addr >= 0x1000 && addr < 0x2000)
		return memory.pattern1 + (addr - 0x1000);

	// Nametable 0
	else if (addr >= 0x2000 && addr < 0x2400)
		return memory.nametable0 + (addr - 0x2000);

	// Nametable 1
	else if (addr >= 0x2400 && addr < 0x2800)
		return memory.nametable1 + (addr - 0x2400);

	// Nametable 2
	else if (addr >= 0x2800 && addr < 0x2C00)
		return memory.nametable2 + (addr - 0x2800);

	// Nametable 3
	else if (addr >= 0x2C00 && addr < 0x3000)
		return memory.nametable3 + (addr - 0x2C00);

	// Palette RAM indices + mirrors
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
		return memory.paletteram + ((addr - 0x3F00) % 0x20);

	return 0;
}

BYTE Memory_ReadByte(int map, WORD addr) {
	if (map == MAP_CPU) {
		if (addr == 0x4015)
			return APU_Read();
		return *(decodecpu(addr));
	}
	else if (map == MAP_PPU)
		return *(decodeppu(addr));
	return 0;
}

WORD Memory_ReadWord(int map, WORD addr) {
	return (WORD)Memory_ReadByte(map, addr) | ((WORD)Memory_ReadByte(map, addr + 1) << 8);
}

void Memory_WriteByte(int map, WORD addr, BYTE val) {
	// TODO: write protection
	if (map == MAP_CPU) {
		BYTE *block = decodecpu(addr);
		if (block == memory.apureg)
			APU_Write(addr, val);
		else
			*block = val;
	}
	else if (map == MAP_PPU)
		*(decodeppu(addr)) = val;
}

void Memory_WriteWord(int map, WORD addr, WORD val) {
	Memory_WriteByte(map, addr, (BYTE)(val & 0x00FF));
	Memory_WriteByte(map, addr + 1, (BYTE)((val & 0xFF00) >> 8));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
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
				BYTE val = Memory_ReadByte(MAP_CPU, (WORD)(i + k));
				fprintf(fp, "%02X ", val);
			}

			fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
}
#endif
