#ifndef MEMORY_H
#define MEMORY_H

#include "../common.h"

typedef struct {
	/* Some stuff */
} Memory;

BYTE Memory_ReadByte(WORD addr);
WORD Memory_ReadWord(WORD addr);
void Memory_WriteByte(WORD addr, BYTE val);
void Memory_WriteWord(WORD addr, WORD val);

#endif
