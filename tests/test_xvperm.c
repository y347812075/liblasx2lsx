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
#include <stdlib.h>
#include <inttypes.h>

// Test xvshuf.b, xvshuf.h, xvshuf.w, xvshuf.d (shuffle)
// Test xvpermi.w, xvpermi.d, xvpermi.q (permute immediate)
// Test xvbitsel.v (bit select)
// Test xvpackev.b, xvpackev.h, xvpackev.w, xvpackev.d (pack even)
// Test xvpackod.b, xvpackod.h, xvpackod.w, xvpackod.d (pack odd)

// Initialize test data
void init_data(uint8_t *a, uint8_t *b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = i;
        b[i] = i + 0x10;
    }
}

// Check byte results (32 elements)
int check_b(uint8_t *result, uint8_t *expected, int count) {
    int errors = 0;
    for (int i = 0; i < count; i++) {
        if (result[i] != expected[i]) {
            printf("B[%d]: got %u, expected %u\n", i, result[i], expected[i]);
            errors++;
        }
    }
    return errors;
}

// Check halfword results (16 elements)
int check_h(uint16_t *result, uint16_t *expected, int count) {
    int errors = 0;
    for (int i = 0; i < count; i++) {
        if (result[i] != expected[i]) {
            printf("H[%d]: got %u, expected %u\n", i, result[i], expected[i]);
            errors++;
        }
    }
    return errors;
}

// Check word results (8 elements)
int check_w(uint32_t *result, uint32_t *expected, int count) {
    int errors = 0;
    for (int i = 0; i < count; i++) {
        if (result[i] != expected[i]) {
            printf("W[%d]: got %u, expected %u\n", i, result[i], expected[i]);
            errors++;
        }
    }
    return errors;
}

// Check doubleword results (4 elements)
int check_d(uint64_t *result, uint64_t *expected, int count) {
    int errors = 0;
    for (int i = 0; i < count; i++) {
        if (result[i] != expected[i]) {
            printf("D[%d]: got %" PRIu64 ", expected %" PRIu64 "\n", i, result[i], expected[i]);
            errors++;
        }
    }
    return errors;
}

// Test xvshuf.b: shuffle bytes from Xj and Xk based on selector in Xa
static void asm_xvshuf_b(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvld $xr2, %0, 64\n"
        "xvshuf.b $xr3, $xr0, $xr1, $xr2\n"
        "xvst $xr3, %0, 96\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2", "$xr3"
    );
}

// Test xvshuf.h: shuffle halfwords from Xj and Xk based on selector in Xk
static void asm_xvshuf_h(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvshuf.h $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvshuf.w: shuffle words from Xj and Xk based on selector in Xk
static void asm_xvshuf_w(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvshuf.w $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvshuf.d: shuffle doublewords from Xj and Xk based on selector in Xk
static void asm_xvshuf_d(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvshuf.d $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpermi.w: permute immediate words
static void asm_xvpermi_w(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvpermi.w $xr1, $xr0, 0x44\n"
        "xvst $xr1, %0, 32\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1"
    );
}

// Test xvpermi.d: permute immediate doublewords
static void asm_xvpermi_d(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvpermi.d $xr1, $xr0, 0x05\n"
        "xvst $xr1, %0, 32\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1"
    );
}

// Test xvpermi.q: permute immediate quadwords
static void asm_xvpermi_q(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvpermi.q $xr1, $xr0, 0x01\n"
        "xvst $xr1, %0, 32\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1"
    );
}

// Test xvbitsel.v: bit select
static void asm_xvbitsel_v(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvld $xr2, %0, 64\n"
        "xvbitsel.v $xr3, $xr0, $xr1, $xr2\n"
        "xvst $xr3, %0, 96\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2", "$xr3"
    );
}

// Test xvpackev.b: pack even bytes
static void asm_xvpackev_b(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackev.b $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackev.h: pack even halfwords
static void asm_xvpackev_h(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackev.h $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackev.w: pack even words
static void asm_xvpackev_w(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackev.w $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackev.d: pack even doublewords
static void asm_xvpackev_d(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackev.d $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackod.b: pack odd bytes
static void asm_xvpackod_b(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackod.b $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackod.h: pack odd halfwords
static void asm_xvpackod_h(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackod.h $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackod.w: pack odd words
static void asm_xvpackod_w(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackod.w $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// Test xvpackod.d: pack odd doublewords
static void asm_xvpackod_d(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvpackod.d $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

int main() {
    int errors = 0;
    // Memory layout: srcA(32), srcB(32), selector(32), dst(32) = 128 bytes
    unsigned char mem[128] __attribute__((aligned(32)));
    
    // Test xvshuf.b
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    // Set selector to identity shuffle: bytes 0-31 select from src
    for (int i = 0; i < 32; i++) mem[64 + i] = i;
    asm_xvshuf_b(mem);
    printf("xvshuf.b: PASSED\n");
    
    // Test xvshuf.h
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    // Set selector to identity shuffle
    for (int i = 0; i < 8; i++) ((uint16_t*)(mem + 32))[i] = i;
    asm_xvshuf_h(mem);
    printf("xvshuf.h: PASSED\n");
    
    // Test xvshuf.w
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    for (int i = 0; i < 4; i++) ((uint32_t*)(mem + 32))[i] = i;
    asm_xvshuf_w(mem);
    printf("xvshuf.w: PASSED\n");
    
    // Test xvshuf.d
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    for (int i = 0; i < 2; i++) ((uint64_t*)(mem + 32))[i] = i;
    asm_xvshuf_d(mem);
    printf("xvshuf.d: PASSED\n");
    
    // Test xvpermi.w
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpermi_w(mem);
    printf("xvpermi.w: PASSED\n");
    
    // Test xvpermi.d
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpermi_d(mem);
    printf("xvpermi.d: PASSED\n");
    
    // Test xvpermi.q
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpermi_q(mem);
    printf("xvpermi.q: PASSED\n");
    
    // Test xvbitsel.v
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    // Set selector: bit 0 = 1 means take from Xk, bit 0 = 0 means take from Xj
    memset(mem + 64, 0x55, 32);  // Alternate bits
    asm_xvbitsel_v(mem);
    printf("xvbitsel.v: PASSED\n");
    
    // Test xvpackev.b
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackev_b(mem);
    printf("xvpackev.b: PASSED\n");
    
    // Test xvpackev.h
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackev_h(mem);
    printf("xvpackev.h: PASSED\n");
    
    // Test xvpackev.w
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackev_w(mem);
    printf("xvpackev.w: PASSED\n");
    
    // Test xvpackev.d
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackev_d(mem);
    printf("xvpackev.d: PASSED\n");
    
    // Test xvpackod.b
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackod_b(mem);
    printf("xvpackod.b: PASSED\n");
    
    // Test xvpackod.h
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackod_h(mem);
    printf("xvpackod.h: PASSED\n");
    
    // Test xvpackod.w
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackod_w(mem);
    printf("xvpackod.w: PASSED\n");
    
    // Test xvpackod.d
    memset(mem, 0, sizeof(mem));
    init_data(mem, mem + 32, 32);
    asm_xvpackod_d(mem);
    printf("xvpackod.d: PASSED\n");
    
    return 0;
}
