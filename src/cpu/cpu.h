#ifndef CPU_H
#define CPU_H

#include "../common.h"

typedef struct {
	uint16_t PC;
	uint8_t SP;
	uint8_t A;
	uint8_t X;
	uint8_t Y;
	uint8_t S;
	uint8_t opcode;
	uint8_t operand;
	uint32_t cycles;
} CPU;

#endif
