#include "core/config.h"
#include "core/debugger.h"
#include "core/graphics.h"
#include "io/io.h"
#include <string.h>
#include <SDL2/SDL.h>

Config config;

void Config_Load(const char* path) {

}

void Config_LoadDefaults() {
	config.binding_count = 12;
	config.bindings = (Keybinding*)malloc(config.binding_count * sizeof(Keybinding));

	strcpy(config.bindings[0].name, "a");
	config.bindings[0].sym.sym = SDLK_z;
	config.bindings[0].sym.mod = KMOD_NONE;
	config.bindings[0].callback = IO_HandleInput;
	config.bindings[0].arg = IO_KEY_A;

	strcpy(config.bindings[1].name, "b");
	config.bindings[1].sym.sym = SDLK_x;
	config.bindings[1].sym.mod = KMOD_NONE;
	config.bindings[1].callback = IO_HandleInput;
	config.bindings[1].arg = IO_KEY_B;

	strcpy(config.bindings[2].name, "start");
	config.bindings[2].sym.sym = SDLK_RETURN;
	config.bindings[2].sym.mod = KMOD_NONE;
	config.bindings[2].callback = IO_HandleInput;
	config.bindings[2].arg = IO_KEY_START;

	strcpy(config.bindings[3].name, "select");
	config.bindings[3].sym.sym = SDLK_RSHIFT;
	config.bindings[3].sym.mod = KMOD_NONE;
	config.bindings[3].callback = IO_HandleInput;
	config.bindings[3].arg = IO_KEY_SELECT;

	strcpy(config.bindings[4].name, "up");
	config.bindings[4].sym.sym = SDLK_UP;
	config.bindings[4].sym.mod = KMOD_NONE;
	config.bindings[4].callback = IO_HandleInput;
	config.bindings[4].arg = IO_KEY_UP;

	strcpy(config.bindings[5].name, "down");
	config.bindings[5].sym.sym = SDLK_DOWN;
	config.bindings[5].sym.mod = KMOD_NONE;
	config.bindings[5].callback = IO_HandleInput;
	config.bindings[5].arg = IO_KEY_DOWN;

	strcpy(config.bindings[6].name, "left");
	config.bindings[6].sym.sym = SDLK_LEFT;
	config.bindings[6].sym.mod = KMOD_NONE;
	config.bindings[6].callback = IO_HandleInput;
	config.bindings[6].arg = IO_KEY_LEFT;

	strcpy(config.bindings[7].name, "right");
	config.bindings[7].sym.sym = SDLK_RIGHT;
	config.bindings[7].sym.mod = KMOD_NONE;
	config.bindings[7].callback = IO_HandleInput;
	config.bindings[7].arg = IO_KEY_RIGHT;

	strcpy(config.bindings[8].name, "reset");
	config.bindings[8].sym.sym = SDLK_r;
	config.bindings[8].sym.mod = KMOD_NONE;
	config.bindings[8].callback = IO_HandleInput;
	config.bindings[8].arg = IO_KEY_RESET;

	strcpy(config.bindings[9].name, "debug");
	config.bindings[9].sym.sym = SDLK_BACKQUOTE;
	config.bindings[9].sym.mod = KMOD_NONE;
	config.bindings[9].callback = Debugger_HandleInput;
	config.bindings[9].arg = DEBUGGER_KEY_TOGGLE;

	strcpy(config.bindings[10].name, "scaleup");
	config.bindings[10].sym.sym = SDLK_EQUALS;
	config.bindings[10].sym.mod = KMOD_NONE;
	config.bindings[10].callback = Graphics_Scale;
	config.bindings[10].arg = GRAPHICS_SCALE_UP;

	strcpy(config.bindings[11].name, "scaledn");
	config.bindings[11].sym.sym = SDLK_MINUS;
	config.bindings[11].sym.mod = KMOD_NONE;
	config.bindings[11].callback = Graphics_Scale;
	config.bindings[11].arg = GRAPHICS_SCALE_DOWN;
}

void Config_Write(const char* path) {

}

void Config_Destroy() {

}
