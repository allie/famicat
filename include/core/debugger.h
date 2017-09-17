#ifndef DEBUGGER_H
#define DEBUGGER_H

enum {
	DEBUGGER_KEY_TOGGLE
};

void Debugger_Init();
void Debugger_Destroy();
void Debugger_HandleInput(int);
void Debugger_Update();
void Debugger_Draw();

#endif
