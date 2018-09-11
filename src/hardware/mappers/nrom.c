#include "hardware/memory.h"
#include "hardware/cart.h"

extern Cart cart;
static int mirror;

static void NROM_Init() {
	mirror = cart.prgromsize < 2;
}

static BYTE NROM_Read(WORD addr) {
	if (addr < 0x2000) {
		return cart.chr[addr];
	}

	if (addr >= 0xC000) {
		return cart.prg[addr - 0x8000 - (mirror * 0x4000)];
	}

	if (addr >= 0x8000) {
		return cart.prg[addr - 0x8000];
	}

	if (addr >= 0x6000) {
		return cart.sram[addr - 0x6000];
	}

	return 0;
}

static void NROM_Write(WORD addr, BYTE val) {
	if (addr < 0x2000) {
		cart.chr[addr] = val;
	} else if (addr >= 0x6000) {
		cart.sram[addr - 0x6000] = val;
	} else {
		// TODO: invalid?
	}
}

static void NROM_Step() {
	/* Intentionally left empty */
}

static void NROM_Save() {

}

static void NROM_Load() {

}

Mapper nrom = {
	NROM_Init,
	NROM_Read,
	NROM_Write,
	NROM_Step,
	NROM_Save,
	NROM_Load
};
