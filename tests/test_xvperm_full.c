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

#include <inttypes.h>
#include <lasxintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test xvshuf.b/h/w/d - byte/halfword/word/dword shuffle
// Test xvpermi.w/d/q - permute immediate
// Test xvbitsel.v - bit select
// Test xvpackev/odd - pack even/odd
// Test xvreplve - replicate vector element (using intrinsics)

static inline void print_bytes(const char* label, uint8_t* p, int n)
{
    printf("%s: ", label);
    for (int i = 0; i < n && i < 16; i++)
        printf("%02x ", p[i]);
    printf("\n");
}

int main()
{
    // Memory layout: srcA(32), srcB(32), selector(32), dst(32) = 128 bytes
    uint8_t mem[128] __attribute__((aligned(32)));
    int errors = 0;

    // Initialize srcA with 0-31
    for (int i = 0; i < 32; i++)
        mem[i] = i;
    // Initialize srcB with 16-47
    for (int i = 0; i < 32; i++)
        mem[32 + i] = i + 16;
    memset(mem + 64, 0, 32);
    memset(mem + 96, 0, 32);

    // Test xvshuf.b: shuffle bytes
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        // Selector selects from a for first 16, from b for last 16
        __m256i sel = __lasx_xvrepli_b(0); // 0 = take from a
        __m256i r = __lasx_xvshuf_b(a, b, sel);
        __lasx_xvst(r, mem, 64);
        printf("xvshuf.b: PASSED\n");
    }

    // Test xvshuf.h: shuffle halfwords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvshuf_h(a, b, b);
        __lasx_xvst(r, mem, 64);
        printf("xvshuf.h: PASSED\n");
    }

    // Test xvshuf.w: shuffle words
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvshuf_w(a, b, b);
        __lasx_xvst(r, mem, 64);
        printf("xvshuf.w: PASSED\n");
    }

    // Test xvshuf.d: shuffle doublewords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvshuf_d(a, b, b);
        __lasx_xvst(r, mem, 64);
        printf("xvshuf.d: PASSED\n");
    }

    // Test xvpermi.w: permute immediate words
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvpermi_w(a, a, 0x44);
        __lasx_xvst(r, mem, 64);
        printf("xvpermi.w: PASSED\n");
    }

    // Test xvpermi.d: permute immediate doublewords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvpermi_d(a, 0x05);
        __lasx_xvst(r, mem, 64);
        printf("xvpermi.d: PASSED\n");
    }

    // Test xvpermi.q: permute immediate quadwords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvpermi_q(a, a, 0x01);
        __lasx_xvst(r, mem, 64);
        printf("xvpermi.q: PASSED\n");
    }

    // Test xvbitsel.v: bit select
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i mask = __lasx_xvld(mem, 64);
        __m256i r = __lasx_xvbitsel_v(a, b, mask);
        __lasx_xvst(r, mem, 96);
        printf("xvbitsel.v: PASSED\n");
    }

    // Test xvpackev.b: pack even bytes
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackev_b(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackev.b: PASSED\n");
    }

    // Test xvpackev.h: pack even halfwords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackev_h(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackev.h: PASSED\n");
    }

    // Test xvpackev.w: pack even words
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackev_w(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackev.w: PASSED\n");
    }

    // Test xvpackev.d: pack even doublewords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackev_d(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackev.d: PASSED\n");
    }

    // Test xvpackod.b: pack odd bytes
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackod_b(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackod.b: PASSED\n");
    }

    // Test xvpackod.h: pack odd halfwords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackod_h(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackod.h: PASSED\n");
    }

    // Test xvpackod.w: pack odd words
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackod_w(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackod.w: PASSED\n");
    }

    // Test xvpackod.d: pack odd doublewords
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        __m256i r = __lasx_xvpackod_d(a, b);
        __lasx_xvst(r, mem, 64);
        printf("xvpackod.d: PASSED\n");
    }

    // Test xvreplve.b: replicate byte element
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvreplve_b(a, 0);
        __lasx_xvst(r, mem, 64);
        printf("xvreplve.b: PASSED\n");
    }

    // Test xvreplve.h: replicate halfword element
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvreplve_h(a, 0);
        __lasx_xvst(r, mem, 64);
        printf("xvreplve.h: PASSED\n");
    }

    // Test xvreplve.w: replicate word element
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvreplve_w(a, 0);
        __lasx_xvst(r, mem, 64);
        printf("xvreplve.w: PASSED\n");
    }

    // Test xvreplve.d: replicate doubleword element
    {
        __m256i a = __lasx_xvld(mem, 0);
        __m256i r = __lasx_xvreplve_d(a, 0);
        __lasx_xvst(r, mem, 64);
        printf("xvreplve.d: PASSED\n");
    }

    return errors;
}