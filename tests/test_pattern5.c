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
#include <string.h>
#define N 100
static float A[N * 8], B[N * 8];
static float C[16] __attribute__((aligned(32)));
extern void run_loop5(float *a, float *b, float *c, int n, int stride);
int main() {
    for (int i = 0; i < N * 8; i++) { A[i] = i + 1; B[i] = i + 1; }
    memset(C, 0, sizeof(C));
    run_loop5(A, B, C, N, 32);
    printf("Pattern 5: done\n");
    return 0;
}
