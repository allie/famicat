#include "core/config.h"
#include <SDL2/SDL.h>

Config config;

void Config_load(const char* path) {

}

void Config_load_defaults() {
	config.keys.a.sym = SDLK_z;
	config.keys.a.mod = KMOD_NONE;

	config.keys.b.sym = SDLK_x;
	config.keys.b.mod = KMOD_NONE;

	config.keys.start.sym = SDLK_RETURN;
	config.keys.start.mod = KMOD_NONE;

	config.keys.select.sym = SDLK_RSHIFT;
	config.keys.select.mod = KMOD_NONE;

	config.keys.reset.sym = SDLK_r;
	config.keys.reset.mod = KMOD_LCTRL;

	config.keys.up.sym = SDLK_UP;
	config.keys.up.mod = KMOD_NONE;

	config.keys.down.sym = SDLK_DOWN;
	config.keys.down.mod = KMOD_NONE;

	config.keys.left.sym = SDLK_LEFT;
	config.keys.left.mod = KMOD_NONE;

	config.keys.right.sym = SDLK_UP;
	config.keys.right.mod = KMOD_NONE;

	config.keys.change_state_1.sym = SDLK_1;
	config.keys.change_state_1.mod = KMOD_NONE;

	config.keys.change_state_2.sym = SDLK_2;
	config.keys.change_state_2.mod = KMOD_NONE;

	config.keys.change_state_3.sym = SDLK_3;
	config.keys.change_state_3.mod = KMOD_NONE;

	config.keys.change_state_4.sym = SDLK_4;
	config.keys.change_state_4.mod = KMOD_NONE;

	config.keys.change_state_5.sym = SDLK_5;
	config.keys.change_state_5.mod = KMOD_NONE;

	config.keys.change_state_6.sym = SDLK_6;
	config.keys.change_state_6.mod = KMOD_NONE;

	config.keys.change_state_7.sym = SDLK_7;
	config.keys.change_state_7.mod = KMOD_NONE;

	config.keys.change_state_8.sym = SDLK_8;
	config.keys.change_state_8.mod = KMOD_NONE;

	config.keys.change_state_9.sym = SDLK_9;
	config.keys.change_state_9.mod = KMOD_NONE;

	config.keys.save_state.sym = SDLK_s;
	config.keys.save_state.mod = KMOD_LCTRL;

	config.keys.load_state.sym = SDLK_l;
	config.keys.load_state.mod = KMOD_LCTRL;

	config.keys.ui_scale_up.sym = SDLK_EQUALS;
	config.keys.ui_scale_up.mod = KMOD_NONE;

	config.keys.ui_scale_down.sym = SDLK_MINUS;
	config.keys.ui_scale_down.mod = KMOD_NONE;

	config.keys.ui_tab_right.sym = SDLK_RIGHT;
	config.keys.ui_tab_right.mod = KMOD_NONE;

	config.keys.ui_tab_left.sym = SDLK_LEFT;
	config.keys.ui_tab_left.mod = KMOD_NONE;

	config.keys.debugger_frame_advance.sym = SDLK_RIGHT;
	config.keys.debugger_frame_advance.mod = KMOD_LSHIFT;

	config.keys.debugger_pause.sym = SDLK_SPACE;
	config.keys.debugger_pause.mod = KMOD_NONE;
}

void Config_write(const char* path) {

}
