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

// Test xvseq, xvsle, xvslt

void init_data(uint8_t* a, uint8_t* b, int size)
{
    for (int i = 0; i < size; i++) {
        a[i] = i * 13 + 7;
        b[i] = i * 17 + 3;
    }
}

// Check seq/sle/slt byte results (32 elements)
int check_cmp_b(uint8_t* a, uint8_t* b, uint8_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        int8_t as = (int8_t)a[i];
        int8_t bs = (int8_t)b[i];
        uint8_t expected = 0;
        if (op == 0) { // xvseq.b signed
            expected = (as == bs) ? 0xFF : 0;
        } else if (op == 1) { // xvseq.bu unsigned
            expected = (a[i] == b[i]) ? 0xFF : 0;
        } else if (op == 2) { // xvsle.b signed
            expected = (as <= bs) ? 0xFF : 0;
        } else if (op == 3) { // xvsle.bu unsigned
            expected = (a[i] <= b[i]) ? 0xFF : 0;
        } else if (op == 4) { // xvslt.b signed
            expected = (as < bs) ? 0xFF : 0;
        } else if (op == 5) { // xvslt.bu unsigned
            expected = (a[i] < b[i]) ? 0xFF : 0;
        }
        if (c[i] != expected) {
            printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check seq/sle/slt half results (16 elements)
int check_cmp_h(uint16_t* a, uint16_t* b, uint16_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        int16_t as = (int16_t)a[i];
        int16_t bs = (int16_t)b[i];
        uint16_t expected = 0;
        if (op == 0) { // xvseq.h signed
            expected = (as == bs) ? 0xFFFF : 0;
        } else if (op == 1) { // xvseq.hu unsigned
            expected = (a[i] == b[i]) ? 0xFFFF : 0;
        } else if (op == 2) { // xvsle.h signed
            expected = (as <= bs) ? 0xFFFF : 0;
        } else if (op == 3) { // xvsle.hu unsigned
            expected = (a[i] <= b[i]) ? 0xFFFF : 0;
        } else if (op == 4) { // xvslt.h signed
            expected = (as < bs) ? 0xFFFF : 0;
        } else if (op == 5) { // xvslt.hu unsigned
            expected = (a[i] < b[i]) ? 0xFFFF : 0;
        }
        if (c[i] != expected) {
            printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check seq/sle/slt word results (8 elements)
int check_cmp_w(uint32_t* a, uint32_t* b, uint32_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        int32_t as = (int32_t)a[i];
        int32_t bs = (int32_t)b[i];
        uint32_t expected = 0;
        if (op == 0) { // xvseq.w signed
            expected = (as == bs) ? 0xFFFFFFFF : 0;
        } else if (op == 1) { // xvseq.wu unsigned
            expected = (a[i] == b[i]) ? 0xFFFFFFFF : 0;
        } else if (op == 2) { // xvsle.w signed
            expected = (as <= bs) ? 0xFFFFFFFF : 0;
        } else if (op == 3) { // xvsle.wu unsigned
            expected = (a[i] <= b[i]) ? 0xFFFFFFFF : 0;
        } else if (op == 4) { // xvslt.w signed
            expected = (as < bs) ? 0xFFFFFFFF : 0;
        } else if (op == 5) { // xvslt.wu unsigned
            expected = (a[i] < b[i]) ? 0xFFFFFFFF : 0;
        }
        if (c[i] != expected) {
            printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Check seq/sle/slt double results (4 elements)
int check_cmp_d(uint64_t* a, uint64_t* b, uint64_t* c, int op)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        int64_t as = (int64_t)a[i];
        int64_t bs = (int64_t)b[i];
        uint64_t expected = 0;
        if (op == 0) { // xvseq.d signed
            expected = (as == bs) ? 0xFFFFFFFFFFFFFFFFULL : 0;
        } else if (op == 1) { // xvseq.du unsigned
            expected = (a[i] == b[i]) ? 0xFFFFFFFFFFFFFFFFULL : 0;
        } else if (op == 2) { // xvsle.d signed
            expected = (as <= bs) ? 0xFFFFFFFFFFFFFFFFULL : 0;
        } else if (op == 3) { // xvsle.du unsigned
            expected = (a[i] <= b[i]) ? 0xFFFFFFFFFFFFFFFFULL : 0;
        } else if (op == 4) { // xvslt.d signed
            expected = (as < bs) ? 0xFFFFFFFFFFFFFFFFULL : 0;
        } else if (op == 5) { // xvslt.du unsigned
            expected = (a[i] < b[i]) ? 0xFFFFFFFFFFFFFFFFULL : 0;
        }
        if (c[i] != expected) {
            printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// Inline assembly wrappers for each instruction

// xvseq.b
static void asm_xvseq_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvseq.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvseq.h
static void asm_xvseq_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvseq.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvseq.w
static void asm_xvseq_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvseq.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvseq.d
static void asm_xvseq_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvseq.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

/* xvseq.bu/hu/wu/du not available in LASX - using signed version only */

// xvsle.b
static void asm_xvsle_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.h
static void asm_xvsle_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.w
static void asm_xvsle_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.d
static void asm_xvsle_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.bu
static void asm_xvsle_bu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.bu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.hu
static void asm_xvsle_hu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.hu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.wu
static void asm_xvsle_wu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.wu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvsle.du
static void asm_xvsle_du(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvsle.du $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.b
static void asm_xvslt_b(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.b $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.h
static void asm_xvslt_h(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.h $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.w
static void asm_xvslt_w(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.w $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.d
static void asm_xvslt_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.bu
static void asm_xvslt_bu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.bu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.hu
static void asm_xvslt_hu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.hu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.wu
static void asm_xvslt_wu(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.wu $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// xvslt.du
static void asm_xvslt_du(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvslt.du $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}

// Test function
void test_inst(const char* name, void (*asm_func)(void* mem), int width, int op)
{
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: // byte
        errors = check_cmp_b((uint8_t*)mem, (uint8_t*)(mem + 32), (uint8_t*)(mem + 64), op);
        break;
    case 2: // half
        errors = check_cmp_h((uint16_t*)mem, (uint16_t*)(mem + 32), (uint16_t*)(mem + 64), op);
        break;
    case 4: // word
        errors = check_cmp_w((uint32_t*)mem, (uint32_t*)(mem + 32), (uint32_t*)(mem + 64), op);
        break;
    case 8: // double
        errors = check_cmp_d((uint64_t*)mem, (uint64_t*)(mem + 32), (uint64_t*)(mem + 64), op);
        break;
    }
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

int main()
{
    // xvseq tests (signed only - unsigned not available in LASX)
    test_inst("xvseq.b", asm_xvseq_b, 1, 0);
    test_inst("xvseq.h", asm_xvseq_h, 2, 0);
    test_inst("xvseq.w", asm_xvseq_w, 4, 0);
    test_inst("xvseq.d", asm_xvseq_d, 8, 0);

    // xvsle tests (op = 2 for signed, 3 for unsigned)
    test_inst("xvsle.b", asm_xvsle_b, 1, 2);
    test_inst("xvsle.h", asm_xvsle_h, 2, 2);
    test_inst("xvsle.w", asm_xvsle_w, 4, 2);
    test_inst("xvsle.d", asm_xvsle_d, 8, 2);
    test_inst("xvsle.bu", asm_xvsle_bu, 1, 3);
    test_inst("xvsle.hu", asm_xvsle_hu, 2, 3);
    test_inst("xvsle.wu", asm_xvsle_wu, 4, 3);
    test_inst("xvsle.du", asm_xvsle_du, 8, 3);

    // xvslt tests (op = 4 for signed, 5 for unsigned)
    test_inst("xvslt.b", asm_xvslt_b, 1, 4);
    test_inst("xvslt.h", asm_xvslt_h, 2, 4);
    test_inst("xvslt.w", asm_xvslt_w, 4, 4);
    test_inst("xvslt.d", asm_xvslt_d, 8, 4);
    test_inst("xvslt.bu", asm_xvslt_bu, 1, 5);
    test_inst("xvslt.hu", asm_xvslt_hu, 2, 5);
    test_inst("xvslt.wu", asm_xvslt_wu, 4, 5);
    test_inst("xvslt.du", asm_xvslt_du, 8, 5);

    return 0;
}
