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

#include "lasx_fbasic.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"
#include <fenv.h>
#include <lsxintrin.h>
#include <math.h>

// ==================== xvfadd.s ====================
void do_lasx_emu_xvfadd_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfadd.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfadd_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
        xk_f[i * 2] = vreg_read_f32(uc, td, xk, i * 2);
        xk_f[i * 2 + 1] = vreg_read_f32(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = xj_f[i] + xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }

}

// ==================== xvfadd.d ====================
void do_lasx_emu_xvfadd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfadd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfadd_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = xj_d[i] + xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }

}

// ==================== xvfsub.s ====================
void do_lasx_emu_xvfsub_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfsub.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfsub_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
        xk_f[i * 2] = vreg_read_f32(uc, td, xk, i * 2);
        xk_f[i * 2 + 1] = vreg_read_f32(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = xj_f[i] - xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }

}

// ==================== xvfsub.d ====================
void do_lasx_emu_xvfsub_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfsub.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfsub_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = xj_d[i] - xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }

}

// ==================== xvfmul.s ====================
void do_lasx_emu_xvfmul_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmul.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfmul_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
        xk_f[i * 2] = vreg_read_f32(uc, td, xk, i * 2);
        xk_f[i * 2 + 1] = vreg_read_f32(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = xj_f[i] * xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }

}

// ==================== xvfmul.d ====================
void do_lasx_emu_xvfmul_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmul.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfmul_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = xj_d[i] * xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }

}

// ==================== xvfdiv.s ====================
void do_lasx_emu_xvfdiv_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfdiv.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfdiv_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
        xk_f[i * 2] = vreg_read_f32(uc, td, xk, i * 2);
        xk_f[i * 2 + 1] = vreg_read_f32(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = xj_f[i] / xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }

}

// ==================== xvfdiv.d ====================
void do_lasx_emu_xvfdiv_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfdiv.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfdiv_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = xj_d[i] / xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }

}

// ==================== xvfmax.s ====================
void do_lasx_emu_xvfmax_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmax.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfmax_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 8; i++) {
        xj_f[i] = vreg_read_f32(uc, td, xj, i);
        xk_f[i] = vreg_read_f32(uc, td, xk, i);
    }

    for (int i = 0; i < 8; i++) {
        float result = xj_f[i] > xk_f[i] ? xj_f[i] : xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }

}

// ==================== xvfmax.d ====================
void do_lasx_emu_xvfmax_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmax.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfmax_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 2; i++) {
        __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
        __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
        __m128d result = __lsx_vfmax_d(xj_lo, xk_lo);
        double* r = (double*)&result;
        vreg_write_f64(uc, td, xd, i * 2, r[0]);
        vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
    }

}

// ==================== xvfmin.s ====================
void do_lasx_emu_xvfmin_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmin.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfmin_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 8; i++) {
        xj_f[i] = vreg_read_f32(uc, td, xj, i);
        xk_f[i] = vreg_read_f32(uc, td, xk, i);
    }

    for (int i = 0; i < 8; i++) {
        float result = xj_f[i] < xk_f[i] ? xj_f[i] : xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }

}

// ==================== xvfmin.d ====================
void do_lasx_emu_xvfmin_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmin.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfmin_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = xj_d[i] < xk_d[i] ? xj_d[i] : xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }

}

// ==================== xvfmaxa.s ====================
void do_lasx_emu_xvfmaxa_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmaxa.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfmaxa_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
        xk_f[i * 2] = vreg_read_f32(uc, td, xk, i * 2);
        xk_f[i * 2 + 1] = vreg_read_f32(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = fabs(xj_f[i]) > fabs(xk_f[i]) ? xj_f[i] : xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfmaxa.d ====================
void do_lasx_emu_xvfmaxa_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmaxa.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfmaxa_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = fabs(xj_d[i]) > fabs(xk_d[i]) ? xj_d[i] : xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvfmina.s ====================
void do_lasx_emu_xvfmina_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmina.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 result = __lsx_vfmina_s(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
        xk_f[i * 2] = vreg_read_f32(uc, td, xk, i * 2);
        xk_f[i * 2 + 1] = vreg_read_f32(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = fabs(xj_f[i]) < fabs(xk_f[i]) ? xj_f[i] : xk_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfmina.d ====================
void do_lasx_emu_xvfmina_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmina.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d result = __lsx_vfmina_d(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = fabs(xj_d[i]) < fabs(xk_d[i]) ? xj_d[i] : xk_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvfsqrt.s ====================
void do_lasx_emu_xvfsqrt_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfsqrt.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfsqrt_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = sqrtf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfsqrt.d ====================
void do_lasx_emu_xvfsqrt_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfsqrt.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfsqrt_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = sqrt(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvfrecip.s ====================
void do_lasx_emu_xvfrecip_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrecip.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrecip_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = 1.0f / xj_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfrecip.d ====================
void do_lasx_emu_xvfrecip_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrecip.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrecip_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = 1.0 / xj_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvfrsqrt.s ====================
void do_lasx_emu_xvfrsqrt_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrsqrt.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrsqrt_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = 1.0f / sqrtf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfrsqrt.d ====================
void do_lasx_emu_xvfrsqrt_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrsqrt.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrsqrt_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = 1.0 / sqrt(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvflogb.s ====================
void do_lasx_emu_xvflogb_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvflogb.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vflogb_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint32_t xj_bits[8];
    for (int i = 0; i < 8; i++) {
        xj_bits[i] = vreg_read_32(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        int sign = (xj_bits[i] >> 31) & 1;
        int exp = (xj_bits[i] >> 23) & 0xff;

        uint32_t result;
        if (sign || exp == 0xff) {
            result = 0x7fc00000;
        } else {
            int unbiased_exp = (exp == 0) ? -126 : (exp - 127);
            float f = (float)unbiased_exp;
            result = *(uint32_t*)&f;
        }
        vreg_write_32(uc, td, xd, i, result);
    }
}

// ==================== xvflogb.d ====================
void do_lasx_emu_xvflogb_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvflogb.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vflogb_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t xj_bits[4];
    for (int i = 0; i < 4; i++) {
        xj_bits[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        int sign = (xj_bits[i] >> 63) & 1;
        int exp = (xj_bits[i] >> 52) & 0x7ff;

        uint64_t result;
        if (sign || exp == 0x7ff) {
            result = 0x7ff8000000000000ULL;
        } else {
            int unbiased_exp = (exp == 0) ? -1022 : (exp - 1023);
            double d = (double)unbiased_exp;
            result = *(uint64_t*)&d;
        }
        vreg_write_64(uc, td, xd, i, result);
    }
}

static uint32_t fpclassify_s(float f)
{
    uint32_t bits = *(uint32_t*)&f;
    int sign = (bits >> 31) & 1;
    uint32_t exp = (bits >> 23) & 0xff;
    uint32_t frac = bits & 0x7fffff;

    if (exp == 0xff) {
        if (frac == 0) {
            return sign ? 0x004 : 0x040;
        } else {
            return (frac & 0x400000) ? 0x002 : 0x001;
        }
    } else if (exp == 0) {
        if (frac == 0) {
            return sign ? 0x020 : 0x200;
        } else {
            return sign ? 0x010 : 0x100;
        }
    } else {
        return sign ? 0x008 : 0x080;
    }
}

static uint64_t fpclassify_d(double d)
{
    uint64_t bits = *(uint64_t*)&d;
    int sign = (bits >> 63) & 1;
    uint64_t exp = (bits >> 52) & 0x7ff;
    uint64_t frac = bits & 0xfffffffffffffULL;

    if (exp == 0x7ff) {
        if (frac == 0) {
            return sign ? 0x004 : 0x040;
        } else {
            return (frac & 0x8000000000000ULL) ? 0x002 : 0x001;
        }
    } else if (exp == 0) {
        if (frac == 0) {
            return sign ? 0x020 : 0x200;
        } else {
            return sign ? 0x010 : 0x100;
        }
    } else {
        return sign ? 0x008 : 0x080;
    }
}

// ==================== xvfclass.s ====================
void do_lasx_emu_xvfclass_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfclass.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vfclass_s(xj_lo);
            uint32_t* r = (uint32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    uint32_t result[8];
    for (int i = 0; i < 8; i++) {
        result[i] = fpclassify_s(xj_f[i]);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, result[i]);
    }
}

// ==================== xvfclass.d ====================
void do_lasx_emu_xvfclass_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfclass.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vfclass_d(xj_lo);
            uint64_t* r = (uint64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    uint64_t result[4];
    for (int i = 0; i < 4; i++) {
        result[i] = fpclassify_d(xj_d[i]);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, result[i]);
    }
}

// ==================== xvfrint.s ====================
void do_lasx_emu_xvfrint_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrint.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrint_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = rintf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfrint.d ====================
void do_lasx_emu_xvfrint_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrint.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrint_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = rint(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvfrintrm.s ====================
void do_lasx_emu_xvfrintrm_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrm.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrintrm_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_DOWNWARD);

    for (int i = 0; i < 8; i++) {
        float result = rintf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

// ==================== xvfrintrm.d ====================
void do_lasx_emu_xvfrintrm_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrm.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrintrm_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_DOWNWARD);

    for (int i = 0; i < 4; i++) {
        double result = rint(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

// ==================== xvfrintrp.s ====================
void do_lasx_emu_xvfrintrp_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrp.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrintrp_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_UPWARD);

    for (int i = 0; i < 8; i++) {
        float result = rintf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

static uint64_t double_to_uint64_sat_trunc(double d);

// ==================== xvftintrz_lu_d ====================
void do_lasx_emu_xvftintrz_lu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrz_lu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftintrz_lu_d(xj_d);
            uint64_t* r = (uint64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        double f = vreg_read_f64(uc, td, xj, i);
        uint64_t c = double_to_uint64_sat_trunc(f);
        vreg_write_64(uc, td, xd, i, c);
    }
}

static uint32_t float_to_uint32_sat_trunc(float f);

static uint32_t float_to_uint32_sat_rnd(float f);

static int32_t float_to_int32_sat_rnd(float f);

static int32_t trunc_int32(double d);

static int32_t ceil_int32(double d);

static int32_t floor_int32(double d);

static uint64_t double_to_uint64_sat(double d);

static uint64_t double_to_uint64_sat_trunc(double d);

// ==================== xvftintrz_wu_s ====================
void do_lasx_emu_xvftintrz_wu_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrz_wu_s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrz_wu_s(xj_lo);
            uint32_t* r = (uint32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    uint32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = float_to_uint32_sat_trunc(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)c[i * 2 + 1] << 32) | c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvffinth_d_w ====================
void do_lasx_emu_xvffinth_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffinth_d_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vffinth_d_w(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int32_t w[8];
    for (int i = 0; i < 8; i++) {
        w[i] = (int32_t)vreg_read_32(uc, td, xj, i);
    }

    double d[4];
    d[0] = (double)w[2];
    d[1] = (double)w[3];
    d[2] = (double)w[6];
    d[3] = (double)w[7];

    for (int i = 0; i < 4; i++) {
        vreg_write_f64(uc, td, xd, i, d[i]);
    }
}

// ==================== xvfrecipe_s ====================
void do_lasx_emu_xvfrecipe_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrecipe_s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

#ifdef FRECIPE
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrecipe_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }
#endif

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = 1.0f / xj_f[i];
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfrecipe_d ====================
void do_lasx_emu_xvfrecipe_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrecipe_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

#ifdef FRECIPE
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrecipe_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }
#endif

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = 1.0 / xj_d[i];
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvfrintrp.d ====================
void do_lasx_emu_xvfrintrp_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrp.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrintrp_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_UPWARD);

    for (int i = 0; i < 4; i++) {
        double result = rint(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

// ==================== xvfrintrz.s ====================
void do_lasx_emu_xvfrintrz_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrz.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrintrz_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TOWARDZERO);

    for (int i = 0; i < 8; i++) {
        float result = rintf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

// ==================== xvfrintrz.d ====================
void do_lasx_emu_xvfrintrz_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrz.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrintrz_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TOWARDZERO);

    for (int i = 0; i < 4; i++) {
        double result = rint(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

// ==================== xvfrintrne.s ====================
void do_lasx_emu_xvfrintrne_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrne.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrintrne_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TONEAREST);

    for (int i = 0; i < 8; i++) {
        float result = rintf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

// ==================== xvfrintrne.d ====================
void do_lasx_emu_xvfrintrne_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrintrne.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrintrne_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TONEAREST);

    for (int i = 0; i < 4; i++) {
        double result = rint(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }

    fesetenv(&old_env);
}

static float half_to_float(uint16_t h)
{
    uint32_t sign = (h & 0x8000) << 16;
    int exponent = (h >> 10) & 0x1f;
    uint32_t mantissa = h & 0x3ff;
    uint32_t ret;

    if (exponent == 0) {
        if (mantissa == 0) {
            ret = sign; // 零
        } else {
            // 非规格化数：重新规格化
            int exp_val = -14;
            while ((mantissa & 0x400) == 0) {
                mantissa <<= 1;
                exp_val--;
            }
            mantissa &= 0x3ff; // 去掉隐含的1
            exp_val += 127;    // 单精度指数偏移
            ret = sign | (exp_val << 23) | (mantissa << 13);
        }
    } else if (exponent == 0x1f) {
        // 无穷或 NaN
        ret = sign | 0x7f800000 | (mantissa << 13);
    } else {
        // 规格化数
        ret = sign | ((exponent + 112) << 23) | (mantissa << 13);
    }

    // 安全的位重解释
    float result;
    memcpy(&result, &ret, sizeof(result));
    return result;
}

// ==================== xvfrsqrte_s ====================
void do_lasx_emu_xvfrsqrte_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrsqrte_s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

#ifdef FRECIPE
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 result = __lsx_vfrsqrte_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }
#endif

    float xj_f[8];
    for (int i = 0; i < 4; i++) {
        xj_f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        xj_f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    for (int i = 0; i < 8; i++) {
        float result = 1.0f / sqrtf(xj_f[i]);
        vreg_write_f32(uc, td, xd, i, result);
    }
}

// ==================== xvfrsqrte_d ====================
void do_lasx_emu_xvfrsqrte_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrsqrte_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

#ifdef FRECIPE
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vfrsqrte_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }
#endif

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        double result = 1.0 / sqrt(xj_d[i]);
        vreg_write_f64(uc, td, xd, i, result);
    }
}

// ==================== xvftint_lu_d ====================
void do_lasx_emu_xvftint_lu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftint_lu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftint_lu_d(xj_d);
            uint64_t* r = (uint64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    double xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
    }

    uint64_t result[4];
    for (int i = 0; i < 4; i++) {
        result[i] = double_to_uint64_sat(xj_d[i]);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, result[i]);
    }
}

static uint16_t float_to_half(float f)
{
    uint32_t val = *(uint32_t*)&f;
    uint32_t sign = (val >> 16) & 0x8000;
    uint32_t float_exp = (val >> 23) & 0xff;
    uint32_t mantissa = val & 0x7fffff;

    if (float_exp == 0xff) {
        return sign | 0x7c00;
    }

    int32_t exponent = (int32_t)float_exp - 127 + 15;

    if (exponent <= 0) {
        if (exponent < -10) {
            return sign;
        }
        mantissa = (mantissa | 0x800000) >> (1 - exponent);
        return sign | (mantissa >> 13);
    } else if (exponent >= 31) {
        return sign | 0x7c00;
    } else {
        uint16_t result = sign | (exponent << 10) | (mantissa >> 13);
        uint32_t round_bits = mantissa & 0x1fff;
        if (round_bits > 0x1000 || (round_bits == 0x1000 && (result & 1))) {
            result++;
        }
        return result;
    }
}

// ==================== xvfcvt.h.s ====================
void do_lasx_emu_xvfcvt_h_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfcvt.h.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128i result = __lsx_vfcvt_h_s(xj_lo, xk_lo);
            uint16_t* r = (uint16_t*)&result;
            vreg_write_16(uc, td, xd, i * 8, r[0]);
            vreg_write_16(uc, td, xd, i * 8 + 1, r[1]);
            vreg_write_16(uc, td, xd, i * 8 + 2, r[2]);
            vreg_write_16(uc, td, xd, i * 8 + 3, r[3]);
            vreg_write_16(uc, td, xd, i * 8 + 4, r[4]);
            vreg_write_16(uc, td, xd, i * 8 + 5, r[5]);
            vreg_write_16(uc, td, xd, i * 8 + 6, r[6]);
            vreg_write_16(uc, td, xd, i * 8 + 7, r[7]);
        }
        return;
    }

    float xj_f[8], xk_f[8];
    for (int i = 0; i < 8; i++) {
        xj_f[i] = vreg_read_f32(uc, td, xj, i);
        xk_f[i] = vreg_read_f32(uc, td, xk, i);
    }

    uint16_t result[16];
    for (int i = 0; i < 4; i++) {
        result[i + 4] = float_to_half(xj_f[i]);
        result[i] = float_to_half(xk_f[i]);
        result[i + 12] = float_to_half(xj_f[i + 4]);
        result[i + 8] = float_to_half(xk_f[i + 4]);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, result[i]);
    }
}

// ==================== xvfcvt.s.d ====================
void do_lasx_emu_xvfcvt_s_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfcvt.s.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128 result = __lsx_vfcvt_s_d(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4 + 0, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    double xj_d[4];
    double xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_f32(uc, td, xd, i + 2, (float)xj_d[i]);
        vreg_write_f32(uc, td, xd, i, (float)xk_d[i]);
        vreg_write_f32(uc, td, xd, i + 6, (float)xj_d[i + 2]);
        vreg_write_f32(uc, td, xd, i + 4, (float)xk_d[i + 2]);
    }
}

// ==================== xvfcvtl.s.h ====================
void do_lasx_emu_xvfcvtl_s_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfcvtl.s.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2 + 0), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128 result = __lsx_vfcvtl_s_h(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4 + 0, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint16_t h[16];
    for (int i = 0; i < 16; i++) {
        h[i] = vreg_read_16(uc, td, xj, i);
    }

    uint32_t f[8];
    float ftmp;
    ftmp = half_to_float(h[0]);
    f[0] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[1]);
    f[1] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[2]);
    f[2] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[3]);
    f[3] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[8]);
    f[4] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[9]);
    f[5] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[10]);
    f[6] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[11]);
    f[7] = *(uint32_t*)&ftmp;

    uint64_t dst[4];
    dst[0] = ((uint64_t)f[1] << 32) | f[0];
    dst[1] = ((uint64_t)f[3] << 32) | f[2];
    dst[2] = ((uint64_t)f[5] << 32) | f[4];
    dst[3] = ((uint64_t)f[7] << 32) | f[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvfcvth.s.h ====================
void do_lasx_emu_xvfcvth_s_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfcvth.s.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2 + 0), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128 result = __lsx_vfcvth_s_h(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4 + 0, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint16_t h[16];
    for (int i = 0; i < 16; i++) {
        h[i] = vreg_read_16(uc, td, xj, i);
    }

    uint32_t f[8];
    float ftmp;
    ftmp = half_to_float(h[4]);
    f[0] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[5]);
    f[1] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[6]);
    f[2] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[7]);
    f[3] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[12]);
    f[4] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[13]);
    f[5] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[14]);
    f[6] = *(uint32_t*)&ftmp;
    ftmp = half_to_float(h[15]);
    f[7] = *(uint32_t*)&ftmp;

    uint64_t dst[4];
    dst[0] = ((uint64_t)f[1] << 32) | f[0];
    dst[1] = ((uint64_t)f[3] << 32) | f[2];
    dst[2] = ((uint64_t)f[5] << 32) | f[4];
    dst[3] = ((uint64_t)f[7] << 32) | f[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvfcvtl.d.s ====================
void do_lasx_emu_xvfcvtl_d_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfcvtl.d.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4 + 0), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128d result = __lsx_vfcvtl_d_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2 + 0, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t idx[] = {0, 1, 4, 5};
    for (int i = 0; i < 4; i++) {
        float fval = vreg_read_f32(uc, td, xj, idx[i]);
        double dval = (double)fval;
        vreg_write_f64(uc, td, xd, i, dval);
    }
}

// ==================== xvfcvth.d.s ====================
void do_lasx_emu_xvfcvth_d_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfcvth.d.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4 + 0), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128d result = __lsx_vfcvth_d_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t idx[] = {2, 3, 6, 7};
    for (int i = 0; i < 4; i++) {
        float fval = vreg_read_f32(uc, td, xj, idx[i]);
        double dval = (double)fval;
        vreg_write_f64(uc, td, xd, i, dval);
    }
}

// ==================== xvffintl.d.w ====================
void do_lasx_emu_xvffintl_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffintl.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vffintl_d_w(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int32_t w[8];
    for (int i = 0; i < 8; i++) {
        w[i] = (int32_t)vreg_read_32(uc, td, xj, i);
    }

    double d[4];
    d[0] = (double)w[0];
    d[1] = (double)w[1];
    d[2] = (double)w[4];
    d[3] = (double)w[5];

    for (int i = 0; i < 4; i++) {
        vreg_write_f64(uc, td, xd, i, d[i]);
    }
}

// ==================== xvffinths.d.w ====================

// ==================== xvffint.s.l ====================
void do_lasx_emu_xvffint_s_l(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffint.s.l %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128 result = __lsx_vffint_s_l(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    int64_t xj_q[4], xk_q[4];
    for (int i = 0; i < 4; i++) {
        xj_q[i] = (int64_t)vreg_read_64(uc, td, xj, i);
        xk_q[i] = (int64_t)vreg_read_64(uc, td, xk, i);
    }

    float f[8];
    f[0] = (float)xk_q[0];
    f[1] = (float)xk_q[1];
    f[2] = (float)xj_q[0];
    f[3] = (float)xj_q[1];
    f[4] = (float)xk_q[2];
    f[5] = (float)xk_q[3];
    f[6] = (float)xj_q[2];
    f[7] = (float)xj_q[3];

    for (int i = 0; i < 8; i++) {
        vreg_write_f32(uc, td, xd, i, f[i]);
    }
}

// ==================== xvftint.w.d ====================
static int32_t double_to_int32_sat_rnd(double d)
{
    if (d > (double)INT32_MAX) {
        return INT32_MAX;
    } else if (d < (double)INT32_MIN) {
        return INT32_MIN;
    } else {
        return (int32_t)rint(d);
    }
}

void do_lasx_emu_xvftint_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftint.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_d = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vftint_w_d(xj_d, xk_d);
            int32_t* r = (int32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    int32_t w[8];
    w[0] = double_to_int32_sat_rnd(xk_d[0]);
    w[1] = double_to_int32_sat_rnd(xk_d[1]);
    w[2] = double_to_int32_sat_rnd(xj_d[0]);
    w[3] = double_to_int32_sat_rnd(xj_d[1]);
    w[4] = double_to_int32_sat_rnd(xk_d[2]);
    w[5] = double_to_int32_sat_rnd(xk_d[3]);
    w[6] = double_to_int32_sat_rnd(xj_d[2]);
    w[7] = double_to_int32_sat_rnd(xj_d[3]);

    uint64_t dst[4];
    dst[0] = ((uint64_t)(uint32_t)w[1] << 32) | (uint32_t)w[0];
    dst[1] = ((uint64_t)(uint32_t)w[3] << 32) | (uint32_t)w[2];
    dst[2] = ((uint64_t)(uint32_t)w[5] << 32) | (uint32_t)w[4];
    dst[3] = ((uint64_t)(uint32_t)w[7] << 32) | (uint32_t)w[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvffint.s.w ====================
void do_lasx_emu_xvffint_s_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffint.s.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128 result = __lsx_vffint_s_w(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    int32_t w[8];
    for (int i = 0; i < 8; i++) {
        w[i] = (int32_t)vreg_read_32(uc, td, xj, i);
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = (float)w[i];
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_f32(uc, td, xd, i, f[i]);
    }
}

// ==================== xvffint.d.l ====================
void do_lasx_emu_xvffint_d_l(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffint.d.l %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vffint_d_l(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int64_t q[4];
    double d[4];
    for (int i = 0; i < 4; i++) {
        q[i] = (int64_t)vreg_read_64(uc, td, xj, i);
        d[i] = (double)q[i];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_f64(uc, td, xd, i, d[i]);
    }
}

// ==================== xvftintrm.w.d ====================
static int32_t floor_int32(double d)
{
    if (d >= 0) {
        return (int32_t)d;
    } else {
        int32_t i = (int32_t)d;
        return (d == (double)i) ? i : i - 1;
    }
}

static int32_t ceil_int32(double d)
{
    if (d <= 0) {
        return (int32_t)d;
    } else {
        int32_t i = (int32_t)d;
        return (d == (double)i) ? i : i + 1;
    }
}

static int32_t trunc_int32(double d) { return (int32_t)d; }

static int32_t float_to_int32_sat_rnd(float f)
{
    if (f > (float)INT32_MAX) {
        return INT32_MAX;
    } else if (f < (float)INT32_MIN) {
        return INT32_MIN;
    } else {
        return (int32_t)rintf(f);
    }
}

static uint32_t float_to_uint32_sat_rnd(float f)
{
    if (f >= (float)UINT32_MAX) {
        return UINT32_MAX;
    } else if (f <= 0.0f) {
        return 0;
    } else {
        return (uint32_t)rintf(f);
    }
}

static uint32_t float_to_uint32_sat_trunc(float f)
{
    if (f >= (float)UINT32_MAX) {
        return UINT32_MAX;
    } else if (f <= 0.0f) {
        return 0;
    } else {
        return (uint32_t)truncf(f);
    }
}

static uint64_t double_to_uint64_sat(double d)
{
    if (d >= (double)UINT64_MAX) {
        return UINT64_MAX;
    } else if (d <= 0.0) {
        return 0;
    } else {
        return (uint64_t)rint(d);
    }
}

static uint64_t double_to_uint64_sat_trunc(double d)
{
    if (d >= (double)UINT64_MAX) {
        return UINT64_MAX;
    } else if (d <= 0.0) {
        return 0;
    } else {
        return (uint64_t)trunc(d);
    }
}

static int32_t double_to_int32_sat_floor(double d)
{
    if (d > (double)INT32_MAX) {
        return INT32_MAX;
    } else if (d < (double)INT32_MIN) {
        return INT32_MIN;
    } else {
        return floor_int32(d);
    }
}

static int32_t double_to_int32_sat_ceil(double d)
{
    if (d > (double)INT32_MAX) {
        return INT32_MAX;
    } else if (d < (double)INT32_MIN) {
        return INT32_MIN;
    } else {
        return ceil_int32(d);
    }
}

static int32_t double_to_int32_sat_trunc(double d)
{
    if (d > (double)INT32_MAX) {
        return INT32_MAX;
    } else if (d < (double)INT32_MIN) {
        return INT32_MIN;
    } else {
        return trunc_int32(d);
    }
}

void do_lasx_emu_xvftintrm_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrm.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_d = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vftintrm_w_d(xj_d, xk_d);
            int32_t* r = (int32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    int32_t w[8];
    w[0] = double_to_int32_sat_floor(xk_d[0]);
    w[1] = double_to_int32_sat_floor(xk_d[1]);
    w[2] = double_to_int32_sat_floor(xj_d[0]);
    w[3] = double_to_int32_sat_floor(xj_d[1]);
    w[4] = double_to_int32_sat_floor(xk_d[2]);
    w[5] = double_to_int32_sat_floor(xk_d[3]);
    w[6] = double_to_int32_sat_floor(xj_d[2]);
    w[7] = double_to_int32_sat_floor(xj_d[3]);

    uint64_t dst[4];
    dst[0] = ((uint64_t)(uint32_t)w[1] << 32) | (uint32_t)w[0];
    dst[1] = ((uint64_t)(uint32_t)w[3] << 32) | (uint32_t)w[2];
    dst[2] = ((uint64_t)(uint32_t)w[5] << 32) | (uint32_t)w[4];
    dst[3] = ((uint64_t)(uint32_t)w[7] << 32) | (uint32_t)w[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

void do_lasx_emu_xvftintrp_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrp.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_d = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vftintrp_w_d(xj_d, xk_d);
            int32_t* r = (int32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    int32_t w[8];
    w[0] = double_to_int32_sat_ceil(xk_d[0]);
    w[1] = double_to_int32_sat_ceil(xk_d[1]);
    w[2] = double_to_int32_sat_ceil(xj_d[0]);
    w[3] = double_to_int32_sat_ceil(xj_d[1]);
    w[4] = double_to_int32_sat_ceil(xk_d[2]);
    w[5] = double_to_int32_sat_ceil(xk_d[3]);
    w[6] = double_to_int32_sat_ceil(xj_d[2]);
    w[7] = double_to_int32_sat_ceil(xj_d[3]);

    uint64_t dst[4];
    dst[0] = ((uint64_t)(uint32_t)w[1] << 32) | (uint32_t)w[0];
    dst[1] = ((uint64_t)(uint32_t)w[3] << 32) | (uint32_t)w[2];
    dst[2] = ((uint64_t)(uint32_t)w[5] << 32) | (uint32_t)w[4];
    dst[3] = ((uint64_t)(uint32_t)w[7] << 32) | (uint32_t)w[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

void do_lasx_emu_xvftintrz_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrz.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_d = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vftintrz_w_d(xj_d, xk_d);
            int32_t* r = (int32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    int32_t w[8];
    w[0] = double_to_int32_sat_trunc(xk_d[0]);
    w[1] = double_to_int32_sat_trunc(xk_d[1]);
    w[2] = double_to_int32_sat_trunc(xj_d[0]);
    w[3] = double_to_int32_sat_trunc(xj_d[1]);
    w[4] = double_to_int32_sat_trunc(xk_d[2]);
    w[5] = double_to_int32_sat_trunc(xk_d[3]);
    w[6] = double_to_int32_sat_trunc(xj_d[2]);
    w[7] = double_to_int32_sat_trunc(xj_d[3]);

    uint64_t dst[4];
    dst[0] = ((uint64_t)(uint32_t)w[1] << 32) | (uint32_t)w[0];
    dst[1] = ((uint64_t)(uint32_t)w[3] << 32) | (uint32_t)w[2];
    dst[2] = ((uint64_t)(uint32_t)w[5] << 32) | (uint32_t)w[4];
    dst[3] = ((uint64_t)(uint32_t)w[7] << 32) | (uint32_t)w[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

void do_lasx_emu_xvftintrne_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrne.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_d = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vftintrne_w_d(xj_d, xk_d);
            int32_t* r = (int32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    double xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_f64(uc, td, xj, i);
        xk_d[i] = vreg_read_f64(uc, td, xk, i);
    }

    int32_t w[8];
    w[0] = double_to_int32_sat_rnd(xk_d[0]);
    w[1] = double_to_int32_sat_rnd(xk_d[1]);
    w[2] = double_to_int32_sat_rnd(xj_d[0]);
    w[3] = double_to_int32_sat_rnd(xj_d[1]);
    w[4] = double_to_int32_sat_rnd(xk_d[2]);
    w[5] = double_to_int32_sat_rnd(xk_d[3]);
    w[6] = double_to_int32_sat_rnd(xj_d[2]);
    w[7] = double_to_int32_sat_rnd(xj_d[3]);

    uint64_t dst[4];
    dst[0] = ((uint64_t)(uint32_t)w[1] << 32) | (uint32_t)w[0];
    dst[1] = ((uint64_t)(uint32_t)w[3] << 32) | (uint32_t)w[2];
    dst[2] = ((uint64_t)(uint32_t)w[5] << 32) | (uint32_t)w[4];
    dst[3] = ((uint64_t)(uint32_t)w[7] << 32) | (uint32_t)w[6];

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvffint.s.wu ====================
void do_lasx_emu_xvffint_s_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffint.s.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128 result = __lsx_vffint_s_wu(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint32_t wu[8];
    for (int i = 0; i < 8; i++) {
        wu[i] = vreg_read_32(uc, td, xj, i);
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = (float)wu[i];
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_f32(uc, td, xd, i, f[i]);
    }
}

// ==================== xvffint.d.lu ====================
void do_lasx_emu_xvffint_d_lu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvffint.d.lu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128d result = __lsx_vffint_d_lu(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t lu[4];
    double d[4];
    for (int i = 0; i < 4; i++) {
        lu[i] = vreg_read_64(uc, td, xj, i);
        d[i] = (double)lu[i];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_f64(uc, td, xd, i, d[i]);
    }
}

// ==================== xvftint.wu.s ====================
void do_lasx_emu_xvftint_wu_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftint.wu.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftint_wu_s(xj_lo);
            uint32_t* r = (uint32_t*)&result;
            vreg_write_32(uc, td, xd, i * 4, r[0]);
            vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    uint32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = float_to_uint32_sat_rnd(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)c[i * 2 + 1] << 32) | c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvftint.w.s ====================
void do_lasx_emu_xvftint_w_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftint.w.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftint_w_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    int32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = float_to_int32_sat_rnd(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)(uint32_t)c[i * 2 + 1] << 32) | (uint32_t)c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvftintrm.w.s ====================
void do_lasx_emu_xvftintrm_w_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrm.w.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrm_w_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_DOWNWARD);

    int32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = (int32_t)rintf(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)(uint32_t)c[i * 2 + 1] << 32) | (uint32_t)c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrp.w.s ====================
void do_lasx_emu_xvftintrp_w_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrp.w.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrp_w_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_UPWARD);

    int32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = (int32_t)rintf(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)(uint32_t)c[i * 2 + 1] << 32) | (uint32_t)c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrp.l.d ====================

// ==================== xvftintrz.w.s ====================
void do_lasx_emu_xvftintrz_w_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrz.w.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrz_w_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TOWARDZERO);

    int32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = (int32_t)rintf(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)(uint32_t)c[i * 2 + 1] << 32) | (uint32_t)c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrz.l.d ====================

// ==================== xvftintrne.w.s ====================
void do_lasx_emu_xvftintrne_w_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrne.w.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrne_w_s(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 8; i++) {
        f[i] = vreg_read_f32(uc, td, xj, i);
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TONEAREST);

    int32_t c[8];
    for (int i = 0; i < 8; i++) {
        c[i] = (int32_t)rintf(f[i]);
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = ((uint64_t)(uint32_t)c[i * 2 + 1] << 32) | (uint32_t)c[i * 2];
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }

    fesetenv(&old_env);
}

// ==================== xvftint_l_d ====================
void do_lasx_emu_xvftint_l_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftint.l.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_d = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftint_l_d(xj_d);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        double f = vreg_read_f64(uc, td, xj, i);
        int64_t c = (int64_t)rint(f);
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvftintl_l_s ====================
void do_lasx_emu_xvftintl_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintl.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
	for (int i = 0; i < 2; i++) {
	    __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}; 
	    __m128i result = __lsx_vftintl_l_s(xj_lo);
	    double* r = (double*)&result;
	    vreg_write_f64(uc, td, xd, i * 2, r[0]);
	    vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
	}    
	return;
    }  

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)rintf(f[0]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)rintf(f[1]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)rintf(f[4]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)rintf(f[5]));
}

// ==================== xvftinth_l_s ====================
void do_lasx_emu_xvftinth_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftinth.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}; 
            __m128i result = __lsx_vftinth_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }    
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)rintf(f[2]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)rintf(f[3]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)rintf(f[6]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)rintf(f[7]));
}

// ==================== xvftintrm_l_d ====================
void do_lasx_emu_xvftintrm_l_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrm.l.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftintrm_l_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_DOWNWARD);

    for (int i = 0; i < 4; i++) {
        double f = vreg_read_f64(uc, td, xj, i);
        int64_t c = (int64_t)rint(f);
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrml_l_s ====================
void do_lasx_emu_xvftintrml_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrml.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
	for (int i = 0; i < 2; i++) {
	    __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
	    __m128i result = __lsx_vftintrml_l_s(xj_lo);
	    double* r = (double*)&result;
	    vreg_write_f64(uc, td, xd, i * 2, r[0]);
	    vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
	}
	return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)floorf(f[0]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)floorf(f[1]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)floorf(f[4]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)floorf(f[5]));
}

// ==================== xvftintrmh_l_s ====================
void do_lasx_emu_xvftintrmh_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrmh.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
	for (int i = 0; i < 2; i++) {
	    __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
	    __m128i result = __lsx_vftintrmh_l_s(xj_lo);
	    double* r = (double*)&result;
	    vreg_write_f64(uc, td, xd, i * 2, r[0]);
	    vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
	}
	return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)floorf(f[2]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)floorf(f[3]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)floorf(f[6]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)floorf(f[7]));
}

// ==================== xvftintrp_l_d ====================
void do_lasx_emu_xvftintrp_l_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrp.l.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftintrp_l_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_UPWARD);

    for (int i = 0; i < 4; i++) {
        double f = vreg_read_f64(uc, td, xj, i);
        int64_t c = (int64_t)rint(f);
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrpl_l_s ====================
void do_lasx_emu_xvftintrpl_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrpl.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}; 
            __m128i result = __lsx_vftintrpl_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }    
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)ceilf(f[0]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)ceilf(f[1]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)ceilf(f[4]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)ceilf(f[5]));
}

// ==================== xvftintrph_l_s ====================
void do_lasx_emu_xvftintrph_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrph.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}; 
            __m128i result = __lsx_vftintrph_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }    
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)ceilf(f[2]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)ceilf(f[3]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)ceilf(f[6]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)ceilf(f[7]));
}

// ==================== xvftintrz_l_d ====================
void do_lasx_emu_xvftintrz_l_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrz.l.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftintrz_l_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TOWARDZERO);

    for (int i = 0; i < 4; i++) {
        double f = vreg_read_f64(uc, td, xj, i);
        int64_t c = (int64_t)rint(f);
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrzl_l_s ====================
void do_lasx_emu_xvftintrzl_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrzl.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}; 
            __m128i result = __lsx_vftintrzl_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }    
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)truncf(f[0]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)truncf(f[1]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)truncf(f[4]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)truncf(f[5]));
}

// ==================== xvftintrzh_l_s ====================
void do_lasx_emu_xvftintrzh_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrzh.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}; 
            __m128i result = __lsx_vftintrzh_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }    
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)truncf(f[2]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)truncf(f[3]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)truncf(f[6]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)truncf(f[7]));
}

// ==================== xvftintrne_l_d ====================
void do_lasx_emu_xvftintrne_l_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrne.l.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128i result = __lsx_vftintrne_l_d(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    fenv_t old_env;
    fegetenv(&old_env);
    fesetround(FE_TONEAREST);

    for (int i = 0; i < 4; i++) {
        double f = vreg_read_f64(uc, td, xj, i);
        int64_t c = (int64_t)rint(f);
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }

    fesetenv(&old_env);
}

// ==================== xvftintrnel_l_s ====================
void do_lasx_emu_xvftintrnel_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrnel.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrnel_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)rintf(f[0]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)rintf(f[1]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)rintf(f[4]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)rintf(f[5]));
}

// ==================== xvftintrneh_l_s ====================
void do_lasx_emu_xvftintrneh_l_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvftintrneh.l.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128i result = __lsx_vftintrneh_l_s(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    float f[8];
    for (int i = 0; i < 4; i++) {
        f[i * 2] = vreg_read_f32(uc, td, xj, i * 2);
        f[i * 2 + 1] = vreg_read_f32(uc, td, xj, i * 2 + 1);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)(int64_t)rintf(f[2]));
    vreg_write_64(uc, td, xd, 1, (uint64_t)(int64_t)rintf(f[3]));
    vreg_write_64(uc, td, xd, 2, (uint64_t)(int64_t)rintf(f[6]));
    vreg_write_64(uc, td, xd, 3, (uint64_t)(int64_t)rintf(f[7]));
}
