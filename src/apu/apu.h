#ifndef APU_H
#define APU_H

#include "../common.h"

typedef struct {
    WORD volume;
    WORD counter;
} Envelope;

typedef struct {
    Envelope envelope;
    BYTE halt;
    BYTE length;
    WORD sample;
    int timer;
    int timer_period;
    BYTE sweep_enable;
    BYTE sweep_period;
    BYTE negate;
    BYTE shift;
    BYTE duty;
} Square;

typedef struct {
    BYTE halt;
    WORD sample;
    BYTE length;
    int timer;
    int timer_period;
} Triangle;

typedef struct {
    Envelope envelope;
    BYTE halt;
    BYTE length;
    WORD sample;
    int timer;
    int timer_period;
} Noise;

typedef struct {
    WORD sample;
    BYTE irq;
    BYTE bytes_remaining;
    int timer;
    int timer_period;
} DMC;

typedef struct {
    Square square1, square2;
    Triangle triangle;
    Noise noise;
    DMC dmc;
    double pulseOut[31];
    double tndOut[203];
} APU;

void APU_Init();
double APU_MixOutput();
void APU_Clock_Square(Square);
void APU_Clock_Noise();
void APU_Clock_Triangle();
void APU_Clock_DMC();
void APU_Read_Square(Square);
void APU_Read_Triangle();
void APU_Read_Noise();
void APU_Read_DMC();

#endif
