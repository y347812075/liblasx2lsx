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

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long mem[128] __attribute__((aligned(256)));

static void asm_xvsll_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsll.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsll_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsll.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsll_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsll.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsll_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsll.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsrl_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsrl.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsrl_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsrl.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsrl_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsrl.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsrl_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsrl.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsra_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsra.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsra_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsra.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsra_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsra.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvsra_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsra.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

int main(void)
{
    int failed = 0;
    uint8_t* a = (uint8_t*)mem;
    uint8_t* b = (uint8_t*)mem + 32;
    uint8_t* c = (uint8_t*)mem + 64;

    for (int i = 0; i < 32; i++) {
        a[i] = (i * 13 + 7) & 0xFF;
        b[i] = i % 8;
    }
    asm_xvsll_b(mem);
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] << b[i];
        if (c[i] != expected) {
            printf("xvsll.b B[%d]: got %u, expected %u\n", i, c[i], expected);
            failed++;
        }
    }
    printf("xvsll.b: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 16; i++) {
        ((uint16_t*)a)[i] = i * 13 + 7;
        ((uint16_t*)b)[i] = i % 16;
    }
    asm_xvsll_h(mem);
    for (int i = 0; i < 16; i++) {
        uint16_t expected = ((uint16_t*)a)[i] << ((uint16_t*)b)[i];
        uint16_t got = ((uint16_t*)c)[i];
        if (got != expected) {
            printf("xvsll.h H[%d]: got %u, expected %u\n", i, got, expected);
            failed++;
        }
    }
    printf("xvsll.h: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 8; i++) {
        ((uint32_t*)a)[i] = i * 13 + 7;
        ((uint32_t*)b)[i] = i % 32;
    }
    asm_xvsll_w(mem);
    for (int i = 0; i < 8; i++) {
        uint32_t expected = ((uint32_t*)a)[i] << ((uint32_t*)b)[i];
        uint32_t got = ((uint32_t*)c)[i];
        if (got != expected) {
            printf("xvsll.w W[%d]: got %u, expected %u\n", i, got, expected);
            failed++;
        }
    }
    printf("xvsll.w: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 4; i++) {
        ((uint64_t*)a)[i] = i * 13 + 7;
        ((uint64_t*)b)[i] = i % 64;
    }
    asm_xvsll_d(mem);
    for (int i = 0; i < 4; i++) {
        uint64_t expected = ((uint64_t*)a)[i] << ((uint64_t*)b)[i];
        uint64_t got = ((uint64_t*)c)[i];
        if (got != expected) {
            printf("xvsll.d D[%d]: got %lu, expected %lu\n", i, (unsigned long)got, (unsigned long)expected);
            failed++;
        }
    }
    printf("xvsll.d: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++) {
        a[i] = (i * 13 + 7) & 0xFF;
        b[i] = i % 8;
    }
    asm_xvsrl_b(mem);
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] >> b[i];
        if (c[i] != expected) {
            printf("xvsrl.b B[%d]: got %u, expected %u\n", i, c[i], expected);
            failed++;
        }
    }
    printf("xvsrl.b: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 16; i++) {
        ((uint16_t*)a)[i] = i * 13 + 7;
        ((uint16_t*)b)[i] = i % 16;
    }
    asm_xvsrl_h(mem);
    for (int i = 0; i < 16; i++) {
        uint16_t expected = ((uint16_t*)a)[i] >> ((uint16_t*)b)[i];
        uint16_t got = ((uint16_t*)c)[i];
        if (got != expected) {
            printf("xvsrl.h H[%d]: got %u, expected %u\n", i, got, expected);
            failed++;
        }
    }
    printf("xvsrl.h: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 8; i++) {
        ((uint32_t*)a)[i] = i * 13 + 7;
        ((uint32_t*)b)[i] = i % 32;
    }
    asm_xvsrl_w(mem);
    for (int i = 0; i < 8; i++) {
        uint32_t expected = ((uint32_t*)a)[i] >> ((uint32_t*)b)[i];
        uint32_t got = ((uint32_t*)c)[i];
        if (got != expected) {
            printf("xvsrl.w W[%d]: got %u, expected %u\n", i, got, expected);
            failed++;
        }
    }
    printf("xvsrl.w: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 4; i++) {
        ((uint64_t*)a)[i] = i * 13 + 7;
        ((uint64_t*)b)[i] = i % 64;
    }
    asm_xvsrl_d(mem);
    for (int i = 0; i < 4; i++) {
        uint64_t expected = ((uint64_t*)a)[i] >> ((uint64_t*)b)[i];
        uint64_t got = ((uint64_t*)c)[i];
        if (got != expected) {
            printf("xvsrl.d D[%d]: got %lu, expected %lu\n", i, (unsigned long)got, (unsigned long)expected);
            failed++;
        }
    }
    printf("xvsrl.d: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    int8_t s8_vals[] = {-1, -2, -4, -8, -16, -32, -64, -128, 127, 63, 31, 15, 7, 3, 1, 0};
    for (int i = 0; i < 32; i++) {
        a[i] = s8_vals[i % 16];
        b[i] = i % 8;
    }
    asm_xvsra_b(mem);
    for (int i = 0; i < 32; i++) {
        int8_t expected = ((int8_t*)a)[i] >> b[i];
        int8_t got = ((int8_t*)c)[i];
        if (got != expected) {
            printf("xvsra.b B[%d]: got %d, expected %d\n", i, (int)got, (int)expected);
            failed++;
        }
    }
    printf("xvsra.b: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    int16_t s16_vals[] = {-1, -2, -4, -8, -16, -32, -64, -128, -256, -512, -1024, -2048, 32767, 16383, 8191, 4095};
    for (int i = 0; i < 16; i++) {
        ((int16_t*)a)[i] = s16_vals[i];
        ((uint16_t*)b)[i] = i % 16;
    }
    asm_xvsra_h(mem);
    for (int i = 0; i < 16; i++) {
        int16_t expected = ((int16_t*)a)[i] >> ((uint16_t*)b)[i];
        int16_t got = ((int16_t*)c)[i];
        if (got != expected) {
            printf("xvsra.h H[%d]: got %d, expected %d\n", i, (int)got, (int)expected);
            failed++;
        }
    }
    printf("xvsra.h: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    int32_t s32_vals[] = {-1, -2, -4, -8, -16, -32, -64, -128, 2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215};
    for (int i = 0; i < 8; i++) {
        ((int32_t*)a)[i] = s32_vals[i];
        ((uint32_t*)b)[i] = i % 32;
    }
    asm_xvsra_w(mem);
    for (int i = 0; i < 8; i++) {
        int32_t expected = ((int32_t*)a)[i] >> ((uint32_t*)b)[i];
        int32_t got = ((int32_t*)c)[i];
        if (got != expected) {
            printf("xvsra.w W[%d]: got %d, expected %d\n", i, (int)got, (int)expected);
            failed++;
        }
    }
    printf("xvsra.w: %s\n", failed ? "FAILED" : "PASSED");
    failed = 0;

    memset(mem, 0, sizeof(mem));
    int64_t s64_vals[] = {-1,
                          -2,
                          -4,
                          -8,
                          -16,
                          -32,
                          -64,
                          -128,
                          9223372036854775807LL,
                          4611686018427387903LL,
                          2305843009213693951LL,
                          1152921504606846975LL,
                          576460752303423487LL,
                          288230376151711743LL,
                          144115188075855871LL,
                          72057594037927935LL};
    for (int i = 0; i < 4; i++) {
        ((int64_t*)a)[i] = s64_vals[i];
        ((uint64_t*)b)[i] = i % 64;
    }
    asm_xvsra_d(mem);
    for (int i = 0; i < 4; i++) {
        int64_t expected = ((int64_t*)a)[i] >> ((uint64_t*)b)[i];
        int64_t got = ((int64_t*)c)[i];
        if (got != expected) {
            printf("xvsra.d D[%d]: got %ld, expected %ld\n", i, (long)got, (long)expected);
            failed++;
        }
    }
    printf("xvsra.d: %s\n", failed ? "FAILED" : "PASSED");

    return 0;
}