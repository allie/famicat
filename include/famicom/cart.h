#ifndef CART_H
#define CART_H

#include "../common.h"

#define V_MIRROR 0
#define H_MIRROR 1
#define Q_SCREEN 2

#define NTSC_MODE 0
#define PAL_MODE 1

typedef struct {
	BYTE prgromsize;
	BYTE chrromsize;
	BYTE prgramsize;
	BYTE vramarr;
	BYTE hassram;
	BYTE hastrainer;
	BYTE mapper;
	BYTE vsunisystem;
	BYTE playchoice;
	BYTE nes2;
	BYTE tvmode;
	BYTE* trainer;
	BYTE* prg;
	BYTE* chr;
	BYTE* sram;
} Cart;

void Cart_Load(const char* file);
void Cart_Unload();

#endif
