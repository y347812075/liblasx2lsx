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
        "xvld $xr1,  %0, 0\n"
        "xvld $xr2,  %0, 32\n"
        "xvld $xr3,  %0, 64\n"
        "xvld $xr14, %0, 448\n"
        "xvld $xr15, %0, 480\n"
        "xvld $xr16, %0, 512\n"
        "xvld $xr17, %0, 544\n"
        "xvld $xr18, %0, 576\n"
        "xvld $xr19, %0, 608\n"
        "xvld $xr20, %0, 640\n"
        "xvld $xr21, %0, 672\n"
        "xvld $xr22, %0, 704\n"
        "xvld $xr23, %0, 736\n"
        "xvld $xr24, %0, 768\n"
        "xvld $xr25, %0, 800\n"

        "xvpermi.d       $xr4, $xr1, 0x4e\n"
        "xvpermi.d       $xr11, $xr2, 0x4e\n"
        "xvpermi.d       $xr10, $xr3, 0x4e\n"
        "xvshuf.b        $xr5, $xr4, $xr1, $xr17\n"
        "xvshuf.b        $xr6, $xr4, $xr1, $xr18\n"
        "xvshuf.b        $xr7, $xr4, $xr1, $xr19\n"
        "xvmulwev.w.h    $xr8, $xr5, $xr23\n"
        "xvmaddwod.w.h   $xr8, $xr5, $xr23\n"
        "xvmaddwev.w.h   $xr8, $xr6, $xr24\n"
        "xvmaddwod.w.h   $xr8, $xr6, $xr24\n"
        "xvmaddwev.w.h   $xr8, $xr7, $xr25\n"
        "xvmaddwod.w.h   $xr8, $xr7, $xr25\n"

        "xvshuf.b        $xr5, $xr11, $xr2, $xr17\n"
        "xvshuf.b        $xr6, $xr11, $xr2, $xr18\n"
        "xvshuf.b        $xr7, $xr11, $xr2, $xr19\n"
        "xvmulwev.w.h    $xr9, $xr5, $xr23\n"
        "xvmaddwod.w.h   $xr9, $xr5, $xr23\n"
        "xvmaddwev.w.h   $xr9, $xr6, $xr24\n"
        "xvmaddwod.w.h   $xr9, $xr6, $xr24\n"
        "xvmaddwev.w.h   $xr9, $xr7, $xr25\n"
        "xvmaddwod.w.h   $xr9, $xr7, $xr25\n"

        "xvshuf.b        $xr5, $xr10, $xr3, $xr20\n"
        "xvshuf.b        $xr6, $xr10, $xr3, $xr21\n"
        "xvshuf.b        $xr7, $xr10, $xr3, $xr22\n"
        "xvmulwev.h.bu.b $xr11, $xr5, $xr14\n"
        "xvmaddwod.h.bu.b $xr11, $xr5, $xr14\n"
        "xvmaddwev.h.bu.b $xr11, $xr6, $xr15\n"
        "xvmaddwod.h.bu.b $xr11, $xr6, $xr15\n"
        "xvmaddwev.h.bu.b $xr11, $xr7, $xr16\n"
        "xvmaddwod.h.bu.b $xr11, $xr7, $xr16\n"

        "xvsrari.w       $xr8, $xr8, 0xa\n"
        "xvsrari.w       $xr9, $xr9, 0xa\n"
        "xvmaxi.w        $xr8, $xr8, 0\n"
        "xvsat.wu        $xr8, $xr8, 0x7\n"
        "xvmaxi.w        $xr9, $xr9, 0\n"
        "xvsat.wu        $xr9, $xr9, 0x7\n"
        "xvpermi.d       $xr12, $xr11, 0x4e\n"
        "xvpickev.h      $xr7, $xr9, $xr8\n"
        "xvpermi.d       $xr6, $xr7, 0xd8\n"
        "xvpickev.b      $xr5, $xr6, $xr6\n"
        "xvssrarni.bu.h  $xr12, $xr11, 0x5\n"
        "xvpermi.d       $xr4, $xr5, 0xd8\n"

        "xvst $xr4,  %0, 0\n"
        "xvst $xr5,  %0, 32\n"
        "xvst $xr6,  %0, 64\n"
        "xvst $xr7,  %0, 96\n"
        "xvst $xr8,  %0, 128\n"
        "xvst $xr9,  %0, 160\n"
        "xvst $xr11, %0, 192\n"
        "xvst $xr12, %0, 224\n"
        ::"r"(mem)
        :"memory","$xr1","$xr2","$xr3","$xr4","$xr5","$xr6","$xr7",
         "$xr8","$xr9","$xr10","$xr11","$xr12",
         "$xr14","$xr15","$xr16","$xr17","$xr18","$xr19",
         "$xr20","$xr21","$xr22","$xr23","$xr24","$xr25");
}

uint64_t data[32][4];

int main(int argc, char *argv[]) {
    unsigned seed = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
            seed = atoi(argv[++i]);
    }
    if (seed == 0) seed = (unsigned)time(NULL);

    srand(seed);

    /* Fill input registers with random data */
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            data[1+i][j] = ((uint64_t)rand() << 32) | rand();

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            data[14+i][j] = ((uint64_t)rand() << 32) | rand();

    /* Shuffle indices: low bytes of 32-bit values */
    uint64_t idx_lo = ((uint64_t)(uint8_t)rand() << 0) |
                      ((uint64_t)(uint8_t)rand() << 8) |
                      ((uint64_t)(uint8_t)rand() << 16) |
                      ((uint64_t)(uint8_t)rand() << 24);
    uint64_t idx_hi = ((uint64_t)(uint8_t)rand() << 0) |
                      ((uint64_t)(uint8_t)rand() << 8) |
                      ((uint64_t)(uint8_t)rand() << 16) |
                      ((uint64_t)(uint8_t)rand() << 24);
    idx_lo |= (idx_lo << 32);
    idx_hi |= (idx_hi << 32);
    for (int i = 0; i < 9; i++) {
        data[17+i][0] = idx_lo;
        data[17+i][1] = idx_hi;
        data[17+i][2] = idx_lo;
        data[17+i][3] = idx_hi;
    }

    /* Multiply operands */
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            data[23+i][j] = ((uint64_t)rand() << 32) | rand();

    fprintf(stderr, "seed=%u\n", seed);
    asm_test(data);

    /* Output 44 hex uint64_t: 8 xr4-xr9 + xr11 + xr12, each 4 lanes */
    for (int r = 4; r < 10; r++)
        for (int j = 0; j < 4; j++)
            printf("0x%016lx\n", data[r][j]);
    for (int r = 11; r < 13; r++)
        for (int j = 0; j < 4; j++)
            printf("0x%016lx\n", data[r][j]);

    return 0;
}
