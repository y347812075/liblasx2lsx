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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void init_data(uint8_t* mem)
{
    for (int i = 0; i < 32; i++) {
        mem[i] = i + 1;
    }
    for (int i = 0; i < 32; i++) {
        mem[32 + i] = 0x80 + i;
    }
    memset(mem + 64, 0, 64);
}

static void asm_xvld(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvst $xr0, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvld_off32(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 32\n"
                     "xvst $xr0, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvst(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvst $xr0, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

int verify_b(uint8_t* src, uint8_t* dst)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        if (dst[i] != src[i]) {
            printf("  B[%d]: got 0x%02x, expected 0x%02x\n", i, dst[i], src[i]);
            errors++;
        }
    }
    return errors;
}

int verify_h(uint16_t* src, uint16_t* dst)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        if (dst[i] != src[i]) {
            printf("  H[%d]: got 0x%04x, expected 0x%04x\n", i, dst[i], src[i]);
            errors++;
        }
    }
    return errors;
}

int verify_w(uint32_t* src, uint32_t* dst)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        if (dst[i] != src[i]) {
            printf("  W[%d]: got 0x%08x, expected 0x%08x\n", i, dst[i], src[i]);
            errors++;
        }
    }
    return errors;
}

int verify_d(uint64_t* src, uint64_t* dst)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        if (dst[i] != src[i]) {
            printf("  D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, dst[i], src[i]);
            errors++;
        }
    }
    return errors;
}

void test_xvld(const char* name, void (*asm_func)(void*), int width)
{
    uint8_t mem[128] __attribute__((aligned(256)));
    memset(mem, 0, sizeof(mem));

    init_data(mem);
    uint8_t* src = mem;
    uint8_t* dst = mem + 64;

    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: errors = verify_b(src, dst); break;
    case 2: errors = verify_h((uint16_t*)src, (uint16_t*)dst); break;
    case 4: errors = verify_w((uint32_t*)src, (uint32_t*)dst); break;
    case 8: errors = verify_d((uint64_t*)src, (uint64_t*)dst); break;
    }

    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

void test_xvld_offset(const char* name, void (*asm_func)(void*), int width)
{
    uint8_t mem[128] __attribute__((aligned(256)));
    memset(mem, 0, sizeof(mem));

    init_data(mem);
    uint8_t* src = mem + 32;
    uint8_t* dst = mem + 64;

    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: errors = verify_b(src, dst); break;
    case 2: errors = verify_h((uint16_t*)src, (uint16_t*)dst); break;
    case 4: errors = verify_w((uint32_t*)src, (uint32_t*)dst); break;
    case 8: errors = verify_d((uint64_t*)src, (uint64_t*)dst); break;
    }

    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

void test_xvst(const char* name, void (*asm_func)(void*), int width)
{
    uint8_t mem[128] __attribute__((aligned(256)));
    memset(mem, 0, sizeof(mem));

    init_data(mem);
    uint8_t* src = mem;
    uint8_t* dst = mem + 64;

    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: errors = verify_b(src, dst); break;
    case 2: errors = verify_h((uint16_t*)src, (uint16_t*)dst); break;
    case 4: errors = verify_w((uint32_t*)src, (uint32_t*)dst); break;
    case 8: errors = verify_d((uint64_t*)src, (uint64_t*)dst); break;
    }

    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

int main()
{
    printf("=== xvld tests (offset 0) ===\n");
    test_xvld("xvld.b", asm_xvld, 1);
    test_xvld("xvld.h", asm_xvld, 2);
    test_xvld("xvld.w", asm_xvld, 4);
    test_xvld("xvld.d", asm_xvld, 8);

    printf("\n=== xvld tests (offset 32) ===\n");
    test_xvld_offset("xvld+32.b", asm_xvld_off32, 1);
    test_xvld_offset("xvld+32.h", asm_xvld_off32, 2);
    test_xvld_offset("xvld+32.w", asm_xvld_off32, 4);
    test_xvld_offset("xvld+32.d", asm_xvld_off32, 8);

    printf("\n=== xvst tests ===\n");
    test_xvst("xvst.b", asm_xvst, 1);
    test_xvst("xvst.h", asm_xvst, 2);
    test_xvst("xvst.w", asm_xvst, 4);
    test_xvst("xvst.d", asm_xvst, 8);

    return 0;
}