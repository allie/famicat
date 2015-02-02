#ifndef MEMORY_H
#define MEMORY_H

#include "../common.h"

typedef struct {
	// CPU memory map
	BYTE* ram;
	BYTE* ppureg;
	BYTE* apureg;
	BYTE* exprom;
	BYTE* sram;
} Memory;

void Memory_Reset();
BYTE Memory_ReadByte(WORD addr);
WORD Memory_ReadWord(WORD addr);
void Memory_WriteByte(WORD addr, BYTE val);
void Memory_WriteWord(WORD addr, WORD val);

#ifdef DEBUG_MODE
void Memory_Dump();
#endif

#endif
