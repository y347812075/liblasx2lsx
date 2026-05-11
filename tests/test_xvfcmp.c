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

#include <float.h>
#include <lasxintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const float srcA_s[8] = {1.0f, 2.0f, 3.0f, 4.0f, 0.5f, -1.0f, 0.0f, -5.0f};
static const float srcB_s[8] = {2.0f, 1.0f, 3.0f, 2.0f, 1.0f, -2.0f, 0.0f, 5.0f};

static const double srcA_d[4] = {1.0, 2.0, 0.5, -1.0};
static const double srcB_d[4] = {2.0, 1.0, 1.0, -2.0};

static const uint64_t expected_s_clt[4] = {
    0x00000000FFFFFFFFULL,
    0x0000000000000000ULL,
    0x00000000FFFFFFFFULL,
    0xFFFFFFFF00000000ULL,
};

// 4 doubles -> 4 x 64-bit
static const uint64_t expected_d_clt[4] = {
    0xFFFFFFFFFFFFFFFFULL, // 1 < 2 = true
    0x0000000000000000ULL, // 2 < 1 = false
    0xFFFFFFFFFFFFFFFFULL, // 0.5 < 1 = true
    0x0000000000000000ULL, // -1 < -2 = false
};

static const uint64_t expected_s_ceq[4] = {
    0x0000000000000000ULL,
    0x00000000FFFFFFFFULL,
    0x0000000000000000ULL,
    0x00000000FFFFFFFFULL,
};

static const uint64_t expected_d_ceq[4] = {
    0x0000000000000000ULL, // 1==2=F
    0x0000000000000000ULL, // 2==1=F
    0x0000000000000000ULL, // 0.5==1=F
    0x0000000000000000ULL, // -1==-2=F
};

int check_cmp_s(uint64_t* result, const uint64_t* expected, int count)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        uint64_t got = result[i];
        uint64_t exp = expected[i];
        if (got != exp) {
            printf("S[%d]: got 0x%016lx, expected 0x%016lx\n", i, (unsigned long)got, (unsigned long)exp);
            errors++;
        }
    }
    return errors;
}

int check_cmp_d(uint64_t* result, const uint64_t* expected, int count)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        uint64_t got = result[i];
        uint64_t exp = expected[i];
        if (got != exp) {
            printf("D[%d]: got 0x%016lx, expected 0x%016lx\n", i, (unsigned long)got, (unsigned long)exp);
            errors++;
        }
    }
    return errors;
}

int test_xvfcmp_s_clt(void)
{
    uint8_t mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    memcpy(mem, srcA_s, sizeof(srcA_s));
    memcpy(mem + 32, srcB_s, sizeof(srcB_s));

    __m256 a = *(__m256*)mem;
    __m256 b = *(__m256*)(mem + 32);
    __m256i r = __lasx_xvfcmp_clt_s(a, b);
    *(__m256i*)(mem + 64) = r;

    int errors = check_cmp_s((uint64_t*)(mem + 64), expected_s_clt, 4);
    printf("xvfcmp.s CLT: %s\n", errors ? "FAILED" : "PASSED");
    return errors;
}

int test_xvfcmp_d_clt(void)
{
    uint8_t mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    memcpy(mem, srcA_d, sizeof(srcA_d));
    memcpy(mem + 32, srcB_d, sizeof(srcB_d));

    __m256d a = *(__m256d*)mem;
    __m256d b = *(__m256d*)(mem + 32);
    __m256i r = __lasx_xvfcmp_clt_d(a, b);
    *(__m256i*)(mem + 64) = r;

    int errors = check_cmp_d((uint64_t*)(mem + 64), expected_d_clt, 4);
    printf("xvfcmp.d CLT: %s\n", errors ? "FAILED" : "PASSED");
    return errors;
}

int test_xvfcmp_s_ceq(void)
{
    uint8_t mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    memcpy(mem, srcA_s, sizeof(srcA_s));
    memcpy(mem + 32, srcB_s, sizeof(srcB_s));

    __m256 a = *(__m256*)mem;
    __m256 b = *(__m256*)(mem + 32);
    __m256i r = __lasx_xvfcmp_ceq_s(a, b);
    *(__m256i*)(mem + 64) = r;

    int errors = check_cmp_s((uint64_t*)(mem + 64), expected_s_ceq, 4);
    printf("xvfcmp.s CEQ: %s\n", errors ? "FAILED" : "PASSED");
    return errors;
}

int test_xvfcmplt_s(void)
{
    uint8_t mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    memcpy(mem, srcA_s, sizeof(srcA_s));
    memcpy(mem + 32, srcB_s, sizeof(srcB_s));

    __m256 a = *(__m256*)mem;
    __m256 b = *(__m256*)(mem + 32);
    __m256i r = __lasx_xvfcmp_clt_s(a, b);
    *(__m256i*)(mem + 64) = r;

    int errors = check_cmp_s((uint64_t*)(mem + 64), expected_s_clt, 4);
    printf("xvfcmplt.s: %s\n", errors ? "FAILED" : "PASSED");
    return errors;
}

int test_xvfcmplt_d(void)
{
    uint8_t mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    memcpy(mem, srcA_d, sizeof(srcA_d));
    memcpy(mem + 32, srcB_d, sizeof(srcB_d));

    __m256d a = *(__m256d*)mem;
    __m256d b = *(__m256d*)(mem + 32);
    __m256i r = __lasx_xvfcmp_clt_d(a, b);
    *(__m256i*)(mem + 64) = r;

    int errors = check_cmp_d((uint64_t*)(mem + 64), expected_d_clt, 4);
    printf("xvfcmplt.d: %s\n", errors ? "FAILED" : "PASSED");
    return errors;
}

int main(void)
{
    int errors = 0;
    errors += test_xvfcmp_s_clt();
    errors += test_xvfcmp_d_clt();
    errors += test_xvfcmp_s_ceq();
    errors += test_xvfcmplt_s();
    errors += test_xvfcmplt_d();
    return errors;
}