#include "famicom/memory.h"
#include "famicom/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void MMC1_Init() {
	prgbank1 = 0;
	prgbank2 = cart.prgromsize - 1;
}

static BYTE MMC1_Read(WORD addr) {
	if (addr < 0x2000) {
		return cart.chr[addr];
	}

	if (addr >= 0xC000) {
		return cart.prg[(prgbank2 * 0x4000) + (addr - 0xC000)];
	}

	if (addr >= 0x8000) {
		return cart.prg[(prgbank1 * 0x4000) + (addr - 0x8000)];
	}

	if (addr >= 0x6000) {
		return cart.sram[addr - 0x6000];
	}
}

static void MMC1_Write(WORD addr, BYTE val) {
	if (addr < 0x2000) {
		cart.chr[addr] = val;
	}

	if (addr >= 0x8000) {
		prgbank1 = val % cart.prgromsize; // TODO: ?
	}

	if (addr >= 0x6000) {
		cart.sram[addr - 0x6000] = val;
	}
}

static void MMC1_Step() {
	/* Intentionally left empty */
}

static void MMC1_Save() {

}

static void MMC1_Load() {

}

Mapper mmc1 = {
	MMC1_Init,
	MMC1_Read,
	MMC1_Write,
	MMC1_Step,
	MMC1_Save,
	MMC1_Load
};
