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

void init_data(uint8_t* a, int size)
{
    for (int i = 0; i < size; i++) {
        a[i] = i * 13 + 7;
    }
}

int check_b(uint8_t* a, uint8_t* c, uint8_t imm, int add)
{
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = add ? (a[i] + imm) : (a[i] - imm);
        if (c[i] != expected) {
            printf("B[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_h(uint16_t* a, uint16_t* c, uint16_t imm, int add)
{
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = add ? (a[i] + imm) : (a[i] - imm);
        if (c[i] != expected) {
            printf("H[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_w(uint32_t* a, uint32_t* c, uint32_t imm, int add)
{
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = add ? (a[i] + imm) : (a[i] - imm);
        if (c[i] != expected) {
            printf("W[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_d(uint64_t* a, uint64_t* c, uint64_t imm, int add)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = add ? (a[i] + imm) : (a[i] - imm);
        if (c[i] != expected) {
            printf("D[%d]: got %" PRIu64 ", expected %" PRIu64 "\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

int check_ldi(uint64_t* c, uint64_t expected)
{
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        if (c[i] != expected) {
            printf("LDI[%d]: got 0x%016lx, expected 0x%016lx\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

static void asm_xvaddi_bu_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.bu $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_bu_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.bu $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_bu_0(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.bu $xr1, $xr0, 0\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_hu_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.hu $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_hu_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.hu $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_wu_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.wu $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_wu_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.wu $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_du_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.du $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvaddi_du_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvaddi.du $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_bu_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.bu $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_bu_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.bu $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_hu_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.hu $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_hu_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.hu $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_wu_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.wu $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_wu_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.wu $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_du_5(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.du $xr1, $xr0, 5\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvsubi_du_31(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvsubi.du $xr1, $xr0, 31\n"
                     "xvst $xr1, %0, 32\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

static void asm_xvldi_0(void* mem)
{
    __asm__ volatile("xvldi $xr0, 0\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvldi_1(void* mem)
{
    __asm__ volatile("xvldi $xr0, 1\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvldi_100(void* mem)
{
    __asm__ volatile("xvldi $xr0, 100\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvldi_2047(void* mem)
{
    __asm__ volatile("xvldi $xr0, 2047\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvldi_m1(void* mem)
{
    __asm__ volatile("xvldi $xr0, -1\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvldi_m100(void* mem)
{
    __asm__ volatile("xvldi $xr0, -100\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

static void asm_xvldi_m2048(void* mem)
{
    __asm__ volatile("xvldi $xr0, -2048\n"
                     "xvst $xr0, %0, 0\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0");
}

void test_inst(const char* name, void (*asm_func)(void* mem), int width, int add, uint32_t imm)
{
    unsigned char mem[64] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    init_data(mem, 32);
    asm_func(mem);

    int errors = 0;
    switch (width) {
    case 1: errors = check_b((uint8_t*)mem, (uint8_t*)(mem + 32), (uint8_t)imm, add); break;
    case 2: errors = check_h((uint16_t*)mem, (uint16_t*)(mem + 32), (uint16_t)imm, add); break;
    case 4: errors = check_w((uint32_t*)mem, (uint32_t*)(mem + 32), imm, add); break;
    case 8: errors = check_d((uint64_t*)mem, (uint64_t*)(mem + 32), imm, add); break;
    }
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

void test_ldi(const char* name, void (*asm_func)(void* mem), int64_t expected)
{
    unsigned char mem[32] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));
    asm_func(mem);

    int errors = check_ldi((uint64_t*)mem, expected);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

int main()
{
    printf("=== Testing xvaddi (add immediate, unsigned) ===\n");
    test_inst("xvaddi.bu (imm=5)", asm_xvaddi_bu_5, 1, 1, 5);
    test_inst("xvaddi.bu (imm=31)", asm_xvaddi_bu_31, 1, 1, 31);
    test_inst("xvaddi.bu (imm=0)", asm_xvaddi_bu_0, 1, 1, 0);

    test_inst("xvaddi.hu (imm=5)", asm_xvaddi_hu_5, 2, 1, 5);
    test_inst("xvaddi.hu (imm=31)", asm_xvaddi_hu_31, 2, 1, 31);

    test_inst("xvaddi.wu (imm=5)", asm_xvaddi_wu_5, 4, 1, 5);
    test_inst("xvaddi.wu (imm=31)", asm_xvaddi_wu_31, 4, 1, 31);

    test_inst("xvaddi.du (imm=5)", asm_xvaddi_du_5, 8, 1, 5);
    test_inst("xvaddi.du (imm=31)", asm_xvaddi_du_31, 8, 1, 31);

    printf("\n=== Testing xvsubi (subtract immediate, unsigned) ===\n");
    test_inst("xvsubi.bu (imm=5)", asm_xvsubi_bu_5, 1, 0, 5);
    test_inst("xvsubi.bu (imm=31)", asm_xvsubi_bu_31, 1, 0, 31);

    test_inst("xvsubi.hu (imm=5)", asm_xvsubi_hu_5, 2, 0, 5);
    test_inst("xvsubi.hu (imm=31)", asm_xvsubi_hu_31, 2, 0, 31);

    test_inst("xvsubi.wu (imm=5)", asm_xvsubi_wu_5, 4, 0, 5);
    test_inst("xvsubi.wu (imm=31)", asm_xvsubi_wu_31, 4, 0, 31);

    test_inst("xvsubi.du (imm=5)", asm_xvsubi_du_5, 8, 0, 5);
    test_inst("xvsubi.du (imm=31)", asm_xvsubi_du_31, 8, 0, 31);

    printf("\n=== Testing xvldi (load immediate, Set_Value encoding) ===\n");
    test_ldi("xvldi (0)", asm_xvldi_0, 0x0000000000000000ULL);
    test_ldi("xvldi (1)", asm_xvldi_1, 0x0101010101010101ULL);
    test_ldi("xvldi (100)", asm_xvldi_100, 0x6464646464646464ULL);
    test_ldi("xvldi (2047)", asm_xvldi_2047, 0xffffffffffffffffULL);
    test_ldi("xvldi (-1)", asm_xvldi_m1, 0x0000000000000000ULL);
    test_ldi("xvldi (-100)", asm_xvldi_m100, 0x0000000000000000ULL);
    test_ldi("xvldi (-2048)", asm_xvldi_m2048, 0x0000000000000000ULL);

    return 0;
}