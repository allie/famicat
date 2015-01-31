#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"

int main() {
	Cart_Load("test.nes");
	CPU_Reset();
	//for (int i = 0; i < 100000000; i++)
	//	CPU_Step();
}
