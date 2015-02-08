#ifndef PPU_H
#define PPU_H

#include "../common.h"
#include "../cpu/cpu.h"
#include "../memory/memory.h"

typedef struct {
	// Registers
	BYTE controller;
	BYTE mask;
	BYTE status;
	BYTE oamaddr;
	BYTE oamdata;
	BYTE scroll;
	BYTE addr;
	BYTE data;
	BYTE oddframe;
	BYTE oamdma;

	// Controller
	BYTE vram_addr_inc;
	WORD sprite_pattern_addr;
	WORD bg_pattern_addr;
	BYTE sprite_height;
	BYTE nmi_on_vblank;

	// Mask
	BYTE grayscale;
	BYTE show_bg_left;
	BYTE show_bg;
	BYTE show_sprites_left;
	BYTE show_sprites;
	BYTE emphasize_red;
	BYTE emphasize_green;
	BYTE emphasize_blue;

	// Scrolling stuff
	BYTE vram_latch;

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
