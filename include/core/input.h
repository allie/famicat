#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

int Input_Init(const char*);
void Input_Handle(SDL_Keysym);

#endif
