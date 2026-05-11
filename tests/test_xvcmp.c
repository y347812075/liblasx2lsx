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

// Test xvmax, xvmin, xvclo, xvclz

void init_data(uint8_t* a, uint8_t* b, int size)
{
    for (int i = 0; i < size; i++) {
        a[i] = i * 13 + 7;
        b[i] = i * 17 + 3;
    }
}

#if 1 // CHECK_MAX_MIN_UNSIGNED_FOR_PLACEHOLDER

// Check max/min byte results (32 elements)
int check_max_min_b(uint8_t* a, uint8_t* b, uint8_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        int8_t as = (int8_t)a[i];
        int8_t bs = (int8_t)b[i];
        uint8_t expected = 0;
        if (op == 0) { // xvmax.b signed
            expected = (as > bs) ? a[i] : b[i];
        } else if (op == 1) { // xvmax.bu unsigned
            expected = (a[i] > b[i]) ? a[i] : b[i];
        } else if (op == 2) { // xvmin.b signed
            expected = (as < bs) ? a[i] : b[i];
        } else if (op == 3) { // xvmin.bu unsigned
            expected = (a[i] < b[i]) ? a[i] : b[i];
        }
        if (c[i] != expected) {
            printf("B[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check max/min half results (16 elements)
int check_max_min_h(uint16_t* a, uint16_t* b, uint16_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        int16_t as = (int16_t)a[i];
        int16_t bs = (int16_t)b[i];
        uint16_t expected = 0;
        if (op == 0) { // xvmax.h signed
            expected = (as > bs) ? a[i] : b[i];
        } else if (op == 1) { // xvmax.hu unsigned
            expected = (a[i] > b[i]) ? a[i] : b[i];
        } else if (op == 2) { // xvmin.h signed
            expected = (as < bs) ? a[i] : b[i];
        } else if (op == 3) { // xvmin.hu unsigned
            expected = (a[i] < b[i]) ? a[i] : b[i];
        }
        if (c[i] != expected) {
            printf("H[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check max/min word results (8 elements)
int check_max_min_w(uint32_t* a, uint32_t* b, uint32_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        int32_t as = (int32_t)a[i];
        int32_t bs = (int32_t)b[i];
        uint32_t expected = 0;
        if (op == 0) { // xvmax.w signed
            expected = (as > bs) ? a[i] : b[i];
        } else if (op == 1) { // xvmax.wu unsigned
            expected = (a[i] > b[i]) ? a[i] : b[i];
        } else if (op == 2) { // xvmin.w signed
            expected = (as < bs) ? a[i] : b[i];
        } else if (op == 3) { // xvmin.wu unsigned
            expected = (a[i] < b[i]) ? a[i] : b[i];
        }
        if (c[i] != expected) {
            printf("W[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check max/min double results (4 elements)
int check_max_min_d(uint64_t* a, uint64_t* b, uint64_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        int64_t as = (int64_t)a[i];
        int64_t bs = (int64_t)b[i];
        uint64_t expected = 0;
        if (op == 0) { // xvmax.d signed
            expected = (as > bs) ? a[i] : b[i];
        } else if (op == 1) { // xvmax.du unsigned
            expected = (a[i] > b[i]) ? a[i] : b[i];
        } else if (op == 2) { // xvmin.d signed
            expected = (as < bs) ? a[i] : b[i];
        } else if (op == 3) { // xvmin.du unsigned
            expected = (a[i] < b[i]) ? a[i] : b[i];
        }
        if (c[i] != expected) {
            printf("D[%d]: got %" PRIu64 ", expected %" PRIu64 "\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

#endif

// Check clo/clz byte results (32 elements)
int check_cl(uint8_t* ab, uint8_t* cs, int op)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = 0;
        if (op == 0) { // xvclo.b
            for (int jt = 7; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 0)
                    break;
                expected++;
            }
        } else if (op == 1) { // xvclz.b
            for (int jt = 7; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 1)
                    break;
                expected++;
            }
        }
        if (cs[i] != expected) {
            printf("CloClzB[%d]: got %u, expected %u\n", i, cs[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check clo/clz word results (8 elements, 32-bit each)
int check_cl_w(uint32_t* ab, uint32_t* cs, int op)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = 0;
        if (op == 0) { // xvclo.w
            for (int jt = 31; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 0)
                    break;
                expected++;
            }
        } else if (op == 1) { // xvclz.w
            for (int jt = 31; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 1)
                    break;
                expected++;
            }
        }
        if (cs[i] != expected) {
            printf("CloClzW[%d]: got %u, expected %u\n", i, cs[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check clo/clz double results (4 elements, 64-bit each)
int check_cl_d(uint64_t* ab, uint64_t* cs, int op)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = 0;
        if (op == 0) { // xvclo.d
            for (int jt = 63; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 0)
                    break;
                expected++;
            }
        } else if (op == 1) { // xvclz.d
            for (int jt = 63; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 1)
                    break;
                expected++;
            }
        }
        if (cs[i] != expected) {
            printf("CloClzD[%d]: got %" PRIu64 ", expected %" PRIu64 "\n", i, cs[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check clo/clz half results (16 elements, 16-bit each)
int check_cl_h(uint16_t* ab, uint16_t* cs, int op)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = 0;
        if (op == 0) { // xvclo.h
            for (int jt = 15; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 0)
                    break;
                expected++;
            }
        } else if (op == 1) { // xvclz.h
            for (int jt = 15; jt >= 0; jt--) {
                if (((ab[i] >> jt) & 1) == 1)
                    break;
                expected++;
            }
        }
        if (cs[i] != expected) {
            printf("CloClzH[%d]: got %u, expected %u\n", i, cs[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check clo/clz double results (4 elements, 64-bit each)

// Test function for max/min operations
void test_max_min(const char* name, void (*asm_func)(void* mem), int width, int op)
{
    // Allocate 96 bytes: srcA(32), srcB(32), dst(32)
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // Initialize source data
    init_data(mem, mem + 32, 32);

    // Call inline assembly instruction
    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: // byte
        errors = check_max_min_b((uint8_t*)mem, (uint8_t*)(mem + 32), (uint8_t*)(mem + 64), op);
        break;
    case 2: // half
        errors = check_max_min_h((uint16_t*)mem, (uint16_t*)(mem + 32), (uint16_t*)(mem + 64), op);
        break;
    case 4: // word
        errors = check_max_min_w((uint32_t*)mem, (uint32_t*)(mem + 32), (uint32_t*)(mem + 64), op);
        break;
    case 8: // double
        errors = check_max_min_d((uint64_t*)mem, (uint64_t*)(mem + 32), (uint64_t*)(mem + 64), op);
        break;
    }
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// Test function for clo/clz operations (single source)
void test_cloz(const char* name, void (*asm_func)(void* mem), int width, int op)
{
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // Initialize source data in first 32 bytes
    init_data(mem, mem + 32, 32);

    // Call inline assembly instruction
    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: // byte
        errors = check_cl((uint8_t*)mem, (uint8_t*)(mem + 64), op);
        break;
    case 2: // half
        errors = check_cl_h((uint16_t*)mem, (uint16_t*)(mem + 64), op);
        break;
    case 4: // word
        errors = check_cl_w((uint32_t*)mem, (uint32_t*)(mem + 64), op);
        break;
    case 8: // double
        errors = check_cl_d((uint64_t*)mem, (uint64_t*)(mem + 64), op);
        break;
    }
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// Inline assembly wrappers for xvmax (signed)
static void asm_xvmax_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmax_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmax_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmax_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// Inline assembly wrappers for xvmax (unsigned)
static void asm_xvmax_bu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.bu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmax_hu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.hu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmax_wu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.wu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmax_du(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmax.du $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// Inline assembly wrappers for xvmin (signed)
static void asm_xvmin_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmin_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmin_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmin_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// Inline assembly wrappers for xvmin (unsigned)
static void asm_xvmin_bu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.bu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmin_hu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.hu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmin_wu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.wu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvmin_du(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvmin.du $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// Inline assembly wrappers for xvclo
static void asm_xvclo_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclo.b $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}
static void asm_xvclo_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclo.h $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}
static void asm_xvclo_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclo.w $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}
static void asm_xvclo_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclo.d $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}

// Inline assembly wrappers for xvclz
static void asm_xvclz_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclz.b $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}
static void asm_xvclz_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclz.h $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}
static void asm_xvclz_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclz.w $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}
static void asm_xvclz_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvclz.d $xr2, $xr0\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr2");
}

int main()
{
    // Test xvmax (signed)
    test_max_min("xvmax.b", asm_xvmax_b, 1, 0);
    test_max_min("xvmax.h", asm_xvmax_h, 2, 0);
    test_max_min("xvmax.w", asm_xvmax_w, 4, 0);
    test_max_min("xvmax.d", asm_xvmax_d, 8, 0);

    // Test xvmax (unsigned)
    test_max_min("xvmax.bu", asm_xvmax_bu, 1, 1);
    test_max_min("xvmax.hu", asm_xvmax_hu, 2, 1);
    test_max_min("xvmax.wu", asm_xvmax_wu, 4, 1);
    test_max_min("xvmax.du", asm_xvmax_du, 8, 1);

    // Test xvmin (signed)
    test_max_min("xvmin.b", asm_xvmin_b, 1, 2);
    test_max_min("xvmin.h", asm_xvmin_h, 2, 2);
    test_max_min("xvmin.w", asm_xvmin_w, 4, 2);
    test_max_min("xvmin.d", asm_xvmin_d, 8, 2);

    // Test xvmin (unsigned)
    test_max_min("xvmin.bu", asm_xvmin_bu, 1, 3);
    test_max_min("xvmin.hu", asm_xvmin_hu, 2, 3);
    test_max_min("xvmin.wu", asm_xvmin_wu, 4, 3);
    test_max_min("xvmin.du", asm_xvmin_du, 8, 3);

    // Test xvclo
    test_cloz("xvclo.b", asm_xvclo_b, 1, 0);
    test_cloz("xvclo.h", asm_xvclo_h, 2, 0);
    test_cloz("xvclo.w", asm_xvclo_w, 4, 0);
    test_cloz("xvclo.d", asm_xvclo_d, 8, 0);

    // Test xvclz
    test_cloz("xvclz.b", asm_xvclz_b, 1, 1);
    test_cloz("xvclz.h", asm_xvclz_h, 2, 1);
    test_cloz("xvclz.w", asm_xvclz_w, 4, 1);
    test_cloz("xvclz.d", asm_xvclz_d, 8, 1);

    return 0;
}
