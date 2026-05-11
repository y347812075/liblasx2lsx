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
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void asm_test(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvld $xr4, %0, 64\n"
        "xvld $xr5, %0, 96\n"
        "nop\n"
        "xvpermi.q $xr2, $xr4, 2\n"
        "xvpermi.q $xr3, $xr5, 2\n"
        "xvsubwev.h.bu $xr4, $xr0, $xr2\n"
        "xvsubwod.h.bu $xr5, $xr0, $xr2\n"
        "xvsubwev.h.bu $xr6, $xr1, $xr3\n"
        "xvsubwod.h.bu $xr7, $xr1, $xr3\n"
        "xvadd.h $xr0, $xr4, $xr5\n"
        "xvsub.h $xr1, $xr4, $xr5\n"
        "xvadd.h $xr2, $xr6, $xr7\n"
        "xvsub.h $xr3, $xr6, $xr7\n"
        "xvpackev.h $xr4, $xr1, $xr0\n"
        "xvpackod.h $xr5, $xr1, $xr0\n"
        "xvpackev.h $xr6, $xr3, $xr2\n"
        "xvpackod.h $xr7, $xr3, $xr2\n"
        "xvadd.h $xr0, $xr4, $xr5\n"
        "xvsub.h $xr1, $xr4, $xr5\n"
        "xvadd.h $xr2, $xr6, $xr7\n"
        "xvsub.h $xr3, $xr6, $xr7\n"
        "xvilvl.h $xr4, $xr1, $xr0\n"
        "xvilvh.h $xr5, $xr1, $xr0\n"
        "xvilvl.h $xr6, $xr3, $xr2\n"
        "xvilvh.h $xr7, $xr3, $xr2\n"
        "xvadd.h $xr0, $xr4, $xr5\n"
        "xvadd.h $xr2, $xr6, $xr7\n"
        "xvsub.h $xr1, $xr4, $xr5\n"
        "xvsub.h $xr3, $xr6, $xr7\n"
        "xvadd.h $xr4, $xr0, $xr2\n"
        "xvadd.h $xr5, $xr1, $xr3\n"
        "xvsub.h $xr6, $xr0, $xr2\n"
        "xvsub.h $xr7, $xr1, $xr3\n"
        "xvadda.h $xr0, $xr4, $xr5\n"
        "xvadda.h $xr1, $xr6, $xr7\n"
        "xvadd.h $xr0, $xr0, $xr1\n"
        "xvhaddw.wu.hu $xr0, $xr0, $xr0\n"
        "xvhaddw.du.wu $xr0, $xr0, $xr0\n"
        "xvhaddw.qu.du $xr0, $xr0, $xr0\n"
        "nop\n"
        "xvst $xr0, %0, 128\n"
        ::"r"(mem)
        :"memory","$xr0","$xr1","$xr2","$xr3","$xr4","$xr5","$xr6","$xr7");
}

uint64_t data[32][4];

int main(int argc, char *argv[]) {
    unsigned seed = 0;
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
            seed = atoi(argv[++i]);
    if (seed == 0) seed = (unsigned)time(NULL);
    srand(seed);

    for (int r = 0; r < 2; r++)
        for (int j = 0; j < 4; j++)
            data[r][j] = ((uint64_t)rand() << 32) | rand();
    for (int r = 4; r < 6; r++)
        for (int j = 0; j < 4; j++)
            data[r][j] = ((uint64_t)rand() << 32) | rand();

    fprintf(stderr, "seed=%u\n", seed);
    asm_test(data);

    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
           data[4][0], data[4][1], data[4][2], data[4][3]);
    return 0;
}
