#ifndef CONFIG_H
#define CONFIG_H

#include "../common.h"
#include "utils/dictionary.h"
#include <SDL2/SDL.h>

typedef struct {
	SDL_Keysym sym;
} Keybinding;

typedef struct {
	SDL_GameControllerButton button;
	int arg;
	void (*callback)(int);
} ControllerBinding;

typedef struct {
	Dictionary* mappings;
	Dictionary* keybindings;
	Dictionary* controllerbindings;

	Vector2 window_pos;
	int window_scale;

	float volume;

	char recent_files[10][256];
} Config;

int Config_Init();
int Config_Load(const char*);
void Config_LoadDefaults();
void Config_Write(const char*);
void Config_Destroy();

#endif
