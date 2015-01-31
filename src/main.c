#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"

int main() {
	CPU_Reset();
	Cart_Load("test.bin");
	for (int i = 0; i < 10; i++)
		CPU_Step();
}
