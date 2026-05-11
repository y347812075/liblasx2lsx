/* SPDX-License-Identifier: MIT
 *
 * liblasx2lsx — LASX instruction emulation library for LoongArch
 * Copyright (c) 2026 liblasx2lsx contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#include <lasxintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_data(uint8_t* a, uint8_t* b, int size)
{
    for (int i = 0; i < size; i++) {
        a[i] = i * 13 + 7;
        b[i] = i * 17 + 3;
    }
}

static inline int8_t sat_b(int val)
{
    if (val > 0x7f)
        return 0x7f;
    if (val < -0x80)
        return -0x80;
    return (int8_t)val;
}

static inline int16_t sat_h(int val)
{
    if (val > 0x7fff)
        return 0x7fff;
    if (val < -0x8000)
        return -0x8000;
    return (int16_t)val;
}

static inline int32_t sat_w(int val)
{
    if (val > 0x7fffffff)
        return 0x7fffffff;
    if (val < -0x80000000)
        return -0x80000000;
    return (int32_t)val;
}

static inline uint8_t sat_bu(int val)
{
    if (val > 0xff)
        return 0xff;
    if (val < 0)
        return 0;
    return (uint8_t)val;
}

int main()
{
    unsigned char mem[96] __attribute__((aligned(32)));

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    __m256i a = __lasx_xvld(mem, 0);
    __m256i b = __lasx_xvld(mem, 32);
    __m256i r = __lasx_xvsadd_b(a, b);
    __lasx_xvst(r, mem, 64);
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        int expected = (int)((int8_t)mem[i]) + (int)((int8_t)(mem + 32)[i]);
        expected = sat_b(expected);
        if (((int8_t*)(mem + 64))[i] != (int8_t)expected) {
            printf("xvsadd.b[%d]: got %d, expected %d\n", i, ((int8_t*)(mem + 64))[i], (int8_t)expected);
            errors++;
        }
    }
    printf("xvsadd.b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    r = __lasx_xvsadd_h(a, b);
    __lasx_xvst(r, mem, 64);
    errors = 0;
    for (int i = 0; i < 16; i++) {
        int expected = (int)((int16_t*)mem)[i] + (int)((int16_t*)(mem + 32))[i];
        expected = sat_h(expected);
        if (((int16_t*)(mem + 64))[i] != (int16_t)expected) {
            printf("xvsadd.h[%d]: got %d, expected %d\n", i, ((int16_t*)(mem + 64))[i], (int16_t)expected);
            errors++;
        }
    }
    printf("xvsadd.h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    r = __lasx_xvsadd_w(a, b);
    __lasx_xvst(r, mem, 64);
    errors = 0;
    for (int i = 0; i < 8; i++) {
        int64_t expected = (int64_t)((int32_t*)mem)[i] + (int64_t)((int32_t*)(mem + 32))[i];
        expected = sat_w((int)expected);
        if (((int32_t*)(mem + 64))[i] != (int32_t)expected) {
            printf("xvsadd.w[%d]: got %d, expected %d\n", i, ((int32_t*)(mem + 64))[i], (int32_t)expected);
            errors++;
        }
    }
    printf("xvsadd.w: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    r = __lasx_xvsadd_bu(a, b);
    __lasx_xvst(r, mem, 64);
    errors = 0;
    for (int i = 0; i < 32; i++) {
        int expected = (int)mem[i] + (int)(mem + 32)[i];
        expected = sat_bu(expected);
        if (((uint8_t*)(mem + 64))[i] != (uint8_t)expected) {
            printf("xvsadd.bu[%d]: got %u, expected %u\n", i, ((uint8_t*)(mem + 64))[i], (uint8_t)expected);
            errors++;
        }
    }
    printf("xvsadd.bu: %s\n", errors ? "FAILED" : "PASSED");

    return 0;
}