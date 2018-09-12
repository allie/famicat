#include "famicom/ppu.h"
#include "famicom/cpu.h"
#include <string.h>

PPU ppu;
extern CPU cpu;
extern Memory memory;

#define SCANLINE_MAX 262

static DWORD colours[] = {
	0x666666, 0x002A88, 0x1412A7, 0x3B00A4, 0x5C007E, 0x6E0040, 0x6C0600, 0x561D00,
	0x333500, 0x0B4800, 0x005200, 0x004F08, 0x00404D, 0x000000, 0x000000, 0x000000,
	0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE, 0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
	0x6B6D00, 0x388700, 0x0C9300, 0x008F32, 0x007C8D, 0x000000, 0x000000, 0x000000,
	0xFFFEFF, 0x64B0FF, 0x9290FF, 0xC676FF, 0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
	0xBCBE00, 0x88D800, 0x5CE430, 0x45E082, 0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
	0xFFFEFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF, 0xFBC2FF, 0xFEC4EA, 0xFECCC5, 0xF7D8A5,
	0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC, 0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000,
};

static void PPU_ClearBuffers() {
	if (ppu.buffer_front) {
		free(ppu.buffer_front);
	}

	if (ppu.buffer_back) {
		free(ppu.buffer_back);
	}

	ppu.buffer_front = (RGBA*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(RGBA));
	ppu.buffer_back = (RGBA*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(RGBA));

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			int index = i * SCREEN_WIDTH + j;
			ppu.buffer_front[index].r = 0;
			ppu.buffer_front[index].g = 0;
			ppu.buffer_front[index].b = 0;
			ppu.buffer_front[index].a = 255;
		}
	}
}

void PPU_Init() {
	if (ppu.oam != NULL)
		free(ppu.oam);

	ppu.oam = (BYTE*)calloc(0x100, sizeof(BYTE));

	ppu.controller = 0;
	ppu.mask = 0;
	ppu.status = 0xA0;
	ppu.oam_addr = 0;
	ppu.scroll = 0;
	ppu.vram_addr = 0;
	ppu.vram_data = 0;
	ppu.odd_frame = 0;

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
	ppu.vram_addr_temp = 0;
	ppu.fine_x = 0;

	for (int i = 0; i < NUM_PALETTES; i++) {
		ppu.palettes[i].r = (BYTE)(colours[i] >> 16);
		ppu.palettes[i].g = (BYTE)(colours[i] >> 8);
		ppu.palettes[i].b = (BYTE)colours[i];
		ppu.palettes[i].a = 255;
	}

	ppu.cycle = 0;
	ppu.scanline = 0;
	ppu.frame = 0;

	PPU_ClearBuffers();
}

void PPU_Reset() {
	ppu.cycle = CYCLE_STEP_END;
	ppu.scanline = SCANLINE_POSTLINE;
	ppu.frame = 0;
	PPU_WriteController(0);
	PPU_WriteMask(0);
	PPU_WriteOAMAddress(0);
	PPU_ClearBuffers();
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
	if (cpu.cycles >= 30000) { // "about" 30000 cycles
		ppu.controller = val;

		ppu.vram_addr_inc = ((val >> 2) & 0x1) ? 1: 32;
		ppu.sprite_pattern_addr = ((val >> 3) & 0x1) * 0x1000;
		ppu.bg_pattern_addr = ((val >> 4) & 0x1) * 0x1000;
		ppu.sprite_height = ((val >> 5) & 0x1) ? 8 : 16;
		ppu.nmi_on_vblank = (val >> 7) & 0x1;

		// Change scroll latch to match base nametable address
		ppu.vram_addr_temp = (ppu.vram_addr_temp & 0xF3FF) | ((WORD)(val & 0x3) << 10);
	}
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

BYTE PPU_ReadStatus() {
	ppu.status &= 0x7F;
	ppu.first_write = 1;
	return ppu.status;
}

void PPU_WriteOAMAddress(BYTE val) {
	ppu.oam_addr = val;
}

void PPU_WriteOAMData(BYTE val) {
	ppu.oam[ppu.oam_addr++] = val;
}

// necessary?
BYTE PPU_ReadOAMData() {
	return ppu.oam[ppu.oam_addr];
	// TODO: ignore writes during rendering
}

void PPU_WriteScroll(BYTE val) {
	if (ppu.first_write) {
		// Horizontal scroll offset
		ppu.fine_x = val & 0x7;
		ppu.vram_addr_temp = (ppu.vram_addr_temp & 0x7FE0) | ((val & 0xF8) >> 3);
	} else {
		// Vertical scroll offset
		ppu.vram_addr_temp = (ppu.vram_addr_temp & 0xC1F) | (((WORD)val & 0xF8) << 2) | (((WORD)val & 7) << 12);
	}

	ppu.first_write = !ppu.first_write;
}

void PPU_WriteAddress(BYTE val) {
	if (ppu.first_write) {
		// Clear bits 14-8 (and 15), save 5-0 of val to 13-8 of temp
		ppu.vram_addr_temp = (ppu.vram_addr_temp & 0x00FF) | (((WORD)val & 0x3F) << 8);
	} else {
		// Copy lower 8 bits to temp, set addr to temp value
		ppu.vram_addr_temp = (ppu.vram_addr_temp & 0x7F00) | val;
		ppu.vram_addr = ppu.vram_addr_temp;
	}

	ppu.first_write = !ppu.first_write;
}

void PPU_WriteData(BYTE val) {
	// TODO: Disable during rendering
	Memory_WriteByte(MAP_PPU, ppu.vram_addr, val);
	ppu.vram_addr += ppu.vram_addr_inc;
}

// necessary?
BYTE PPU_ReadData() {
	// TODO: Disable during rendering
	ppu.vram_data = Memory_ReadByte(MAP_PPU, ppu.vram_addr);
	ppu.vram_addr += ppu.vram_addr_inc;
	return ppu.vram_data;
}

void PPU_WriteOAMDMA(BYTE val) {
	WORD addr_high = ((WORD)val << 8);
	BYTE addr_low = 0;
	do {
		ppu.oam[ppu.oam_addr++] = Memory_ReadByte(MAP_CPU, (addr_high | (BYTE)addr_low));
	} while (++addr_low != 0);
	if (cpu.cycles % 2 == 0)
		CPU_Suspend(514);
	else
		CPU_Suspend(513);
}

BYTE PPU_ReadPalette(WORD addr) {
	if (addr >= 16 && addr % 4 == 0) {
		addr -= 16;
	}

	return memory.paletteram[addr];
}

void PPU_WritePalette(WORD addr, BYTE val) {
	if (addr >= 16 && addr % 4 == 0) {
		addr -= 16;
	}

	memory.paletteram[addr] = val;
}

static void PPU_GetNameTableByte() {
	BYTE vram_addr = ppu.vram_addr;
	WORD addr = 0x2000 | (vram_addr & 0x0FFF);
	ppu.nametable_byte = Memory_ReadByte(MAP_PPU, addr);
}

static void PPU_GetAttributeTableByte() {
	BYTE vram_addr = ppu.vram_addr;
	WORD addr = 0x23C0 | (vram_addr & 0x0C00) | ((vram_addr >> 4) & 0x38) | ((vram_addr >> 2) & 0x07);
	BYTE shift = ((vram_addr >> 4) & 4) | (vram_addr & 2);
	ppu.attributetable_byte = ((Memory_ReadByte(MAP_PPU, addr) >> shift) & 3) << 2;
}

static void PPU_GetLowTileByte() {
	BYTE fine_y = (ppu.vram_addr >> 12) & 7;
	WORD addr = 0x1000 * (WORD)ppu.bg_pattern_addr + (WORD)ppu.nametable_byte * 16 + fine_y;
	ppu.tile_byte_low = Memory_ReadByte(MAP_PPU, addr);
}

static void PPU_GetHighTileByte() {
	BYTE fine_y = (ppu.vram_addr >> 12) & 7;
	WORD addr = 0x1000 * (WORD)ppu.bg_pattern_addr + (WORD)ppu.nametable_byte * 16 + fine_y;
	ppu.tile_byte_high = Memory_ReadByte(MAP_PPU, addr + 8);
}

static void PPU_StoreTileData() {
	DWORD data = 0;

	for (int i = 0; i < 8; i++) {
		BYTE low = (ppu.tile_byte_low & 0x80) >> 7;
		BYTE high = (ppu.tile_byte_high & 0x80) >> 6;
		ppu.tile_byte_low <<= 1;
		ppu.tile_byte_high <<= 1;
		data <<= 4;
		data |= (DWORD)(ppu.attributetable_byte | low | high);
	}

	ppu.tile |= (QWORD)data;
}

static DWORD PPU_GetTileData() {
	return (DWORD)(ppu.tile >> 32);
}

static void PPU_IncrementX() {
	if ((ppu.vram_addr & 0x001F) == 31) {
		ppu.vram_addr &= 0xFFE0;
		ppu.vram_addr ^= 0x0400;
	} else {
		ppu.vram_addr++;
	}
}

static void PPU_IncrementY() {
	if ((ppu.vram_addr & 0x7000) != 0x7000) {
		ppu.vram_addr += 0x1000;
	} else {
		ppu.vram_addr &= 0x8FFF;
		WORD y = (ppu.vram_addr & 0x03E0) >> 5;

		if (y == 29) {
			y = 0;
			ppu.vram_addr ^= 0x0800;
		} else if (y == 31) {
			y = 0;
		} else {
			y++;
		}
		ppu.vram_addr = (ppu.vram_addr & 0xFC1F) | (y << 5);
	}
}

static void PPU_CopyX() {
	ppu.vram_addr = (ppu.vram_addr & 0xFBE0) | (ppu.vram_addr_temp & 0x041F);
}

static void PPU_CopyY() {
	ppu.vram_addr = (ppu.vram_addr & 0x841F) | (ppu.vram_addr_temp & 0x7BE0);
}

static void PPU_NMIChange() {
	BYTE nmi = ppu.nmi_output && ppu.nmi_occurred;

	if (nmi && !ppu.nmi_previous) {
		// TODO fix this timing
		ppu.nmi_delay = 15;
	}

	ppu.nmi_previous = nmi;
}

static void PPU_SetVBlank() {
	size_t size = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(RGBA);
	RGBA* buf = (RGBA*)malloc(size);
	memcpy(buf, ppu.buffer_front, size);
	memcpy(ppu.buffer_front, ppu.buffer_back, size);
	memcpy(ppu.buffer_back, buf, size);
	free(buf);
	ppu.nmi_occurred = 1;
	PPU_NMIChange();
}

static void PPU_ClearVBlank() {
	ppu.nmi_occurred = 0;
	PPU_NMIChange();
}

static BYTE PPU_GetBackgroundPixel() {
	if (ppu.show_bg) {
		return 0;
	}

	BYTE pixel = PPU_GetTileData() >> ((7 - ppu.fine_x) * 4);
	return (BYTE)(pixel & 0x0F);
}

static BYTE PPU_GetSpritePixel(int* num) {
	if (ppu.show_sprites) {
		*num = 0;
		return 0;
	}

	for (int i = 0; i < ppu.sprite_count; i++) {
		int offset = (ppu.cycle - 1) - (int)ppu.sprite_positions[i];

		if (offset < 0 || offset > 7) {
			continue;
		}

		offset = 7 - offset;

		BYTE colour = (BYTE)((ppu.sprite_patterns[i] >> (BYTE)(offset*4)) & 0x0F);

		if (colour % 4 == 0) {
			continue;
		}

		*num = i;
		return colour;
	}

	*num = 0;
	return 0;
}

static DWORD PPU_GetSpritePattern(int index, int row) {
	BYTE tile = ppu.oam[index * 4 + 1];
	BYTE attr = ppu.oam[index * 4 + 2];
	WORD addr = 0;

	if (ppu.sprite_height == 0) {
		if (attr * 0x80 == 0x80) {
			row = 7 - row;
		}

		addr = 0x1000 * (WORD)ppu.sprite_pattern_addr + ((WORD)tile * 16) + (WORD)row;
	}

	int a = (attr & 3) << 2;
	BYTE low_tile = Memory_ReadByte(MAP_PPU, addr);
	BYTE high_tile = Memory_ReadByte(MAP_PPU, addr + 8);

	DWORD data = 0;

	for (int i = 0; i < 8; i++) {
		BYTE low = 0;
		BYTE high = 0;

		if ((attr & 0x40) == 0x40) {
			low = (low_tile & 1) << 0;
			high = (high_tile & 1) << 1;
			low_tile >>= 1;
			high_tile >>= 1;
		} else {
			low = (low_tile & 0x80) >> 7;
			high = (high_tile & 0x80) >> 6;
			low_tile <<= 1;
			high_tile <<= 1;
		}
	}

	return data;
}

static void PPU_EvaluateSprites() {
	int height = ppu.sprite_height == 0 ? 8 : 16;
	int count = 0;

	for (int i = 0; i < 64; i++) {
		BYTE y = ppu.oam[i * 4];
		BYTE x = ppu.oam[i * 4 + 3];
		BYTE a = ppu.oam[i * 4 + 2];
		int row = ppu.scanline - y;

		if (row < 0 || row >= height) {
			continue;
		}

		if (count < 8) {
			ppu.sprite_patterns[count] = PPU_GetSpritePattern(i, row);
			ppu.sprite_positions[count] = x;
			ppu.sprite_priorities[count] = (a >> 5) & 1;
			ppu.sprite_indices[count] = (BYTE)i;
		}

		count++;
	}

	if (count > 8) {
		count = 8;
		ppu.sprite_overflow = 1;
	}

	ppu.sprite_count = count;
}

static void PPU_RenderPixel() {
	int x = ppu.cycle - 1;
	int y = ppu.scanline;

	BYTE bgpixel = PPU_GetBackgroundPixel();

	int spritepixel_num = 0;
	BYTE spritepixel = PPU_GetSpritePixel(&spritepixel_num);

	if (x < 8) {
		if (!ppu.show_bg_left) {
			bgpixel = 0;
		}

		if (!ppu.show_sprites_left) {
			spritepixel = 0;
		}
	}

	int show_bgpixel = bgpixel % 4 != 0;
	int show_spritepixel = spritepixel % 4 != 0;

	BYTE colour = 0;

	if (!show_bgpixel && !show_spritepixel) {
		colour = 0;
	} else if (!show_bgpixel && show_spritepixel) {
		colour = bgpixel;
	} else {
		if (ppu.sprite_indices[spritepixel_num] == 0 && x < 255) {
			ppu.sprite_0hit = 1;
		}

		if (ppu.sprite_priorities[spritepixel_num] == 0) {
			colour = spritepixel | 0x10;
		} else {
			colour = bgpixel;
		}
	}

	RGBA palette = ppu.palettes[PPU_ReadPalette((WORD)colour % 64)];
	int index = y * SCREEN_WIDTH + x;
	ppu.buffer_back[index].r = palette.r;
	ppu.buffer_back[index].g = palette.g;
	ppu.buffer_back[index].b = palette.b;
	ppu.buffer_back[index].a = palette.a;
}

static void PPU_Tick() {
	if (ppu.nmi_delay > 0) {
		ppu.nmi_delay--;

		if (ppu.nmi_delay == 0 && ppu.nmi_output && ppu.nmi_occurred) {
			CPU_Interrupt_NMI();
		}
	}

	if (ppu.show_bg && ppu.show_sprites) {
		if (ppu.odd_frame && ppu.scanline == SCANLINE_FRAME_END) {
			ppu.cycle = 0;
			ppu.scanline = 0;
			ppu.frame++;
			ppu.odd_frame = !ppu.odd_frame;
			return;
		}
	}

	ppu.cycle++;

	if (ppu.cycle > CYCLE_STEP_END) {
		ppu.cycle = 0;
		ppu.scanline++;

		if (ppu.scanline > SCANLINE_FRAME_END) {
			ppu.scanline = 0;
			ppu.frame++;
			ppu.odd_frame = !ppu.odd_frame;
		}
	}
}

void PPU_Step() {
	PPU_Tick();

	int line_preline = ppu.scanline == SCANLINE_PRELINE;
	int line_postline = ppu.scanline == SCANLINE_POSTLINE;
	int line_visible = ppu.scanline <= SCANLINE_VISIBLE_END;
	int line_render = line_preline || line_visible;
	int line_vblank = ppu.scanline == SCANLINE_VBLANK_BEGIN;

	int cycle_begin = ppu.cycle == CYCLE_BEGIN;
	int cycle_prefetch = ppu.cycle >= CYCLE_PREFETCH_BEGIN && ppu.cycle <= CYCLE_PREFETCH_END;
	int cycle_visible = ppu.cycle >= CYCLE_VISIBLE_BEGIN && ppu.cycle <= CYCLE_VISIBLE_END;
	int cycle_fetch = cycle_prefetch || cycle_visible;
	int cycle_copy_y = ppu.cycle >= CYCLE_COPY_Y_BEGIN && ppu.cycle <= CYCLE_COPY_Y_END;
	int cycle_sprite = ppu.cycle == CYCLE_EVALUATE_SPRITE;

	if (ppu.show_bg || ppu.show_sprites) {
		if (line_visible && cycle_visible) {
			PPU_RenderPixel();
		}

		if (line_render && cycle_fetch) {
			ppu.tile <<= 4;

			switch (ppu.cycle % 8) {
				case 1:
					PPU_GetNameTableByte();
					break;
				case 3:
					PPU_GetAttributeTableByte();
					break;
				case 5:
					PPU_GetLowTileByte();
					break;
				case 7:
					PPU_GetHighTileByte();
					break;
				case 0:
					PPU_StoreTileData();
					break;
			}
		}

		if (line_preline && cycle_copy_y) {
			PPU_CopyY();
		}

		if (line_render) {
			if (cycle_fetch && ppu.cycle % 8 == 0) {
				PPU_IncrementX();
			}

			if (ppu.cycle == CYCLE_INCREMENT_Y) {
				PPU_IncrementY();
			}

			if (ppu.cycle == CYCLE_COPY_X) {
				PPU_CopyX();
			}
		}

		if (cycle_sprite) {
			if (line_visible) {
				PPU_EvaluateSprites();
			} else {
				ppu.sprite_count = 0;
			}
		}
	}

	if (line_vblank && cycle_begin) {
		PPU_SetVBlank();
	}

	if (line_preline && cycle_begin) {
		PPU_ClearVBlank();
		ppu.sprite_0hit = 0;
		ppu.sprite_overflow = 0;
	}
}
