#include "core/input.h"
#include "core/config.h"
#include "core/debugger.h"
#include "core/graphics.h"
#include "utils/dictionary.h"
#include "hardware/famicom.h"
#include "hardware/io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern Config config;

Dictionary* keys;
Input buttons[SDL_CONTROLLER_BUTTON_MAX];

int Input_Init(const char* db_path) {
	SDL_GameControllerAddMappingsFromFile(db_path);

	int mapping_count = 0;
	Entry** entries = Dictionary_GetAll(config.mappings, &mapping_count);

	for (int i = 0; i < mapping_count; i++) {
		Entry* entry = entries[i];
		char* mapping = (char*)(entry->value);
		SDL_GameControllerAddMapping(mapping);
	}

	keys = Dictionary_New();

	Input* key = NULL;

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_RESET;
	Dictionary_Add(keys, "key_reset", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_PAUSE;
	Dictionary_Add(keys, "key_pause", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Debugger_HandleInput;
	key->arg = DEBUGGER_KEY_TOGGLE;
	Dictionary_Add(keys, "key_debug", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Graphics_Scale;
	key->arg = GRAPHICS_SCALE_UP;
	Dictionary_Add(keys, "key_scale_up", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Graphics_Scale;
	key->arg = GRAPHICS_SCALE_DOWN;
	Dictionary_Add(keys, "key_scale_down", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_NextState;
	key->arg = 0;
	Dictionary_Add(keys, "key_next_state", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_PreviousState;
	key->arg = 0;
	Dictionary_Add(keys, "key_previous_state", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 1;
	Dictionary_Add(keys, "key_state_1", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 2;
	Dictionary_Add(keys, "key_state_2", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 3;
	Dictionary_Add(keys, "key_state_3", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 4;
	Dictionary_Add(keys, "key_state_4", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 5;
	Dictionary_Add(keys, "key_state_5", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 6;
	Dictionary_Add(keys, "key_state_6", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 7;
	Dictionary_Add(keys, "key_state_7", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 8;
	Dictionary_Add(keys, "key_state_8", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SelectState;
	key->arg = 9;
	Dictionary_Add(keys, "key_state_9", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_SaveState;
	key->arg = 0;
	Dictionary_Add(keys, "key_save_state", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = Famicom_LoadState;
	key->arg = 0;
	Dictionary_Add(keys, "key_load_state", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_A;
	Dictionary_Add(keys, "key_1p_a", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_B;
	Dictionary_Add(keys, "key_1p_b", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_START;
	Dictionary_Add(keys, "key_1p_start", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_SELECT;
	Dictionary_Add(keys, "key_1p_select", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_UP;
	Dictionary_Add(keys, "key_1p_up", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_DOWN;
	Dictionary_Add(keys, "key_1p_down", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_LEFT;
	Dictionary_Add(keys, "key_1p_left", key);

	key = (Input*)malloc(sizeof(Input));
	key->callback = IO_HandleInput;
	key->arg = IO_KEY_RIGHT;
	Dictionary_Add(keys, "key_1p_right", key);

	buttons[SDL_CONTROLLER_BUTTON_A].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_A].arg = IO_KEY_A;

	buttons[SDL_CONTROLLER_BUTTON_B].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_B].arg = IO_KEY_B;

	buttons[SDL_CONTROLLER_BUTTON_START].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_START].arg = IO_KEY_START;

	buttons[SDL_CONTROLLER_BUTTON_BACK].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_BACK].arg = IO_KEY_SELECT;

	buttons[SDL_CONTROLLER_BUTTON_DPAD_UP].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_DPAD_UP].arg = IO_KEY_UP;

	buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN].arg = IO_KEY_DOWN;

	buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT].arg = IO_KEY_LEFT;

	buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT].callback = IO_HandleInput;
	buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT].arg = IO_KEY_RIGHT;

	return 1;
}

void Input_Handle(SDL_Keysym sym, SDL_GameControllerButton pressed) {
	if (pressed == SDL_CONTROLLER_BUTTON_INVALID) {
		int key_count = 0;
		Entry** entries = Dictionary_GetAll(config.keybindings, &key_count);

		for (int i = 0; i < key_count; i++) {
			Entry* entry = entries[i];
			Keybinding* binding = (Keybinding*)(entry->value);

			if (sym.scancode == binding->sym.scancode) {
				Input* key = Dictionary_Get(keys, (const char*)(entry->key));

				if (key) {
					key->callback(key->arg);
				}
			}
		}
	} else {
		if (pressed >= SDL_CONTROLLER_BUTTON_MAX) {
			return;
		}

		Input button = buttons[pressed];

		if (button.callback) {
			button.callback(button.arg);
		}
	}
}
