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
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

unsigned char mem[96] __attribute__((aligned(32)));

void set_data(uint8_t *a, uint8_t *b, int size) {
    for (int i = 0; i < size; i++) { a[i] = i * 13 + 7; b[i] = i * 17 + 3; }
}

int check_clri_b(uint8_t *a, uint8_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] & ~(1U << pos);
        if (c[i] != expected) { printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_clri_h(uint16_t *a, uint16_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = a[i] & ~(1U << pos);
        if (c[i] != expected) { printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_clri_w(uint32_t *a, uint32_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = a[i] & ~(1U << pos);
        if (c[i] != expected) { printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_clri_d(uint64_t *a, uint64_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = a[i] & ~(1ULL << pos);
        if (c[i] != expected) { printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_seti_b(uint8_t *a, uint8_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] | (1U << pos);
        if (c[i] != expected) { printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_seti_h(uint16_t *a, uint16_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = a[i] | (1U << pos);
        if (c[i] != expected) { printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_seti_w(uint32_t *a, uint32_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = a[i] | (1U << pos);
        if (c[i] != expected) { printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_seti_d(uint64_t *a, uint64_t *c, int pos) {
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = a[i] | (1ULL << pos);
        if (c[i] != expected) { printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected); errors++; }
    }
    return errors;
}

// xvbitclri: bit clear immediate (pos encoded in instruction)
static void asm_xvbitclri_b(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitclri.b $xr2, $xr0, 3\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}
static void asm_xvbitclri_h(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitclri.h $xr2, $xr0, 5\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}
static void asm_xvbitclri_w(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitclri.w $xr2, $xr0, 7\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}
static void asm_xvbitclri_d(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitclri.d $xr2, $xr0, 11\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}

// xvbitseti: bit set immediate
static void asm_xvbitseti_b(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitseti.b $xr2, $xr0, 4\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}
static void asm_xvbitseti_h(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitseti.h $xr2, $xr0, 8\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}
static void asm_xvbitseti_w(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitseti.w $xr2, $xr0, 15\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}
static void asm_xvbitseti_d(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvbitseti.d $xr2, $xr0, 20\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr2");
}

void test_xvbitclri(const char *name, void (*asm_func)(void *mem), int width, int pos) {
    memset(mem, 0, sizeof(mem)); set_data(mem, mem + 32, 32); asm_func(mem);
    int errors = 0;
    if (width == 1) errors = check_clri_b((uint8_t*)mem, (uint8_t*)(mem+64), pos);
    else if (width == 2) errors = check_clri_h((uint16_t*)mem, (uint16_t*)(mem+64), pos);
    else if (width == 4) errors = check_clri_w((uint32_t*)mem, (uint32_t*)(mem+64), pos);
    else if (width == 8) errors = check_clri_d((uint64_t*)mem, (uint64_t*)(mem+64), pos);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

void test_xvbitseti(const char *name, void (*asm_func)(void *mem), int width, int pos) {
    memset(mem, 0, sizeof(mem)); set_data(mem, mem + 32, 32); asm_func(mem);
    int errors = 0;
    if (width == 1) errors = check_seti_b((uint8_t*)mem, (uint8_t*)(mem+64), pos);
    else if (width == 2) errors = check_seti_h((uint16_t*)mem, (uint16_t*)(mem+64), pos);
    else if (width == 4) errors = check_seti_w((uint32_t*)mem, (uint32_t*)(mem+64), pos);
    else if (width == 8) errors = check_seti_d((uint64_t*)mem, (uint64_t*)(mem+64), pos);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

int main() {
    printf("=== Bit clear/set immediate tests ===\n");
    test_xvbitclri("xvbitclri.b", asm_xvbitclri_b, 1, 3);
    test_xvbitclri("xvbitclri.h", asm_xvbitclri_h, 2, 5);
    test_xvbitclri("xvbitclri.w", asm_xvbitclri_w, 4, 7);
    test_xvbitclri("xvbitclri.d", asm_xvbitclri_d, 8, 11);
    test_xvbitseti("xvbitseti.b", asm_xvbitseti_b, 1, 4);
    test_xvbitseti("xvbitseti.h", asm_xvbitseti_h, 2, 8);
    test_xvbitseti("xvbitseti.w", asm_xvbitseti_w, 4, 15);
    test_xvbitseti("xvbitseti.d", asm_xvbitseti_d, 8, 20);
    return 0;
}
