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

#include "lasx_muldiv.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"
#include <lsxintrin.h>

// ==================== xvmul.b ====================
void do_lasx_emu_xvmul_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmul.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmul_b(xj_lo, xk_lo);
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
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c = a * b;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvmul.h ====================
void do_lasx_emu_xvmul_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmul.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmul_h(xj_lo, xk_lo);
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
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c = a * b;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvmul.w ====================
void do_lasx_emu_xvmul_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmul.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmul_w(xj_lo, xk_lo);
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
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c = a * b;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvmul.d ====================
void do_lasx_emu_xvmul_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmul.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmul_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        int64_t sa = (int64_t)a;
        int64_t sb = (int64_t)b;
        int64_t c = sa * sb;
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }

}

// ==================== xvmuh.b ====================
void do_lasx_emu_xvmuh_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_b(xj_lo, xk_lo);
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
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int16_t res = (int16_t)a * b;
            int8_t c = (int8_t)(res >> 8);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmuh.h ====================
void do_lasx_emu_xvmuh_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_h(xj_lo, xk_lo);
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
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t res = (int32_t)a * b;
            int16_t c = (int16_t)(res >> 16);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmuh.w ====================
void do_lasx_emu_xvmuh_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_w(xj_lo, xk_lo);
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
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int64_t res = (int64_t)a * b;
            int32_t c = (int32_t)(res >> 32);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmuh.d ====================
void do_lasx_emu_xvmuh_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        __int128 res = (__int128)(int64_t)a * (int64_t)b;
        uint64_t c = (uint64_t)(res >> 64);
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvmuh.bu ====================
void do_lasx_emu_xvmuh_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_bu(xj_lo, xk_lo);
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
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint16_t res = (uint16_t)a * b;
            uint8_t c = (uint8_t)(res >> 8);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmuh.hu ====================
void do_lasx_emu_xvmuh_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_hu(xj_lo, xk_lo);
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
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint32_t res = (uint32_t)a * b;
            uint16_t c = (uint16_t)(res >> 16);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmuh.wu ====================
void do_lasx_emu_xvmuh_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_wu(xj_lo, xk_lo);
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
            uint32_t a = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t b = (uint32_t)((src_k >> shift) & 0xffffffff);
            uint64_t res = (uint64_t)a * b;
            uint32_t c = (uint32_t)(res >> 32);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmuh.du ====================
void do_lasx_emu_xvmuh_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmuh.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmuh_du(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        __int128 res = (__int128)a * b;
        uint64_t c = (uint64_t)(res >> 64);
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvmulwod.d.w ====================
void do_lasx_emu_xvmulwod_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_d_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 1; i < 8; i += 2) {
        int dword_idx = i / 2;
        int shift = (i % 2) * 32;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
        int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
        int64_t c = (int64_t)a * b;
        vreg_write_64(uc, td, xd, i / 2, (uint64_t)c);
    }
}

// ==================== xvmulwod.q.d ====================
void do_lasx_emu_xvmulwod_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_q_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 1; i < 4; i += 2) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        __int128 c = (__int128)(int64_t)a * (int64_t)b;
        uint64_t c_lo = (uint64_t)c;
        uint64_t c_hi = (uint64_t)(c >> 64);
        vreg_write_128(uc, td, xd, i / 2, c_lo, c_hi);
    }
}

// ==================== xvmulwev.h.bu ====================
void do_lasx_emu_xvmulwev_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_h_bu(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 4; i++) {
            int byte_idx = i * 2;
            int shift = (byte_idx % 8) * 8;
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint16_t c = (uint16_t)a * b;
            vreg_write_16(uc, td, xd, dword_idx * 4 + i, c);
        }
    }
}

// ==================== xvmulwev.w.hu ====================
void do_lasx_emu_xvmulwev_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_w_hu(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2;
            int shift = (half_idx % 4) * 16;
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint32_t c = (uint32_t)a * b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, c);
        }
    }
}

// ==================== xvmulwev.d.wu ====================
void do_lasx_emu_xvmulwev_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_d_wu(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint32_t a = (uint32_t)src_j;
        uint32_t b = (uint32_t)src_k;
        uint64_t c = (uint64_t)a * b;
        vreg_write_64(uc, td, xd, slot, c);
    }
}

// ==================== xvmulwev.q.du ====================
void do_lasx_emu_xvmulwev_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_q_du(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int qword_idx = 0; qword_idx < 2; qword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, qword_idx * 2);
        uint64_t src_k = vreg_read_64(uc, td, xk, qword_idx * 2);
        __uint128_t prod = (__uint128_t)src_j * src_k;
        uint64_t lo = (uint64_t)prod;
        uint64_t hi = (uint64_t)(prod >> 64);
        vreg_write_128(uc, td, xd, qword_idx, lo, hi);
    }
}

// ==================== xvmulwod.h.bu ====================
void do_lasx_emu_xvmulwod_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_h_bu(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);

        for (int i = 0; i < 4; i++) {
            int shift = (i * 2 + 1) * 8;
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint16_t c = (uint16_t)a * b;
            vreg_write_16(uc, td, xd, slot * 4 + i, c);
        }
    }
}

// ==================== xvmulwev.h.b ====================
void do_lasx_emu_xvmulwev_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_h_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 4; i++) {
            int byte_idx = i * 2;
            int shift = (byte_idx % 8) * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int16_t c = (int16_t)a * b;
            vreg_write_16(uc, td, xd, dword_idx * 4 + i, (uint16_t)c);
        }
    }
}

// ==================== xvmulwev.w.h ====================
void do_lasx_emu_xvmulwev_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_w_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2;
            int shift = (half_idx % 4) * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t c = (int32_t)a * b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, (uint32_t)c);
        }
    }
}

// ==================== xvmulwev.d.w ====================
void do_lasx_emu_xvmulwev_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_d_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 1; i++) {
            int word_idx = i * 2;
            int shift = (word_idx % 2) * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int64_t c = (int64_t)a * b;
            vreg_write_64(uc, td, xd, dword_idx, (uint64_t)c);
        }
    }
}

// ==================== xvmulwev.q.d ====================
void do_lasx_emu_xvmulwev_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_q_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int qword_idx = 0; qword_idx < 2; qword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, qword_idx * 2);
        uint64_t src_k = vreg_read_64(uc, td, xk, qword_idx * 2);
        __int128 prod = (__int128)(int64_t)src_j * (int64_t)src_k;
        uint64_t lo = (uint64_t)prod;
        uint64_t hi = (uint64_t)(prod >> 64);
        vreg_write_128(uc, td, xd, qword_idx, lo, hi);
    }
}

// ==================== xvmulwod.h.b ====================
void do_lasx_emu_xvmulwod_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_h_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int shift = (byte_idx % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        int8_t a = (int8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        int16_t c = (int16_t)a * b;

        vreg_write_16(uc, td, xd, i, (uint16_t)c);
    }
}

// ==================== xvmulwod.w.h ====================
void do_lasx_emu_xvmulwod_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_w_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 1; i < 16; i += 2) {
        int word_idx = i / 4;
        int shift = (i % 4) * 16;
        uint64_t src_j = vreg_read_64(uc, td, xj, word_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, word_idx);
        int16_t a = (int16_t)((src_j >> shift) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift) & 0xffff);
        int32_t c = (int32_t)a * b;

        int dword_idx = (i / 2) / 2;
        int shift_out = ((i / 2) % 2) * 32;
        dst[dword_idx] &= ~(0xffffffffULL << shift_out);
        dst[dword_idx] |= ((uint64_t)(uint32_t)c << shift_out);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmulwod.w.hu ====================
void do_lasx_emu_xvmulwod_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_w_hu(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 1; i < 16; i += 2) {
        int word_idx = i / 4;
        int shift = (i % 4) * 16;
        uint64_t src_j = vreg_read_64(uc, td, xj, word_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, word_idx);
        uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
        uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
        uint32_t c = (uint32_t)a * b;

        int dword_idx = (i / 2) / 2;
        int shift_out = ((i / 2) % 2) * 32;
        dst[dword_idx] &= ~(0xffffffffULL << shift_out);
        dst[dword_idx] |= ((uint64_t)c << shift_out);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmulwod.d.wu ====================
void do_lasx_emu_xvmulwod_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_d_wu(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 1; i < 8; i += 2) {
        int dword_idx = i / 2;
        int shift = (i % 2) * 32;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint32_t a = (uint32_t)((src_j >> shift) & 0xffffffff);
        uint32_t b = (uint32_t)((src_k >> shift) & 0xffffffff);
        uint64_t c = (uint64_t)a * b;
        vreg_write_64(uc, td, xd, i / 2, c);
    }
}

// ==================== xvmulwod.q.du ====================
void do_lasx_emu_xvmulwod_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_q_du(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 1; i < 4; i += 2) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        __uint128_t c = (__uint128_t)a * b;
        uint64_t c_lo = (uint64_t)c;
        uint64_t c_hi = (uint64_t)(c >> 64);
        vreg_write_128(uc, td, xd, i / 2, c_lo, c_hi);
    }
}

// ==================== xvmulwev_h_bu_b ====================
void do_lasx_emu_xvmulwev_h_bu_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev_h_bu_b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_h_bu_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 4; i++) {
            int byte_idx = i * 2;
            int shift = (byte_idx % 8) * 8;
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int16_t c = (int16_t)a * b;
            vreg_write_16(uc, td, xd, dword_idx * 4 + i, (uint16_t)c);
        }
    }
}

// ==================== xvmulwev_w_hu_h ====================
void do_lasx_emu_xvmulwev_w_hu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev_w_hu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_w_hu_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2;
            int shift = (half_idx % 4) * 16;
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t c = (int32_t)a * b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, (uint32_t)c);
        }
    }
}

// ==================== xvmulwev.d.wuw ====================
void do_lasx_emu_xvmulwev_d_wu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.d.wu.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_d_wu_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i);
    }
    for (int i = 0; i < 4; i++) {
        uint32_t src_j = xj_w[2 * i];
        int32_t src_k = xk_w[2 * i];
        int64_t c = (int64_t)src_j * src_k;
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvmulwod_h_bu_b ====================
void do_lasx_emu_xvmulwod_h_bu_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod_h_bu_b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_h_bu_b(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint8_t xj_b[32];
    uint8_t xk_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
        xk_b[i] = vreg_read_8(uc, td, xk, i);
    }
    for (int i = 0; i < 16; i++) {
        uint8_t src_j = xj_b[2 * i + 1];
        int8_t src_k = xk_b[2 * i + 1];
        int16_t c = (int16_t)src_j * src_k;
        vreg_write_16(uc, td, xd, i, (uint16_t)c);
    }
}

// ==================== xvmulwod_w_hu_h ====================
void do_lasx_emu_xvmulwod_w_hu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod_w_hu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_w_hu_h(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
        xk_h[i] = vreg_read_16(uc, td, xk, i);
    }
    for (int i = 0; i < 8; i++) {
        uint16_t src_j = xj_h[2 * i + 1];
        int16_t src_k = xk_h[2 * i + 1];
        int32_t c = (int32_t)src_j * src_k;
        vreg_write_32(uc, td, xd, i, (uint32_t)c);
    }
}

// ==================== xvmulwod.d.wuw ====================
void do_lasx_emu_xvmulwod_d_wu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.d.wu.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_d_wu_w(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i);
    }
    for (int i = 0; i < 4; i++) {
        uint32_t src_j = xj_w[2 * i + 1];
        int32_t src_k = xk_w[2 * i + 1];
        int64_t c = (int64_t)src_j * src_k;
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvmulwev.q.dud ====================
void do_lasx_emu_xvmulwev_q_du_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwev.q.du.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwev_q_du_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int qword_idx = 0; qword_idx < 2; qword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, qword_idx * 2);
        uint64_t src_k = vreg_read_64(uc, td, xk, qword_idx * 2);
        __int128_t prod = (__int128_t)(uint64_t)src_j * (int64_t)src_k;
        uint64_t lo = (uint64_t)prod;
        uint64_t hi = (uint64_t)(prod >> 64);
        vreg_write_128(uc, td, xd, qword_idx, lo, hi);
    }
}

// ==================== xvmulwod.q.dud ====================
void do_lasx_emu_xvmulwod_q_du_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmulwod.q.du.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmulwod_q_du_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 1; i < 4; i += 2) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        __int128_t c = (__int128_t)(uint64_t)a * (int64_t)b;
        uint64_t c_lo = (uint64_t)c;
        uint64_t c_hi = (uint64_t)(c >> 64);
        vreg_write_128(uc, td, xd, i / 2, c_lo, c_hi);
    }
}

// ==================== xvmadd.b ====================
void do_lasx_emu_xvmadd_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmadd.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmadd_b(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c = (int8_t)((dst[slot] >> shift) & 0xff);
            int8_t res = c + a * b;
            dst[slot] &= ~(0xffULL << shift);
            dst[slot] |= ((uint64_t)(uint8_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }

}

// ==================== xvmadd.h ====================
void do_lasx_emu_xvmadd_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmadd.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmadd_h(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c = (int16_t)((dst[slot] >> shift) & 0xffff);
            int16_t res = c + a * b;
            dst[slot] &= ~(0xffffULL << shift);
            dst[slot] |= ((uint64_t)(uint16_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }

}

// ==================== xvmadd.w ====================
void do_lasx_emu_xvmadd_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmadd.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmadd_w(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c = (int32_t)((dst[slot] >> shift) & 0xffffffff);
            int32_t res = c + a * b;
            dst[slot] &= ~(0xffffffffULL << shift);
            dst[slot] |= ((uint64_t)(uint32_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }

}

// ==================== xvmadd.d ====================
void do_lasx_emu_xvmadd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmadd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmadd_d(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        int64_t res = (int64_t)dst[i] + (int64_t)a * (int64_t)b;
        dst[i] = (uint64_t)res;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }

}

// ==================== xvmsub.b ====================
void do_lasx_emu_xvmsub_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmsub.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmsub_b(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c = (int8_t)((dst[slot] >> shift) & 0xff);
            int8_t res = c - a * b;
            dst[slot] &= ~(0xffULL << shift);
            dst[slot] |= ((uint64_t)(uint8_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvmsub.h ====================
void do_lasx_emu_xvmsub_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmsub.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmsub_h(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c = (int16_t)((dst[slot] >> shift) & 0xffff);
            int16_t res = c - a * b;
            dst[slot] &= ~(0xffffULL << shift);
            dst[slot] |= ((uint64_t)(uint16_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvmsub.w ====================
void do_lasx_emu_xvmsub_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmsub.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmsub_w(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int slot = 0; slot < 4; slot++) {
        dst[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c = (int32_t)((dst[slot] >> shift) & 0xffffffff);
            int32_t res = c - a * b;
            dst[slot] &= ~(0xffffffffULL << shift);
            dst[slot] |= ((uint64_t)(uint32_t)res << shift);
        }
    }

    for (int slot = 0; slot < 4; slot++) {
        vreg_write_64(uc, td, xd, slot, dst[slot]);
    }
}

// ==================== xvmsub.d ====================
void do_lasx_emu_xvmsub_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmsub.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmsub_d(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        int64_t res = (int64_t)dst[i] - (int64_t)a * (int64_t)b;
        dst[i] = (uint64_t)res;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwev.h.b ====================
void do_lasx_emu_xvmaddwev_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_h_b(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t dst[16];
    for (int i = 0; i < 16; i++) {
        dst[i] = vreg_read_16(uc, td, xd, i);
    }

    for (int i = 0; i < 16; i++) {
        int8_t a = (int8_t)vreg_read_u8(uc, td, xj, i * 2);
        int8_t b = (int8_t)vreg_read_u8(uc, td, xk, i * 2);
        int16_t prod = (int16_t)a * (int16_t)b;
        int16_t curr = (int16_t)dst[i];
        int16_t res = curr + prod;
        dst[i] = (uint16_t)res;
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwev.w.h ====================
void do_lasx_emu_xvmaddwev_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_w_h(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t dst[8];
    for (int i = 0; i < 8; i++) {
        dst[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        int16_t a = (int16_t)vreg_read_16(uc, td, xj, i * 2);
        int16_t b = (int16_t)vreg_read_16(uc, td, xk, i * 2);
        int32_t prod = (int32_t)a * (int32_t)b;
        int32_t curr = (int32_t)dst[i];
        int32_t res = curr + prod;
        dst[i] = (uint32_t)res;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwev.d.w ====================
void do_lasx_emu_xvmaddwev_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_d_w(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        int32_t a = (int32_t)vreg_read_32(uc, td, xj, i * 2);
        int32_t b = (int32_t)vreg_read_32(uc, td, xk, i * 2);
        int64_t prod = (int64_t)a * (int64_t)b;
        int64_t curr = (int64_t)dst[i];
        int64_t res = curr + prod;
        dst[i] = (uint64_t)res;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwev.q.d ====================
void do_lasx_emu_xvmaddwev_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_q_d(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst_lo[2], dst_hi[2];
    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xd, i, &dst_lo[i], &dst_hi[i]);
    }

    for (int i = 0; i < 2; i++) {
        int dword_idx = i * 2;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        __int128 prod = (__int128)(int64_t)src_j * (int64_t)src_k;

        __int128 curr = ((__int128)dst_hi[i] << 64) | dst_lo[i];
        __int128 res = curr + prod;

        dst_lo[i] = (uint64_t)res;
        dst_hi[i] = (uint64_t)(res >> 64);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_128(uc, td, xd, i, dst_lo[i], dst_hi[i]);
    }
}

// ==================== xvmaddwev.d.wu ====================
void do_lasx_emu_xvmaddwev_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_d_wu(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = vreg_read_32(uc, td, xj, i * 2);
        uint32_t b = vreg_read_32(uc, td, xk, i * 2);
        uint64_t prod = (uint64_t)a * (uint64_t)b;
        uint64_t curr = dst[i];
        uint64_t res = curr + prod;
        dst[i] = res;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwev.q.du ====================
void do_lasx_emu_xvmaddwev_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_q_du(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst_lo[2], dst_hi[2];
    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xd, i, &dst_lo[i], &dst_hi[i]);
    }

    for (int i = 0; i < 2; i++) {
        int dword_idx = i * 2;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        __uint128_t prod = (__uint128_t)src_j * src_k;

        __uint128_t curr = ((__uint128_t)dst_hi[i] << 64) | dst_lo[i];
        __uint128_t res = curr + prod;

        dst_lo[i] = (uint64_t)res;
        dst_hi[i] = (uint64_t)(res >> 64);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_128(uc, td, xd, i, dst_lo[i], dst_hi[i]);
    }
}

// ==================== xvmaddwod.d.wu ====================
void do_lasx_emu_xvmaddwod_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_d_wu(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst[4];
    for (int i = 0; i < 4; i++) {
        dst[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        int word_idx = i * 2 + 1;
        int dword_idx = word_idx / 2;
        int word_in_dword = word_idx % 2;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint32_t a = (uint32_t)(src_j >> (word_in_dword * 32));
        uint32_t b = (uint32_t)(src_k >> (word_in_dword * 32));
        uint64_t prod = (uint64_t)a * b;
        uint64_t curr = dst[i];
        uint64_t res = curr + prod;
        dst[i] = res;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwod.q.du ====================
void do_lasx_emu_xvmaddwod_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_q_du(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint64_t dst_lo[2], dst_hi[2];
    for (int i = 0; i < 2; i++) {
        vreg_read_128(uc, td, xd, i, &dst_lo[i], &dst_hi[i]);
    }

    for (int qword_idx = 0; qword_idx < 2; qword_idx++) {
        int src_dword_idx = qword_idx * 2 + 1;
        uint64_t a = vreg_read_64(uc, td, xj, src_dword_idx);
        uint64_t b = vreg_read_64(uc, td, xk, src_dword_idx);
        __uint128_t prod = (__uint128_t)a * b;

        __uint128_t curr = ((__uint128_t)dst_hi[qword_idx] << 64) | dst_lo[qword_idx];
        __uint128_t res = curr + prod;

        dst_lo[qword_idx] = (uint64_t)res;
        dst_hi[qword_idx] = (uint64_t)(res >> 64);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_128(uc, td, xd, i, dst_lo[i], dst_hi[i]);
    }
}

// ==================== xvmaddwev_h_bu_b ====================
void do_lasx_emu_xvmaddwev_h_bu_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev_h_bu_b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_h_bu_b(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t dst[16];
    for (int i = 0; i < 16; i++) {
        dst[i] = vreg_read_16(uc, td, xd, i);
    }

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2;
        int dword_idx = byte_idx / 8;
        int byte_in_dword = byte_idx % 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint8_t a = (uint8_t)((src_j >> (byte_in_dword * 8)) & 0xff);
        int8_t b = (int8_t)((src_k >> (byte_in_dword * 8)) & 0xff);
        int16_t prod = (int16_t)a * b;
        int16_t curr = (int16_t)dst[i];
        int16_t res = curr + prod;
        dst[i] = (uint16_t)res;
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwev_w_hu_h ====================
void do_lasx_emu_xvmaddwev_w_hu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev_w_hu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_w_hu_h(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t dst[8];
    for (int i = 0; i < 8; i++) {
        dst[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2;
        int dword_idx = half_idx / 4;
        int half_in_dword = half_idx % 4;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint16_t a = (uint16_t)((src_j >> (half_in_dword * 16)) & 0xffff);
        int16_t b = (int16_t)((src_k >> (half_in_dword * 16)) & 0xffff);
        int32_t prod = (int32_t)a * b;
        int32_t curr = (int32_t)dst[i];
        int32_t res = curr + prod;
        dst[i] = (uint32_t)res;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwod_h_bu_b ====================
void do_lasx_emu_xvmaddwod_h_bu_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod_h_bu_b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_h_bu_b(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t dst[16];
    for (int i = 0; i < 16; i++) {
        dst[i] = vreg_read_16(uc, td, xd, i);
    }

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int byte_in_dword = byte_idx % 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint8_t a = (uint8_t)((src_j >> (byte_in_dword * 8)) & 0xff);
        int8_t b = (int8_t)((src_k >> (byte_in_dword * 8)) & 0xff);
        int16_t prod = (int16_t)a * (int16_t)b;
        int16_t cur = (int16_t)dst[i];
        int16_t res = cur + prod;
        dst[i] = (uint16_t)res;
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvmaddwod_w_hu_h ====================
void do_lasx_emu_xvmaddwod_w_hu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod_w_hu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_w_hu_h(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int32_t cur[8];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot * 2] = (int32_t)vreg_read_32(uc, td, xd, slot * 2);
        cur[slot * 2 + 1] = (int32_t)vreg_read_32(uc, td, xd, slot * 2 + 1);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2 + 1;
        int slot = half_idx / 4;
        int half_in_dword = half_idx % 4;

        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint16_t a = (uint16_t)((src_j >> (half_in_dword * 16)) & 0xffff);
        int16_t b = (int16_t)((src_k >> (half_in_dword * 16)) & 0xffff);
        int32_t prod = (int32_t)a * (int32_t)b;
        int32_t res = cur[i] + prod;
        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }
}

// ==================== xvmaddwod.h.b ====================
void do_lasx_emu_xvmaddwod_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_h_b(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int16_t cur[16];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        for (int j = 0; j < 4; j++) {
            cur[slot * 4 + j] = (int16_t)vreg_read_16(uc, td, xd, slot * 4 + j);
        }
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 16; i++) {
        int slot = i / 4;
        int byte_idx = i * 2 + 1;
        int byte_in_slot = byte_idx % 8;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        int8_t a = (int8_t)((src_j >> (byte_in_slot * 8)) & 0xff);
        int8_t b = (int8_t)((src_k >> (byte_in_slot * 8)) & 0xff);
        int16_t prod = (int16_t)a * (int16_t)b;
        int16_t res = cur[i] + prod;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

// ==================== xvmaddwod.w.h ====================
void do_lasx_emu_xvmaddwod_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_w_h(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int32_t cur[8];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot * 2] = (int32_t)vreg_read_32(uc, td, xd, slot * 2);
        cur[slot * 2 + 1] = (int32_t)vreg_read_32(uc, td, xd, slot * 2 + 1);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int slot = i / 2;
        int half_idx = i * 2 + 1;
        int half_in_slot = half_idx % 4;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        int16_t a = (int16_t)((src_j >> (half_in_slot * 16)) & 0xffff);
        int16_t b = (int16_t)((src_k >> (half_in_slot * 16)) & 0xffff);
        int32_t prod = (int32_t)a * (int32_t)b;
        int32_t res = cur[i] + prod;
        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }
}

// ==================== xvmaddwod.d.w ====================
void do_lasx_emu_xvmaddwod_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_d_w(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int64_t cur[4];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot] = (int64_t)vreg_read_64(uc, td, xd, slot);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int slot = i;
        int word_idx = i * 2 + 1;
        int word_in_slot = word_idx % 2;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        int32_t a = (int32_t)((src_j >> (word_in_slot * 32)) & 0xffffffff);
        int32_t b = (int32_t)((src_k >> (word_in_slot * 32)) & 0xffffffff);
        int64_t prod = (int64_t)a * (int64_t)b;
        int64_t res = cur[i] + prod;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvmaddwod.q.d ====================
void do_lasx_emu_xvmaddwod_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_q_d(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    __int128 cur[2];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int i = 0; i < 2; i++) {
        uint64_t curr_lo, curr_hi;
        vreg_read_128(uc, td, xd, i, &curr_lo, &curr_hi);
        cur[i] = ((__int128)curr_hi << 64) | curr_lo;
        src_j_cache[i * 2] = vreg_read_64(uc, td, xj, i * 2);
        src_j_cache[i * 2 + 1] = vreg_read_64(uc, td, xj, i * 2 + 1);
        src_k_cache[i * 2] = vreg_read_64(uc, td, xk, i * 2);
        src_k_cache[i * 2 + 1] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        int dword_idx = i * 2 + 1;
        uint64_t src_j = src_j_cache[dword_idx];
        uint64_t src_k = src_k_cache[dword_idx];
        __int128 prod = (__int128)(int64_t)src_j * (int64_t)src_k;
        __int128 res = cur[i] + prod;

        uint64_t res_lo = (uint64_t)res;
        uint64_t res_hi = (uint64_t)(res >> 64);
        vreg_write_128(uc, td, xd, i, res_lo, res_hi);
    }
}

// ==================== xvmaddwev.d.wu.w ====================
void do_lasx_emu_xvmaddwev_d_wu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.d.wu.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_d_wu_w(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int64_t cur[4];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot] = (int64_t)vreg_read_64(uc, td, xd, slot);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int slot = i;
        int word_idx = i * 2;
        int word_in_slot = word_idx % 2;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint32_t a = (uint32_t)((src_j >> (word_in_slot * 32)) & 0xffffffff);
        int32_t b = (int32_t)((src_k >> (word_in_slot * 32)) & 0xffffffff);
        int64_t prod = (int64_t)a * b;
        int64_t res = cur[i] + prod;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvmaddwev.q.du.d ====================
void do_lasx_emu_xvmaddwev_q_du_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.q.du.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_q_du_d(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    __int128_t cur[2];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int i = 0; i < 2; i++) {
        uint64_t curr_lo, curr_hi;
        vreg_read_128(uc, td, xd, i, &curr_lo, &curr_hi);
        cur[i] = ((__int128_t)curr_hi << 64) | curr_lo;
        src_j_cache[i * 2] = vreg_read_64(uc, td, xj, i * 2);
        src_j_cache[i * 2 + 1] = vreg_read_64(uc, td, xj, i * 2 + 1);
        src_k_cache[i * 2] = vreg_read_64(uc, td, xk, i * 2);
        src_k_cache[i * 2 + 1] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        int dword_idx = i * 2;
        uint64_t src_j = src_j_cache[dword_idx];
        uint64_t src_k = src_k_cache[dword_idx];
        __int128_t prod = (__int128_t)(uint64_t)src_j * (int64_t)src_k;
        __int128_t res = cur[i] + prod;

        uint64_t res_lo = (uint64_t)res;
        uint64_t res_hi = (uint64_t)(res >> 64);
        vreg_write_128(uc, td, xd, i, res_lo, res_hi);
    }
}

// ==================== xvmaddwod.d.wu.w ====================
void do_lasx_emu_xvmaddwod_d_wu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.d.wu.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_d_wu_w(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    int64_t cur[4];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot] = (int64_t)vreg_read_64(uc, td, xd, slot);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int slot = i;
        int word_idx = i * 2 + 1;
        int word_in_slot = word_idx % 2;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint32_t a = (uint32_t)((src_j >> (word_in_slot * 32)) & 0xffffffff);
        int32_t b = (int32_t)((src_k >> (word_in_slot * 32)) & 0xffffffff);
        int64_t prod = (int64_t)a * b;
        int64_t res = cur[i] + prod;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvmaddwod.q.du.d ====================
void do_lasx_emu_xvmaddwod_q_du_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.q.du.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_q_du_d(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    __int128_t cur[2];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int i = 0; i < 2; i++) {
        uint64_t curr_lo, curr_hi;
        vreg_read_128(uc, td, xd, i, &curr_lo, &curr_hi);
        cur[i] = ((__int128_t)curr_hi << 64) | curr_lo;
        src_j_cache[i * 2] = vreg_read_64(uc, td, xj, i * 2);
        src_j_cache[i * 2 + 1] = vreg_read_64(uc, td, xj, i * 2 + 1);
        src_k_cache[i * 2] = vreg_read_64(uc, td, xk, i * 2);
        src_k_cache[i * 2 + 1] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        int dword_idx = i * 2 + 1;
        uint64_t src_j = src_j_cache[dword_idx];
        uint64_t src_k = src_k_cache[dword_idx];
        __int128_t prod = (__int128_t)(uint64_t)src_j * (int64_t)src_k;
        __int128_t res = cur[i] + prod;

        uint64_t res_lo = (uint64_t)res;
        uint64_t res_hi = (uint64_t)(res >> 64);
        vreg_write_128(uc, td, xd, i, res_lo, res_hi);
    }
}

// ==================== xvmaddwev.h.bu ====================
void do_lasx_emu_xvmaddwev_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_h_bu(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t cur[16];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        for (int j = 0; j < 4; j++) {
            cur[slot * 4 + j] = vreg_read_16(uc, td, xd, slot * 4 + j);
        }
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 16; i++) {
        int slot = i / 4;
        int byte_idx = i * 2;
        int byte_in_slot = byte_idx % 8;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint8_t a = (uint8_t)((src_j >> (byte_in_slot * 8)) & 0xff);
        uint8_t b = (uint8_t)((src_k >> (byte_in_slot * 8)) & 0xff);
        uint16_t prod = (uint16_t)a * (uint16_t)b;
        uint16_t res = cur[i] + prod;
        vreg_write_16(uc, td, xd, i, res);
    }
}

// ==================== xvmaddwev.w.hu ====================
void do_lasx_emu_xvmaddwev_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwev.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwev_w_hu(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t cur[8];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot * 2] = vreg_read_32(uc, td, xd, slot * 2);
        cur[slot * 2 + 1] = vreg_read_32(uc, td, xd, slot * 2 + 1);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int slot = i / 2;
        int half_idx = i * 2;
        int half_in_slot = half_idx % 4;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint16_t a = (uint16_t)((src_j >> (half_in_slot * 16)) & 0xffff);
        uint16_t b = (uint16_t)((src_k >> (half_in_slot * 16)) & 0xffff);
        uint32_t prod = (uint32_t)a * (uint32_t)b;
        uint32_t res = cur[i] + prod;
        vreg_write_32(uc, td, xd, i, res);
    }
}

// ==================== xvmaddwod.h.bu ====================
void do_lasx_emu_xvmaddwod_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_h_bu(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint16_t cur[16];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        for (int j = 0; j < 4; j++) {
            cur[slot * 4 + j] = vreg_read_16(uc, td, xd, slot * 4 + j);
        }
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 16; i++) {
        int slot = i / 4;
        int byte_idx = i * 2 + 1;
        int byte_in_slot = byte_idx % 8;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint8_t a = (uint8_t)((src_j >> (byte_in_slot * 8)) & 0xff);
        uint8_t b = (uint8_t)((src_k >> (byte_in_slot * 8)) & 0xff);
        uint16_t prod = (uint16_t)a * b;
        uint16_t res = cur[i] + prod;
        vreg_write_16(uc, td, xd, i, res);
    }
}

// ==================== xvmaddwod.w.hu ====================
void do_lasx_emu_xvmaddwod_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaddwod.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xd_lo = (__m128i){vreg_read_64(uc, td, xd, i * 2), vreg_read_64(uc, td, xd, i * 2 + 1)};
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmaddwod_w_hu(xd_lo, xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    uint32_t cur[8];
    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        cur[slot * 2] = vreg_read_32(uc, td, xd, slot * 2);
        cur[slot * 2 + 1] = vreg_read_32(uc, td, xd, slot * 2 + 1);
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int slot = i / 2;
        int half_idx = i * 2 + 1;
        int half_in_slot = half_idx % 4;
        uint64_t src_j = src_j_cache[slot];
        uint64_t src_k = src_k_cache[slot];
        uint16_t a = (uint16_t)((src_j >> (half_in_slot * 16)) & 0xffff);
        uint16_t b = (uint16_t)((src_k >> (half_in_slot * 16)) & 0xffff);
        uint32_t prod = (uint32_t)a * b;
        uint32_t res = cur[i] + prod;
        vreg_write_32(uc, td, xd, i, res);
    }
}

// ==================== xvdiv.b ====================
void do_lasx_emu_xvdiv_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_b(xj_lo, xk_lo);
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
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c;
            if (b != 0) {
                c = a / b;
            } else {
                c = 0;
            }
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvdiv.h ====================
void do_lasx_emu_xvdiv_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_h(xj_lo, xk_lo);
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
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c;
            if (b != 0) {
                c = a / b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvdiv.w ====================
void do_lasx_emu_xvdiv_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_w(xj_lo, xk_lo);
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
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c;
            if (b != 0) {
                c = a / b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvdiv.d ====================
void do_lasx_emu_xvdiv_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)vreg_read_64(uc, td, xj, i);
        int64_t b = (int64_t)vreg_read_64(uc, td, xk, i);
        int64_t c;
        if (b != 0) {
            c = a / b;
        } else {
            c = 0;
        }
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvdiv.du ====================
void do_lasx_emu_xvdiv_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_du(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        uint64_t c;
        if (b != 0) {
            c = a / b;
        } else {
            c = 0;
        }
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvmod.b ====================
void do_lasx_emu_xvmod_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_b(xj_lo, xk_lo);
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
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c;
            if (b != 0) {
                c = a % b;
            } else {
                c = 0;
            }
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.h ====================
void do_lasx_emu_xvmod_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_h(xj_lo, xk_lo);
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
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c;
            if (b != 0) {
                c = a % b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.w ====================
void do_lasx_emu_xvmod_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_w(xj_lo, xk_lo);
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
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c;
            if (b != 0) {
                c = a % b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.d ====================
void do_lasx_emu_xvmod_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_d(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)vreg_read_64(uc, td, xj, i);
        int64_t b = (int64_t)vreg_read_64(uc, td, xk, i);
        int64_t c;
        if (b != 0) {
            c = a % b;
        } else {
            c = 0;
        }
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvdiv.bu ====================
void do_lasx_emu_xvdiv_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_bu(xj_lo, xk_lo);
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
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint8_t c;
            if (b != 0) {
                c = a / b;
            } else {
                c = 0;
            }
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvdiv.hu ====================
void do_lasx_emu_xvdiv_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_hu(xj_lo, xk_lo);
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
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint16_t c;
            if (b != 0) {
                c = a / b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvdiv.wu ====================
void do_lasx_emu_xvdiv_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvdiv.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vdiv_wu(xj_lo, xk_lo);
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
            uint32_t a = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t b = (uint32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c;
            if (b != 0) {
                c = a / b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.bu ====================
void do_lasx_emu_xvmod_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_bu(xj_lo, xk_lo);
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
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint8_t c;
            if (b != 0) {
                c = a % b;
            } else {
                c = 0;
            }
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.hu ====================
void do_lasx_emu_xvmod_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_hu(xj_lo, xk_lo);
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
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint16_t c;
            if (b != 0) {
                c = a % b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.wu ====================
void do_lasx_emu_xvmod_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_wu(xj_lo, xk_lo);
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
            uint32_t a = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t b = (uint32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c;
            if (b != 0) {
                c = a % b;
            } else {
                c = 0;
            }
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmod.du ====================
void do_lasx_emu_xvmod_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmod.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    if (use_lsx_intrinsics) {
        for (int i = 0; i < 2; i++) {
            __m128i xj_lo = (__m128i){vreg_read_64(uc, td, xj, i * 2), vreg_read_64(uc, td, xj, i * 2 + 1)};
            __m128i xk_lo = (__m128i){vreg_read_64(uc, td, xk, i * 2), vreg_read_64(uc, td, xk, i * 2 + 1)};
            __m128i result = __lsx_vmod_du(xj_lo, xk_lo);
            int64_t* r = (int64_t*)&result;
            vreg_write_64(uc, td, xd, i * 2, r[0]);
            vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        uint64_t a = vreg_read_64(uc, td, xj, i);
        uint64_t b = vreg_read_64(uc, td, xk, i);
        uint64_t c;
        if (b != 0) {
            c = a % b;
        } else {
            c = 0;
        }
        vreg_write_64(uc, td, xd, i, c);
    }
}
