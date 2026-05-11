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
#include <lasxintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int16_t test_values_h[] = {0, 1, 2, 3, -1, -2, 32767, -32768, 0, 1, 2, 3, -1, -2, 32767, -32768};
static const int32_t test_values_w[] = {0, 1, 2, 3, -1, -2, 2147483647, -2147483648};

int check_xvmulwev_w_h(int16_t* a, int16_t* b, int32_t* result)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        int j = i * 2;
        int32_t expected = (int32_t)a[j] * (int32_t)b[j];
        if (result[i] != expected) {
            printf("xvmulwev.w.h[%d]: got %d, expected %d (a=%d, b=%d)\n", i, result[i], expected, a[j], b[j]);
            errors++;
        }
    }
    return errors;
}

int check_xvmulwod_w_h(int16_t* a, int16_t* b, int32_t* result)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        int j = i * 2 + 1;
        int32_t expected = (int32_t)a[j] * (int32_t)b[j];
        if (result[i] != expected) {
            printf("xvmulwod.w.h[%d]: got %d, expected %d (a=%d, b=%d)\n", i, result[i], expected, a[j], b[j]);
            errors++;
        }
    }
    return errors;
}

int check_xvmulwev_d_w(int32_t* a, int32_t* b, int64_t* result)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        int j = i * 2;
        int64_t expected = (int64_t)a[j] * (int64_t)b[j];
        if (result[i] != expected) {
            printf("xvmulwev.d.w[%d]: got %" PRId64 ", expected %" PRId64 " (a=%d, b=%d)\n", i, result[i], expected, a[j], b[j]);
            errors++;
        }
    }
    return errors;
}

int check_xvmulwod_d_w(int32_t* a, int32_t* b, int64_t* result)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        int j = i * 2 + 1;
        int64_t expected = (int64_t)a[j] * (int64_t)b[j];
        if (result[i] != expected) {
            printf("xvmulwod.d.w[%d]: got %" PRId64 ", expected %" PRId64 " (a=%d, b=%d)\n", i, result[i], expected, a[j], b[j]);
            errors++;
        }
    }
    return errors;
}

int main()
{
    unsigned char mem[96] __attribute__((aligned(32)));

    printf("=== Extended multiplication tests (widening) ===\n");

    memset(mem, 0, sizeof(mem));
    memcpy(mem, test_values_h, 32);
    memcpy(mem + 32, test_values_h, 32);
    __m256i a = __lasx_xvld(mem, 0);
    __m256i b = __lasx_xvld(mem, 32);
    __m256i r = __lasx_xvmulwev_w_h(a, b);
    __lasx_xvst(r, mem, 64);
    int errors = check_xvmulwev_w_h((int16_t*)mem, (int16_t*)mem + 32, (int32_t*)(mem + 64));
    printf("xvmulwev.w.h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    memcpy(mem, test_values_h, 32);
    memcpy(mem + 32, test_values_h, 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    r = __lasx_xvmulwod_w_h(a, b);
    __lasx_xvst(r, mem, 64);
    errors = check_xvmulwod_w_h((int16_t*)mem, (int16_t*)mem + 32, (int32_t*)(mem + 64));
    printf("xvmulwod.w.h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    memcpy(mem, test_values_w, 32);
    memcpy(mem + 32, test_values_w, 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    r = __lasx_xvmulwev_d_w(a, b);
    __lasx_xvst(r, mem, 64);
    errors = check_xvmulwev_d_w((int32_t*)mem, (int32_t*)(mem + 32), (int64_t*)(mem + 64));
    printf("xvmulwev.d.w: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    memcpy(mem, test_values_w, 32);
    memcpy(mem + 32, test_values_w, 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    r = __lasx_xvmulwod_d_w(a, b);
    __lasx_xvst(r, mem, 64);
    errors = check_xvmulwod_d_w((int32_t*)mem, (int32_t*)(mem + 32), (int64_t*)(mem + 64));
    printf("xvmulwod.d.w: %s\n", errors ? "FAILED" : "PASSED");

    return 0;
}