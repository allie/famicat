#ifndef GCORE_H
#define GCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../common.h"

int Graphics_Init(int w, int h);
void Graphics_RenderTexture(SDL_Texture*, SDL_Rect*, SDL_Rect*);
void Graphics_DrawString(const char*, unsigned, unsigned);
void Graphics_DrawHex(unsigned long, unsigned, unsigned, unsigned);
void Graphics_Clear();
void Graphics_Present();
void Graphics_Destroy();
SDL_Window* Graphics_GetWindow();
SDL_Renderer* Graphics_GetRenderer();

#endif
