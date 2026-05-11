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

// Test xvfmadd, xvfnmadd, xvfmsub, xvfnmsub (fused multiply-add)

// 测试值
#define NUM_TEST_VALUES 6
static const float test_values_f[NUM_TEST_VALUES] = {1.0f, 2.0f, -1.0f, 0.5f, 3.0f, 0.25f};
static const double test_values_d[NUM_TEST_VALUES] = {1.0, 2.0, -1.0, 0.5, 3.0, 0.25};

// 初始化单精度浮点数据
void init_data_f(float* a, float* b, float* c, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_f[i % NUM_TEST_VALUES];
        b[i] = test_values_f[(i + 2) % NUM_TEST_VALUES];
        c[i] = test_values_f[(i + 1) % NUM_TEST_VALUES];
    }
}

// 初始化双精度浮点数据
void init_data_d(double* a, double* b, double* c, int count)
{
    for (int i = 0; i < count; i++) {
        a[i] = test_values_d[i % NUM_TEST_VALUES];
        b[i] = test_values_d[(i + 2) % NUM_TEST_VALUES];
        c[i] = test_values_d[(i + 1) % NUM_TEST_VALUES];
    }
}

// 检查单精度浮点结果
// op: 0=xvfmadd_s, 1=xvfnmadd_s, 2=xvfmsub_s, 3=xvfnmsub_s
int check_float_f(float* a, float* b, float* c, float* result, int count, int op)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        float expected;
        switch (op) {
        case 0: expected = a[i] * b[i] + c[i]; break;    // xvfmadd: a*b + c
        case 1: expected = -(a[i] * b[i]) + c[i]; break; // xvfnmadd: -(a*b) + c
        case 2: expected = a[i] * b[i] - c[i]; break;    // xvfmsub: a*b - c
        case 3: expected = -(a[i] * b[i]) - c[i]; break; // xvfnmsub: -(a*b) - c
        default: expected = 0; break;
        }
        float diff = fabsf(result[i] - expected);
        float rel = (expected != 0) ? diff / fabsf(expected) : diff;
        if (rel > 1e-5f) {
            printf("F[%d]: got %f, expected %f (diff=%e, rel=%e)\n", i, result[i], expected, diff, rel);
            errors++;
        }
    }
    return errors;
}

// 检查双精度浮点结果
int check_float_d(double* a, double* b, double* c, double* result, int count, int op)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        double expected;
        switch (op) {
        case 0: expected = a[i] * b[i] + c[i]; break;    // xvfmadd: a*b + c
        case 1: expected = -(a[i] * b[i]) + c[i]; break; // xvfnmadd: -(a*b) + c
        case 2: expected = a[i] * b[i] - c[i]; break;    // xvfmsub: a*b - c
        case 3: expected = -(a[i] * b[i]) - c[i]; break; // xvfnmsub: -(a*b) - c
        default: expected = 0; break;
        }
        double diff = fabs(result[i] - expected);
        double rel = (expected != 0) ? diff / fabs(expected) : diff;
        if (rel > 1e-10) {
            printf("D[%d]: got %f, expected %f (diff=%e, rel=%e)\n", i, result[i], expected, diff, rel);
            errors++;
        }
    }
    return errors;
}

// 单精度 FMA 测试
void test_fma_s(const char* name, void (*asm_func)(void* mem), int op)
{
    // 分配128字节：srcA(32), srcB(32), srcC(32), dst(32)
    unsigned char mem[128] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // 初始化源数据 - 8个float
    init_data_f((float*)mem, (float*)(mem + 32), (float*)(mem + 64), 8);

    // 调用内联汇编指令
    asm_func(mem);

    int errors = check_float_f((float*)mem, (float*)(mem + 32), (float*)(mem + 64), (float*)(mem + 96), 8, op);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// 双精度 FMA 测试
void test_fma_d(const char* name, void (*asm_func)(void* mem), int op)
{
    // 分配128字节：srcA(32), srcB(32), srcC(32), dst(32)
    unsigned char mem[128] __attribute__((aligned(32)));
    memset(mem, 0, sizeof(mem));

    // 初始化源数据 - 4个double
    init_data_d((double*)mem, (double*)(mem + 32), (double*)(mem + 64), 4);

    // 调用内联汇编指令
    asm_func(mem);

    int errors = check_float_d((double*)mem, (double*)(mem + 32), (double*)(mem + 64), (double*)(mem + 96), 4, op);
    printf("%s: %s\n", name, errors ? "FAILED" : "PASSED");
}

// xvfmadd.s
static void asm_xvfmadd_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfmadd.s $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfmadd.d
static void asm_xvfmadd_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfmadd.d $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfnmadd.s
static void asm_xvfnmadd_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfnmadd.s $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfnmadd.d
static void asm_xvfnmadd_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfnmadd.d $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfmsub.s
static void asm_xvfmsub_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfmsub.s $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfmsub.d
static void asm_xvfmsub_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfmsub.d $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfnmsub.s
static void asm_xvfnmsub_s(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfnmsub.s $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

// xvfnmsub.d
static void asm_xvfnmsub_d(void* mem)
{
    __asm__ volatile("xvld $xr0, %0, 0\n"
                     "xvld $xr1, %0, 32\n"
                     "xvld $xr2, %0, 64\n"
                     "xvfnmsub.d $xr3, $xr0, $xr1, $xr2\n"
                     "xvst $xr3, %0, 96\n"
                     :
                     : "r"(mem)
                     : "memory", "$xr0", "$xr1", "$xr2", "$xr3");
}

int main()
{
    printf("=== FMA single precision tests ===\n");
    test_fma_s("xvfmadd.s", asm_xvfmadd_s, 0);
    test_fma_s("xvfnmadd.s", asm_xvfnmadd_s, 1);
    test_fma_s("xvfmsub.s", asm_xvfmsub_s, 2);
    test_fma_s("xvfnmsub.s", asm_xvfnmsub_s, 3);

    printf("=== FMA double precision tests ===\n");
    test_fma_d("xvfmadd.d", asm_xvfmadd_d, 0);
    test_fma_d("xvfnmadd.d", asm_xvfnmadd_d, 1);
    test_fma_d("xvfmsub.d", asm_xvfmsub_d, 2);
    test_fma_d("xvfnmsub.d", asm_xvfnmsub_d, 3);

    return 0;
}