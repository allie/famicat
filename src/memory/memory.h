#ifndef MEMORY_H
#define MEMORY_H

#include "../common.h"

typedef struct {
	/* Some stuff */
} Memory;

uint8_t Memory_ReadByte(uint16_t addr);
uint16_t Memory_ReadWord(uint16_t addr);
void Memory_WriteByte(uint16_t addr, uint8_t val);
void Memory_WriteWord(uint16_t addr, uint16_t val);

#endif
