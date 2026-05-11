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

#include "lasx_shift.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include <lsxintrin.h>

// ==================== xvbsll.v ====================
void do_lasx_emu_xvbsll_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbsll.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int ui5 = (instr >> 10) & 0x1f;
    unsigned int idx = (ui5 & 0xF) * 8;

    union u128_split {
        struct {
            uint64_t lo;
            uint64_t hi;
        };
        __uint128_t u128;
    } d128;

    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xj, i, &d128.lo, &d128.hi);
        d128.u128 <<= idx;
        vreg_write_128(uc, td, xd, i, d128.lo, d128.hi);
    }
}

// ==================== xvbsrl.v ====================
void do_lasx_emu_xvbsrl_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbsrl.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int ui5 = (instr >> 10) & 0x1f;
    unsigned int idx = (ui5 & 0xF) * 8;

    union u128_split {
        struct {
            uint64_t lo;
            uint64_t hi;
        };
        __uint128_t u128;
    } d128;

    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xj, i, &d128.lo, &d128.hi);
        d128.u128 >>= idx;
        vreg_write_128(uc, td, xd, i, d128.lo, d128.hi);
    }
}

// ==================== xvsll.b ====================
void do_lasx_emu_xvsll_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsll.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsll_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t shamt = (src_k >> shift) & 0x7;
            uint8_t res = (uint8_t)(a << shamt);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsll.h ====================
void do_lasx_emu_xvsll_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsll.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsll_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint8_t shamt = (src_k >> shift) & 0xf;
            uint16_t res = (uint16_t)(a << shamt);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsll.w ====================
void do_lasx_emu_xvsll_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsll.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsll_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint8_t shamt = (src_k >> shift) & 0x1f;
            uint32_t res = (uint32_t)(a << shamt);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsll.d ====================
void do_lasx_emu_xvsll_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsll.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsll_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t k = vreg_read_64(uc, td, xk, i);
        uint8_t shamt = k & 0x3f;
        uint64_t res = a << shamt;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsrl.b ====================
void do_lasx_emu_xvsrl_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrl.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrl_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t shamt = (src_k >> shift) & 0x7;
            uint8_t res = (uint8_t)(a >> shamt);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrl.h ====================
void do_lasx_emu_xvsrl_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrl.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrl_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint8_t shamt = (src_k >> shift) & 0xf;
            uint16_t res = (uint16_t)(a >> shamt);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrl.w ====================
void do_lasx_emu_xvsrl_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrl.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrl_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint8_t shamt = (src_k >> shift) & 0x1f;
            uint32_t res = (uint32_t)(a >> shamt);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrl.d ====================
void do_lasx_emu_xvsrl_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrl.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrl_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t k = vreg_read_64(uc, td, xk, i);
        uint8_t shamt = k & 0x3f;
        uint64_t res = a >> shamt;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsra.b ====================
void do_lasx_emu_xvsra_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsra.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsra_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            uint8_t shamt = (src_k >> shift) & 0x7;
            int8_t res = a >> shamt;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsra.h ====================
void do_lasx_emu_xvsra_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsra.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsra_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            uint8_t shamt = (src_k >> shift) & 0xf;
            int16_t res = a >> shamt;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsra.w ====================
void do_lasx_emu_xvsra_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsra.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsra_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            uint8_t shamt = (src_k >> shift) & 0x1f;
            int32_t res = a >> shamt;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsra.d ====================
void do_lasx_emu_xvsra_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsra.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsra_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t k = vreg_read_64(uc, td, xk, i);
        uint8_t shamt = k & 0x3f;
        int64_t a_signed = (int64_t)a;
        int64_t res = a_signed >> shamt;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvrotr.b ====================
void do_lasx_emu_xvrotr_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotr.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vrotr_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t shamt = (src_k >> shift) & 0x7;
            uint8_t res = (a >> shamt) | (a << (8 - shamt));
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvrotr.h ====================
void do_lasx_emu_xvrotr_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotr.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vrotr_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint8_t shamt = (src_k >> shift) & 0xf;
            uint16_t res = (a >> shamt) | (a << (16 - shamt));
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvrotr.w ====================
void do_lasx_emu_xvrotr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vrotr_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint8_t shamt = (src_k >> shift) & 0x1f;
            uint32_t res = (a >> shamt) | (a << (32 - shamt));
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvrotr.d ====================
void do_lasx_emu_xvrotr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vrotr_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t k = vreg_read_64(uc, td, xk, i);
        uint8_t shamt = k & 0x3f;
        uint64_t res = (a >> shamt) | (a << (64 - shamt));
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsrlr.b ====================
void do_lasx_emu_xvsrlr_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlr.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrlr_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t shamt = (src_k >> shift) & 0x7;
            uint8_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrlr.h ====================
void do_lasx_emu_xvsrlr_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlr.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrlr_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint8_t shamt = (src_k >> shift) & 0xf;
            uint16_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrlr.w ====================
void do_lasx_emu_xvsrlr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrlr_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint8_t shamt = (src_k >> shift) & 0x1f;
            uint32_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrlr.d ====================
void do_lasx_emu_xvsrlr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrlr_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t k = vreg_read_64(uc, td, xk, i);
        uint8_t shamt = k & 0x3f;
        uint64_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsrar.b ====================
void do_lasx_emu_xvsrar_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrar.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrar_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            uint8_t shamt = (src_k >> shift) & 0x7;
            int8_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrar.h ====================
void do_lasx_emu_xvsrar_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrar.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrar_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            uint8_t shamt = (src_k >> shift) & 0xf;
            int16_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrar.w ====================
void do_lasx_emu_xvsrar_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrar.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrar_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            uint8_t shamt = (src_k >> shift) & 0x1f;
            int32_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrar.d ====================
void do_lasx_emu_xvsrar_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrar.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsrar_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }


    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t k = vreg_read_64(uc, td, xk, i);
        uint8_t shamt = k & 0x3f;
        int64_t a_signed = (int64_t)a;
        int64_t res = (a_signed >> shamt) + (shamt > 0 ? ((a_signed >> (shamt - 1)) & 1) : 0);
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvrotri.b ====================
void do_lasx_emu_xvrotri_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotri.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t res = (a >> shamt) | (a << (8 - shamt));
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvrotri.h ====================
void do_lasx_emu_xvrotri_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotri.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t res = (a >> shamt) | (a << (16 - shamt));
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvrotri.w ====================
void do_lasx_emu_xvrotri_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotri.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t res = (a >> shamt) | (a << (32 - shamt));
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvrotri.d ====================
void do_lasx_emu_xvrotri_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrotri.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t res = (a >> shamt) | (a << (64 - shamt));
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsrlri.b ====================
void do_lasx_emu_xvsrlri_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlri.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrlri.h ====================
void do_lasx_emu_xvsrlri_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlri.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrlri.w ====================
void do_lasx_emu_xvsrlri_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlri.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrlri.d ====================
void do_lasx_emu_xvsrlri_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlri.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsrari.b ====================
void do_lasx_emu_xvsrari_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrari.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7;

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst[slot] &= ~(0xffULL << shift);
            dst[slot] |= ((uint64_t)(uint8_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvsrari.h ====================
void do_lasx_emu_xvsrari_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrari.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst[slot] &= ~(0xffffULL << shift);
            dst[slot] |= ((uint64_t)(uint16_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvsrari.w ====================
void do_lasx_emu_xvsrari_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrari.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t res = (a >> shamt) + (shamt > 0 ? ((a >> (shamt - 1)) & 1) : 0);
            dst[slot] &= ~(0xffffffffULL << shift);
            dst[slot] |= ((uint64_t)(uint32_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvsrari.d ====================
void do_lasx_emu_xvsrari_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrari.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        int64_t a_signed = (int64_t)a;
        int64_t res = (a_signed >> shamt) + (shamt > 0 ? ((a_signed >> (shamt - 1)) & 1) : 0);
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvslli.b ====================
void do_lasx_emu_xvslli_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslli.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7;

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t res = a << shamt;
            dst[slot] &= ~(0xffULL << shift);
            dst[slot] |= ((uint64_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvslli.h ====================
void do_lasx_emu_xvslli_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslli.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t res = a << shamt;
            dst[slot] &= ~(0xffffULL << shift);
            dst[slot] |= ((uint64_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvslli.w ====================
void do_lasx_emu_xvslli_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslli.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t res = a << shamt;
            dst[slot] &= ~(0xffffffffULL << shift);
            dst[slot] |= ((uint64_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvslli.d ====================
void do_lasx_emu_xvslli_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslli.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t res = a << shamt;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsrli.b ====================
void do_lasx_emu_xvsrli_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrli.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t res = a >> shamt;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvsrli.h ====================
void do_lasx_emu_xvsrli_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrli.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t res = a >> shamt;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvsrli.w ====================
void do_lasx_emu_xvsrli_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrli.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t res = a >> shamt;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvsrli.d ====================
void do_lasx_emu_xvsrli_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrli.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t res = a >> shamt;
        vreg_write_64(uc, td, xd, i, res);
    }

}

// ==================== xvsrai.b ====================
void do_lasx_emu_xvsrai_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrai.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t res = a >> shamt;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrai.h ====================
void do_lasx_emu_xvsrai_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrai.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t res = a >> shamt;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrai.w ====================
void do_lasx_emu_xvsrai_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrai.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t res = a >> shamt;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvsrai.d ====================
void do_lasx_emu_xvsrai_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrai.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        int64_t a_signed = (int64_t)a;
        int64_t res = a_signed >> shamt;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}
