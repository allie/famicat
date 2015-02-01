#include "cart.h"
#include "../memory/memory.h"
#include <string.h>

Cart cart;
extern Memory memory;

void Cart_Load(const char* file) {
	Memory_Reset();

	FILE* fp = fopen(file, "rb");

	fseek(fp, 0L, SEEK_END);
	long int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	BYTE* buf = calloc(size, sizeof(BYTE));
	fread(buf, sizeof(BYTE), size / sizeof(BYTE), fp);
	fclose(fp);

	// Check for NES[EOF]
	if (buf[0] != 'N' || buf[1] != 'E' || buf[2] != 'S' || buf[3] != 0x1A) {
		printf("Invalid NES ROM file.\n");
		exit(0);
	}

	cart.prgromsize = buf[4];
	cart.chrromsize = buf[5];
	cart.prgramsize = buf[8];

	// Flags 6
	if (!(buf[6] & 0x01) && !((buf[6] & 0x08) >> 3))
		cart.vramarr = V_MIRROR;
	else if (!(buf[6] & 0x01) && ((buf[6] & 0x08) >> 3))
		cart.vramarr = H_MIRROR;
	else if (buf[6] & 0x01)
		cart.vramarr = Q_SCREEN;

	cart.hassram = (buf[6] & 0x02) >> 1;
	cart.hastrainer = (buf[6] & 0x04) >> 2;

	// Flags 7
	cart.vsunisystem = buf[7] & 0x01;
	cart.playchoice = (buf[7] & 0x02) >> 1;
	cart.nes2 = ((buf[7] & 0x0C) >> 2) == 2;

	// Mapper number (flags 6 and 7)
	cart.mapper = ((buf[6] & 0xF0) >> 4) | (buf[7] & 0xF0);

	// Flags 9
	cart.tvmode = buf[9] & 0x01;

	// Flags 10 is ignored

	// Load trainer data into memory if necessary
	if (cart.hastrainer) {
		cart.trainer = calloc(512, sizeof(BYTE));
		memcpy(cart.trainer, buf + 16, 512 * sizeof(BYTE));
	}

	// Load PRG ROM into memory
	cart.prg = calloc(0x4000 * cart.prgromsize, sizeof(BYTE));
	memcpy(
		cart.prg,
		buf + 16 + (cart.hastrainer * 512),
		0x4000 * cart.prgromsize * sizeof(BYTE)
	);

	// Load CHR ROM into memory if necessary
	if (cart.chrromsize) {
		cart.chr = calloc(0x2000 * cart.chrromsize, sizeof(BYTE));
		memcpy(
			cart.chr,
			buf + 16 + (cart.hastrainer * 512) + (0x4000 * cart.prgromsize),
			0x2000 * cart.chrromsize * sizeof(BYTE)
		);
	}

	free(buf);

/*
	printf("PRG ROM SIZE: %d\n", cart.prgromsize);
	printf("CHR ROM SIZE: %d\n", cart.chrromsize);
	printf("PRG RAM SIZE: %d\n", cart.prgramsize);
	printf("VRAM ARRANGEMENT: %s\n",
		(cart.vramarr == V_MIRROR) ? "VERTICAL MIRROR" :
		((cart.vramarr == H_MIRROR) ? "HORIZONTAL MIRROR" : "QUAD SCREEN"));
	printf("USES SRAM: %s\n", (cart.hassram) ? "YES" : "NO");
	printf("HAS TRAINER: %s\n", (cart.hastrainer) ? "YES" : "NO");
	printf("MAPPER NUMBER: %d\n", cart.mapper);
	printf("USES VSUNISYSTEM: %s\n", (cart.vsunisystem) ? "YES" : "NO");
	printf("USES PLAYCHOICE: %s\n", (cart.playchoice) ? "YES" : "NO");
	printf("NES 2.0: %s\n", (cart.nes2) ? "YES" : "NO");
	printf("TV MODE: %s\n", (cart.tvmode) ? "PAL" : "NTSC");
*/
}

void Cart_Unload() {

}
