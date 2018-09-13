#include "famicom/famicom.h"
#include "famicom/cpu.h"
#include "famicom/apu.h"
#include "famicom/ppu.h"
#include "famicom/memory.h"
#include "time.h"

extern CPU cpu;
extern APU apu;
extern PPU ppu;
extern Memory memory;

static struct timespec diff(struct timespec start, struct timespec end) {
	struct timespec temp;

	if ((end.tv_nsec-start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec-start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}

	return temp;
}

static DWORD step(int* last_apu_tick, int* flip) {
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

	if (cpu.cycles - *last_apu_tick >= ((CLOCK_SPEED / 44100) + *flip)) {
		APU_Push();
		*last_apu_tick = cpu.cycles;
		*flip = (*flip + 1) & 0x1;
	}

	return cycles;
}

// Step all hardware for the given amount of time
void Famicom_Step(double dt) {
	int cycles = (int)(CLOCK_SPEED * dt);

	int last_apu_tick = 0;
	int flip = 0;

	while (cycles > 0) {
		cycles -= step(&last_apu_tick, &flip);
	}
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

// Main emulation thread
int Famicom_Emulate(void* args) {
	struct timespec current_time;
	struct timespec new_time;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &current_time);

	while (2) {
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &new_time);
		struct timespec dt = diff(current_time, new_time);

		Famicom_Step((double)dt.tv_nsec / 1000000000.0);

		memcpy(&current_time, &new_time, sizeof(struct timespec));
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
