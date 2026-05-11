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
#include <inttypes.h>
static void asm_test(void *mem) {
    for (volatile int _xv = 0; _xv < 1; _xv++) {
        __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvssrarn.b.h $xr2, $xr1, $xr0\n\r"
        "xvst $xr2, %0, 64\n"
        ::"r"(mem):"memory","$xr0","$xr1","$xr2");
    }
}

uint64_t data[32][4];

int main(int argc, char *argv[]) {
    const char *filename = NULL;
    for (int i = 1; i < argc; i++) { if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) { filename = argv[++i]; } }
    if (!filename) { fprintf(stderr, "Usage: %s -f <filename>\n", argv[0]); return 1; }
    FILE *in = fopen(filename, "rb"); if (!in) { perror("fopen input file"); return 1; }
    uint64_t input[8];
    int n = fread(input, sizeof(uint64_t), 8, in);
    if (n != 8) { perror("read less than 8 data"); return 1; }
    data[0][0] = input[0]; data[0][1] = input[1]; data[0][2] = input[2]; data[0][3] = input[3];
    data[1][0] = input[4]; data[1][1] = input[5]; data[1][2] = input[6]; data[1][3] = input[7];
    asm_test(data);
    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n", data[2][0], data[2][1], data[2][2], data[2][3]);
    return 0;
}
