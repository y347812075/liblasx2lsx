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
#include <stdint.h>
#define N 4
uint16_t A[N * 16] __attribute__((aligned(32)));
uint16_t B[N * 16] __attribute__((aligned(32)));
uint16_t C[16] __attribute__((aligned(32)));
extern void run_xvmsub_h(uint16_t *a, uint16_t *b, uint16_t *c, int n);
int main() {
    for (int i = 0; i < N * 16; i++) { A[i] = i * 3 + 1; B[i] = i * 5 + 2; }
    run_xvmsub_h(A, B, C, N);
    for (int i = 0; i < 16; i++) printf("%u\n", C[i]);
    return 0;
}
