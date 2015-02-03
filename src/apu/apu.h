#ifndef APU_H
#define APU_H

#include "../common.h"
#include "../memory/memory.h"

#define HI_PASS_STRONG 225574
#define HI_PASS_WEAK 57593

typedef struct {
    WORD volume;
    WORD counter;
    BYTE loop;
    BYTE disable;
    WORD period;
} Envelope;

typedef struct {
    Envelope envelope;
    BYTE enabled;
    BYTE length_halt;
    BYTE length;
    DWORD timer;
    WORD timer_period;
    BYTE sweep_enable;
    BYTE sweep_period;
    BYTE negate;
    BYTE shift;
    BYTE duty;
    WORD dac;
} Square;

typedef struct {
    BYTE enabled;
    BYTE control;
    BYTE halt;
    BYTE length;
    BYTE length_halt;
    DWORD timer;
    WORD timer_period;
    BYTE linear_counter;
    BYTE linear_halt;
    BYTE linear_reload;
    BYTE lookup_counter;
    WORD dac;
} Triangle;

typedef struct {
    Envelope envelope;
    BYTE period;
    BYTE enabled;
    BYTE length_halt;
    BYTE mode;
    BYTE shift;
    BYTE halt;
    BYTE length;
    DWORD timer;
    WORD timer_period;
    WORD dac;
} Noise;

typedef struct {
    BYTE enabled;
    BYTE irq_enable;
    BYTE irq_throw;
    BYTE loop_enable;
    BYTE rate_index;
    BYTE rate_counter;
    BYTE direct_load;
    BYTE direct_counter;
    WORD data;
    WORD sample_address;
    WORD current_address;
    BYTE sample_length;
    BYTE sample_counter;
    BYTE shift_counter;
    WORD frequency;
    BYTE has_sample;
    DWORD timer;
    WORD timer_period;
    WORD dac;
} DMC;

typedef struct {
    Square square1, square2;
    Triangle triangle;
    Noise noise;
    DMC dmc;
    BYTE irq_enable;

    BYTE frame_irq_throw;
    BYTE frame_mode;
    int frame_counter;
    int frame_tick;
    int last_frame_tick;

    double pulseOut[31];
    double tndOut[203];
    double output;
    int16_t sample;
} APU;

void APU_Init();
void APU_Step();
void APU_MixOutput();
void APU_Clock_Square(Square, BYTE);
void APU_Clock_Noise();
void APU_Clock_Triangle();
void APU_Clock_DMC();
void APU_Read();
void APU_Write();
void APU_Write_Square1();
void APU_Write_Square2();
void APU_Write_Triangle();
void APU_Write_Noise();
void APU_Write_DMC();
void APU_Write_Common();

#endif
