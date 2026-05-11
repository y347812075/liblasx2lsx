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

void init_data(uint8_t* a, uint8_t* b)
{
    for (int i = 0; i < 32; i++) {
        a[i] = i * 13 + 7;
        b[i] = i * 17 + 3;
    }
}

int check_result(const char* name, uint64_t* got, uint64_t* expected)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        if (got[i] != expected[i]) {
            printf("%s: mismatch at dword %d: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", name, i, got[i], expected[i]);
            errors++;
        }
    }
    return errors;
}

static inline uint64_t calc_and(uint64_t a, uint64_t b) { return a & b; }
static inline uint64_t calc_or(uint64_t a, uint64_t b) { return a | b; }
static inline uint64_t calc_xor(uint64_t a, uint64_t b) { return a ^ b; }
static inline uint64_t calc_nor(uint64_t a, uint64_t b) { return ~(a | b); }
static inline uint64_t calc_andn(uint64_t a, uint64_t b) { return b & ~a; }
static inline uint64_t calc_orn(uint64_t a, uint64_t b) { return b | ~a; }

int main()
{
    unsigned char mem[96] __attribute__((aligned(32)));

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32);

    __m256i a = __lasx_xvld(mem, 0);
    __m256i b = __lasx_xvld(mem, 32);

    __m256i r_and = __lasx_xvand_v(a, b);
    __lasx_xvst(r_and, mem, 64);
    uint64_t expected_and[4];
    for (int i = 0; i < 4; i++)
        expected_and[i] = calc_and(((uint64_t*)mem)[i], ((uint64_t*)(mem + 32))[i]);
    int errors = check_result("xvand.v", (uint64_t*)(mem + 64), expected_and);
    printf("xvand.v: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    __m256i r_or = __lasx_xvor_v(a, b);
    __lasx_xvst(r_or, mem, 64);
    uint64_t expected_or[4];
    for (int i = 0; i < 4; i++)
        expected_or[i] = calc_or(((uint64_t*)mem)[i], ((uint64_t*)(mem + 32))[i]);
    errors = check_result("xvor.v", (uint64_t*)(mem + 64), expected_or);
    printf("xvor.v: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    __m256i r_xor = __lasx_xvxor_v(a, b);
    __lasx_xvst(r_xor, mem, 64);
    uint64_t expected_xor[4];
    for (int i = 0; i < 4; i++)
        expected_xor[i] = calc_xor(((uint64_t*)mem)[i], ((uint64_t*)(mem + 32))[i]);
    errors = check_result("xvxor.v", (uint64_t*)(mem + 64), expected_xor);
    printf("xvxor.v: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    __m256i r_nor = __lasx_xvnor_v(a, b);
    __lasx_xvst(r_nor, mem, 64);
    uint64_t expected_nor[4];
    for (int i = 0; i < 4; i++)
        expected_nor[i] = calc_nor(((uint64_t*)mem)[i], ((uint64_t*)(mem + 32))[i]);
    errors = check_result("xvnor.v", (uint64_t*)(mem + 64), expected_nor);
    printf("xvnor.v: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    __m256i r_andn = __lasx_xvandn_v(a, b);
    __lasx_xvst(r_andn, mem, 64);
    uint64_t expected_andn[4];
    for (int i = 0; i < 4; i++)
        expected_andn[i] = calc_andn(((uint64_t*)mem)[i], ((uint64_t*)(mem + 32))[i]);
    errors = check_result("xvandn.v", (uint64_t*)(mem + 64), expected_andn);
    printf("xvandn.v: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32);
    a = __lasx_xvld(mem, 0);
    b = __lasx_xvld(mem, 32);
    __m256i r_orn = __lasx_xvorn_v(a, b);
    __lasx_xvst(r_orn, mem, 64);
    uint64_t expected_orn[4];
    for (int i = 0; i < 4; i++)
        expected_orn[i] = calc_orn(((uint64_t*)mem)[i], ((uint64_t*)(mem + 32))[i]);
    errors = check_result("xvorn.v", (uint64_t*)(mem + 64), expected_orn);
    printf("xvorn.v: %s\n", errors ? "FAILED" : "PASSED");

    return 0;
}