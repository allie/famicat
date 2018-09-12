#include "famicom/famicom.h"
#include "famicom/cpu.h"
#include "famicom/apu.h"
#include "famicom/ppu.h"
#include "famicom/memory.h"

extern CPU cpu;
extern APU apu;
extern PPU ppu;
extern Memory memory;

static void Famicom_Step() {
	int last_apu_tick;
	int flip;

	while (cpu.cycles < (CLOCK_SPEED / 60)) {
		int cycles = CPU_Step();

		for (int i = 0; i < cycles * 3; i++) {
			PPU_Step();
			memory.mapper.step();
		}

		for (int i = 0; i < cycles; i++)
			APU_Step();

		if (cpu.cycles - apu.last_frame_tick >= (CLOCK_SPEED / 240)) {
			APU_FrameSequencerStep();
			apu.last_frame_tick = cpu.cycles;
		}

		if (cpu.cycles - last_apu_tick >= ((CLOCK_SPEED / 44100) + flip)) {
			APU_Push();
			last_apu_tick = cpu.cycles;
			flip = (flip + 1) & 0x1;
		}
	}

	cpu.cycles -= (CLOCK_SPEED / 60);

#ifdef DEBUG_MODE
	Memory_Dump();
#endif
}

void Famicom_PowerOn() {
	printf("Powering on Famicom...\n");
	CPU_Init();
	APU_Init();
	PPU_Init();
}

void Famicom_Reset() {
	printf("Resetting Famicom...\n");
	CPU_Reset();
	APU_Reset();
	PPU_Reset();
}

int Famicom_Emulate(void* args) {
	while (2) {
		Famicom_Step();
	}

	return 0;
}

void Famicom_LoadState() {

}

void Famicom_SaveState() {

}

void Famicom_SelectState(int state) {

}

void Famicom_NextState() {

}

void Famicom_PreviousState() {

}
