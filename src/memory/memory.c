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
	else if (addr >= 0x2000 && addr < 0x2008)
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
	else if (addr >= 0xC000 && addr <= 0xFFFF)
		return cart.prg + (addr - 0x8000);

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
