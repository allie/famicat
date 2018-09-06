#include "famicom/memory.h"
#include "famicom/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void MMC3_Init() {
	
}

static BYTE MMC3_Read(WORD addr) {

}

static void MMC3_Write(WORD addr, BYTE val) {

}

static void MMC3_Step() {

}

static void MMC3_Save() {

}

static void MMC3_Load() {

}

Mapper mmc3 = {
	MMC3_Init,
	MMC3_Read,
	MMC3_Write,
	MMC3_Step,
	MMC3_Save,
	MMC3_Load
};
