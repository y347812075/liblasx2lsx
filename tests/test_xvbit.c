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

int check_clr_b(uint8_t *a, uint8_t *b, uint8_t *c) {
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] & ~(1U << (b[i] & 7));
        if (c[i] != expected) { printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_clr_h(uint16_t *a, uint16_t *b, uint16_t *c) {
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = a[i] & ~(1U << (b[i] & 15));
        if (c[i] != expected) { printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_clr_w(uint32_t *a, uint32_t *b, uint32_t *c) {
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = a[i] & ~(1U << (b[i] & 31));
        if (c[i] != expected) { printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_clr_d(uint64_t *a, uint64_t *b, uint64_t *c) {
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = a[i] & ~(1ULL << (b[i] & 63));
        if (c[i] != expected) { printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_set_b(uint8_t *a, uint8_t *b, uint8_t *c) {
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] | (1U << (b[i] & 7));
        if (c[i] != expected) { printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_set_h(uint16_t *a, uint16_t *b, uint16_t *c) {
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = a[i] | (1U << (b[i] & 15));
        if (c[i] != expected) { printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_set_w(uint32_t *a, uint32_t *b, uint32_t *c) {
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = a[i] | (1U << (b[i] & 31));
        if (c[i] != expected) { printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_set_d(uint64_t *a, uint64_t *b, uint64_t *c) {
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = a[i] | (1ULL << (b[i] & 63));
        if (c[i] != expected) { printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_rev_b(uint8_t *a, uint8_t *b, uint8_t *c) {
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = a[i] ^ (1U << (b[i] & 7));
        if (c[i] != expected) { printf("B[%d]: got 0x%02x, expected 0x%02x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_rev_h(uint16_t *a, uint16_t *b, uint16_t *c) {
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = a[i] ^ (1U << (b[i] & 15));
        if (c[i] != expected) { printf("H[%d]: got 0x%04x, expected 0x%04x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_rev_w(uint32_t *a, uint32_t *b, uint32_t *c) {
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = a[i] ^ (1U << (b[i] & 31));
        if (c[i] != expected) { printf("W[%d]: got 0x%08x, expected 0x%08x\n", i, c[i], expected); errors++; }
    }
    return errors;
}

int check_rev_d(uint64_t *a, uint64_t *b, uint64_t *c) {
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = a[i] ^ (1ULL << (b[i] & 63));
        if (c[i] != expected) { printf("D[%d]: got 0x%016" PRIx64 ", expected 0x%016" PRIx64 "\n", i, c[i], expected); errors++; }
    }
    return errors;
}

static void asm_xvbitclr_b(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitclr.b $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitclr_h(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitclr.h $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitclr_w(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitclr.w $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitclr_d(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitclr.d $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitset_b(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitset.b $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitset_h(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitset.h $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitset_w(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitset.w $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitset_d(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitset.d $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitrev_b(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitrev.b $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitrev_h(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitrev.h $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitrev_w(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitrev.w $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvbitrev_d(void *mem) {
    __asm__ volatile("xvld $xr0, %0, 0\nxvld $xr1, %0, 32\nxvbitrev.d $xr2, $xr0, $xr1\nxvst $xr2, %0, 64" : : "r"(mem) : "memory", "$xr0", "$xr1", "$xr2");
}

void test_xvbitclr_reg(const char *name, void (*asm_func)(void *mem), int width) {
    memset(mem, 0, sizeof(mem)); set_data(mem, mem + 32, 32); asm_func(mem);
    int errors = 0;
    if (width == 1) errors = check_clr_b((uint8_t*)mem, (uint8_t*)(mem+32), (uint8_t*)(mem+64));
    else if (width == 2) errors = check_clr_h((uint16_t*)mem, (uint16_t*)(mem+32), (uint16_t*)(mem+64));
    else if (width == 4) errors = check_clr_w((uint32_t*)mem, (uint32_t*)(mem+32), (uint32_t*)(mem+64));
    else if (width == 8) errors = check_clr_d((uint64_t*)mem, (uint64_t*)(mem+32), (uint64_t*)(mem+64));
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

void test_xvbitset_reg(const char *name, void (*asm_func)(void *mem), int width) {
    memset(mem, 0, sizeof(mem)); set_data(mem, mem + 32, 32); asm_func(mem);
    int errors = 0;
    if (width == 1) errors = check_set_b((uint8_t*)mem, (uint8_t*)(mem+32), (uint8_t*)(mem+64));
    else if (width == 2) errors = check_set_h((uint16_t*)mem, (uint16_t*)(mem+32), (uint16_t*)(mem+64));
    else if (width == 4) errors = check_set_w((uint32_t*)mem, (uint32_t*)(mem+32), (uint32_t*)(mem+64));
    else if (width == 8) errors = check_set_d((uint64_t*)mem, (uint64_t*)(mem+32), (uint64_t*)(mem+64));
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

void test_xvbitrev_reg(const char *name, void (*asm_func)(void *mem), int width) {
    memset(mem, 0, sizeof(mem)); set_data(mem, mem + 32, 32); asm_func(mem);
    int errors = 0;
    if (width == 1) errors = check_rev_b((uint8_t*)mem, (uint8_t*)(mem+32), (uint8_t*)(mem+64));
    else if (width == 2) errors = check_rev_h((uint16_t*)mem, (uint16_t*)(mem+32), (uint16_t*)(mem+64));
    else if (width == 4) errors = check_rev_w((uint32_t*)mem, (uint32_t*)(mem+32), (uint32_t*)(mem+64));
    else if (width == 8) errors = check_rev_d((uint64_t*)mem, (uint64_t*)(mem+32), (uint64_t*)(mem+64));
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

int main() {
    printf("=== Bit clear/set/reverse (register) tests ===\n");
    test_xvbitclr_reg("xvbitclr.b", asm_xvbitclr_b, 1);
    test_xvbitclr_reg("xvbitclr.h", asm_xvbitclr_h, 2);
    test_xvbitclr_reg("xvbitclr.w", asm_xvbitclr_w, 4);
    test_xvbitclr_reg("xvbitclr.d", asm_xvbitclr_d, 8);
    test_xvbitset_reg("xvbitset.b", asm_xvbitset_b, 1);
    test_xvbitset_reg("xvbitset.h", asm_xvbitset_h, 2);
    test_xvbitset_reg("xvbitset.w", asm_xvbitset_w, 4);
    test_xvbitset_reg("xvbitset.d", asm_xvbitset_d, 8);
    test_xvbitrev_reg("xvbitrev.b", asm_xvbitrev_b, 1);
    test_xvbitrev_reg("xvbitrev.h", asm_xvbitrev_h, 2);
    test_xvbitrev_reg("xvbitrev.w", asm_xvbitrev_w, 4);
    test_xvbitrev_reg("xvbitrev.d", asm_xvbitrev_d, 8);
    return 0;
}
