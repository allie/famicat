#include "core/graphics.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static int width;
static int height;
static int lwidth;
static int lheight;

static SDL_Texture* font;

int Graphics_Init(int w, int h) {
	width = w;
	height = h;
	lwidth = 256;
	lheight = 240;

	window = SDL_CreateWindow(
		"famicat",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL) {
		printf("Error creating window: %s\n", SDL_GetError());
		return 0;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	if (renderer == NULL) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		return 0;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(renderer, lwidth, lheight);

	return 1;
}

void Graphics_RenderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst) {
	SDL_RenderCopy(renderer, texture, src, dst);
}

void Graphics_DrawString(const char* text, unsigned x, unsigned y) {
	for (int i = 0; i < strlen(text); i++) {
		char c = text[i];
		int tx = c % 16;
		int ty = c / 16;
		SDL_Rect src = {tx * 8, ty * 8, 8, 8};
		SDL_Rect dst = {x * 8 + i * 8, y * 8, 8, 8};
		SDL_RenderCopy(renderer, font, &src, &dst);
	}
}

void Graphics_DrawHex(unsigned long val, unsigned bytes, unsigned x, unsigned y) {
	char hex[17];
	snprintf(hex, bytes * 2 + 1, "%0*lX", bytes * 2, val);
	Graphics_DrawString(hex, x, y);
}

void Graphics_Clear() {
	if (renderer == NULL)
		return;

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
