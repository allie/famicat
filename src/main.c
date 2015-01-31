#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"

int main() {
	Cart_Load("test.nes");
	CPU_Reset();
	int i;
	for (i = 0; i < 10000; i++)
		CPU_Step();

	return 0;
}
