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

#include "lasx_fcmp.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include <lsxintrin.h>
#include <stdint.h>

// Helper unions for type-punning to avoid strict-aliasing warnings
union float_bits {
    uint32_t u;
    float f;
};

union double_bits {
    uint64_t u;
    double d;
};

// Helper macros for floating point classification
#define FP_IS_NAN(a) ((a) != (a))

// Single precision floating point comparison with condition code
void do_lasx_emu_xvfcmp_cond_s(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    int fcond = (instr >> 15) & 0x1F;
    int xd = instr & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    tdlog("%16lx : emulate xvfcmp_cond_s fcond=%d %08x\n", (unsigned long)UC_PC(uc), fcond, instr);

    if (use_lsx_intrinsics) {
        switch (fcond) {
        case 0x00: // CAF - always false
        case 0x01: // SAF - signaling always false
            for (int i = 0; i < 4; i++) {
                vreg_write_64(uc, td, xd, i, 0);
            }
            return;
        case 0x02: // CLT
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_clt_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x03: // SLT
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_slt_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x04: // CEQ
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_ceq_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x05: // SEQ
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_seq_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x06: // CLE
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cle_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x07: // SLE
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sle_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x08: // CUN
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cun_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x09: // SUN
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sun_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x0A: // CULT
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cult_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x0B: // SULT
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sult_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x0C: // CUEQ
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cueq_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x0D: // SUEQ
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sueq_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x0E: // CULE
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cule_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x0F: // SULE
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sule_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x10: // CNE
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cne_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x11: // SNE
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sne_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x12: // COR
        case 0x14: // COR (alternate)
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_cor_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x13: // SOR
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sor_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        case 0x15: // SUNE
        case 0x18: // CUNE (alt)
        case 0x19: // SUNE (alt)
            for (int i = 0; i < 2; i++) {
                __m128 xj_lo = (__m128){vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)};
                __m128 xk_lo = (__m128){vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)};
                __m128i result = __lsx_vfcmp_sune_s(xj_lo, xk_lo);
                uint32_t* r = (uint32_t*)&result;
                vreg_write_32(uc, td, xd, i * 4, r[0]);
                vreg_write_32(uc, td, xd, i * 4 + 1, r[1]);
                vreg_write_32(uc, td, xd, i * 4 + 2, r[2]);
                vreg_write_32(uc, td, xd, i * 4 + 3, r[3]);
            }
            return;
        default: break;
        }
    }

    for (int i = 0; i < 4; i++) {
        uint64_t reg_j = vreg_read_64(uc, td, xj, i);
        uint64_t reg_k = vreg_read_64(uc, td, xk, i);
        uint64_t dst = 0;

        for (int j = 0; j < 2; j++) {
            uint32_t bits_j = (j == 0) ? (uint32_t)(reg_j & 0xffffffff) : (uint32_t)(reg_j >> 32);
            uint32_t bits_k = (j == 0) ? (uint32_t)(reg_k & 0xffffffff) : (uint32_t)(reg_k >> 32);
            union float_bits fb_j = {.u = bits_j};
            float a = fb_j.f;
            union float_bits fb_k = {.u = bits_k};
            float b = fb_k.f;

            int ordered = !FP_IS_NAN(a) && !FP_IS_NAN(b);
            int unordered = FP_IS_NAN(a) || FP_IS_NAN(b);
            int eq = (a == b);
            int lt = (a < b);
            int le = (a <= b);

            int cond = 0;
            switch (fcond) {
            case 0x00: cond = 0; break;                 // CAF - always false
            case 0x01: cond = unordered; break;         // SAF - signaling always false
            case 0x02: cond = ordered && lt; break;     // CLT - compare less than
            case 0x03: cond = unordered || lt; break;   // SLT - signaling less than
            case 0x04: cond = ordered && eq; break;     // CEQ - compare equal
            case 0x05: cond = unordered || eq; break;   // SEQ - signaling equal
            case 0x06: cond = ordered && le; break;     // CLE - compare less or equal
            case 0x07: cond = unordered || le; break;   // SLE - signaling less or equal
            case 0x08: cond = unordered; break;         // CUN - compare unordered
            case 0x09: cond = !unordered; break;        // SUN - signaling ordered
            case 0x0A: cond = unordered || lt; break;   // CULT - compare unordered or less
            case 0x0B: cond = !unordered && !lt; break; // SULT - signaling unordered or less
            case 0x0C: cond = unordered || eq; break;   // CUEQ - compare unordered or equal
            case 0x0D: cond = !unordered && eq; break;  // SUEQ - signaling unordered or equal
            case 0x0E: cond = unordered || le; break;   // CULE - compare unordered or less or equal
            case 0x0F: cond = !unordered && le; break;  // SULE - signaling unordered or less or equal
            case 0x10: cond = ordered && !eq; break;    // CNE - compare not equal
            case 0x11: cond = !ordered; break;          // SNE - signaling not equal
            case 0x12: cond = ordered; break;           // COR - compare ordered
            case 0x13: cond = unordered; break;         // SOR - signaling unordered
            case 0x14: cond = ordered; break;           // COR (alternate) - compare ordered (fix for binutils encoding)
            case 0x15: cond = unordered || !eq; break;  // SUNE - signaling unordered or not equal
            case 0x18: cond = unordered || !eq; break;  // CUNE (alt encoding) - compare unordered or not equal
            case 0x19: cond = unordered || !eq; break;  // SUNE (alt encoding) - signaling unordered or not equal
            default: cond = 0; break;
            }

            if (cond) {
                dst |= (j == 0) ? 0xffffffffULL : (0xffffffffULL << 32);
            }
        }
        vreg_write_64(uc, td, xd, i, dst);
    }
}

// Double precision floating point comparison with condition code
void do_lasx_emu_xvfcmp_cond_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    int fcond = (instr >> 15) & 0x1F;
    int xd = instr & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    tdlog("%16lx : emulate xvfcmp_cond_d fcond=%d %08x\n", (unsigned long)UC_PC(uc), fcond, instr);

    if (use_lsx_intrinsics) {
        switch (fcond) {
        case 0x00: // CAF - always false
        case 0x01: // SAF - signaling always false
            for (int i = 0; i < 4; i++) {
                vreg_write_64(uc, td, xd, i, 0);
            }
            return;
        case 0x02: // CLT
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_clt_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x03: // SLT
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_slt_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x04: // CEQ
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_ceq_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x05: // SEQ
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_seq_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x06: // CLE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cle_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x07: // SLE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sle_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x08: // CUN
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cun_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x09: // SUN
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sun_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x0A: // CULT
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cult_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x0B: // SULT
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sult_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x0C: // CUEQ
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cueq_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x0D: // SUEQ
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sueq_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x0E: // CULE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cule_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x0F: // SULE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sule_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x10: // CNE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cne_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x11: // SNE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sne_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x12: // COR
        case 0x14: // CUNE
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_cor_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x13: // SOR
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sor_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        case 0x15: // SUNE
        case 0x18: // CUNE (alt)
        case 0x19: // SUNE (alt)
            for (int i = 0; i < 2; i++) {
                __m128d xj_lo = (__m128d){vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)};
                __m128d xk_lo = (__m128d){vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)};
                __m128i result = __lsx_vfcmp_sune_d(xj_lo, xk_lo);
                uint64_t* r = (uint64_t*)&result;
                vreg_write_64(uc, td, xd, i * 2, r[0]);
                vreg_write_64(uc, td, xd, i * 2 + 1, r[1]);
            }
            return;
        default: break;
        }
    }

    for (int i = 0; i < 4; i++) {
        uint64_t reg_j = vreg_read_64(uc, td, xj, i);
        uint64_t reg_k = vreg_read_64(uc, td, xk, i);

        union double_bits db_j = {.u = reg_j};
        union double_bits db_k = {.u = reg_k};
        double a = db_j.d;
        double b = db_k.d;

        int ordered = !FP_IS_NAN(a) && !FP_IS_NAN(b);
        int unordered = FP_IS_NAN(a) || FP_IS_NAN(b);
        int eq = (a == b);
        int lt = (a < b);
        int le = (a <= b);

        int cond = 0;
        switch (fcond) {
        case 0x00: cond = 0; break;                 // CAF - always false
        case 0x01: cond = unordered; break;         // SAF - signaling always false
        case 0x02: cond = ordered && lt; break;     // CLT - compare less than
        case 0x03: cond = unordered || lt; break;   // SLT - signaling less than
        case 0x04: cond = ordered && eq; break;     // CEQ - compare equal
        case 0x05: cond = unordered || eq; break;   // SEQ - signaling equal
        case 0x06: cond = ordered && le; break;     // CLE - compare less or equal
        case 0x07: cond = unordered || le; break;   // SLE - signaling less or equal
        case 0x08: cond = unordered; break;         // CUN - compare unordered
        case 0x09: cond = !unordered; break;        // SUN - signaling ordered
        case 0x0A: cond = unordered || lt; break;   // CULT - compare unordered or less
        case 0x0B: cond = !unordered && !lt; break; // SULT - signaling unordered or less
        case 0x0C: cond = unordered || eq; break;   // CUEQ - compare unordered or equal
        case 0x0D: cond = !unordered && eq; break;  // SUEQ - signaling unordered or equal
        case 0x0E: cond = unordered || le; break;   // CULE - compare unordered or less or equal
        case 0x0F: cond = !unordered && le; break;  // SULE - signaling unordered or less or equal
        case 0x10: cond = ordered && !eq; break;    // CNE - compare not equal
        case 0x11: cond = !ordered; break;          // SNE - signaling not equal
        case 0x12: cond = ordered; break;           // COR - compare ordered
        case 0x13: cond = unordered; break;         // SOR - signaling unordered
        case 0x14: cond = unordered || !eq; break;  // CUNE - compare unordered or not equal
        case 0x15: cond = unordered || !eq; break;  // SUNE - signaling unordered or not equal
        case 0x18: cond = unordered || !eq; break;  // CUNE (alt encoding) - compare unordered or not equal
        case 0x19: cond = unordered || !eq; break;  // SUNE (alt encoding) - signaling unordered or not equal
        default: cond = 0; break;
        }

        uint64_t dst = cond ? 0xFFFFFFFFFFFFFFFFULL : 0ULL;
        vreg_write_64(uc, td, xd, i, dst);
    }
}
