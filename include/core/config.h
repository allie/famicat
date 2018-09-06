#ifndef CONFIG_H
#define CONFIG_H

#include "../common.h"
#include <SDL2/SDL.h>

#define NUM_KEYS 26

enum {
	KEY_RESET = 0,
	KEY_PAUSE,
	KEY_DEBUG,
	KEY_SCALE_UP,
	KEY_SCALE_DOWN,
	KEY_NEXT_STATE,
	KEY_PREVIOUS_STATE,
	KEY_STATE_1,
	KEY_STATE_2,
	KEY_STATE_3,
	KEY_STATE_4,
	KEY_STATE_5,
	KEY_STATE_6,
	KEY_STATE_7,
	KEY_STATE_8,
	KEY_STATE_9,
	KEY_SAVE_STATE,
	KEY_LOAD_STATE,
	KEY_1P_START,
	KEY_1P_SELECT,
	KEY_1P_A,
	KEY_1P_B,
	KEY_1P_UP,
	KEY_1P_DOWN,
	KEY_1P_LEFT,
	KEY_1P_RIGHT
} famicat_keys;

typedef struct {
	SDL_Keysym sym;
	int arg;
	void (*callback)(int);
} Keybinding;

typedef struct {
	Keybinding keybindings[NUM_KEYS];

	char** mappings;
	int mapping_count;

	Vector2 window_pos;
	int window_scale;

	float volume;

	char recent_files[10][256];
} Config;

int Config_Load(const char*);
void Config_LoadDefaults();
void Config_Write(const char*);
void Config_Destroy();

#endif
