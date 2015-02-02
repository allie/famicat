#include "core.h"
#include "sprite.h"
#include "../utils/dictionary.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static int width;
static int height;

static Dictionary* textures;

int Graphics_Init() {
	width = 512;
	height = 480;

	textures = Dictionary_New();

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
	SDL_RenderSetLogicalSize(renderer, 512, 480);

	Sprite_Init();

	return 1;
}

SDL_Texture* Graphics_LoadPNG(const char* key, const char* file) {
	SDL_Surface* s = IMG_Load(file);

	if (s == NULL) {
		printf("Error loading PNG: %s\n", IMG_GetError());
		return NULL;
	}

	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	SDL_FreeSurface(s);

	if (t == NULL) {
		printf("Error creating PNG texture: %s\n", SDL_GetError());
		return NULL;
	}

	Dictionary_Add(textures, key, t);

	return t;
}

SDL_Texture* Graphics_LoadPNGDir(const char* path, int recurse) {
	/* TODO */
	return NULL;
}

SDL_Texture* Graphics_GetTexture(const char* key) {
	return (SDL_Texture*)Dictionary_Get(textures, key);
}

void Graphics_RenderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst) {
	SDL_RenderCopy(renderer, texture, src, dst);
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
	Dictionary_Destroy(textures);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

SDL_Window* Graphics_GetWindow() {
	return window;
}

SDL_Renderer* Graphics_GetRenderer() {
	return renderer;
}
