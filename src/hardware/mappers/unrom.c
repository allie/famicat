#include "hardware/memory.h"
#include "hardware/cart.h"

extern Cart cart;
static int prgbank1;
static int prgbank2;

static void UNROM_Init() {
	
}

static BYTE UNROM_Read(WORD addr) {

}

static void UNROM_Write(WORD addr, BYTE val) {

}

static void UNROM_Step() {

}

static void UNROM_Save() {

}

static void UNROM_Load() {

}

Mapper unrom = {
	UNROM_Init,
	UNROM_Read,
	UNROM_Write,
	UNROM_Step,
	UNROM_Save,
	UNROM_Load
};
