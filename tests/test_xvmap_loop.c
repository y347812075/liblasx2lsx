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

#define N 4

float A[N * 8] __attribute__((aligned(32)));
float B[N * 8] __attribute__((aligned(32)));
float C[8] __attribute__((aligned(32)));

extern void run_xvmap_loop(float *a, float *b, float *c, int n);

int main() {
    for (int i = 0; i < N * 8; i++) {
        A[i] = i + 1;
        B[i] = i + 1;
    }
    memset(C, 0, sizeof(C));

    run_xvmap_loop(A, B, C, N);

    float expected[8] = {
        A[0]*B[0] + A[8]*B[8] + A[16]*B[16] + A[24]*B[24],
        A[1]*B[1] + A[9]*B[9] + A[17]*B[17] + A[25]*B[25],
        A[2]*B[2] + A[10]*B[10] + A[18]*B[18] + A[26]*B[26],
        A[3]*B[3] + A[11]*B[11] + A[19]*B[19] + A[27]*B[27],
        A[4]*B[4] + A[12]*B[12] + A[20]*B[20] + A[28]*B[28],
        A[5]*B[5] + A[13]*B[13] + A[21]*B[21] + A[29]*B[29],
        A[6]*B[6] + A[14]*B[14] + A[22]*B[22] + A[30]*B[30],
        A[7]*B[7] + A[15]*B[15] + A[23]*B[23] + A[31]*B[31],
    };

    int ok = 1;
    for (int i = 0; i < 8; i++) {
        if (C[i] != expected[i]) {
            printf("Mismatch C[%d]: got %f expected %f\n", i, C[i], expected[i]);
            ok = 0;
        }
    }
    printf("xvmap_loop xvfmadd.s: %s\n", ok ? "PASSED" : "FAILED");
    return ok ? 0 : 1;
}
