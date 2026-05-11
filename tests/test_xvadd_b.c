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

// 辅助：随机初始化数据，但保持可重复性
void init_data(uint8_t *a, uint8_t *b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = i * 13 + 7;
        b[i] = i * 17 + 3;
    }
}

// 检查字节宽度结果 (32个元素)
int check_b(uint8_t *a, uint8_t *b, uint8_t *c, int add) {
    int errors = 0;
    for (int i = 0; i < 32; i++) {
        uint8_t expected = add ? (a[i] + b[i]) : (a[i] - b[i]);
        if (c[i] != expected) {
            printf("B[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// 检查半字宽度结果 (16个元素)
int check_h(uint16_t *a, uint16_t *b, uint16_t *c, int add) {
    int errors = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t expected = add ? (a[i] + b[i]) : (a[i] - b[i]);
        if (c[i] != expected) {
            printf("H[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// 检查字宽度结果 (8个元素)
int check_w(uint32_t *a, uint32_t *b, uint32_t *c, int add) {
    int errors = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t expected = add ? (a[i] + b[i]) : (a[i] - b[i]);
        if (c[i] != expected) {
            printf("W[%d]: got %u, expected %u\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// 检查双字宽度结果 (4个元素)
int check_d(uint64_t *a, uint64_t *b, uint64_t *c, int add) {
    int errors = 0;
    for (int i = 0; i < 4; i++) {
        uint64_t expected = add ? (a[i] + b[i]) : (a[i] - b[i]);
        if (c[i] != expected) {
            printf("D[%d]: got %" PRIu64 ", expected %" PRIu64 "\n", i, c[i], expected);
            errors++;
        }
    }
    return errors;
}

// 检查四字宽度结果 (2个元素)
int check_q(unsigned __int128 *a, unsigned __int128 *b, unsigned __int128 *c, int add) {
    int errors = 0;
    for (int i = 0; i < 2; i++) {
        unsigned __int128 expected = add ? (a[i] + b[i]) : (a[i] - b[i]);
        if (c[i] != expected) {
            uint64_t c_lo = (uint64_t)c[i];
            uint64_t c_hi = (uint64_t)(c[i] >> 64);
            uint64_t e_lo = (uint64_t)expected;
            uint64_t e_hi = (uint64_t)(expected >> 64);
            printf("Q[%d]: got 0x%016" PRIx64 "%016" PRIx64
                   ", expected 0x%016" PRIx64 "%016" PRIx64 "\n",
                   i, c_hi, c_lo, e_hi, e_lo);
            errors++;
        }
    }
    return errors;
}

// 通用测试函数：调用指定指令并检查
void test_inst(const char *name, void (*asm_func)(void *mem), int width, int add) {
    // 分配96字节：srcA(32), srcB(32), dst(32)
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // 初始化源数据
    init_data(mem, mem + 32, 32);  // 各32字节

    // 调用内联汇编指令
    asm_func(mem);

    int errors = 0;
    switch (width) {
        case 1: // byte
            errors = check_b((uint8_t*)mem, (uint8_t*)(mem+32), (uint8_t*)(mem+64), add);
            break;
        case 2: // half
            errors = check_h((uint16_t*)mem, (uint16_t*)(mem+32), (uint16_t*)(mem+64), add);
            break;
        case 4: // word
            errors = check_w((uint32_t*)mem, (uint32_t*)(mem+32), (uint32_t*)(mem+64), add);
            break;
        case 8: // double
            errors = check_d((uint64_t*)mem, (uint64_t*)(mem+32), (uint64_t*)(mem+64), add);
            break;
        case 16: // quad
            errors = check_q((unsigned __int128*)mem, (unsigned __int128*)(mem+32),
                             (unsigned __int128*)(mem+64), add);
            break;
    }
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// 内联汇编包装器（每种宽度一个）
static void asm_xvadd_b(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvadd.b $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

int main() {
    test_inst("xvadd.b", asm_xvadd_b, 1, 1);
    test_inst("xvadd.b", asm_xvadd_b, 1, 1);
    test_inst("xvadd.b", asm_xvadd_b, 1, 1);
    test_inst("xvadd.b", asm_xvadd_b, 1, 1);
    return 0;
}
