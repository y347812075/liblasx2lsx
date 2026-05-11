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

static const int8_t test_values_b[] = {0, 1, 2, 3, -1, -2, 127, -128, 0, 1, 2, 3, -1, -2, 127, -128, 0, 1, 2, 3, -1, -2, 127, -128, 0, 1, 2, 3, -1, -2, 127, -128};

static const int16_t test_values_h[] = {0, 1, 2, 3, -1, -2, 32767, -32768, 0, 1, 2, 3, -1, -2, 32767, -32768};

static const int32_t test_values_w[] = {0, 1, 2, 3, -1, -2, 2147483647, -2147483648};

static const int64_t test_values_d[] = {0, 1, 2, 3};

int check_b(int8_t* a, int8_t* b, int8_t* c)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        int16_t expected = (int16_t)a[i] * (int16_t)b[i];
        int8_t result = (int8_t)expected;
        if (c[i] != result) {
            errors++;
        }
    }
    return errors;
}

int check_h(int16_t* a, int16_t* b, int16_t* c)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        int32_t expected = (int32_t)a[i] * (int32_t)b[i];
        int16_t result = (int16_t)expected;
        if (c[i] != result) {
            printf("H[%d]: got %d, expected %d (a=%d, b=%d)\n", i, c[i], result, a[i], b[i]);
            errors++;
        }
    }
    return errors;
}

int check_w(int32_t* a, int32_t* b, int32_t* c)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        int64_t expected = (int64_t)a[i] * (int64_t)b[i];
        int32_t result = (int32_t)expected;
        if (c[i] != result) {
            printf("W[%d]: got %d, expected %d (a=%d, b=%d)\n", i, c[i], result, a[i], b[i]);
            errors++;
        }
    }
    return errors;
}

int check_d(int64_t* a, int64_t* b, int64_t* c)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        __int128 expected = (__int128)a[i] * (__int128)b[i];
        int64_t result = (int64_t)expected;
        if (c[i] != result) {
            printf("D[%d]: got %" PRId64 ", expected %" PRId64 " (a=%" PRId64 ", b=%" PRId64 ")\n", i, c[i], result, a[i], b[i]);
            errors++;
        }
    }
    return errors;
}

static void asm_xvmul_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmul.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmul_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmul.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmul_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmul.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmul_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmul.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmuh_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmuh.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmuh_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmuh.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmuh_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmuh.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmuh_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmuh.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

static void asm_xvmulwod_d_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmulwod.d.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

void test_inst(const char* name, void (*asm_func)(void* mem), int width, void* test_vals_a, void* test_vals_b)
{
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    memcpy(mem, test_vals_a, 32);
    memcpy(mem + 32, test_vals_b, 32);

    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: errors = check_b((int8_t*)mem, (int8_t*)(mem + 32), (int8_t*)(mem + 64)); break;
    case 2: errors = check_h((int16_t*)mem, (int16_t*)(mem + 32), (int16_t*)(mem + 64)); break;
    case 4: errors = check_w((int32_t*)mem, (int32_t*)(mem + 32), (int32_t*)(mem + 64)); break;
    case 8: errors = check_d((int64_t*)mem, (int64_t*)(mem + 32), (int64_t*)(mem + 64)); break;
    }
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

int main()
{
    printf("=== xvmul (signed multiply) tests ===\n");
    test_inst("xvmul.b", asm_xvmul_b, 1, (void*)test_values_b, (void*)test_values_b);
    test_inst("xvmul.h", asm_xvmul_h, 2, (void*)test_values_h, (void*)test_values_h);
    test_inst("xvmul.w", asm_xvmul_w, 4, (void*)test_values_w, (void*)test_values_w);
    test_inst("xvmul.d", asm_xvmul_d, 8, (void*)test_values_d, (void*)test_values_d);

    return 0;
}