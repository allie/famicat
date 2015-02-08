#ifndef PPU_H
#define PPU_H

#include "../common.h"

typedef struct {
	BYTE controller;
	BYTE mask;
	BYTE status;
	BYTE oamaddr;
	BYTE oamdata;
	BYTE scrolllatch;
	BYTE scroll;
	BYTE addrlatch;
	BYTE addr;
	BYTE data;
	BYTE oddframe;
	BYTE oamdma;
	BYTE* oam;
} PPU;

void PPU_Init();
void PPU_Reset();

void PPU_WriteController(BYTE val);
void PPU_WriteMask(BYTE val);
BYTE PPU_ReadStatus(); // necessary?
void PPU_WriteOAMAddress(BYTE val);
void PPU_WriteOAMData(BYTE val);
BYTE PPU_ReadOAMData(); // necessary?
void PPU_WriteScroll(BYTE val);
void PPU_WriteAddress(BYTE val);
void PPU_WriteData(BYTE val);
BYTE PPU_ReadData(); // necessary?
void PPU_WriteOAMDMA(BYTE val);

#endif
