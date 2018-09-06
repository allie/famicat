#include "famicom/memory.h"
#include "famicom/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void MMC4_Init() {
	
}

static BYTE MMC4_Read(WORD addr) {

}

static void MMC4_Write(WORD addr, BYTE val) {

}

static void MMC4_Step() {

}

static void MMC4_Save() {

}

static void MMC4_Load() {

}

Mapper mmc4 = {
	MMC4_Init,
	MMC4_Read,
	MMC4_Write,
	MMC4_Step,
	MMC4_Save,
	MMC4_Load
};
