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

#include "lasx_arith.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== xvadd.b ====================
void do_lasx_emu_xvadd_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadd.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vadd_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint8_t xj_b[32];
    uint8_t xk_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
        xk_b[i] = vreg_read_u8(uc, td, xk, i);
    }

    for (int i = 0; i < 32; i++) {
        vreg_write_8(uc, td, xd, i, xj_b[i] + xk_b[i]);
    }

}

// ==================== xvadd.h ====================
void do_lasx_emu_xvadd_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadd.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vadd_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, xj_h[i] + xk_h[i]);
    }

}

// ==================== xvadd.w ====================
void do_lasx_emu_xvadd_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadd.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vadd_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_u32(uc, td, xj, i);
        xk_w[i] = vreg_read_u32(uc, td, xk, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, xj_w[i] + xk_w[i]);
    }

}

// ==================== xvadd.d ====================
void do_lasx_emu_xvadd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vadd_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, xj_d[i] + xk_d[i]);
    }

}

// ==================== xvadd.q ====================
void do_lasx_emu_xvadd_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadd.q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vadd_q(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t xj_lo[2], xj_hi[2];
    uint64_t xk_lo[2], xk_hi[2];
    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xj, i, &xj_lo[i], &xj_hi[i]);
        vreg_read_128(uc, td, xk, i, &xk_lo[i], &xk_hi[i]);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = ((unsigned __int128)xj_hi[i] << 64) | xj_lo[i];
        unsigned __int128 b = ((unsigned __int128)xk_hi[i] << 64) | xk_lo[i];
        unsigned __int128 c = a + b;
        uint64_t c_lo = (uint64_t)c;
        uint64_t c_hi = (uint64_t)(c >> 64);
        vreg_write_128(uc, td, xd, i, c_lo, c_hi);
    }

}

// ==================== xvsub.b ====================
void do_lasx_emu_xvsub_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsub.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsub_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint8_t xj_b[32];
    uint8_t xk_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
        xk_b[i] = vreg_read_u8(uc, td, xk, i);
    }

    for (int i = 0; i < 32; i++) {
        vreg_write_8(uc, td, xd, i, xj_b[i] - xk_b[i]);
    }

}

// ==================== xvsub.h ====================
void do_lasx_emu_xvsub_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsub.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsub_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, xj_h[i] - xk_h[i]);
    }

}

// ==================== xvsub.w ====================
void do_lasx_emu_xvsub_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsub.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsub_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_u32(uc, td, xj, i);
        xk_w[i] = vreg_read_u32(uc, td, xk, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, xj_w[i] - xk_w[i]);
    }

}

// ==================== xvsub.d ====================
void do_lasx_emu_xvsub_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsub.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsub_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, xj_d[i] - xk_d[i]);
    }

}

// ==================== xvsub.q ====================
void do_lasx_emu_xvsub_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsub.q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vsub_q(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t xj_lo[2], xj_hi[2];
    uint64_t xk_lo[2], xk_hi[2];
    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xj, i, &xj_lo[i], &xj_hi[i]);
        vreg_read_128(uc, td, xk, i, &xk_lo[i], &xk_hi[i]);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = ((unsigned __int128)xj_hi[i] << 64) | xj_lo[i];
        unsigned __int128 b = ((unsigned __int128)xk_hi[i] << 64) | xk_lo[i];
        unsigned __int128 c = a - b;
        uint64_t c_lo = (uint64_t)c;
        uint64_t c_hi = (uint64_t)(c >> 64);
        vreg_write_128(uc, td, xd, i, c_lo, c_hi);
    }

}
