#include "famicom/memory.h"
#include "famicom/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void MMC5_Init() {
	
}

static BYTE MMC5_Read(WORD addr) {

}

static void MMC5_Write(WORD addr, BYTE val) {

}

static void MMC5_Step() {

}

static void MMC5_Save() {

}

static void MMC5_Load() {

}

Mapper mmc5 = {
	MMC5_Init,
	MMC5_Read,
	MMC5_Write,
	MMC5_Step,
	MMC5_Save,
	MMC5_Load
};
