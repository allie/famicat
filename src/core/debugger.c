#include "core/debugger.h"
#include "core/graphics.h"
#include "hardware/cpu.h"
#include "SDL2/SDL.h"
#include <stdio.h>

enum {
	DEBUGGER_TAB_CPU = 0,
	DEBUGGER_TAB_MPU,
	DEBUGGER_TAB_PPU,
	DEBUGGER_TAB_APU
};

static SDL_Renderer* renderer;
static SDL_Texture* target;
static SDL_mutex* debugger_mutex;
static char ins_str[40 * 26];
static char reg_str[40 * 8];
static int visible = 0;
static int tab = DEBUGGER_TAB_CPU;

extern CPU cpu;
extern CPU_History cpu_history;

static void Debugger_Toggle() {
	visible = visible == 0 ? 1 : 0;
}

static void Debugger_UpdateCPU() {
	// char ins_buf[40 * 26];
	// char reg_buf[20 * 13];

	// memset(ins_buf, 0, 40 * 26);
	// memset(reg_buf, 0, 20 * 13);

	// for (int i = 0; i < 26; i++) {
	// 	if (cpu_history.ptr >= i + 1) {
	// 		DWORD history_addr = cpu_history.ptr - i - 1;
	// 		snprintf(ins_buf + (i * 40), 9, "%08X", cpu_history.states[history_addr].line);
	// 		ins_buf[8 + (i * 40)] = ' ';
	// 		Disassembler_Print(cpu_history.states[history_addr].cpu, ins_buf + 9 + (i * 40), 31);
	// 	}
	// }

	// DWORD history_addr = cpu_history.ptr != 0 ? cpu_history.ptr - 1 : 0xFFFF;
	// CPU state = cpu_history.states[history_addr].cpu;
}

static void Debugger_RenderCPU() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Tab bar
	Graphics_RenderString("<CPU> MMU  PPU  APU", 0, 0);

	// Print registers and flags
	Graphics_RenderString("REGISTERS", 23, 2);
	Graphics_RenderString("A  0x", 23, 4);
	Graphics_RenderString("X  0x", 23, 5);
	Graphics_RenderString("Y  0x", 23, 6);
	Graphics_RenderString("S  0x", 23, 7);
	Graphics_RenderString("P  0x", 23, 8);
	Graphics_RenderString("PC 0x", 23, 9);

	Graphics_RenderString("FLAGS", 23, 11);
	Graphics_RenderString("C", 23, 13);
	Graphics_RenderString("Z", 23, 14);
	Graphics_RenderString("I", 23, 15);
	Graphics_RenderString("D", 23, 16);
	Graphics_RenderString("B", 23, 17);
	Graphics_RenderString("V", 23, 18);
	Graphics_RenderString("N", 23, 19);

	Graphics_RenderHex(cpu.A, 1, 28, 4);
	Graphics_RenderHex(cpu.X, 1, 28, 5);
	Graphics_RenderHex(cpu.Y, 1, 28, 6);
	Graphics_RenderHex(cpu.SP, 1, 28, 7);
	Graphics_RenderHex(cpu.S, 1, 28, 8);
	Graphics_RenderHex(cpu.PC, 2, 28, 9);

	if (GET_FLAG(FLAG_C)) {
		Graphics_RenderString("+", 25, 13);
	} else {
		Graphics_RenderString("-", 25, 13);
	}

	if (GET_FLAG(FLAG_Z)) {
		Graphics_RenderString("+", 25, 14);
	} else {
		Graphics_RenderString("-", 25, 14);
	}

	if (GET_FLAG(FLAG_I)) {
		Graphics_RenderString("+", 25, 15);
	} else {
		Graphics_RenderString("-", 25, 15);
	}

	if (GET_FLAG(FLAG_D)) {
		Graphics_RenderString("+", 25, 16);
	} else {
		Graphics_RenderString("-", 25, 16);
	}

	if (GET_FLAG(FLAG_B)) {
		Graphics_RenderString("+", 25, 17);
	} else {
		Graphics_RenderString("-", 25, 17);
	}

	if (GET_FLAG(FLAG_V)) {
		Graphics_RenderString("+", 25, 18);
	} else {
		Graphics_RenderString("-", 25, 18);
	}

	if (GET_FLAG(FLAG_N)) {
		Graphics_RenderString("+", 25, 19);
	} else {
		Graphics_RenderString("-", 25, 19);
	}
}

int Debugger_Init() {
	renderer = Graphics_GetRenderer();

	target = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_TARGET,
		GRAPHICS_LWIDTH,
		GRAPHICS_LHEIGHT
	);

	if (target == NULL) {
		return -1;
	}

	SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(target, 128);

	return 1;
}

void Debugger_Destroy() {
	SDL_DestroyTexture(target);
}

void Debugger_HandleInput(int arg) {
	switch (arg) {
	case DEBUGGER_KEY_TOGGLE:
		Debugger_Toggle();
		break;
	}
}

void Debugger_Update() {

}

void Debugger_Draw() {
	if (!visible) {
		return;
	}

	SDL_SetRenderTarget(renderer, target);

	switch (tab) {
	case DEBUGGER_TAB_CPU:
		Debugger_RenderCPU();
		break;
	}

	Graphics_RenderString("--------------------------------", 0, 1);

	SDL_SetRenderTarget(renderer, NULL);

	SDL_RenderCopy(renderer, target, NULL, NULL);
}
