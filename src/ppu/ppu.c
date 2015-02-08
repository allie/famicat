#include "ppu.h"

PPU ppu;

void PPU_Init() {
	if (ppu.oam != NULL)
		free(ppu.oam);

	ppu.oam = (BYTE*)calloc(0x100, sizeof(BYTE));

	ppu.controller = 0;
	ppu.mask = 0;
	ppu.status = 0xA0;
	ppu.scroll = 0;
	ppu.addr = 0;
	ppu.data = 0;
	ppu.oddframe = 0;

	ppu.vram_addr_inc = 1;
	ppu.sprite_pattern_addr = 0;
	ppu.bg_pattern_addr = 0;
	ppu.sprite_height = 8;
	ppu.nmi_on_vblank = 0;

	ppu.vram_latch = 0;
}

void PPU_Reset() {
	// TODO
}

void PPU_WriteController(BYTE val) {
/*
	7654 3210
	|||| ||||
	|||| ||++- Base nametable address
	|||| ||    (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
	|||| |+--- VRAM address increment per CPU read/write of PPUDATA
	|||| |     (0: add 1, going across; 1: add 32, going down)
	|||| +---- Sprite pattern table address for 8x8 sprites
	||||       (0: $0000; 1: $1000; ignored in 8x16 mode)
	|||+------ Background pattern table address (0: $0000; 1: $1000)
	||+------- Sprite size (0: 8x8; 1: 8x16)
	|+-------- PPU master/slave select
	|          (0: read backdrop from EXT pins; 1: output color on EXT pins)
	+--------- Generate an NMI at the start of the
               vertical blanking interval (0: off; 1: on)
*/

	ppu.controller = val;

	ppu.vram_addr_inc = ((val >> 2) & 0x1) ? 1: 32;
	ppu.sprite_pattern_addr = ((val >> 3) & 0x1) * 0x1000;
	ppu.bg_pattern_addr = ((val >> 4) & 0x1) * 0x1000;
	ppu.sprite_height = ((val >> 5) & 0x1) ? 8 : 16;
	ppu.nmi_on_vblank = (val >> 7) & 0x1;

	// Change scroll latch to match base nametable address
	ppu.vram_latch = (ppu.vram_latch & 0xF3FF) | ((WORD)(val & 0x3) << 10);
}

void PPU_WriteMask(BYTE val) {

}

// necessary?
BYTE PPU_ReadStatus() {

}

void PPU_WriteOAMAddress(BYTE val) {

}

void PPU_WriteOAMData(BYTE val) {

}

// necessary?
BYTE PPU_ReadOAMData() {

}

void PPU_WriteScroll(BYTE val) {

}

void PPU_WriteAddress(BYTE val) {

}

void PPU_WriteData(BYTE val) {

}

// necessary?
BYTE PPU_ReadData() {

}

void PPU_WriteOAMDMA(BYTE val) {

}

