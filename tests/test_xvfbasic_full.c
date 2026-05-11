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

#include <float.h>
#include <lasxintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TEST_VALUES 6
static const float test_values_f[NUM_TEST_VALUES] = {1.0f, 2.0f, -1.0f, 0.0f, 0.5f, 3.14159f};
static const double test_values_d[NUM_TEST_VALUES] = {1.0, 2.0, -1.0, 0.0, 0.5, 3.14159};
static const int16_t test_values_i16[NUM_TEST_VALUES] = {1, -1, 0, 32767, -32768, 100};

void init_data_f(float* a, float* b, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_f[i % NUM_TEST_VALUES];
        b[i] = test_values_f[(i + 3) % NUM_TEST_VALUES];
    }
}

void init_data_d(double* a, double* b, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_d[i % NUM_TEST_VALUES];
        b[i] = test_values_d[(i + 3) % NUM_TEST_VALUES];
    }
}

void init_data_i16(int16_t* a, int16_t* b, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_i16[i % NUM_TEST_VALUES];
        b[i] = test_values_i16[(i + 3) % NUM_TEST_VALUES];
    }
}

int check_recippe_s(float* c, int count)
{
    int errors = 0;
    float* a = (float*)((char*)c - 64);
    for (int i = 0; i < count; i++) {
        float expected = 1.0f / a[i];
        float diff = fabsf(c[i] - expected);
        float rel = (expected != 0) ? diff / fabsf(expected) : diff;
        if (rel > 1e-3f) {
            printf("F[%d]: got %f, expected %f (rel=%e)\n", i, c[i], expected, rel);
            errors++;
        }
    }
    return errors;
}

int check_rsqrt_s(float* c, int count)
{
    int errors = 0;
    float* a = (float*)((char*)c - 64);
    for (int i = 0; i < count; i++) {
        if (a[i] < 0)
            continue;
        float expected = 1.0f / sqrtf(a[i]);
        float diff = fabsf(c[i] - expected);
        float rel = (expected != 0) ? diff / fabsf(expected) : diff;
        if (rel > 1e-3f) {
            printf("F[%d]: got %f, expected %f (rel=%e)\n", i, c[i], expected, rel);
            errors++;
        }
    }
    return errors;
}

int check_rsqrt_d(double* c, int count)
{
    int errors = 0;
    double* a = (double*)((char*)c - 64);
    for (int i = 0; i < count; i++) {
        if (a[i] < 0)
            continue;
        double expected = 1.0 / sqrt(a[i]);
        double diff = fabs(c[i] - expected);
        double rel = (expected != 0) ? diff / fabs(expected) : diff;
        if (rel > 1e-3) {
            printf("D[%d]: got %f, expected %f (rel=%e)\n", i, c[i], expected, rel);
            errors++;
        }
    }
    return errors;
}

int check_exth_s(int32_t* c, int count)
{
    int errors = 0;
    int16_t* a = (int16_t*)((char*)c - 32);
    for (int i = 0; i < count; i++) {
        int32_t expected = (int32_t)a[i];
        if (c[i] != expected) {
            printf("S[%d]: got %d, expected %d\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_exth_d(int64_t* c, int count)
{
    int errors = 0;
    int16_t* a = (int16_t*)((char*)c - 32);
    for (int i = 0; i < count; i++) {
        int64_t expected = (int64_t)a[i];
        if (c[i] != expected) {
            printf("D[%d]: got %ld, expected %ld\n", i, (long)c[i], (long)expected);
            errors++;
        }
    }
    return errors;
}

int main()
{
    printf("=== Extended Float Basic Tests ===\n");

    unsigned char mem[96] __attribute__((aligned(32)));

    printf("\n--- Approximate Reciprocal (.w) ---\n");
    memset(mem, 0, sizeof(mem));
    init_data_f((float*)mem, (float*)(mem + 32), 8);
    __m256 v = *(__m256*)mem;
    __m256 r = __lasx_xvfrecip_s(v);
    __lasx_xvst((__m256i)r, mem, 64);
    int errors = check_recippe_s((float*)(mem + 64), 8);
    printf("xvrecippe.w: %s\n", errors ? "FAILED" : "PASSED");

    printf("\n--- Reciprocal Square Root ---\n");
    memset(mem, 0, sizeof(mem));
    init_data_f((float*)mem, (float*)(mem + 32), 8);
    v = *(__m256*)mem;
    r = __lasx_xvfrsqrt_s(v);
    __lasx_xvst((__m256i)r, mem, 64);
    errors = check_rsqrt_s((float*)(mem + 64), 8);
    printf("xvrsqrt.s: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data_d((double*)mem, (double*)(mem + 32), 4);
    __m256d vd = *(__m256d*)mem;
    __m256d rd = __lasx_xvfrsqrt_d(vd);
    __lasx_xvst((__m256i)rd, mem, 64);
    errors = check_rsqrt_d((double*)(mem + 64), 4);
    printf("xvrsqrt.d: %s\n", errors ? "FAILED" : "PASSED");

    printf("\n--- Sign Extend Half ---\n");
    memset(mem, 0, sizeof(mem));
    init_data_i16((int16_t*)mem, (int16_t*)(mem + 16), 16);
    __m256i vi = *(__m256i*)mem;
    __m256i ri = __lasx_xvexth_w_h(vi);
    __lasx_xvst(ri, mem, 32);
    errors = check_exth_s((int32_t*)(mem + 32), 16);
    printf("xvexth.s: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data_i16((int16_t*)mem, (int16_t*)(mem + 16), 16);
    vi = *(__m256i*)mem;
    ri = __lasx_xvexth_d_w(vi);
    __lasx_xvst(ri, mem, 32);
    errors = check_exth_d((int64_t*)(mem + 32), 16);
    printf("xvexth.d: %s\n", errors ? "FAILED" : "PASSED");

    return errors;
}