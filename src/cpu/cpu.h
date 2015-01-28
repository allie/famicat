#ifndef CPU_H
#define CPU_H

#include "common.h"

/* Empty macros for noting "implied" and "accumulator" addressing modes;
   these modes do not need any implementation */
#define IMP
#define ACC

/* Status flag bitmasks */
#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_I 0x04
#define FLAG_D 0x08
#define FLAG_B 0x10
#define FLAG_V 0x40
#define FLAG_N 0x80

/* Stack base address */
#define STACK_ADDR 0x100

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

void CPU_Reset();

#endif
