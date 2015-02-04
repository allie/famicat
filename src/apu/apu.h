#ifndef APU_H
#define APU_H

#include "../common.h"
#include "../memory/memory.h"
#include "../audio/core.h"

#define HI_PASS_STRONG 225574
#define HI_PASS_WEAK 57593

typedef struct {
    WORD volume;
    WORD counter;
    WORD divider_counter;
    BYTE loop;
    BYTE disable;
    WORD period;
    BYTE reset;
} Envelope;

typedef struct {
    Envelope envelope;
    BYTE enabled;
    BYTE halt;
    BYTE length;
    WORD timer;
    WORD period;
    BYTE sweep_enable;
    BYTE sweep_period;
    BYTE sweep_counter;
    BYTE sweep_reload;
    BYTE sweep_negate;
    BYTE sweep_shift;
    BYTE duty_cycle;
    BYTE duty_count;
    BYTE num;
    SWORD sample;
} Square;

typedef struct {
    BYTE enabled;
    BYTE control;
    BYTE halt;
    BYTE length;
    DWORD counter;
    DWORD timer;
    DWORD timer_count;
    WORD timer_period;
    BYTE linear_halt;
    BYTE linear_reload;
    BYTE lookup_counter;
    SWORD sample;
} Triangle;

typedef struct {
    Envelope envelope;
    BYTE period;
    BYTE enabled;
    BYTE mode;
    BYTE shift;
    BYTE halt;
    BYTE length;
    DWORD timer;
    DWORD timer_count;
    WORD timer_period;
    SWORD sample;
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
    SWORD sample;
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

    int64_t hipass_strong;
    int64_t hipass_weak;

    double pulseOut[31];
    double tndOut[203];
    double output;
    SWORD sample;
} APU;

void APU_Init();
void APU_Step();
void APU_MixOutput();
void APU_ClockSquare(Square*);
void APU_ClockNoise();
void APU_ClockTriangle();
void APU_ClockDMC();
void APU_Read();
void APU_Write(WORD addr);
void APU_WriteSquare(Square*);
void APU_WriteTriangle();
void APU_WriteNoise();
void APU_WriteDMC();
void APU_WriteCommon();
void APU_HipassStrong();
void APU_HipassWeak();
void APU_Push();
void APU_FrameSequencerStep();
void APU_ClockSweep(Square*);
void APU_ClockDecay(Envelope*);

#endif
