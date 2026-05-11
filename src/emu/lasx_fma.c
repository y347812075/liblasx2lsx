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

#include "lasx_fma.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== xvfmadd.s ====================
void do_lasx_emu_xvfmadd_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmadd.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 xa_lo = (__m128){vreg_read_f32(uc, td, xa, i * 4), vreg_read_f32(uc, td, xa, i * 4 + 1), vreg_read_f32(uc, td, xa, i * 4 + 2), vreg_read_f32(uc, td, xa, i * 4 + 3)};
            __m128 result = __lsx_vfmadd_s(xj_lo, xk_lo, xa_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t res = 0;

        for (int j = 0; j < 2; j++) {
            int shift = j * 32;
            union {
                uint32_t i;
                float f;
            } uj, uk, ua, ures;
            uj.i = (src_j[i] >> shift) & 0xffffffff;
            uk.i = (src_k[i] >> shift) & 0xffffffff;
            ua.i = (src_a[i] >> shift) & 0xffffffff;
            ures.f = uj.f * uk.f + ua.f;
            res |= ((uint64_t)ures.i << shift);
        }

        vreg_write_64(uc, td, xd, i, res);
    }

}

// ==================== xvfmadd.d ====================
void do_lasx_emu_xvfmadd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmadd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d xa_lo = (__m128d){vreg_read_f64(uc, td, xa, i * 2), vreg_read_f64(uc, td, xa, i * 2 + 1)};
            __m128d result = __lsx_vfmadd_d(xj_lo, xk_lo, xa_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        union {
            uint64_t i;
            double d;
        } uj, uk, ua, ures;
        uj.i = src_j[i];
        uk.i = src_k[i];
        ua.i = src_a[i];
        ures.d = uj.d * uk.d + ua.d;
        vreg_write_64(uc, td, xd, i, ures.i);
    }

}

// ==================== xvfmsub.s ====================
void do_lasx_emu_xvfmsub_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmsub.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 xa_lo = (__m128){vreg_read_f32(uc, td, xa, i * 4), vreg_read_f32(uc, td, xa, i * 4 + 1), vreg_read_f32(uc, td, xa, i * 4 + 2), vreg_read_f32(uc, td, xa, i * 4 + 3)};
            __m128 result = __lsx_vfmsub_s(xj_lo, xk_lo, xa_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t res = 0;

        for (int j = 0; j < 2; j++) {
            int shift = j * 32;
            union {
                uint32_t i;
                float f;
            } uj, uk, ua, ures;
            uj.i = (src_j[i] >> shift) & 0xffffffff;
            uk.i = (src_k[i] >> shift) & 0xffffffff;
            ua.i = (src_a[i] >> shift) & 0xffffffff;
            ures.f = uj.f * uk.f - ua.f;
            res |= ((uint64_t)ures.i << shift);
        }

        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvfmsub.d ====================
void do_lasx_emu_xvfmsub_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfmsub.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d xa_lo = (__m128d){vreg_read_f64(uc, td, xa, i * 2), vreg_read_f64(uc, td, xa, i * 2 + 1)};
            __m128d result = __lsx_vfmsub_d(xj_lo, xk_lo, xa_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        union {
            uint64_t i;
            double d;
        } uj, uk, ua, ures;
        uj.i = src_j[i];
        uk.i = src_k[i];
        ua.i = src_a[i];
        ures.d = uj.d * uk.d - ua.d;
        vreg_write_64(uc, td, xd, i, ures.i);
    }
}

// ==================== xvfnmadd.s ====================
void do_lasx_emu_xvfnmadd_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfnmadd.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 xa_lo = (__m128){vreg_read_f32(uc, td, xa, i * 4), vreg_read_f32(uc, td, xa, i * 4 + 1), vreg_read_f32(uc, td, xa, i * 4 + 2), vreg_read_f32(uc, td, xa, i * 4 + 3)};
            __m128 result = __lsx_vfnmadd_s(xj_lo, xk_lo, xa_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t res = 0;

        for (int j = 0; j < 2; j++) {
            int shift = j * 32;
            union {
                uint32_t i;
                float f;
            } uj, uk, ua, ures;
            uj.i = (src_j[i] >> shift) & 0xffffffff;
            uk.i = (src_k[i] >> shift) & 0xffffffff;
            ua.i = (src_a[i] >> shift) & 0xffffffff;
            ures.f = -uj.f * uk.f - ua.f;
            res |= ((uint64_t)ures.i << shift);
        }

        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvfnmadd.d ====================
void do_lasx_emu_xvfnmadd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfnmadd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d xa_lo = (__m128d){vreg_read_f64(uc, td, xa, i * 2), vreg_read_f64(uc, td, xa, i * 2 + 1)};
            __m128d result = __lsx_vfnmadd_d(xj_lo, xk_lo, xa_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        union {
            uint64_t i;
            double d;
        } uj, uk, ua, ures;
        uj.i = src_j[i];
        uk.i = src_k[i];
        ua.i = src_a[i];
        ures.d = -uj.d * uk.d - ua.d;
        vreg_write_64(uc, td, xd, i, ures.i);
    }
}

// ==================== xvfnmsub.s ====================
void do_lasx_emu_xvfnmsub_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfnmsub.s %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
            __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
            __m128 xa_lo = (__m128){vreg_read_f32(uc, td, xa, i * 4), vreg_read_f32(uc, td, xa, i * 4 + 1), vreg_read_f32(uc, td, xa, i * 4 + 2), vreg_read_f32(uc, td, xa, i * 4 + 3)};
            __m128 result = __lsx_vfnmsub_s(xj_lo, xk_lo, xa_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t res = 0;

        for (int j = 0; j < 2; j++) {
            int shift = j * 32;
            union {
                uint32_t i;
                float f;
            } uj, uk, ua, ures;
            uj.i = (src_j[i] >> shift) & 0xffffffff;
            uk.i = (src_k[i] >> shift) & 0xffffffff;
            ua.i = (src_a[i] >> shift) & 0xffffffff;
            ures.f = -uj.f * uk.f + ua.f;
            res |= ((uint64_t)ures.i << shift);
        }

        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvfnmsub.d ====================
void do_lasx_emu_xvfnmsub_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfnmsub.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
            __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
            __m128d xa_lo = (__m128d){vreg_read_f64(uc, td, xa, i * 2), vreg_read_f64(uc, td, xa, i * 2 + 1)};
            __m128d result = __lsx_vfnmsub_d(xj_lo, xk_lo, xa_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t src_j[4], src_k[4], src_a[4];
    for (int i = 0; i < 4; i++) {
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
        src_a[i] = vreg_read_64(uc, td, xa, i);
    }

    for (int i = 0; i < 4; i++) {
        union {
            uint64_t i;
            double d;
        } uj, uk, ua, ures;
        uj.i = src_j[i];
        uk.i = src_k[i];
        ua.i = src_a[i];
        ures.d = -uj.d * uk.d + ua.d;
        vreg_write_64(uc, td, xd, i, ures.i);
    }
}
