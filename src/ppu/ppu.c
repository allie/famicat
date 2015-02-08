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
	oamaddr = val;
}

void PPU_WriteOAMData(BYTE val) {
	oam[oamaddr++] = val;
}

// necessary?
BYTE PPU_ReadOAMData() {
	return oam[oamaddr];
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
	WORD addr_high = ((WORD)val << 8);
	for (BYTE addr_low = 0; addr_low < 256; addr_low++) {
		oam[oamaddr++] = Memory_ReadByte(MAP_CPU, (addr_high | addr_low));
	}
	if (cpu.cycles % 2 == 0)
		CPU_Suspend(514);
	else
		CPU_Suspend(513);
}
