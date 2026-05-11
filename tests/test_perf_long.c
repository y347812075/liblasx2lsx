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

// test_perf_long.c - 长时间运行测试，用于验证性能统计功能
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// 简单的向量加法测试（字节宽度）
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

// 向量加法（半字宽度）
static void asm_xvadd_h(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvadd.h $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量加法（字宽度）
static void asm_xvadd_w(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvadd.w $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量加法（双字宽度）
static void asm_xvadd_d(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvadd.d $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量乘法（字节宽度）
static void asm_xvmul_b(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvmul.b $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量AND
static void asm_xvand_v(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvand.v $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量OR
static void asm_xvor_v(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvor.v $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量XOR
static void asm_xvxor_v(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvxor.v $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

// 向量左移
static void asm_xvsll_b(void *mem) {
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvsll.b $xr2, $xr0, $xr1\n"
        "xvst $xr2, %0, 64\n"
        :
        : "r"(mem)
        : "memory", "$xr0", "$xr1", "$xr2"
    );
}

int main(int argc, char *argv[]) {
    // 默认运行5秒
    int duration = 5;
    if (argc > 1) {
        duration = atoi(argv[1]);
        if (duration <= 0) duration = 5;
    }

    printf("Running performance test for %d seconds...\n", duration);
    printf("Set LASX_PERF_STATS=1 to see statistics output.\n\n");

    unsigned char mem[96] __attribute__((aligned(32)));
    
    // 初始化数据
    for (int i = 0; i < 64; i++) {
        mem[i] = i + 1;
    }

    // 函数指针数组，用于随机调用不同的指令
    void (*funcs[])(void*) = {
        asm_xvadd_b, asm_xvadd_h, asm_xvadd_w, asm_xvadd_d,
        asm_xvmul_b, asm_xvand_v, asm_xvor_v, asm_xvxor_v,
        asm_xvsll_b
    };
    int num_funcs = sizeof(funcs) / sizeof(funcs[0]);

    // 运行指定时间
    int elapsed = 0;
    uint64_t iterations = 0;
    
    while (elapsed < duration) {
        // 每批运行100000次迭代
        for (int i = 0; i < 100000; i++) {
            // 轮流调用不同的函数
            funcs[iterations % num_funcs](mem);
            iterations++;
        }
        
        // 每批后休眠100毫秒，让性能统计有机会打印
        usleep(100000);
        elapsed++;
    }

    printf("\nCompleted %lu iterations in %d seconds.\n", iterations, duration);
    
    return 0;
}
