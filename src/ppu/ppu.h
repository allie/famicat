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

#endif
