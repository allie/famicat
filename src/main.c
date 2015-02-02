#include <SDL2/SDL.h>
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "cart/cart.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Please supply a ROM file.");
		return 0;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s", SDL_GetError());
		return 0;
	}

	if (!Graphics_Init())
		return 0;

	if (!Timer_Init())
		return 0;
	
	Cart_Load(argv[1]);

	CPU_Reset();

	for (int i = 0; i < 8991; i++)
		CPU_Step();

	Memory_Dump();

	while (1) {
		Timer_UpdateAll();

		SDL_Event e;
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				break;

		Sprite_UpdateAll(Timer_GetDelta());
		Graphics_Clear();
		Graphics_Present();
	}

	Graphics_Destroy();
	SDL_Quit();

	return 0;
}
