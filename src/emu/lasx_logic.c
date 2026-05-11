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

#include "lasx_logic.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"
#include <lsxintrin.h>

// ==================== xvand.v ====================
void do_lasx_emu_xvand_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvand.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vand_v(xj_lo, xk_lo);
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
        vreg_write_64(uc, td, xd, i, xj_d[i] & xk_d[i]);
    }

}

// ==================== xvor.v ====================
void do_lasx_emu_xvor_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvor.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vor_v(xj_lo, xk_lo);
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
        vreg_write_64(uc, td, xd, i, xj_d[i] | xk_d[i]);
    }

}

// ==================== xvxor.v ====================
void do_lasx_emu_xvxor_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvxor.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vxor_v(xj_lo, xk_lo);
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
        vreg_write_64(uc, td, xd, i, xj_d[i] ^ xk_d[i]);
    }

}

// ==================== xvnor.v ====================
void do_lasx_emu_xvnor_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvnor.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vnor_v(xj_lo, xk_lo);
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
        vreg_write_64(uc, td, xd, i, ~(xj_d[i] | xk_d[i]));
    }

}

// ==================== xvandn.v ====================
void do_lasx_emu_xvandn_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvandn.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vandn_v(xj_lo, xk_lo);
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
        vreg_write_64(uc, td, xd, i, xk_d[i] & ~xj_d[i]);
    }

}

// ==================== xvorn.v ====================
void do_lasx_emu_xvorn_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvorn.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vorn_v(xj_lo, xk_lo);
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
        vreg_write_64(uc, td, xd, i, xj_d[i] | ~xk_d[i]);
    }

}
