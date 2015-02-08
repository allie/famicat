#include "ppu.h"

PPU ppu;
extern CPU cpu;

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

	ppu.grayscale = 0;
	ppu.show_bg_left = 1;
	ppu.show_bg = 1;
	ppu.show_sprites_left = 1;
	ppu.show_sprites = 1;
	ppu.emphasize_red = 0;
	ppu.emphasize_green = 0;
	ppu.emphasize_blue = 0;

	ppu.first_write = 1;
	ppu.vram_temp = 0;
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
/*
	7654 3210
	|||| ||||
	|||| |||+- Grayscale (0: normal color, 1: produce a greyscale display)
	|||| ||+-- 1: Show background in leftmost 8 pixels of screen, 0: Hide
	|||| |+--- 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
	|||| +---- 1: Show background
	|||+------ 1: Show sprites
	||+------- Emphasize red*
	|+-------- Emphasize green*
	+--------- Emphasize blue*
*/
	ppu.grayscale = val & 0x1;
	ppu.show_bg_left = (val >> 1) & 0x1;
	ppu.show_bg = (val >> 2) & 0x1;
	ppu.show_sprites = (val >> 3) & 0x1;
	ppu.show_sprites_left = (val >> 4) & 0x1;
	ppu.emphasize_red = (val >> 5) & 0x1;
	ppu.emphasize_green = (val >> 6) & 0x1;
	ppu.emphasize_blue = (val >> 7) & 0x1;
}

// necessary?
BYTE PPU_ReadStatus() {

}

void PPU_WriteOAMAddress(BYTE val) {
	ppu.oamaddr = val;
}

void PPU_WriteOAMData(BYTE val) {
	ppu.oam[ppu.oamaddr++] = val;
}

// necessary?
BYTE PPU_ReadOAMData() {
	return ppu.oam[ppu.oamaddr];
	// TODO: ignore writes during rendering
}

void PPU_WriteScroll(BYTE val) {
	if (ppu.first_write) {
		// Horizontal scroll offset
		ppu.fine_x = val & 0x7;
		ppu.vram_temp = (ppu.vram_temp & 0x7FE0) | ((val & 0xF8) >> 3);
	} else {
		// Vertical scroll offset
		ppu.vram_temp = (ppu.vram_temp & 0xC1F) | (((WORD)val & 0xF8) << 2) | (((WORD)val & 7) << 12);
	}

	ppu.first_write = !ppu.first_write;
}

void PPU_WriteAddress(BYTE val) {
	if (ppu.first_write) {
		// Clear bits 14-8 (and 15), save 5-0 of val to 13-8 of temp
		ppu.vram_temp = (ppu.vram_temp & 0x00FF) | (((WORD)val & 0x3F) << 8);
	} else {
		// Copy lower 8 bits to temp, set addr to temp value
		ppu.vram_temp = (ppu.vram_temp & 0x7F00) | val;
		ppu.addr = ppu.vram_temp;
	}

	ppu.first_write = !ppu.first_write;
}

void PPU_WriteData(BYTE val) {
	// TODO: Disable during rendering
	Memory_WriteByte(MAP_PPU, ppu.addr, val);
	ppu.addr += vram_addr_inc;
}

// necessary?
BYTE PPU_ReadData() {
	// TODO: Disable during rendering
	Memory_ReadByte(MAP_PPU, ppu.addr);
	ppu.addr += vram_addr_inc;
}

void PPU_WriteOAMDMA(BYTE val) {
	WORD addr_high = ((WORD)val << 8);
	for (BYTE addr_low = 0; addr_low < 256; addr_low++) {
		ppu.oam[ppu.oamaddr++] = Memory_ReadByte(MAP_CPU, (addr_high | addr_low));
	}
	if (cpu.cycles % 2 == 0)
		CPU_Suspend(514);
	else
		CPU_Suspend(513);
}
