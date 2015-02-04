#ifndef MEMORY_H
#define MEMORY_H

#define MAP_CPU 0
#define MAP_PPU 1

#include "../common.h"

typedef struct {
	// CPU memory map
	BYTE* ram;
	BYTE* ppureg;
	BYTE* apureg;
	BYTE* exprom;
	BYTE* sram;

	// PPU memory map
	BYTE* pattern0;
	BYTE* pattern1;
	BYTE* nametable0;
	BYTE* nametable1;
	BYTE* nametable2;
	BYTE* nametable3;
	BYTE* paletteram;
} Memory;

void Memory_Reset();
BYTE Memory_ReadByte(int map, WORD addr);
WORD Memory_ReadWord(int map, WORD addr);
void Memory_WriteByte(int map, WORD addr, BYTE val);
void Memory_WriteWord(int map, WORD addr, WORD val);

#ifdef DEBUG_MODE
void Memory_Dump();
#endif

#endif
