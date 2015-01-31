#include "cart.h"
#include "../memory/memory.h"

extern Memory memory;

void Cart_Load(const char* file) {
	Memory_Reset();

	FILE* fp = fopen(file, "rb");

	fseek(fp, 0L, SEEK_END);
	long int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	fread(memory.ram, sizeof(BYTE), size / sizeof(BYTE), fp);

	fclose(fp);
}
