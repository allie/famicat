#include <string.h>
#include "core/audio.h"
#include "utils/queue.h"

static WORD samplesize = 1024;
static SDL_AudioSpec* desired;
static SDL_AudioSpec* audio;
static SWORD* samples;
static SWORD* lastchunk;
static int current;

static Queue* queue;

void Audio_Callback(void* userdata, Uint8* stream, int len) {
	Queue* q = (Queue*)userdata;

	if (q->size == 0) {
		if (lastchunk != NULL)
			memcpy(stream, lastchunk, len);
		else
			memset(stream, 0, len);
	} else {
		SWORD* buffer = (SWORD*)Queue_Dequeue(q);
		memcpy(stream, buffer, len);
		free(lastchunk);
		lastchunk = buffer;
	}
}

int Audio_Init() {
	queue = Queue_New();

	desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

	desired->freq = 44100;
	desired->format = AUDIO_S16SYS;
	desired->channels = 1;
	desired->silence = 0;
	desired->samples = samplesize;
	desired->size = 0;
	desired->callback = Audio_Callback;
	desired->userdata = queue;

	if (SDL_OpenAudio(desired, audio) < 0) {
		printf("Unable to open audio device: %s\n", SDL_GetError());
		return 0;
	}

	free(desired);

	SDL_PauseAudio(0);

	samples = (SWORD*)malloc(samplesize * sizeof(SWORD));
	lastchunk = NULL;

	return 1;
}

void Audio_AddSample(SWORD sample) {
	samples[current] = sample;
	current++;

	if (current == samplesize) {
		Queue_Enqueue(queue, samples);
		samples = (SWORD*)malloc(samplesize * sizeof(SWORD));
		current = 0;
	}
}

void Audio_Destroy() {
	SDL_CloseAudio();
	free(samples);

	while (queue->size > 0)
		free(Queue_Dequeue(queue));

	Queue_Destroy(queue);
}
