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

/* xvperm.w XR[vd], XR[vj], XR[vk]:  vd.W[i] = vj.W[vk.W[i][2:0]]
   xk is the selector (lower 3 bits select from xj's 8 words).
   Tests 5 register-alias modes: all-distinct, xd==xj, xd==xk, all-equal, xj==xk. */

static void mode_all_distinct(void *mem)
{
    /* xd=2, xj=0 (source), xk=1 (selector) */
    for (volatile int _xv = 0; _xv < 1; _xv++) {
        __asm__ volatile(
            "xvld $xr0, %0, 0\n"     /* source vj      */
            "xvld $xr1, %0, 32\n"    /* selector vk    */
            "xvperm.w $xr2, $xr0, $xr1\n"
            "xvst $xr2, %0, 64\n"    /* store at data[2] */
            ::"r"(mem):"memory","$xr0","$xr1","$xr2");
    }
}

static void mode_dst_eq_src(void *mem)
{
    /* xd==xj=0, xk=1: target == source.
       xr0 is both source (xj) and target (xd).  Selector is xr1=vk. */
    for (volatile int _xv = 0; _xv < 1; _xv++) {
        __asm__ volatile(
            "xvld $xr0, %0, 0\n"     /* source vj (=target) */
            "xvld $xr1, %0, 32\n"    /* selector vk         */
            "xvperm.w $xr0, $xr0, $xr1\n"
            "xvst $xr0, %0, 96\n"    /* store at data[3] */
            ::"r"(mem):"memory","$xr0","$xr1");
    }
}

static void mode_dst_eq_sel(void *mem)
{
    /* xd==xk=0, xj=1: target == selector.
       xr0 is both selector (xk) and target (xd).  Source is xr1=vj. */
    for (volatile int _xv = 0; _xv < 1; _xv++) {
        __asm__ volatile(
            "xvld $xr0, %0, 32\n"    /* selector vk (=target) */
            "xvld $xr1, %0, 0\n"     /* source vj             */
            "xvperm.w $xr0, $xr1, $xr0\n"
            "xvst $xr0, %0, 128\n"   /* store at data[4] */
            ::"r"(mem):"memory","$xr0","$xr1");
    }
}

static void mode_all_equal(void *mem)
{
    /* xd=xj=xk=0: all three equal.  xr0 is source, selector, and target. */
    for (volatile int _xv = 0; _xv < 1; _xv++) {
        __asm__ volatile(
            "xvld $xr0, %0, 0\n"     /* source = selector = target */
            "xvperm.w $xr0, $xr0, $xr0\n"
            "xvst $xr0, %0, 160\n"   /* store at data[5] */
            ::"r"(mem):"memory","$xr0");
    }
}

static void mode_src_eq_sel(void *mem)
{
    /* xj=xk=0, xd=2: source == selector.
       xr0 is both source (xj) and selector (xk).  Target is xr2. */
    for (volatile int _xv = 0; _xv < 1; _xv++) {
        __asm__ volatile(
            "xvld $xr0, %0, 0\n"     /* source = selector       */
            "xvperm.w $xr2, $xr0, $xr0\n"
            "xvst $xr2, %0, 192\n"   /* store at data[6] */
            ::"r"(mem):"memory","$xr0","$xr2");
    }
}

uint64_t data[32][4];

int main(int argc, char *argv[])
{
    const char *filename = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        }
    }
    if (!filename) {
        fprintf(stderr, "Usage: %s -f <filename>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(filename, "rb");
    if (!in) {
        perror("fopen input file");
        return 1;
    }

    uint64_t input[8];
    int n = fread(input, sizeof(uint64_t), 8, in);
    if (n != 8) {
        perror("read less than 8 data");
        return 1;
    }

    /* data[0] = vj source, data[1] = vk selector */
    data[0][0] = input[0]; data[0][1] = input[1];
    data[0][2] = input[2]; data[0][3] = input[3];
    data[1][0] = input[4]; data[1][1] = input[5];
    data[1][2] = input[6]; data[1][3] = input[7];

    mode_all_distinct(data);
    mode_dst_eq_src(data);
    mode_dst_eq_sel(data);
    mode_all_equal(data);
    mode_src_eq_sel(data);

    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
           data[2][0], data[2][1], data[2][2], data[2][3]);
    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
           data[3][0], data[3][1], data[3][2], data[3][3]);
    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
           data[4][0], data[4][1], data[4][2], data[4][3]);
    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
           data[5][0], data[5][1], data[5][2], data[5][3]);
    printf("0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
           data[6][0], data[6][1], data[6][2], data[6][3]);

    return 0;
}
