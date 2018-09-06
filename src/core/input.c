#include "core/input.h"
#include "core/config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern Config config;

int Input_Init(const char* db_path) {
	SDL_GameControllerAddMappingsFromFile(db_path);

	for (int i = 0; i < config.mapping_count; i++) {
		SDL_GameControllerAddMapping(config.mappings[i]);
	}

	return 1;
}

void Input_Handle(SDL_Keysym sym) {
	for (int i = 0; i < NUM_KEYS; i++) {
		if (sym.scancode == config.keybindings[i].sym.scancode) {
			config.keybindings[i].callback(config.keybindings[i].arg);
		}
	}
}
