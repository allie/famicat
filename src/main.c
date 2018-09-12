#include <SDL2/SDL.h>
#include <math.h>
#include "core/graphics.h"
#include "core/audio.h"
#include "core/config.h"
#include "core/input.h"
#include "core/debugger.h"
#include "famicom/memory.h"
#include "famicom/cart.h"
#include "famicom/cpu.h"
#include "famicom/apu.h"
#include "famicom/ppu.h"
#include "famicom/famicom.h"

extern CPU cpu;
extern APU apu;
extern PPU ppu;

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 0;
	}

	if (!Config_Init()) {
		return 0;
	}

	if (!Config_Load("famicat.ini")) {
		Config_LoadDefaults();
		Config_Write("famicat.ini");
	}

	if (!Graphics_Init()) {
		return 0;
	}

	if (!Audio_Init()) {
		return 0;
	}

	if (!Input_Init("gamecontrollerdb.txt")) {
		return 0;
	}

	if (!Debugger_Init()) {
		return 0;
	};

	if (argc > 1) {
		Cart_Load(argv[1]);
		Famicom_PowerOn();
	} else {
		printf("NOTE: No ROM file supplied.\n");
	}

	SDL_Thread* system_thread = SDL_CreateThread(Famicom_Emulate, "system", (void*)NULL);
	if (system_thread == NULL) {
		printf("SDL_Thread error: %s\n", SDL_GetError());
		return 0;
	}

	int running = 1;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = 0;
			}

			else if (e.type == SDL_KEYDOWN) {
				Input_Handle(e.key.keysym, SDL_CONTROLLER_BUTTON_INVALID);
			}
		}

		Graphics_Clear();
		Graphics_RenderBuffer(ppu.buffer_back, SCREEN_WIDTH, SCREEN_HEIGHT);
		Graphics_RenderBuffer(ppu.buffer_front, SCREEN_WIDTH, SCREEN_HEIGHT);
		Debugger_Draw();
		Graphics_Present();
	}

	Graphics_Destroy();
	Audio_Destroy();
	Debugger_Destroy();
	Config_Destroy();

	SDL_Quit();

	return 0;
}
