#ifndef PPU_H
#define PPU_H

#include "../common.h"
#include "cpu.h"
#include "memory.h"

typedef struct {
	// Registers
	BYTE controller;
	BYTE mask;
	BYTE status;
	BYTE oam_addr;
	BYTE scroll;
	BYTE addr;
	BYTE data;
	BYTE odd_frame;
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

	// Scroll
	BYTE first_write;
	BYTE vram_temp;
	BYTE fine_x;

	BYTE* oam;

	WORD cycle;
	WORD scanline;
	QWORD frame;
} PPU;

void PPU_Init();
void PPU_Reset();
void PPU_WriteController(BYTE);
void PPU_WriteMask(BYTE);
BYTE PPU_ReadStatus(); // necessary?
void PPU_WriteOAMAddress(BYTE);
void PPU_WriteOAMData(BYTE);
BYTE PPU_ReadOAMData(); // necessary?
void PPU_WriteScroll(BYTE);
void PPU_WriteAddress(BYTE);
void PPU_WriteData(BYTE);
BYTE PPU_ReadData(); // necessary?
void PPU_WriteOAMDMA(BYTE);

#endif
