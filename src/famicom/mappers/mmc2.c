#include "famicom/memory.h"
#include "famicom/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void MMC2_Init() {

}

static BYTE MMC2_Read(WORD addr) {

}

static void MMC2_Write(WORD addr, BYTE val) {

}

static void MMC2_Step() {

}

static void MMC2_Save() {

}

static void MMC2_Load() {

}

Mapper mmc2 = {
	MMC2_Init,
	MMC2_Read,
	MMC2_Write,
	MMC2_Step,
	MMC2_Save,
	MMC2_Load
};
