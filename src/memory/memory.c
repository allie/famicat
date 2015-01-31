#include "memory.h"

Memory memory;

void Memory_Reset() {
	if (memory.ram != NULL)
		free(memory.ram);

	memory.ram = calloc(0x10000, sizeof(BYTE));
}

BYTE Memory_ReadByte(WORD addr) {
	return memory.ram[addr];
}

WORD Memory_ReadWord(WORD addr) {
	return (WORD)Memory_ReadByte(addr) | ((WORD)Memory_ReadByte(addr + 1) << 8);
}

void Memory_WriteByte(WORD addr, BYTE val) {
	memory.ram[addr] = val;
}

void Memory_WriteWord(WORD addr, WORD val) {
	Memory_WriteByte(addr, (BYTE)(val & 0x00FF));
	Memory_WriteByte(addr + 1, (BYTE)((val & 0xFF00) >> 8));
}
