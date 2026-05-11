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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

long long mem[128] __attribute__((aligned(256)));

int main(void)
{
    memset(mem, 0, sizeof(mem));

    for (int i = 0; i < 32; i++) {
        ((uint8_t*)mem)[i] = i + 1;
    }
    for (int i = 0; i < 32; i++) {
        ((uint8_t*)mem)[32 + i] = 2;
    }

    printf("Input A (offset 0): ");
    for (int i = 0; i < 16; i++)
        printf("%02x ", ((uint8_t*)mem)[i]);
    printf("\n");

    printf("Input B (offset 32): ");
    for (int i = 0; i < 16; i++)
        printf("%02x ", ((uint8_t*)mem)[32 + i]);
    printf("\n");

    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsll.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");

    printf("Result (offset 64): ");
    for (int i = 0; i < 16; i++)
        printf("%02x ", ((uint8_t*)mem)[64 + i]);
    printf("\n");

    return 0;
}