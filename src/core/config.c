#include "core/config.h"
#include "core/debugger.h"
#include "core/graphics.h"
#include "famicom/io.h"
#include "famicom/famicom.h"
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
		Keybinding* key = (Keybinding*)malloc(sizeof(Keybinding));
		SDL_Keysym sym = unpack_keysym(atoll(value));
		key->sym.scancode = sym.scancode;
		key->sym.mod = sym.mod;
		Dictionary_Add(config.keybindings, name, key);
	}

	else if (MATCH_SECTION("controller")) {
		Dictionary_Add(config.mappings, name, (void*)value);
	}

	return 1;
}

int Config_Init() {
	config.keybindings = Dictionary_New();
	config.mappings = Dictionary_New();

	return 1;
}

int Config_Load(const char* path) {
	if (ini_parse(path, ini_callback, &config) < 0) {
		printf("Could not parse configuration file. Attempting to generate a new one with default settings.\n");
		return 0;
	}

	return 1;
}

void Config_LoadDefaults() {
	config.window_pos.x = 100;
	config.window_pos.y = 100;
	config.window_scale = 1;
	config.volume = 1.0f;

	Keybinding* key = NULL;

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_R;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_reset", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_P;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_pause", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_F1;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_debug", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_PERIOD;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_scale_up", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_COMMA;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_scale_down", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_EQUALS;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_next_state", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_MINUS;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_previous_state", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_1;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_1", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_2;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_2", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_3;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_3", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_4;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_4", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_5;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_5", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_6;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_6", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_7;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_7", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_8;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_8", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_9;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_state_9", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_S;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_save_state", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_L;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_load_state", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_Z;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_a", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_X;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_b", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_RETURN;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_start", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_RSHIFT;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_select", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_UP;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_up", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_DOWN;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_down", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_LEFT;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_left", key);

	key = (Keybinding*)malloc(sizeof(Keybinding));
	key->sym.scancode = SDL_SCANCODE_RIGHT;
	key->sym.mod = KMOD_NONE;
	Dictionary_Add(config.keybindings, "key_1p_right", key);
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
	int key_count = 0;
	Entry** entries = Dictionary_GetAll(config.keybindings, &key_count);
	for (int i = 0; i < key_count; i++) {
		Entry* entry = entries[i];
		Keybinding* keybinding = (Keybinding*)(entry->value);
		QWORD sym = pack_keysym(keybinding->sym);
		fprintf(fp, "%s=%llu\n", entry->key, sym);
	}
	fprintf(fp, "\n");

	fprintf(fp, "[controller]\n");
	int mapping_count = 0;
	entries = Dictionary_GetAll(config.mappings, &mapping_count);
	for (int i = 0; i < mapping_count; i++) {
		Entry* entry = entries[i];
		char* mapping = (char*)(entry->value);
		fprintf(fp, "%s=%s\n", entry->key, mapping);
	}

	fclose(fp);
}

void Config_Destroy() {
	Dictionary_Destroy(config.keybindings);
	Dictionary_Destroy(config.mappings);
}
