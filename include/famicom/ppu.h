#ifndef PPU_H
#define PPU_H

#include "../common.h"
#include "cpu.h"
#include "memory.h"

#define NUM_PALETTES 64
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

// Scanline timing
#define SCANLINE_VBLANK_BEGIN 241
#define SCANLINE_VBLANK_END 260
#define SCANLINE_PRELINE 261
#define SCANLINE_POSTLINE 240
#define SCANLINE_VISIBLE_END 239
#define SCANLINE_FRAME_END 261

// Cycle timing
#define CYCLE_COPY_Y_BEGIN 280
#define CYCLE_COPY_Y_END 304
#define CYCLE_INCREMENT_Y 256
#define CYCLE_COPY_X 257
#define CYCLE_PREFETCH_BEGIN 321
#define CYCLE_PREFETCH_END 336
#define CYCLE_VISIBLE_BEGIN 1
#define CYCLE_VISIBLE_END 256
#define CYCLE_STEP_BEGIN 1
#define CYCLE_STEP_END 340
#define CYCLE_BEGIN 1
#define CYCLE_EVALUATE_SPRITE 257

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

	// Palettes
	RGBA palettes[NUM_PALETTES];

	// Data buffers
	RGBA* buffer_front;
	RGBA* buffer_back;
	BYTE* oam;
	QWORD tile;

	// NMI
	int nmi_occurred;
	int nmi_output;
	int nmi_previous;
	int nmi_delay;

	// Temporary variables + counters
	BYTE addr_temp;
	WORD cycle;
	WORD scanline;
	QWORD frame;
	int vblank;
	int sprite_0hit;
	int sprite_overflow;
	int sprite_count;
} PPU;

void PPU_Init();
void PPU_Reset();
void PPU_WriteController(BYTE);
void PPU_WriteMask(BYTE);
BYTE PPU_ReadStatus();
void PPU_WriteOAMAddress(BYTE);
void PPU_WriteOAMData(BYTE);
BYTE PPU_ReadOAMData();
void PPU_WriteScroll(BYTE);
void PPU_WriteAddress(BYTE);
void PPU_WriteData(BYTE);
BYTE PPU_ReadData();
void PPU_WriteOAMDMA(BYTE);
void PPU_ReadPalette(WORD);
void PPU_WritePalette(WORD, BYTE);
void PPU_Step();

#endif
