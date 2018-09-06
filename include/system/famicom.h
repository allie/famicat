#ifndef FAMICOM_H
#define FAMICOM_H

void Famicom_Reset();
int Famicom_Emulate(void*);
void Famicom_LoadState();
void Famicom_SaveState();
void Famicom_SelectState(int);
void Famicom_NextState();
void Famicom_PreviousState();

#endif
