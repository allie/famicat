#include <SDL2/SDL.h>
#include <math.h>
#include "cpu/cpu.h"
#include "apu/apu.h"
#include "core/graphics.h"
#include "core/audio.h"
#include "core/config.h"
#include "core/input.h"
#include "core/debugger.h"
#include "memory/memory.h"
#include "memory/cart.h"

extern CPU cpu;
extern APU apu;

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s", SDL_GetError());
		return 0;
	}

	if (!Graphics_Init(256, 240))
		return 0;

	if (!Audio_Init())
		return 0;

	Debugger_Init();
	Config_LoadDefaults();

	if (argc > 1) {
		Cart_Load(argv[1]);
		CPU_Reset();
		APU_Init();
	} else {
		printf("NOTE: No ROM file supplied.\n");
	}

	int running = 1;
	int odd;
	int lastaputick;
	int flip;

	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = 0;
			}

			else if (e.type == SDL_KEYDOWN) {
				Input_Handle(e.key.keysym);
			}
		}

		Graphics_Clear();

		while (cpu.cycles < (CLOCK_SPEED / 60)) {
			int cycles = CPU_Step();

			for (int i = 0; i < cycles; i++)
				APU_Step();


			if (cpu.cycles - apu.last_frame_tick >= (CLOCK_SPEED / 240)) {
				APU_FrameSequencerStep();
				apu.last_frame_tick = cpu.cycles;
			}

			if (cpu.cycles - lastaputick >= ((CLOCK_SPEED / 44100) + flip)) {
				APU_Push();
				lastaputick = cpu.cycles;
				flip = (flip + 1) & 0x1;
			}
		}

		cpu.cycles -= (CLOCK_SPEED / 60);

		Debugger_Draw();

		Graphics_Present();

		SDL_Delay(17);
	}

	Graphics_Destroy();
	SDL_Quit();

	return 0;
}
