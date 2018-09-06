#ifndef MEMORY_H
#define MEMORY_H

#define MAP_CPU 0
#define MAP_PPU 1

#include "../common.h"

typedef struct {
	void (*init)();
	BYTE (*read)(WORD);
	void (*write)(WORD, BYTE);
	void (*step)();
	void (*save)();
	void (*load)();
} Mapper;

typedef struct {
	// CPU memory map
	BYTE* ram;
	BYTE* exprom;

	// PPU memory map
	BYTE* pattern0;
	BYTE* pattern1;
	BYTE* nametable0;
	BYTE* nametable1;
	BYTE* nametable2;
	BYTE* nametable3;
	BYTE* paletteram;

	// Currently loaded mapper
	Mapper mapper;
} Memory;

void Memory_Reset();
void Memory_SetMapper(int);
BYTE Memory_ReadByte(int, WORD);
WORD Memory_ReadWord(int, WORD);
void Memory_WriteByte(int, WORD, BYTE);
void Memory_WriteWord(int, WORD, WORD);
BYTE Memory_InvalidRead(int, WORD);
void Memory_InvalidWrite(int, WORD, BYTE);

#ifdef DEBUG_MODE
void Memory_Dump();
#endif

#endif
