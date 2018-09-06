#include "famicom/memory.h"
#include "famicom/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void CNROM_Init() {

}

static BYTE CNROM_Read(WORD addr) {

}

static void CNROM_Write(WORD addr, BYTE val) {

}

static void CNROM_Step() {

}

static void CNROM_Save() {

}

static void CNROM_Load() {

}

Mapper cnrom = {
	CNROM_Init,
	CNROM_Read,
	CNROM_Write,
	CNROM_Step,
	CNROM_Save,
	CNROM_Load
};
