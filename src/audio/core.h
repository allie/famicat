#ifndef ACORE_H
#define ACORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "../common.h"

int Audio_Init();
void Audio_AddSample(SWORD sample);
void Audio_Destroy();
void Audio_Callback();

#endif
