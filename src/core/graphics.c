#include "core/graphics.h"
#include "core/config.h"
#include <string.h>

static SDL_Window* window;
static SDL_Renderer* renderer;

#include "font.partial.c"
static SDL_Texture* font;

extern Config config;

int Graphics_Init() {
	window = SDL_CreateWindow(
		"famicat",
		config.window_pos.x,
		config.window_pos.y,
		GRAPHICS_LWIDTH * config.window_scale,
		GRAPHICS_LHEIGHT * config.window_scale,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL) {
		printf("Error creating window: %s\n", SDL_GetError());
		return 0;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (renderer == NULL) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		return 0;
	}

	font = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 128, 128);
	SDL_UpdateTexture(font, NULL, font_tex, 128 * 3);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(renderer, GRAPHICS_LWIDTH, GRAPHICS_LHEIGHT);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return 1;
}

void Graphics_RenderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst) {
	SDL_RenderCopy(renderer, texture, src, dst);
}

void Graphics_RenderString(const char* text, unsigned x, unsigned y) {
	for (int i = 0; i < strlen(text); i++) {
		char c = text[i];
		int tx = c % 16;
		int ty = c / 16;
		SDL_Rect src = {tx * 8, ty * 8, 8, 8};
		SDL_Rect dst = {x * 8 + i * 8, y * 8, 8, 8};
		SDL_RenderCopy(renderer, font, &src, &dst);
	}
}

void Graphics_RenderHex(unsigned long val, unsigned bytes, unsigned x, unsigned y) {
	char hex[17];
	snprintf(hex, bytes * 2 + 1, "%0*lX", bytes * 2, val);
	Graphics_RenderString(hex, x, y);
}

void Graphics_Scale(int direction) {
	if (direction == GRAPHICS_SCALE_UP) {
		config.window_scale++;
	} else if (direction == GRAPHICS_SCALE_DOWN && config.window_scale > 1) {
		config.window_scale--;
	}

	SDL_SetWindowSize(window, GRAPHICS_LWIDTH * config.window_scale, GRAPHICS_LHEIGHT * config.window_scale);
}

void Graphics_Clear() {
	if (renderer == NULL)
		return;

	SDL_SetRenderDrawColor(Graphics_GetRenderer(), 255, 255, 255, 1);
	SDL_RenderClear(renderer);
}

void Graphics_Present() {
	if (renderer == NULL)
		return;

	SDL_RenderPresent(renderer);
}

void Graphics_Destroy() {
	SDL_DestroyTexture(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

SDL_Window* Graphics_GetWindow() {
	return window;
}

SDL_Renderer* Graphics_GetRenderer() {
	return renderer;
}
