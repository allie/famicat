#ifndef APU_H
#define APU_H

#include "../common.h"
#include "../memory/memory.h"
#include "../audio/core.h"
#include "../cpu/cpu.h"
#include <stdio.h>

#define HI_PASS_STRONG 225574
#define HI_PASS_WEAK 57593

typedef struct {
    WORD volume;
    WORD counter;
    WORD divider_counter;
    BYTE decay_rate;
    BYTE decay_enabled;
    BYTE decay_counter;
    BYTE loop_enabled;
    BYTE loop;
    BYTE disabled;
    WORD period;
    BYTE reset;
} Envelope;

typedef struct {
    Envelope envelope;
    BYTE enabled;
    BYTE halt;
    BYTE length;
    BYTE length_enabled;
    WORD timer;
    BYTE timer_count;
    BYTE timer_counter;
    WORD period;
    WORD shift;
    BYTE sweep_enabled;
    BYTE sweep_mode;
    BYTE sweep_period;
    BYTE sweep_counter;
    BYTE sweep_reload;
    BYTE negative;
    BYTE sweep_shift;
    BYTE duty_cycle;
    BYTE duty_count;
    BYTE sequencer_count;
    BYTE sequencer_reload;
    BYTE num;
    SWORD sample;
} Square;

typedef struct {
    BYTE enabled;
    BYTE control;
    BYTE halt;
    BYTE length;
    BYTE length_enabled;
    BYTE reload_value;
    DWORD counter;
    WORD timer;
    WORD timer_count;
    BYTE linear_reload;
    BYTE lookup_counter;
    SWORD sample;
} Triangle;

typedef struct {
    Envelope envelope;
    BYTE enabled;
    BYTE base_envelope;
    BYTE noise_length;
    BYTE length_enabled;
    BYTE timer_count;
    BYTE mode;
    BYTE shift;
    BYTE halt;
    BYTE length;
    WORD timer;
    WORD period;
    SWORD sample;
} Noise;

typedef struct {
    BYTE enabled;
    BYTE silence;
    BYTE shift;
    BYTE irq_disabled;
    BYTE irq_throw;
    BYTE loop_enabled;
    WORD sample_address;
    WORD sample_address_start;
    BYTE sample_buffer;
    BYTE sample_length;
    BYTE sample_length_start;
    WORD period;
    WORD timer;
    WORD dac;
    WORD dac_counter;
    SWORD sample;
} DMC;

typedef struct {
    Square square1, square2;
    Triangle triangle;
    Noise noise;
    DMC dmc;
    BYTE irq_disable;

    BYTE frame_irq_throw;
    BYTE frame_mode;
    int frame_counter;
    int frame_tick;
    int last_frame_tick;
    BYTE push_tick;

    int64_t hipass_strong;
    int64_t hipass_weak;

    double pulse_out[31];
    double tnd_out[203];
    SWORD sample;
} APU;

void APU_Init();
void APU_Step();
void APU_MixOutput();
void APU_ClockSquare(Square*);
void APU_ClockNoise();
void APU_ClockTriangle();
void APU_ClockDMC();

BYTE APU_Read();
void APU_Write(WORD addr, BYTE val);
void APU_WriteSquareControl(Square*, BYTE);
void APU_WriteSquareSweep(Square*, BYTE);
void APU_WriteSquareLow(Square*, BYTE);
void APU_WriteSquareHigh(Square*, BYTE);
void APU_WriteTriangleControl(BYTE);
void APU_WriteTriangleLow(BYTE);
void APU_WriteTriangleHigh(BYTE);
void APU_WriteNoiseBase(BYTE);
void APU_WriteNoisePeriod(BYTE);
void APU_WriteNoiseLength(BYTE);
void APU_WriteDMCBase(BYTE);
void APU_WriteDMCSample(BYTE);
void APU_WriteDMCSampleAddress(BYTE);
void APU_WriteDMCSampleLength(BYTE);
void APU_WriteFlags1(BYTE);
void APU_WriteFlags2(BYTE);

void APU_HiPassStrong();
void APU_HiPassWeak();
void APU_Push();
void APU_FrameSequencerStep();
void APU_ClockSweep(Square*);
void APU_ClockDecay(Envelope*);
void APU_ClockLengthsAndSweeps();
void APU_ClockLinearCounter();

#endif
