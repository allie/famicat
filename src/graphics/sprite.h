#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../common.h"

#define INITIAL_SPRITE_CAPACITY 128

typedef struct {
	int animated;
	int loop;
	int frames;
	int current;
	int width;
	int height;
	double interval;
	double timer;
	SDL_Texture* texture;
	SDL_Rect* src;
} Sprite;

int Sprite_Init();
int Sprite_Add(SDL_Texture* texture, int width, int height);
int Sprite_AddAnimated(SDL_Texture* texture, int loop, int frames, double interval, int width, int height);
void Sprite_Destroy(int id);
Sprite* Sprite_Get(int id);
void Sprite_UpdateAll(double dt);
void Sprite_Render(int id, double x, double y);

#endif
