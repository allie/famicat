#include <SDL2/SDL.h>
#include <math.h>
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"
#include "graphics/core.h"
#include "utils/timer.h"
#include "graphics/sprite.h"
#include "audio/core.h"

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

		for (int i = 0; i < 8991; i++)
			CPU_Step();

		Memory_Dump();
	} else {
		printf("NOTE: No ROM file supplied.\n");
	}

	SDL_Texture* tex = Graphics_LoadPNG("splash", "res/splash.png");
	int splash = (tex) ? Sprite_Add(tex, 512, 480) : -1;

	double pi2 = 3.14159 * 2;
	double current = 0.0;

	for (int i = 0; i < 88200; i++) {
		Audio_AddSample((sin(current) - 0.5) * 0xFFFF);
		current += (pi2 / 256);
	}

	while (1) {
		Timer_UpdateAll();

		SDL_Event e;
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				break;

		Sprite_UpdateAll(Timer_GetDelta());
		Graphics_Clear();

		Sprite_Render(splash, 0, 0);

		Graphics_Present();
	}

	Graphics_Destroy();
	SDL_Quit();

	return 0;
}
