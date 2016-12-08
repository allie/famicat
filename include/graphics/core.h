#ifndef GCORE_H
#define GCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../common.h"

int Graphics_Init(int w, int h);
SDL_Texture* Graphics_LoadPNG(const char* key, const char* file);
SDL_Texture* Graphics_LoadPNGDir(const char* path, int recurse);
SDL_Texture* Graphics_GetTexture(const char* key);
void Graphics_RenderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst);
void Graphics_Clear();
void Graphics_Present();
void Graphics_Destroy();
SDL_Window* Graphics_GetWindow();
SDL_Renderer* Graphics_GetRenderer();

#endif
