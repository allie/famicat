#ifndef GCORE_H
#define GCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../common.h"

#define GRAPHICS_LWIDTH 256
#define GRAPHICS_LHEIGHT 240

enum {
	GRAPHICS_SCALE_UP = 0,
	GRAPHICS_SCALE_DOWN
};

int Graphics_Init();
void Graphics_RenderBuffer(RGBA*, unsigned, unsigned);
void Graphics_RenderTexture(SDL_Texture*, SDL_Rect*, SDL_Rect*);
void Graphics_RenderString(const char*, unsigned, unsigned);
void Graphics_RenderHex(unsigned long, unsigned, unsigned, unsigned);
void Graphics_Scale(int);
void Graphics_Clear();
void Graphics_Present();
void Graphics_Destroy();
SDL_Window* Graphics_GetWindow();
SDL_Renderer* Graphics_GetRenderer();

#endif
