#include "ppu.h"

PPU ppu;

void PPU_Init() {
	if (ppu.oam != NULL)
		free(ppu.oam);

	ppu.oam = (BYTE*)calloc(0x100, sizeof(BYTE));

	ppu.controller = 0;
	ppu.mask = 0;
	ppu.status = 0xA0;
	ppu.scrolllatch = 0;
	ppu.scroll = 0;
	ppu.addrlatch = 0;
	ppu.addr = 0;
	ppu.data = 0;
	ppu.oddframe = 0;
}

void PPU_Reset() {
	// TODO
}

void PPU_WriteController(BYTE val) {

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

