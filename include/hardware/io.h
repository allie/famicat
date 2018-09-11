#ifndef IO_H
#define IO_H

enum {
	IO_KEY_A = 0,
	IO_KEY_B,
	IO_KEY_START,
	IO_KEY_SELECT,
	IO_KEY_UP,
	IO_KEY_DOWN,
	IO_KEY_LEFT,
	IO_KEY_RIGHT,
	IO_KEY_RESET,
	IO_KEY_PAUSE
};

void IO_HandleInput(int);

#endif
