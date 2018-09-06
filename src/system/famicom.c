#include "system/famicom.h"
#include "cpu/cpu.h"
#include "apu/apu.h"

extern CPU cpu;
extern APU apu;

static void Famicom_Step() {
	int last_apu_tick;
	int flip;

	while (cpu.cycles < (CLOCK_SPEED / 60)) {
		int cycles = CPU_Step();

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
}

void Famicom_Reset() {
	CPU_Reset();
	APU_Init();
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
