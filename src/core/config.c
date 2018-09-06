#include "core/config.h"
#include "core/debugger.h"
#include "core/graphics.h"
#include "io/io.h"
#include "system/famicom.h"
#include "inih/ini.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#define MATCH_SECTION(s) strcmp(section, s) == 0
#define MATCH_KEY(k) strcmp(name, k) == 0

Config config;

static QWORD pack_keysym(SDL_Keysym sym) {
	return ((((QWORD)sym.mod) & 0x0000FFFF) << 32) | (((QWORD)sym.scancode) & 0x0000FFFF);
}

static SDL_Keysym unpack_keysym(QWORD val) {
	SDL_Keysym sym;
	sym.scancode = (DWORD)((val & 0x0000FFFF));
	sym.mod = (DWORD)((val & 0xFFFF0000) >> 32);
	return sym;
}

static int ini_callback(void* user, const char* section, const char* name, const char* value) {
	if (MATCH_SECTION("interface")) {
		if (MATCH_KEY("window_pos_x")) {
			config.window_pos.x = atoi(value);
		} else if (MATCH_KEY("window_pos_y")) {
			config.window_pos.y = atoi(value);
		} else if (MATCH_KEY("window_scale")) {
			config.window_scale = atoi(value);
		} else if (MATCH_KEY("volume")) {
			config.volume = atof(value);
		}
	}

	else if (MATCH_SECTION("keyboard")) {
		Keybinding* key = Dictionary_Get(config.keybindings, name);

		if (!key) {
			return 1;
		}

		SDL_Keysym sym = unpack_keysym(atoll(value));
		key->sym.scancode = sym.scancode;
		key->sym.mod = sym.mod;
	}

	else if (MATCH_SECTION("controller")) {
		// TODO
	}

	return 1;
}

int Config_Init() {
	config.keybindings = Dictionary_New();
	config.mappings = Dictionary_New();

	Keybinding* key = (Keybinding*)malloc(sizeof(Keybinding));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_LEFT;
	Dictionary_Set(config.keybindings, "key_1p_left", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_RIGHT;
	Dictionary_Set(config.keybindings, "key_1p_right", key);

	return 1;
}

int Config_Load(const char* path) {
	if (ini_parse(path, ini_callback, &config) < 0) {
		printf("Could not parse configuration file.\n");
		return 0;
	}

	return 1;
}

void Config_LoadDefaults() {
	config.window_pos.x = 100;
	config.window_pos.y = 100;
	config.window_scale = 1;
	config.volume = 1.0f;

	// config.keybindings[KEY_RESET].sym.scancode = SDL_SCANCODE_R;
	// config.keybindings[KEY_RESET].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_RESET].callback = IO_HandleInput;
	// config.keybindings[KEY_RESET].arg = IO_KEY_RESET;

	// config.keybindings[KEY_PAUSE].sym.scancode = SDL_SCANCODE_P;
	// config.keybindings[KEY_PAUSE].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_PAUSE].callback = IO_HandleInput;
	// config.keybindings[KEY_PAUSE].arg = 0;

	// config.keybindings[KEY_DEBUG].sym.scancode = SDL_SCANCODE_F1;
	// config.keybindings[KEY_DEBUG].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_DEBUG].callback = Debugger_HandleInput;
	// config.keybindings[KEY_DEBUG].arg = DEBUGGER_KEY_TOGGLE;

	// config.keybindings[KEY_SCALE_UP].sym.scancode = SDL_SCANCODE_PERIOD;
	// config.keybindings[KEY_SCALE_UP].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_SCALE_UP].callback = Graphics_Scale;
	// config.keybindings[KEY_SCALE_UP].arg = GRAPHICS_SCALE_UP;

	// config.keybindings[KEY_SCALE_DOWN].sym.scancode = SDL_SCANCODE_COMMA;
	// config.keybindings[KEY_SCALE_DOWN].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_SCALE_DOWN].callback = Graphics_Scale;
	// config.keybindings[KEY_SCALE_DOWN].arg = GRAPHICS_SCALE_DOWN;

	// config.keybindings[KEY_NEXT_STATE].sym.scancode = SDL_SCANCODE_EQUALS;
	// config.keybindings[KEY_NEXT_STATE].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_NEXT_STATE].callback = Famicom_NextState;
	// config.keybindings[KEY_NEXT_STATE].arg = 0;

	// config.keybindings[KEY_PREVIOUS_STATE].sym.scancode = SDL_SCANCODE_MINUS;
	// config.keybindings[KEY_PREVIOUS_STATE].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_PREVIOUS_STATE].callback = Famicom_PreviousState;
	// config.keybindings[KEY_PREVIOUS_STATE].arg = 0;

	// config.keybindings[KEY_STATE_1].sym.scancode = SDL_SCANCODE_1;
	// config.keybindings[KEY_STATE_1].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_1].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_1].arg = 1;

	// config.keybindings[KEY_STATE_2].sym.scancode = SDL_SCANCODE_2;
	// config.keybindings[KEY_STATE_2].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_2].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_2].arg = 2;

	// config.keybindings[KEY_STATE_3].sym.scancode = SDL_SCANCODE_3;
	// config.keybindings[KEY_STATE_3].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_3].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_3].arg = 3;

	// config.keybindings[KEY_STATE_4].sym.scancode = SDL_SCANCODE_4;
	// config.keybindings[KEY_STATE_4].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_4].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_4].arg = 4;

	// config.keybindings[KEY_STATE_5].sym.scancode = SDL_SCANCODE_5;
	// config.keybindings[KEY_STATE_5].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_5].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_5].arg = 5;

	// config.keybindings[KEY_STATE_6].sym.scancode = SDL_SCANCODE_6;
	// config.keybindings[KEY_STATE_6].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_6].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_6].arg = 6;

	// config.keybindings[KEY_STATE_7].sym.scancode = SDL_SCANCODE_7;
	// config.keybindings[KEY_STATE_7].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_7].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_7].arg = 7;

	// config.keybindings[KEY_STATE_8].sym.scancode = SDL_SCANCODE_8;
	// config.keybindings[KEY_STATE_8].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_8].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_8].arg = 8;

	// config.keybindings[KEY_STATE_9].sym.scancode = SDL_SCANCODE_9;
	// config.keybindings[KEY_STATE_9].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_STATE_9].callback = Famicom_SelectState;
	// config.keybindings[KEY_STATE_9].arg = 9;

	// config.keybindings[KEY_SAVE_STATE].sym.scancode = SDL_SCANCODE_S;
	// config.keybindings[KEY_SAVE_STATE].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_SAVE_STATE].callback = Famicom_SaveState;
	// config.keybindings[KEY_SAVE_STATE].arg = 0;

	// config.keybindings[KEY_LOAD_STATE].sym.scancode = SDL_SCANCODE_L;
	// config.keybindings[KEY_LOAD_STATE].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_LOAD_STATE].callback = Famicom_LoadState;
	// config.keybindings[KEY_LOAD_STATE].arg = 0;

	// config.keybindings[KEY_1P_A].sym.scancode = SDL_SCANCODE_Z;
	// config.keybindings[KEY_1P_A].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_A].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_A].arg = IO_KEY_A;

	// config.keybindings[KEY_1P_B].sym.scancode = SDL_SCANCODE_X;
	// config.keybindings[KEY_1P_B].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_B].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_B].arg = IO_KEY_B;

	// config.keybindings[KEY_1P_START].sym.scancode = SDL_SCANCODE_RETURN;
	// config.keybindings[KEY_1P_START].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_START].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_START].arg = IO_KEY_START;

	// config.keybindings[KEY_1P_SELECT].sym.scancode = SDL_SCANCODE_RSHIFT;
	// config.keybindings[KEY_1P_SELECT].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_SELECT].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_SELECT].arg = IO_KEY_SELECT;

	// config.keybindings[KEY_1P_UP].sym.scancode = SDL_SCANCODE_UP;
	// config.keybindings[KEY_1P_UP].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_UP].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_UP].arg = IO_KEY_UP;

	// config.keybindings[KEY_1P_DOWN].sym.scancode = SDL_SCANCODE_DOWN;
	// config.keybindings[KEY_1P_DOWN].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_DOWN].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_DOWN].arg = IO_KEY_DOWN;

	// config.keybindings[KEY_1P_LEFT].sym.scancode = SDL_SCANCODE_LEFT;
	// config.keybindings[KEY_1P_LEFT].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_LEFT].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_LEFT].arg = IO_KEY_LEFT;

	// config.keybindings[KEY_1P_RIGHT].sym.scancode = SDL_SCANCODE_RIGHT;
	// config.keybindings[KEY_1P_RIGHT].sym.mod = KMOD_NONE;
	// config.keybindings[KEY_1P_RIGHT].callback = IO_HandleInput;
	// config.keybindings[KEY_1P_RIGHT].arg = IO_KEY_RIGHT;

	config.mapping_count = 0;
	config.mappings = NULL;
}

void Config_Write(const char* path) {
	FILE* fp = fopen(path, "w");

	if (!fp) {
		printf("Error opening configuration file for writing.\n");
		exit(0);
	}

	fprintf(fp, "[interface]\n");
	fprintf(fp, "window_pos_x=%d\n", config.window_pos.x);
	fprintf(fp, "window_pos_y=%d\n", config.window_pos.y);
	fprintf(fp, "window_scale=%d\n", config.window_scale);
	fprintf(fp, "volume=%f\n", config.volume);
	fprintf(fp, "\n");

	fprintf(fp, "[keyboard]\n");
	fprintf(fp, "key_reset=%llu\n", pack_keysym(config.keybindings[KEY_RESET].sym));
	fprintf(fp, "key_pause=%llu\n", pack_keysym(config.keybindings[KEY_PAUSE].sym));
	fprintf(fp, "key_debug=%llu\n", pack_keysym(config.keybindings[KEY_DEBUG].sym));
	fprintf(fp, "key_scaleup=%llu\n", pack_keysym(config.keybindings[KEY_SCALE_UP].sym));
	fprintf(fp, "key_scaledn=%llu\n", pack_keysym(config.keybindings[KEY_SCALE_DOWN].sym));
	fprintf(fp, "key_next_state=%llu\n", pack_keysym(config.keybindings[KEY_NEXT_STATE].sym));
	fprintf(fp, "key_previous_state=%llu\n", pack_keysym(config.keybindings[KEY_PREVIOUS_STATE].sym));
	fprintf(fp, "key_state_1=%llu\n", pack_keysym(config.keybindings[KEY_STATE_1].sym));
	fprintf(fp, "key_state_2=%llu\n", pack_keysym(config.keybindings[KEY_STATE_2].sym));
	fprintf(fp, "key_state_3=%llu\n", pack_keysym(config.keybindings[KEY_STATE_3].sym));
	fprintf(fp, "key_state_4=%llu\n", pack_keysym(config.keybindings[KEY_STATE_4].sym));
	fprintf(fp, "key_state_5=%llu\n", pack_keysym(config.keybindings[KEY_STATE_5].sym));
	fprintf(fp, "key_state_6=%llu\n", pack_keysym(config.keybindings[KEY_STATE_6].sym));
	fprintf(fp, "key_state_7=%llu\n", pack_keysym(config.keybindings[KEY_STATE_7].sym));
	fprintf(fp, "key_state_8=%llu\n", pack_keysym(config.keybindings[KEY_STATE_8].sym));
	fprintf(fp, "key_state_9=%llu\n", pack_keysym(config.keybindings[KEY_STATE_9].sym));
	fprintf(fp, "key_save_state=%llu\n", pack_keysym(config.keybindings[KEY_SAVE_STATE].sym));
	fprintf(fp, "key_load_state=%llu\n", pack_keysym(config.keybindings[KEY_LOAD_STATE].sym));
	fprintf(fp, "key_1p_start=%llu\n", pack_keysym(config.keybindings[KEY_1P_START].sym));
	fprintf(fp, "key_1p_select=%llu\n", pack_keysym(config.keybindings[KEY_1P_SELECT].sym));
	fprintf(fp, "key_1p_a=%llu\n", pack_keysym(config.keybindings[KEY_1P_A].sym));
	fprintf(fp, "key_1p_b=%llu\n", pack_keysym(config.keybindings[KEY_1P_B].sym));
	fprintf(fp, "key_1p_up=%llu\n", pack_keysym(config.keybindings[KEY_1P_UP].sym));
	fprintf(fp, "key_1p_down=%llu\n", pack_keysym(config.keybindings[KEY_1P_DOWN].sym));
	fprintf(fp, "key_1p_left=%llu\n", pack_keysym(config.keybindings[KEY_1P_LEFT].sym));
	fprintf(fp, "key_1p_right=%llu\n", pack_keysym(config.keybindings[KEY_1P_RIGHT].sym));
	fprintf(fp, "\n");

	fprintf(fp, "[controller]\n");

	fclose(fp);
}

void Config_Destroy() {
	Dictionary_Destroy(config.keybindings);
	Dictionary_Destroy(config.mappings);
}
