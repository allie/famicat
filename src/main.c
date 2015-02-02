#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Please supply a ROM file.");
		return 0;
	} else {
		Cart_Load(argv[1]);
	}

	CPU_Reset();

	for (int i = 0; i < 8991; i++)
		CPU_Step();

	Memory_Dump();

	return 0;
}
