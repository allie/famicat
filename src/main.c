#include <SDL2/SDL.h>
#include <math.h>
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"
#include "graphics/core.h"
#include "utils/timer.h"
#include "graphics/sprite.h"
#include "audio/core.h"
#include "apu/apu.h"

extern CPU cpu;
extern APU apu;

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s", SDL_GetError());
		return 0;
	}

	if (!Graphics_Init(512, 480))
		return 0;

	if (!Timer_Init())
		return 0;

	if (!Audio_Init())
		return 0;

	if (argc > 1) {
		Cart_Load(argv[1]);
		CPU_Reset();
		APU_Init();
	} else {
		printf("NOTE: No ROM file supplied.\n");
	}

	SDL_Texture* tex = Graphics_LoadPNG("splash", "res/splash.png");
	int splash = (tex) ? Sprite_Add(tex, 512, 480) : -1;

	int odd;
	int lastaputick;
	int flip;

	while (1) {
		Timer_UpdateAll();

		SDL_Event e;
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				break;

		Sprite_UpdateAll(Timer_GetDelta());
		Graphics_Clear();

		Sprite_Render(splash, 0, 0);

		while (cpu.cycles < (CLOCK_SPEED / 60)) {
			int cycles = CPU_Step();

			for (int i = 0; i < cycles; i++) {
				APU_FrameSequencerStep();
				APU_Step();
			}
		}

		cpu.cycles -= (CLOCK_SPEED / 60);

		Graphics_Present();

		SDL_Delay(17);
	}

	Graphics_Destroy();
	SDL_Quit();

	return 0;
}
