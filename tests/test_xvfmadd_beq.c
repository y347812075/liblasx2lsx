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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define N 4
#define ITERATIONS 1000000

static float A[N * 8];
static float B[N * 8];
static float C[8] __attribute__((aligned(32)));

extern void run_loop(float *a, float *b, float *c);

int main(int argc, char *argv[])
{
    for (int i = 0; i < N * 8; i++) {
        A[i] = (float)(i + 1);
        B[i] = (float)(i + 1);
    }

    int cnt = N;
    memset(C, 0, sizeof(C));
    __asm__ volatile("move $t6, %[cnt]" : : [cnt]"r"(cnt));
    run_loop(A, B, C);

    float expected[8] = {0};
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < N; i++) {
            expected[j] += A[i * 8 + j] * B[i * 8 + j];
        }
    }

    int errors = 0;
    for (int j = 0; j < 8; j++) {
        if (C[j] != expected[j]) {
            printf("C[%d]: got %f, expected %f\n", j, C[j], expected[j]);
            errors++;
        }
    }

    if (errors) {
        printf("test_xvfmadd_beq: FAILED\n");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int iter = 0; iter < ITERATIONS; iter++) {
        memset(C, 0, sizeof(C));
        __asm__ volatile("move $t6, %[cnt]" : : [cnt]"r"(N));
        run_loop(A, B, C);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1e9;
    double us_per_iter = elapsed * 1e6 / ITERATIONS;

    printf("test_xvfmadd_beq: PASSED\n");
    printf("%.2f us/iter (%d iters)\n", us_per_iter, ITERATIONS);
    return 0;
}
