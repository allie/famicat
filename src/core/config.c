#include "core/config.h"
#include "core/debugger.h"
#include "core/graphics.h"
#include "io/io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <jsmn.h>

Config config;

// From the jsmn example files
static int jsoncmp(const char* json, jsmntok_t* token, const char* str) {
	if (token->type == JSMN_STRING && (int)strlen(str) == token->end - token->start &&
		strncmp(json + token->start, str, token->end - token->start) == 0) {
		return 0;
	}
	return -1;
}

static char* extract_string(char* str, size_t size) {
	char* buf = (char*)malloc(size + 1);

	if (buf) {
		int i;

		for (i = 0; i < size && str[i] != '\0'; i++) {
			buf[i] = str[i];
		}

		buf[i] = '\0';
	}

	return buf;
}

int Config_Load(const char* path) {
	FILE* fp = fopen(path, "rb");
	if (fp == NULL) {
		printf("Error opening config file.\n");
		return -1;
	}

	long length;
	char* buf;

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (char*)malloc(length);
	fread(buf, 1, length, fp);
	fclose(fp);

	jsmn_parser parser;
	jsmntok_t tokens[256];

	jsmn_init(&parser);

	int count = jsmn_parse(&parser, buf, strlen(buf), tokens, 256);

	if (count < 0) {
		printf("Error: malformed config file; ");
		switch (count) {
		case JSMN_ERROR_INVAL:
			printf("Bad token.\n");
			break;
		case JSMN_ERROR_NOMEM:
			printf("Not enough tokens.\n");
			break;
		case JSMN_ERROR_PART:
			printf("Expected more JSON data.\n");
			break;
		default:
			printf("\n");
			break;
		}
		return -1;
	}

	if (count < 1 || tokens[0].type != JSMN_OBJECT) {
		printf("Error: root object expected.\n");
		return -1;
	}

	// Parse JSON keys in the root object
	for (int i = 1; i < count; i++) {
		// Window position
		if (jsoncmp(buf, &tokens[i], "pos") == 0) {
			i++;
			if (tokens[i].type == JSMN_ARRAY && tokens[i].size == 2) {
				char* x = extract_string(buf + tokens[i + 1].start, tokens[i + 1].end - tokens[i + 1].start);
				char* y = extract_string(buf + tokens[i + 2].start, tokens[i + 2].end - tokens[i + 2].start);
				config.window_pos.x = atoi(x);
				config.window_pos.y = atoi(y);
				free(x);
				free(y);
				i += 2;
			}
		}

		// Window scale
		else if (jsoncmp(buf, &tokens[i], "scale") == 0) {
			i++;
			char* scale = extract_string(buf + tokens[i].start, tokens[i].end - tokens[i].start);
			config.window_scale = atoi(scale);
			free(scale);
		}

		// Volume
		else if (jsoncmp(buf, &tokens[i], "volume") == 0) {
			i++;
			char* volume = extract_string(buf + tokens[i].start, tokens[i].end - tokens[i].start);
			config.volume = atoi(volume) / 100.0f;
			free(volume);
		}

		// Keybindings
		else if (jsoncmp(buf, &tokens[i], "bindings") == 0) {
			i++;
			if (tokens[i].type == JSMN_OBJECT) {
				int n = tokens[i].size;

				config.binding_count = n;
				config.bindings = (Keybinding*)malloc(n * sizeof(Keybinding));

				i++;
				for (int j = 0; j < n; j++) {
					if (tokens[i + 1].type == JSMN_ARRAY && tokens[i + 1].size == 2) {
						char* name = extract_string(buf + tokens[i].start, tokens[i].end - tokens[i].start);
						char* mod = extract_string(buf + tokens[i + 2].start, tokens[i + 2].end - tokens[i + 2].start);
						char* sym = extract_string(buf + tokens[i + 3].start, tokens[i + 3].end - tokens[i + 3].start);

						strcpy(config.bindings[j].name, name);
						config.bindings[j].sym.sym = atoi(sym);
						config.bindings[j].sym.mod = atoi(mod);

						if (strcmp(name, "a") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_A;
						}

						else if (strcmp(name, "b") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_B;
						}

						else if (strcmp(name, "start") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_START;
						}

						else if (strcmp(name, "select") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_SELECT;
						}

						else if (strcmp(name, "up") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_UP;
						}

						else if (strcmp(name, "down") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_DOWN;
						}

						else if (strcmp(name, "left") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_LEFT;
						}

						else if (strcmp(name, "right") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_RIGHT;
						}

						else if (strcmp(name, "reset") == 0) {
							config.bindings[j].callback = IO_HandleInput;
							config.bindings[j].arg = IO_KEY_RESET;
						}

						else if (strcmp(name, "debug") == 0) {
							config.bindings[j].callback = Debugger_HandleInput;
							config.bindings[j].arg = DEBUGGER_KEY_TOGGLE;
						}

						else if (strcmp(name, "scaleup") == 0) {
							config.bindings[j].callback = Graphics_Scale;
							config.bindings[j].arg = GRAPHICS_SCALE_UP;
						}

						else if (strcmp(name, "scaledn") == 0) {
							config.bindings[j].callback = Graphics_Scale;
							config.bindings[j].arg = GRAPHICS_SCALE_DOWN;
						}

						free(name);
						free(sym);
						free(mod);
						i += 4;
					}
				}
			}
		}
	}

	return 1;
}

void Config_LoadDefaults() {
	config.window_pos.x = 100;
	config.window_pos.y = 100;
	config.window_scale = 1;

	config.binding_count = 12;
	config.bindings = (Keybinding*)malloc(config.binding_count * sizeof(Keybinding));

	strcpy(config.bindings[0].name, "a");
	config.bindings[0].sym.sym = SDLK_z;
	config.bindings[0].sym.mod = KMOD_NONE;
	config.bindings[0].callback = IO_HandleInput;
	config.bindings[0].arg = IO_KEY_A;

	strcpy(config.bindings[1].name, "b");
	config.bindings[1].sym.sym = SDLK_x;
	config.bindings[1].sym.mod = KMOD_NONE;
	config.bindings[1].callback = IO_HandleInput;
	config.bindings[1].arg = IO_KEY_B;

	strcpy(config.bindings[2].name, "start");
	config.bindings[2].sym.sym = SDLK_RETURN;
	config.bindings[2].sym.mod = KMOD_NONE;
	config.bindings[2].callback = IO_HandleInput;
	config.bindings[2].arg = IO_KEY_START;

	strcpy(config.bindings[3].name, "select");
	config.bindings[3].sym.sym = SDLK_RSHIFT;
	config.bindings[3].sym.mod = KMOD_NONE;
	config.bindings[3].callback = IO_HandleInput;
	config.bindings[3].arg = IO_KEY_SELECT;

	strcpy(config.bindings[4].name, "up");
	config.bindings[4].sym.sym = SDLK_UP;
	config.bindings[4].sym.mod = KMOD_NONE;
	config.bindings[4].callback = IO_HandleInput;
	config.bindings[4].arg = IO_KEY_UP;

	strcpy(config.bindings[5].name, "down");
	config.bindings[5].sym.sym = SDLK_DOWN;
	config.bindings[5].sym.mod = KMOD_NONE;
	config.bindings[5].callback = IO_HandleInput;
	config.bindings[5].arg = IO_KEY_DOWN;

	strcpy(config.bindings[6].name, "left");
	config.bindings[6].sym.sym = SDLK_LEFT;
	config.bindings[6].sym.mod = KMOD_NONE;
	config.bindings[6].callback = IO_HandleInput;
	config.bindings[6].arg = IO_KEY_LEFT;

	strcpy(config.bindings[7].name, "right");
	config.bindings[7].sym.sym = SDLK_RIGHT;
	config.bindings[7].sym.mod = KMOD_NONE;
	config.bindings[7].callback = IO_HandleInput;
	config.bindings[7].arg = IO_KEY_RIGHT;

	strcpy(config.bindings[8].name, "reset");
	config.bindings[8].sym.sym = SDLK_r;
	config.bindings[8].sym.mod = KMOD_NONE;
	config.bindings[8].callback = IO_HandleInput;
	config.bindings[8].arg = IO_KEY_RESET;

	strcpy(config.bindings[9].name, "debug");
	config.bindings[9].sym.sym = SDLK_BACKQUOTE;
	config.bindings[9].sym.mod = KMOD_NONE;
	config.bindings[9].callback = Debugger_HandleInput;
	config.bindings[9].arg = DEBUGGER_KEY_TOGGLE;

	strcpy(config.bindings[10].name, "scaleup");
	config.bindings[10].sym.sym = SDLK_EQUALS;
	config.bindings[10].sym.mod = KMOD_NONE;
	config.bindings[10].callback = Graphics_Scale;
	config.bindings[10].arg = GRAPHICS_SCALE_UP;

	strcpy(config.bindings[11].name, "scaledn");
	config.bindings[11].sym.sym = SDLK_MINUS;
	config.bindings[11].sym.mod = KMOD_NONE;
	config.bindings[11].callback = Graphics_Scale;
	config.bindings[11].arg = GRAPHICS_SCALE_DOWN;
}

void Config_Write(const char* path) {

}

void Config_Destroy() {
	free(config.bindings);
}
