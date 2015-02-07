#include "apu.h"

BYTE length_lookup[] = {
    0x0A, 0xFE,
    0x14, 0x02,
    0x28, 0x04,
    0x50, 0x06,
    0xA0, 0x08,
    0x3C, 0x0A,
    0x0E, 0x0C,
    0x1A, 0x0E,
    0x0C, 0x10,
    0x18, 0x12,
    0x30, 0x14,
    0x60, 0x16,
    0xC0, 0x18,
    0x48, 0x1A,
    0x10, 0x1C,
    0x20, 0x1E
};

WORD noise_period_lookup[] = { 0x004, 0x008, 0x010, 0x020, 0x040, 0x060, 0x080, 0x0A0, 0x0CA, 0x0FE, 0x17C, 0x1FC, 0x2FA, 0x3F8, 0x7F2, 0xFE4 };

WORD dmc_period_lookup[] = { 0x1AC, 0x17C, 0x154, 0x140, 0x11E, 0x0FE, 0x0E2, 0x0D6, 0x0BE, 0x0A0, 0x08E, 0x080, 0x06A, 0x054, 0x048, 0x036 };

BYTE square_lookup[] = {
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0,
    1, 0, 0, 1, 1, 1, 1, 1
};

BYTE triangle_lookup[] = { 0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

APU apu;

void APU_Init() {

    for (int i = 0; i < sizeof(apu.pulse_out)/sizeof(double); i++) {
        apu.pulse_out[i] = 95.52 / (8128.0 / i + 100);
    }
    for (int i = 0; i < sizeof(apu.tnd_out)/sizeof(double); i++) {
        apu.tnd_out[i] = 163.67 / (24329.0 / i + 100);
    }
    apu.square1.num = 1;
    apu.square2.num = 2;
    apu.noise.shift = 1;
    apu.dmc.dac_counter = 0;
}

void APU_Step() {
    if (apu.square1.enabled)
        APU_ClockSquare(&apu.square1);

    if (apu.square2.enabled)
        APU_ClockSquare(&apu.square2);

    if (apu.triangle.enabled)
        APU_ClockTriangle();

    if (apu.noise.enabled)
        APU_ClockNoise();

    //if (apu.dmc.enabled)
        //APU_ClockDMC();

}

void APU_Push() {
    APU_MixOutput();
    APU_HiPassStrong();
    APU_HiPassWeak();
    Audio_AddSample(apu.sample);
}

void APU_HiPassStrong() {
    SWORD sample = apu.sample;
    apu.hipass_strong += ((((int64_t)sample << 16) - (apu.hipass_strong >> 16 )) * HI_PASS_STRONG) >> 16;
    apu.sample = (SWORD)((int64_t)sample - (apu.hipass_strong >> 32));
}

void APU_HiPassWeak() {
    SWORD sample = apu.sample;
    apu.hipass_weak += ((((int64_t)sample << 16) - (apu.hipass_weak >> 16 )) * HI_PASS_WEAK) >> 16;
    apu.sample = (SWORD)((int64_t)sample - (apu.hipass_weak >> 32));
}

void APU_MixOutput() {
    apu.sample = (apu.pulse_out[apu.square1.sample + apu.square2.sample] + apu.tnd_out[3 * apu.triangle.sample + 2 * apu.noise.sample]) * 40000;
}

void APU_FrameSequencerStep() {
    apu.frame_tick++;

        if (apu.frame_tick == 1 || apu.frame_tick == apu.frame_counter - 1)
            APU_ClockLengthsAndSweeps();

        APU_ClockDecay(&apu.square1.envelope);
        APU_ClockDecay(&apu.square2.envelope);
        APU_ClockDecay(&apu.noise.envelope);
        APU_ClockLinearCounter();

        if (apu.frame_tick >= apu.frame_counter)
            apu.frame_tick = 0;
}

void APU_ClockLengthsAndSweeps() {
    if (apu.square1.length_enabled && apu.square1.length > 0)
        apu.square1.length--;

    if (apu.square2.length_enabled && apu.square2.length > 0)
        apu.square2.length--;

    if (apu.triangle.length_enabled && apu.triangle.length > 0)
        apu.triangle.length--;

    if(apu.noise.length_enabled && apu.noise.length > 0)
        apu.noise.length--;

    APU_ClockSweep(&apu.square1);
    APU_ClockSweep(&apu.square2);
}

void APU_ClockSweep(Square* square) {
    square->sweep_counter--;

    if (square->sweep_counter == 0) {
        square->sweep_counter = square->sweep_period + 1;
        if (square->sweep_enabled && square->sweep_counter > 0 && square->shift > 0) {
            WORD delta = square->timer >> square->shift;

            if (square->negative)
                square->timer -= delta;
            else if (square->timer + delta < 0x800)
                square->timer += delta;
        }
    }

    if (square->sweep_reload) {
        square->sweep_reload = 0;
        square->sweep_counter = square->sweep_period + 1;
    }
}

void APU_ClockDecay(Envelope* envelope) {
    envelope->decay_counter--;

    if (envelope->reset) {
        envelope->reset = 0;
        envelope->decay_counter = envelope->decay_rate + 1;
        envelope->counter = 0xF;
    }
    else if (envelope->decay_counter == 0) {
        envelope->decay_counter = envelope->decay_rate + 1;

        if (envelope->counter > 0)
            envelope->counter--;
        else if (envelope->loop_enabled)
            envelope->counter = 0xF;
    }

    if (envelope->decay_enabled)
        envelope->volume = envelope->counter;
    else
        envelope->volume = envelope->decay_rate;
}

void APU_ClockSquare(Square* square) {
    if (square->length > 0 && square->timer > 7) {
        if (square->timer_counter == 0) {
            square->duty_count = (square->duty_count + 1) & 0x7;
            square->timer_count = (square->timer + 1) << 1;
        }

        if (square->timer < 8)
            square->sample = 0;
        else if (square_lookup[(square->duty_cycle * 8) + square->duty_count])
            square->sample = square->envelope.volume;
        else
            square->sample = 0;

        square->timer_count--;
    }
    else
        square->sample = 0;
}

void APU_ClockTriangle() {
    if (apu.triangle.length > 0 && apu.triangle.counter > 0) {
        if (apu.triangle.timer_count == 0) {
            apu.triangle.lookup_counter = (apu.triangle.lookup_counter + 1) % 32;
            apu.triangle.timer_count = (apu.triangle.timer + 1) * 2;
        }
        apu.triangle.sample = triangle_lookup[apu.triangle.lookup_counter];
        apu.triangle.timer_count--;
    }
}

void APU_ClockLinearCounter() {
    if (apu.triangle.halt)
        apu.triangle.counter = apu.triangle.reload_value;
    else if (apu.triangle.counter > 0)
        apu.triangle.counter--;

    if (!apu.triangle.control)
        apu.triangle.halt = 0;
}

void APU_ClockNoise() {
    WORD feedback, tmp;

    if (apu.noise.length == 0) {
        apu.noise.sample = 0;
        return;
    }

    if ((apu.noise.shift & 0x1) == 0x0)
        apu.noise.sample = apu.noise.envelope.volume;
    else
        apu.noise.sample = 0;

    apu.noise.timer_count--;

    if (apu.noise.timer_count == 0) {
        if (apu.noise.mode)
            tmp = apu.noise.shift & 0x40 >> 6;
        else
            tmp = apu.noise.shift & 0x2 >> 1;

        feedback = apu.noise.shift & 0x1 ^ tmp;
        apu.noise.shift = (apu.noise.shift >> 1) | (feedback << 14);
        apu.noise.timer_count = apu.noise.timer;
    }
}

void APU_ClockDMC() {
    /* DMA Reader */
    if (apu.dmc.sample_buffer == 0 && apu.dmc.sample_length > 0) {
        apu.dmc.sample_buffer = Memory_ReadByte(MAP_CPU, apu.dmc.sample_address);
        // cpu.cycles += 4;
        apu.dmc.sample_address++;
        if (apu.dmc.sample_address > 0xFFFF)
            apu.dmc.sample_address = 0x8000;
        apu.dmc.sample_length--;
        if (apu.dmc.sample_length == 0) {
            if (apu.dmc.loop_enabled) {
                apu.dmc.sample_address = apu.dmc.sample_address_start;
                apu.dmc.sample_length = apu.dmc.sample_length_start;
            }
            else if (!apu.dmc.irq_disabled)
                apu.dmc.irq_throw = 1;
        }
    }

    /* Output Unit */
    apu.dmc.timer--;
    if (apu.dmc.timer == 0) {
        if (apu.dmc.dac_counter == 0) {
            apu.dmc.dac_counter = 8;
            if (apu.dmc.sample_buffer) {
                apu.dmc.shift = apu.dmc.sample_buffer;
                apu.dmc.sample_buffer = 0;
                apu.dmc.silence = 0;
            }
            else
                apu.dmc.silence = 1;
        }
        apu.dmc.timer = apu.dmc.period;
        if (!apu.dmc.silence) {
            if (apu.dmc.shift & 0x01) {
                if (apu.dmc.dac < 126)
                    apu.dmc.dac += 2;
            }
            else if (apu.dmc.dac > 1)
                apu.dmc.dac -= 2;

            apu.dmc.sample = apu.dmc.dac;
        }
        else
            apu.dmc.sample = 0;

        apu.dmc.shift = apu.dmc.shift >> 1;

        apu.dmc.dac_counter--;
        apu.dmc.timer = apu.dmc.period;
    }

}

void APU_Write(WORD addr, BYTE val) {
    switch (addr & 0x00FF) {
        case 0x00:
            APU_WriteSquareControl(&apu.square1, val); break;
        case 0x01:
            APU_WriteSquareSweep(&apu.square1, val); break;
        case 0x02:
            APU_WriteSquareLow(&apu.square1, val); break;
        case 0x03:
            APU_WriteSquareHigh(&apu.square1, val); break;
        case 0x04:
            APU_WriteSquareControl(&apu.square2, val); break;
        case 0x05:
            APU_WriteSquareSweep(&apu.square2, val); break;
        case 0x06:
            APU_WriteSquareLow(&apu.square2, val); break;
        case 0x07:
            APU_WriteSquareHigh(&apu.square2, val); break;
        case 0x08:
            APU_WriteTriangleControl(val); break;
        case 0x0A:
            APU_WriteTriangleLow(val); break;
        case 0x0B:
            APU_WriteTriangleHigh(val); break;
        case 0x0C:
            APU_WriteNoiseBase(val); break;
        case 0x0E:
            APU_WriteNoisePeriod(val); break;
        case 0x0F:
            APU_WriteNoiseLength(val); break;
        case 0x10:
            APU_WriteDMCBase(val); break;
        case 0x11:
            APU_WriteDMCSample(val); break;
        case 0x12:
            APU_WriteDMCSampleAddress(val); break;
        case 0x13:
            APU_WriteDMCSampleLength(val); break;
        case 0x15:
            APU_WriteFlags1(val); break;
        case 0x17:
            APU_WriteFlags2(val); break;

    }
}

void APU_WriteSquareControl(Square* square, BYTE val) {
    /* ddle nnnn   duty, loop env/disable length, env disable, vol/env */
    square->envelope.disabled = (((val >> 4) & 0x1) == 1);
    square->length_enabled = ((val & 0x20) != 0x20);
    square->envelope.loop_enabled = ((val & 0x20) == 0x20);
    square->duty_cycle = (val >> 6) & 0x3;
    square->envelope.decay_rate = val & 0xF;
    square->envelope.decay_enabled = ((val & 0x10) == 0);

    if (square->envelope.decay_enabled)
        square->envelope.volume = square->envelope.decay_rate;
    else
        square->envelope.volume = val & 0xF;
}

void APU_WriteSquareSweep(Square* square, BYTE val) {
    /* eppp nsss   enable sweep, period, negative, shift */
    square->sweep_enabled = ((val & 0x80) == 0x80);
    square->sweep_period = (val >> 4) & 0x7;
    square->sweep_mode = (val >> 3) & 1;
    square->negative = ((val & 0x10) == 0x10);
    square->shift = val & 0x7;

    square->sweep_reload = 1;
}

void APU_WriteSquareLow(Square* square, BYTE val) {
    /* pppp pppp   period low */
    square->timer = (square->timer & 0x700) | val;
}

void APU_WriteSquareHigh(Square* square, BYTE val) {
    /* llll lppp   length index, period high */
    square->timer = (square->timer & 0xFF) | ((val & 0x07) << 8);

    if(square->enabled)
        square->length = square_lookup[val >> 3];

    square->duty_count = 0;
    square->envelope.reset = 1;
}

void APU_WriteTriangleControl(BYTE val) {
    /* clll llll   control, linear counter load */
    apu.triangle.reload_value = val & 0x7F;
    apu.triangle.control = (val & 0x80) != 0;
    apu.triangle.length_enabled = !apu.triangle.control;
}

void APU_WriteTriangleLow(BYTE val) {
    /* pppp pppp   period low */
    apu.triangle.timer = (apu.triangle.timer & 0x700) | val;
}

void APU_WriteTriangleHigh(BYTE val) {
    /* llll lppp   length index, period high */
    if (apu.triangle.enabled)
        apu.triangle.length = length_lookup[val >> 3];
    apu.triangle.timer = (apu.triangle.timer & 0x00FF) | ((val & 0x07) << 8);
}

void APU_WriteNoiseBase(BYTE val) {
/* --le nnnn   loop env/disable length, env disable, vol/env period */
    apu.noise.length_enabled = ((val & 0x20) != 0x20);
    apu.noise.envelope.counter = val & 0x1F;
    apu.noise.envelope.loop_enabled = ((val & 0x20) == 0x20);
    apu.noise.envelope.decay_rate = val & 0xF;
    apu.noise.base_envelope = apu.noise.envelope.decay_rate;
    apu.noise.envelope.decay_enabled = ((val & 0x10) == 0);
}

void APU_WriteNoisePeriod(BYTE val) {
    /* s--- pppp   short mode, period index */
    apu.noise.mode = ((val & 0x80) == 0x80);
    apu.noise.timer = noise_period_lookup[val & 0xF];
    apu.noise.timer_count = apu.noise.timer;
}

void APU_WriteNoiseLength(BYTE val) {
    /* llll l---   length index */
    apu.noise.length = length_lookup[val >> 3];
}

void APU_WriteDMCBase(BYTE val) {
    /* il-- ffff   IRQ enable, loop, frequency index */
    apu.dmc.irq_disabled = ~val >> 7;
    apu.dmc.loop_enabled = (val >> 6) & 0x01;
    apu.dmc.period = dmc_period_lookup[val & 0x0F];
    apu.dmc.timer = apu.dmc.period;
}

void APU_WriteDMCSample(BYTE val) {
    /* -ddd dddd   DAC */
    apu.dmc.dac = val & 0x7F;
    apu.dmc.sample = ((SWORD)(apu.dmc.dac << 1)) + ((SWORD)val & 0x1);
}

void APU_WriteDMCSampleAddress(BYTE val) {
    /* aaaa aaaa   sample address */
    apu.dmc.sample_address = apu.dmc.sample_address_start = val * 0x40 + 0xC000;
}

void APU_WriteDMCSampleLength(BYTE val) {
    /* llll llll   sample length */
    apu.dmc.sample_length = apu.dmc.sample_length_start = val * 0x10 + 1;
}

void APU_WriteFlags1(BYTE val) {
    /* ---d nt21   length ctr enable: DMC, noise, triangle, pulse 2, 1 */
    apu.dmc.enabled = (val >> 4) & 0x01;
    apu.noise.enabled = (val >> 3) & 0x01;
    apu.triangle.enabled = (val >> 2) & 0x01;
    apu.square2.enabled = (val >> 1) & 0x01;
    apu.square1.enabled = val & 0x01;
}

void APU_WriteFlags2(BYTE val) {
    /* fd-- ----   5-frame cycle, disable frame interrupt */
    apu.frame_mode = val >> 7;
    apu.irq_disable = (val >> 6) & 0x01;
    if(apu.frame_mode) {
        apu.frame_counter = 5;
        APU_FrameSequencerStep();
    } else {
        apu.frame_counter = 4;
    }
}

BYTE APU_Read() {
    BYTE val = 0;
    /* if-d nt21   DMC IRQ, frame IRQ, length counter statuses */
    val |= apu.dmc.irq_throw << 7;
    val |= apu.frame_irq_throw << 6;
    val |= apu.dmc.sample_length << 4;
    val |= apu.noise.length << 3;
    val |= apu.triangle.length << 2;
    val |= apu.square2.length << 1;
    val |= apu.square1.length;
    return val;
}
