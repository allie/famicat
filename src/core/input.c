#include "core/input.h"
#include "core/config.h"
#include <stdlib.h>
#include <stdio.h>

extern Config config;

void Input_Handle(SDL_Keysym sym) {
	for (int i = 0; i < config.binding_count; i++) {
		if (sym.sym == config.bindings[i].sym.sym) {
			config.bindings[i].callback(config.bindings[i].arg);
		}
	}
}
