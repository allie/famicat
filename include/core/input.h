#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef struct {
	int arg;
	void (*callback)(int);
} Input;

int Input_Init(const char*);
void Input_Handle(SDL_Keysym, SDL_GameControllerButton);

#endif
