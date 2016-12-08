#ifndef TIMER_H
#define TIMER_H

#include "../common.h"

#define INITIAL_TIMER_CAPACITY 128

typedef struct {
	long int duration;
	long int current;
	int loop;
	int running;
	void (*event)();
} Timer;

int Timer_Init();
int Timer_Add(double duration, int loop, void (*event)());
void Timer_Remove(int id);
double Timer_GetProgress(int id);
double Timer_GetDelta();
void Timer_UpdateAll();

#endif
