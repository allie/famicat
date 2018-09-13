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
#define CYCLE_FRAME_END 339
#define CYCLE_BEGIN 1
#define CYCLE_EVALUATE_SPRITE 257

// Fetch cycles
#define FETCH_NAMETABLE 1
#define FETCH_ATTRIBUTETABLE 3
#define FETCH_TILE_LOW 5
#define FETCH_TILE_HIGH 7
#define FETCH_STORE 0

typedef struct {
	// Registers
	BYTE controller;
	BYTE mask;
	BYTE status;
	BYTE oam_addr;
	BYTE scroll;
	BYTE vram_addr;
	BYTE vram_data;
	BYTE odd_frame;
	BYTE oamdma;

	// Controller
	BYTE nametable_addr;
	BYTE vram_addr_inc;
	BYTE sprite_pattern_addr;
	BYTE bg_pattern_addr;
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
	BYTE fine_x;
	BYTE fine_y;

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

	// Sprites
	int sprite_0hit;
	int sprite_overflow;
	int sprite_count;
	DWORD sprite_patterns[8];
	BYTE sprite_positions[8];
	BYTE sprite_priorities[8];
	BYTE sprite_indices[8];

	// Temporary variables + counters
	BYTE vram_addr_temp;
	WORD cycle;
	WORD scanline;
	QWORD frame;
	int vblank;
	BYTE nametable_byte;
	BYTE attributetable_byte;
	BYTE tile_byte_low;
	BYTE tile_byte_high;
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
BYTE PPU_ReadPalette(WORD);
void PPU_WritePalette(WORD, BYTE);
void PPU_Step();

#endif
