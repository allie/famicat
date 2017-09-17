#ifndef CONFIG_H
#define CONFIG_H

#include "../common.h"
#include <SDL2/SDL.h>

typedef struct {
	struct {
		SDL_Keysym a;
		SDL_Keysym b;
		SDL_Keysym start;
		SDL_Keysym select;
		SDL_Keysym reset;
		SDL_Keysym up;
		SDL_Keysym down;
		SDL_Keysym left;
		SDL_Keysym right;

		SDL_Keysym change_state_1;
		SDL_Keysym change_state_2;
		SDL_Keysym change_state_3;
		SDL_Keysym change_state_4;
		SDL_Keysym change_state_5;
		SDL_Keysym change_state_6;
		SDL_Keysym change_state_7;
		SDL_Keysym change_state_8;
		SDL_Keysym change_state_9;
		SDL_Keysym save_state;
		SDL_Keysym load_state;

		SDL_Keysym ui_scale_up;
		SDL_Keysym ui_scale_down;
		SDL_Keysym ui_tab_right;
		SDL_Keysym ui_tab_left;
		SDL_Keysym ui_scroll_down;
		SDL_Keysym ui_scroll_up;
		SDL_Keysym ui_page_flip;

		SDL_Keysym debugger_frame_advance;
		SDL_Keysym debugger_pause;
	} keys;

	Vector2 lcd_pos;
	Vector2 options_pos;
	Vector2 debugger_pos;
	Vector2 lcd_size;
	Vector2 options_size;
	Vector2 debugger_size;

	float volume;

	char recent_files[10][256];
} Config;

void Config_Load(const char*);
void Config_LoadDefaults();
void Config_Write(const char*);

#endif
