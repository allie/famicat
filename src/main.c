#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"

int main() {
	Cart_Load("test.nes");
	CPU_Reset();
	for (int i = 0; i < 8991; i++)
		CPU_Step();

	Memory_Dump();

	return 0;
}
