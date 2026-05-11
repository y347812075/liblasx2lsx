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
#include <string.h>

unsigned char mem[96] __attribute__((aligned(32)));

int check_frstp_b(uint8_t* c)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t val = mem[i];
        uint8_t expected = 0;
        for (int b = 0; b < 8; b++) {
            if (val & (1U << b)) {
                expected = b + 1;
                break;
            }
        }
        if (c[i] != expected) {
            printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_frstp_h(uint16_t* c)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t val = ((uint16_t*)mem)[i];
        uint16_t expected = 0;
        for (int b = 0; b < 16; b++) {
            if (val & (1U << b)) {
                expected = b + 1;
                break;
            }
        }
        if (c[i] != expected) {
            printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_signcov_b(uint8_t* c)
{
    int errors = 0;
    uint32_t mask = 0;
    for (int i = 0; i < 32; i++) {
        int8_t val = (int8_t)((uint8_t*)(mem + 32))[i];
        if (val < 0)
            mask |= (1U << i);
    }
    for (int i = 0; i < 32; i++) {
        uint8_t val = ((uint8_t*)mem)[i];
        int elem = 31 - i;
        uint8_t expected = (mask & (1U << elem)) ? (val | 0x80) : (val & 0x7f);
        if (c[i] != expected) {
            printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_signcov_h(uint16_t* c)
{
    int errors = 0;
    uint16_t mask = 0;
    for (int i = 0; i < 16; i++) {
        int16_t val = (int16_t)((uint16_t*)(mem + 32))[i];
        if (val < 0)
            mask |= (1U << i);
    }
    for (int i = 0; i < 16; i++) {
        uint16_t val = ((uint16_t*)mem)[i];
        int elem = 15 - i;
        uint16_t expected = (mask & (1U << elem)) ? (val | 0x8000) : (val & 0x7fff);
        if (c[i] != expected) {
            printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_signcov_w(uint32_t* c)
{
    int errors = 0;
    uint8_t mask = 0;
    for (int i = 0; i < 8; i++) {
        int32_t val = (int32_t)((uint32_t*)(mem + 32))[i];
        if (val < 0)
            mask |= (1U << i);
    }
    for (int i = 0; i < 8; i++) {
        uint32_t val = ((uint32_t*)mem)[i];
        int elem = 7 - i;
        uint32_t expected = (mask & (1U << elem)) ? (val | 0x80000000) : (val & 0x7fffffff);
        if (c[i] != expected) {
            printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_signcov_d(uint64_t* c)
{
    int errors = 0;
    uint8_t mask = 0;
    for (int i = 0; i < 4; i++) {
        int64_t val = (int64_t)((uint64_t*)(mem + 32))[i];
        if (val < 0)
            mask |= (1U << i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t val = ((uint64_t*)mem)[i];
        int elem = 3 - i;
        uint64_t expected = (mask & (1U << elem)) ? (val | (1ULL << 63)) : (val & ~(1ULL << 63));
        if (c[i] != expected) {
            printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int main()
{
    printf("=== xvfrstp (find first set bit) tests ===\n");

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++)
        mem[i] = (i * 17 + 5) & 0xFF;
    __m256i va = __lasx_xvld(mem, 0);
    __m256i vb = __lasx_xvld(mem, 32);
    __m256i vc = __lasx_xvrepli_b(0);
    __m256i vr = __lasx_xvfrstp_b(va, vb, vc);
    __lasx_xvst(vr, mem, 64);
    int errors = check_frstp_b((uint8_t*)(mem + 64));
    printf("xvfrstp.b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++)
        mem[i] = (i * 17 + 5) & 0xFF;
    va = __lasx_xvld(mem, 0);
    vb = __lasx_xvld(mem, 32);
    vc = __lasx_xvrepli_h(0);
    vr = __lasx_xvfrstp_h(va, vb, vc);
    __lasx_xvst(vr, mem, 64);
    errors = check_frstp_h((uint16_t*)(mem + 64));
    printf("xvfrstp.h: %s\n", errors ? "FAILED" : "PASSED");

    printf("=== xvsigncov (sign coverage) tests ===\n");

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++) {
        ((uint8_t*)mem)[i] = i * 13 + 7;
        ((uint8_t*)(mem + 32))[i] = i * 17 + 3;
    }
    va = __lasx_xvld(mem, 0);
    vb = __lasx_xvld(mem, 32);
    vr = __lasx_xvsigncov_b(va, vb);
    __lasx_xvst(vr, mem, 64);
    errors = check_signcov_b((uint8_t*)(mem + 64));
    printf("xvsigncov.b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++) {
        ((uint8_t*)mem)[i] = i * 13 + 7;
        ((uint8_t*)(mem + 32))[i] = i * 17 + 3;
    }
    va = __lasx_xvld(mem, 0);
    vb = __lasx_xvld(mem, 32);
    vr = __lasx_xvsigncov_h(va, vb);
    __lasx_xvst(vr, mem, 64);
    errors = check_signcov_h((uint16_t*)(mem + 64));
    printf("xvsigncov.h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++) {
        ((uint8_t*)mem)[i] = i * 13 + 7;
        ((uint8_t*)(mem + 32))[i] = i * 17 + 3;
    }
    va = __lasx_xvld(mem, 0);
    vb = __lasx_xvld(mem, 32);
    vr = __lasx_xvsigncov_w(va, vb);
    __lasx_xvst(vr, mem, 64);
    errors = check_signcov_w((uint32_t*)(mem + 64));
    printf("xvsigncov.w: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    for (int i = 0; i < 32; i++) {
        ((uint8_t*)mem)[i] = i * 13 + 7;
        ((uint8_t*)(mem + 32))[i] = i * 17 + 3;
    }
    va = __lasx_xvld(mem, 0);
    vb = __lasx_xvld(mem, 32);
    vr = __lasx_xvsigncov_d(va, vb);
    __lasx_xvst(vr, mem, 64);
    errors = check_signcov_d((uint64_t*)(mem + 64));
    printf("xvsigncov.d: %s\n", errors ? "FAILED" : "PASSED");

    return 0;
}