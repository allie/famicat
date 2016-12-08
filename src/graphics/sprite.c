#include <string.h>
#include "graphics/sprite.h"
#include "graphics/core.h"

static Sprite** sprites;
static int capacity;
static int count;

static int Sprite_GetFreeId() {
	if (count == capacity) {
		sprites = (Sprite**)realloc(sprites, capacity * 2 * sizeof(Sprite*));
		memset(sprites + (capacity * sizeof(Sprite*)), 0, capacity * sizeof(Sprite*));
		capacity *= 2;
		return count;
	}

	int id = 0;
	while (sprites[id] != NULL)
		id++;

	return id;
}

int Sprite_Init() {
	sprites = (Sprite**)calloc(INITIAL_SPRITE_CAPACITY, sizeof(Sprite*));
	capacity = INITIAL_SPRITE_CAPACITY;
	count = 0;

	return 1;
}

int Sprite_Add(SDL_Texture* texture, int width, int height) {
	Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));

	sprite->texture = texture;
	sprite->animated = 0;
	sprite->src = NULL;
	sprite->width = width;
	sprite->height = height;

	int id = Sprite_GetFreeId();
	sprites[id] = sprite;
	count++;

	return id;
}

int Sprite_AddAnimated(SDL_Texture* texture, int loop, int frames, double interval, int width, int height) {
	Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));

	sprite->texture = texture;
	sprite->animated = 1;
	sprite->loop = loop;
	sprite->frames = frames;
	sprite->current = 0;
	sprite->interval = interval;
	sprite->timer = 0.0;
	sprite->width = width;
	sprite->height = height;

	sprite->src = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	sprite->src->x = 0;
	sprite->src->y = 0;
	sprite->src->w = width;
	sprite->src->h = height;

	int id = Sprite_GetFreeId();
	sprites[id] = sprite;
	count++;

	return id;
}

void Sprite_Destroy(int id) {
	if (id > capacity || sprites[id] == NULL)
		return;

	free(sprites[id]->src);
	free(sprites[id]);

	sprites[id] = NULL;
	count--;
}

Sprite* Sprite_Get(int id) {
	if (id >= capacity)
		return NULL;

	return sprites[id];
}

void Sprite_UpdateAll(double dt) {
	int c = 0;
	int i;

	for (i = 0; i < capacity && c < count; i++) {
		if (sprites[i] == NULL || !sprites[i]->animated)
			continue;

		Sprite* sprite = sprites[i];
		sprite->timer += dt;

		if (sprite->timer >= sprite->interval) {
			sprite->timer -= sprite->interval;
			sprite->current++;

			if (sprite->current >= sprite->frames) {
				if (sprite->loop) {
					sprite->current = 0;
				} else {
					Sprite_Destroy(i);
					continue;
				}
			}

			sprite->src->x = sprite->current * sprite->width;
		}

		c++;
	}
}

void Sprite_Render(int id, double x, double y) {
	if (id >= capacity || sprites[id] == NULL)
		return;

	SDL_Rect dst;

	dst.x = x;
	dst.y = y;
	dst.w = sprites[id]->width;
	dst.h = sprites[id]->height;

	Graphics_RenderTexture(sprites[id]->texture, sprites[id]->src, &dst);
}
