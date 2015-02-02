#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "timer.h"

static struct timespec last;
static struct timespec dt;

static Timer** timers;
static int capacity;
static int count;

#ifdef __MACH__

#include <stdint.h>
#include <mach/mach_time.h>
#define CLOCK_MONOTONIC 0

static double timebase = 0.0;
static unsigned long int timestart = 0;

static void clock_gettime(int clockid, struct timespec* ts) {
	if (!timestart) {
		mach_timebase_info_data_t tb;
		mach_timebase_info(&tb);
		timebase = tb.numer;
		timebase /= tb.denom;
		timestart = mach_absolute_time();
	}

	double diff = (mach_absolute_time() - timestart) * timebase;
	ts->tv_sec = diff * (+1.0E-9);
	ts->tv_nsec = diff - (ts->tv_sec * UINT64_C(1000000000));
}

#endif

static int Timer_GetFreeId() {
	if (count == capacity) {
		timers = (Timer**)realloc(timers, capacity * 2 * sizeof(Timer*));
		memset(timers + (capacity * sizeof(Timer*)), 0, capacity * sizeof(Timer*));
		capacity *= 2;
		return count;
	}

	int id = 0;
	while (timers[id] != NULL)
		id++;

	return id;
}

static void Timer_CalcDelta(struct timespec* now) {
	if ((now->tv_nsec - last.tv_nsec) < 0) {
		dt.tv_sec = now->tv_sec - last.tv_sec - 1;
		dt.tv_nsec = 1000000000L + now->tv_nsec - last.tv_nsec;
	} else {
		dt.tv_sec = now->tv_sec - last.tv_sec;
		dt.tv_nsec = now->tv_nsec - last.tv_nsec;
	}
}

int Timer_Init() {
	timers = (Timer**)calloc(INITIAL_TIMER_CAPACITY, sizeof(Timer*));
	capacity = INITIAL_TIMER_CAPACITY;
	count = 0;

	dt.tv_sec = 0;
	dt.tv_nsec = 0;
	last.tv_sec = 0;
	last.tv_nsec = 0;

	Timer_UpdateAll();

	return 1;
}

int Timer_Add(double duration, int loop, void (*event)()) {
	Timer* timer = (Timer*)malloc(sizeof(Timer));

	timer->duration = (long int)(duration * 1000000L);
	timer->loop = loop;
	timer->event = event;
	timer->current = 0;
	timer->running = 1;

	int id = Timer_GetFreeId();
	timers[id] = timer;
	count++;

	return id;
}

void Timer_Remove(int id) {
	if (id > capacity)
		return;

	timers[id] = NULL;
	count--;
}

double Timer_GetProgress(int id) {
	if (id > capacity || timers[id] == NULL)
		return -1;

	return timers[id]->current / 1000000.0;
}

double Timer_GetDelta() {
	return (dt.tv_sec * 1000000000L + dt.tv_nsec) / 1000000.0;
}

void Timer_UpdateAll() {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	Timer_CalcDelta(&now);
	last = now;

	int c = 0;
	int i;

	for (i = 0; i < capacity && c < count; i++) {
		if (timers[i] == NULL || !timers[i]->running)
			continue;

		Timer* timer = timers[i];
		timer->current += (dt.tv_sec * 1000000000L + dt.tv_nsec);

		if (timer->current >= timer->duration) {
			if (timer->event != NULL)
				timer->event();

			if (timer->loop) {
				timer->current -= timer->duration;
			} else {
				Timer_Remove(i);
				continue;
			}
		}

		c++;
	}
}
