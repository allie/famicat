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

WORD dmc_frequency_lookup[] = { 0x1AC, 0x17C, 0x154, 0x140, 0x11E, 0x0FE, 0x0E2, 0x0D6, 0x0BE, 0x0A0, 0x08E, 0x080, 0x06A, 0x054, 0x048, 0x036 };

BYTE square_lookup[] = {
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0,
    1, 0, 0, 1, 1, 1, 1, 1
};

BYTE triangle_lookup[] = { 0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

APU apu;

void APU_Init() {

    for (int i = 0; i < sizeof(apu.pulseOut)/sizeof(double); i++) {
        apu.pulseOut[i] = 95.52 / (8128.0 / i + 100);
    }
    for (int i = 0; i < sizeof(apu.tndOut)/sizeof(double); i++) {
        apu.tndOut[i] = 163.67 / (24329.0 / i + 100);
    }
}

void APU_Step() {
    if (apu.square1.enabled) {
        APU_Clock_Square(apu.square1, 1);
    }
    if (apu.square2.enabled) {
        APU_Clock_Square(apu.square2, 2);
    }
    if (apu.triangle.enabled) {
        APU_Clock_Triangle();
    }
    if (apu.noise.enabled) {
        APU_Clock_Noise();
    }
    if (apu.dmc.enabled) {
        APU_Clock_DMC();
    }
}

void dac_MixOutput() {
    apu.output = apu.pulseOut[apu.square1.dac + apu.square2.dac] + apu.tndOut[3 * apu.triangle.dac + 2 * apu.noise.dac + apu.dmc.dac];
}

void APU_Clock_Square(Square square, BYTE square_num) {

}

void APU_Clock_Triangle() {

}

void APU_Clock_Noise() {

}

void APU_Clock_DMC() {

}

void APU_Write() {
    APU_Write_Square1();
    APU_Write_Square2();
    APU_Write_Triangle();
    APU_Write_Noise();
    APU_Write_DMC();
    APU_Write_Common();
}

void APU_Write_Square1() {
    BYTE tmp;
    /* ddle nnnn   duty, loop env/disable length, env disable, vol/env */
    tmp = Memory_ReadByte(0x4000);
    apu.square1.duty = tmp >> 6;
    apu.square1.envelope.loop = apu.square1.length_halt = (tmp >> 5) & 0x01;
    apu.square1.envelope.disable = (tmp >> 4) & 0x01;
    apu.square1.envelope.volume = tmp & 0x0F;
    apu.square1.envelope.period = apu.square1.envelope.volume + 1;

    /* eppp nsss   enable sweep, period, negative, shift */
    tmp = Memory_ReadByte(0x4001);
    apu.square1.sweep_enable = tmp >> 7;
    apu.square1.sweep_period = (tmp >> 4) & 0x07;
    apu.square1.negate = (tmp >> 3) & 0x01;
    apu.square1.shift = tmp & 0x07;

    /* pppp pppp   period low */
    tmp = Memory_ReadByte(0x4002);
    apu.square1.timer_period = tmp;

    /* llll lppp   length index, period high */
    tmp = Memory_ReadByte(0x4003);
    apu.square1.length = length_lookup[tmp >> 3];
    apu.square1.timer_period |= (tmp & 0x07) << 8;
}

void APU_Write_Square2() {
    BYTE tmp;
    /* ddle nnnn   duty, loop env/disable length, env disable, vol/env */
    tmp = Memory_ReadByte(0x4004);
    apu.square2.duty = tmp >> 6;
    apu.square2.envelope.loop = apu.square2.length_halt = (tmp >> 5) & 0x01;
    apu.square2.envelope.disable = (tmp >> 4) & 0x01;
    apu.square2.envelope.volume = tmp & 0x0F;
    apu.square2.envelope.period = apu.square2.envelope.volume + 1;

    /* eppp nsss   enable sweep, period, negative, shift */
    tmp = Memory_ReadByte(0x4005);
    apu.square2.sweep_enable = tmp >> 7;
    apu.square2.sweep_period = (tmp >> 4) & 0x07;
    apu.square2.negate = (tmp >> 3) & 0x01;
    apu.square2.shift = tmp & 0x07;

    /* pppp pppp   period low */
    tmp = Memory_ReadByte(0x4006);
    apu.square2.timer_period = tmp;

    /* llll lppp   length index, period high */
    tmp = Memory_ReadByte(0x4007);
    apu.square2.length = length_lookup[tmp >> 3];
    apu.square2.timer_period |= (tmp & 0x07) << 8;
}

void APU_Write_Triangle() {
    BYTE tmp;
    /* clll llll   control, linear counter load */
    tmp = Memory_ReadByte(0x4008);
    apu.triangle.control = apu.triangle.length_halt = tmp >> 7;
    apu.triangle.linear_reload = tmp & 0x7F;

    /* pppp pppp   period low */
    tmp = Memory_ReadByte(0x400A);
    apu.triangle.timer_period = tmp;

    /* llll lppp   length index, period high */
    tmp = Memory_ReadByte(0x400B);
    apu.triangle.length = length_lookup[tmp >> 3];
    apu.triangle.timer_period |= tmp & 0x07;
}

void APU_Write_Noise() {
    BYTE tmp;
/* --le nnnn   loop env/disable length, env disable, vol/env period */
    tmp = Memory_ReadByte(0x400C);
    apu.noise.envelope.loop = apu.noise.length_halt = tmp >> 5;
    apu.noise.envelope.disable = (tmp >> 4) & 0x01;
    apu.noise.envelope.volume = tmp & 0x0F;
    apu.noise.envelope.period = apu.noise.envelope.volume + 1;

    /* s--- pppp   short mode, period index */
    tmp = Memory_ReadByte(0x400E);
    apu.noise.mode = (tmp >> 7) & 0x01;
    apu.noise.period = noise_period_lookup[tmp & 0x0F];

    /* llll l---   length index */
    tmp = Memory_ReadByte(0x400F);
    apu.noise.length = length_lookup[tmp >> 3];
}

void APU_Write_DMC() {
    BYTE tmp;
    /* il-- ffff   IRQ enable, loop, frequency index */
    tmp = Memory_ReadByte(0x4010);
    apu.dmc.irq_enable = tmp >> 7;
    apu.dmc.loop_enable = (tmp >> 6) & 0x01;
    apu.dmc.frequency = dmc_frequency_lookup[tmp & 0x0F];

    tmp = Memory_ReadByte(0x4011);
    /* -ddd dddd   DAC */
    apu.dmc.dac = tmp;

    /* aaaa aaaa   sample address */
    tmp = Memory_ReadByte(0x4012);
    apu.dmc.sample_address = tmp * 0x40 + 0xC000;

    /* llll llll   sample length */
    tmp = Memory_ReadByte(0x4013);
    apu.dmc.sample_length = tmp * 0x10 + 1;
}

void APU_Write_Common() {
    BYTE tmp;
    /* ---d nt21   length ctr enable: DMC, noise, triangle, pulse 2, 1 */
    tmp = Memory_ReadByte(0x4015);
    apu.dmc.enabled = (tmp >> 4) & 0x01;
    apu.noise.enabled = (tmp >> 3) & 0x01;
    apu.triangle.enabled = (tmp >> 2) & 0x01;
    apu.square2.enabled = (tmp >> 1) & 0x01;
    apu.square1.enabled = tmp & 0x01;

    /* fd-- ----   5-frame cycle, disable frame interrupt */
    tmp = Memory_ReadByte(0x4017);
    apu.frame_mode = tmp >> 7;
    apu.irq_enable = (~tmp >> 6) & 0x01;
}

void APU_Read() {
    BYTE tmp = 0;
    /* if-d nt21   DMC IRQ, frame IRQ, length counter statuses */
    tmp |= apu.dmc.irq_throw << 7;
    tmp |= apu.frame_irq_throw << 6;
    tmp |= apu.dmc.sample_counter << 4;
    tmp |= apu.noise.length << 3;
    tmp |= apu.triangle.length << 2;
    tmp |= apu.square2.length << 1;
    tmp |= apu.square1.length;
}
