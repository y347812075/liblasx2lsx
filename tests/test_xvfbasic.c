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
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 测试值
#define NUM_TEST_VALUES 6
static const float test_values_f[NUM_TEST_VALUES] = {1.0f, 2.0f, -1.0f, 0.0f, 0.5f, 3.14159f};
static const double test_values_d[NUM_TEST_VALUES] = {1.0, 2.0, -1.0, 0.0, 0.5, 3.14159};

// 初始化单精度浮点数据
void init_data_f(float* a, float* b, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_f[i % NUM_TEST_VALUES];
        b[i] = test_values_f[(i + 3) % NUM_TEST_VALUES];
    }
}

// 初始化双精度浮点数据
void init_data_d(double* a, double* b, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_d[i % NUM_TEST_VALUES];
        b[i] = test_values_d[(i + 3) % NUM_TEST_VALUES];
    }
}

// 检查单精度浮点结果
int check_float(float* a, float* b, float* c, int count, int op)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        float expected;
        switch (op) {
        case 0: expected = a[i] + b[i]; break;       // add
        case 1: expected = a[i] - b[i]; break;       // sub
        case 2: expected = a[i] * b[i]; break;       // mul
        case 3: expected = a[i] / b[i]; break;       // div
        case 4: expected = fmaxf(a[i], b[i]); break; // max
        case 5: expected = fminf(a[i], b[i]); break; // min
        case 6: expected = sqrtf(a[i]); break;       // sqrt
        case 7: expected = 1.0f / a[i]; break;       // recip
        default: expected = 0; break;
        }
        float diff = fabsf(c[i] - expected);
        float rel = (expected != 0) ? diff / fabsf(expected) : diff;
        if (rel > 1e-5f) {
            printf("F[%d]: got %f, expected %f (diff=%e, rel=%e)\n", i, c[i], expected, diff, rel);
            errors++;
        }
    }
    return errors;
}

// 检查双精度浮点结果
int check_double(double* a, double* b, double* c, int count, int op)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        double expected;
        switch (op) {
        case 0: expected = a[i] + b[i]; break;      // add
        case 1: expected = a[i] - b[i]; break;      // sub
        case 2: expected = a[i] * b[i]; break;      // mul
        case 3: expected = a[i] / b[i]; break;      // div
        case 4: expected = fmax(a[i], b[i]); break; // max
        case 5: expected = fmin(a[i], b[i]); break; // min
        case 6: expected = sqrt(a[i]); break;       // sqrt
        case 7: expected = 1.0 / a[i]; break;       // recip
        default: expected = 0; break;
        }
        double diff = fabs(c[i] - expected);
        double rel = (expected != 0) ? diff / fabs(expected) : diff;
        if (rel > 1e-10) {
            printf("D[%d]: got %f, expected %f (diff=%e, rel=%e)\n", i, c[i], expected, diff, rel);
            errors++;
        }
    }
    return errors;
}

// 单精度浮点运算 - 8个元素
void test_float_op(const char* name, void (*asm_func)(void* mem), int op)
{
    // 分配96字节：srcA(32), srcB(32), dst(32)
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // 初始化源数据 - 8个float
    init_data_f((float*)mem, (float*)(mem + 32), 8);

    // 调用内联汇编指令
    asm_func(mem);

    int errors = check_float((float*)mem, (float*)(mem + 32), (float*)(mem + 64), 8, op);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// 双精度浮点运算 - 4个元素
void test_double_op(const char* name, void (*asm_func)(void* mem), int op)
{
    // 分配96字节：srcA(32), srcB(32), dst(32)
    unsigned char mem[96] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // 初始化源数据 - 4个double
    init_data_d((double*)mem, (double*)(mem + 32), 4);

    // 调用内联汇编指令
    asm_func(mem);

    int errors = check_double((double*)mem, (double*)(mem + 32), (double*)(mem + 64), 4, op);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// 单精度浮点指令包装器
static void asm_xvfadd_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfadd.s $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfsub_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfsub.s $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfmul_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfmul.s $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfdiv_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfdiv.s $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfmax_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfmax.s $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfmin_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfmin.s $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfsqrt_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvfsqrt.s $xr1, $xr0\n"
                     "xvst $xr1, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}
static void asm_xvfrecip_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvfrecip.s $xr1, $xr0\n"
                     "xvst $xr1, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

// 双精度浮点指令包装器
static void asm_xvfadd_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfadd.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfsub_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfsub.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfmul_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfmul.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfdiv_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfdiv.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfmax_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfmax.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfmin_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvfmin.d $xr2, $xr0, $xr1\n"
                     "xvst $xr2, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2");
}
static void asm_xvfsqrt_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvfsqrt.d $xr1, $xr0\n"
                     "xvst $xr1, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}
static void asm_xvfrecip_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvfrecip.d $xr1, $xr0\n"
                     "xvst $xr1, %0, 64\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1");
}

int main()
{
    printf("=== Single Precision Float (.s) ===\n");
    test_float_op("xvfadd.s", asm_xvfadd_s, 0);
    test_float_op("xvfsub.s", asm_xvfsub_s, 1);
    test_float_op("xvfmul.s", asm_xvfmul_s, 2);
    test_float_op("xvfdiv.s", asm_xvfdiv_s, 3);
    test_float_op("xvfmax.s", asm_xvfmax_s, 4);
    test_float_op("xvfmin.s", asm_xvfmin_s, 5);
    test_float_op("xvfsqrt.s", asm_xvfsqrt_s, 6);
    test_float_op("xvfrecip.s", asm_xvfrecip_s, 7);

    printf("=== Double Precision Float (.d) ===\n");
    test_double_op("xvfadd.d", asm_xvfadd_d, 0);
    test_double_op("xvfsub.d", asm_xvfsub_d, 1);
    test_double_op("xvfmul.d", asm_xvfmul_d, 2);
    test_double_op("xvfdiv.d", asm_xvfdiv_d, 3);
    test_double_op("xvfmax.d", asm_xvfmax_d, 4);
    test_double_op("xvfmin.d", asm_xvfmin_d, 5);
    test_double_op("xvfsqrt.d", asm_xvfsqrt_d, 6);
    test_double_op("xvfrecip.d", asm_xvfrecip_d, 7);

    return 0;
}