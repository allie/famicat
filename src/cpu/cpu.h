#ifndef CPU_H
#define CPU_H

#include "../common.h"

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

#define NONE 0
#define IRQ 1
#define NMI 2
#define RESET 3

typedef struct {
	WORD PC;
	BYTE SP;
	BYTE A;
	BYTE X;
	BYTE Y;
	BYTE S;
	BYTE opcode;
	WORD operaddr;
	BYTE operand;
	WORD indoperaddr;
	BYTE indoperand;
	DWORD cycles;
	BYTE interrupt;
} CPU;

void CPU_Reset();
void CPU_Step();

#endif
