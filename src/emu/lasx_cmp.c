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

#include "lasx_cmp.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== xvmax.b ====================
void do_lasx_emu_xvmax_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c = a > b ? a : b;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmax.h ====================
void do_lasx_emu_xvmax_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c = a > b ? a : b;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmax.w ====================
void do_lasx_emu_xvmax_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c = a > b ? a : b;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmax.d ====================
void do_lasx_emu_xvmax_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    int64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
        xk_d[i] = (int64_t)vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t c = xj_d[i] > xk_d[i] ? xj_d[i] : xk_d[i];
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvmin.b ====================
void do_lasx_emu_xvmin_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int8_t c = a < b ? a : b;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmin.h ====================
void do_lasx_emu_xvmin_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int16_t c = a < b ? a : b;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmin.w ====================
void do_lasx_emu_xvmin_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int32_t c = a < b ? a : b;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmin.d ====================
void do_lasx_emu_xvmin_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    int64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
        xk_d[i] = (int64_t)vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t c = xj_d[i] < xk_d[i] ? xj_d[i] : xk_d[i];
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}
// ==================== xvmax.bu ====================
void do_lasx_emu_xvmax_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint8_t c = a > b ? a : b;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmax.hu ====================
void do_lasx_emu_xvmax_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint16_t c = a > b ? a : b;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmax.wu ====================
void do_lasx_emu_xvmax_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t b = (uint32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c = a > b ? a : b;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmax.du ====================
void do_lasx_emu_xvmax_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmax.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t c = xj_d[i] > xk_d[i] ? xj_d[i] : xk_d[i];
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvmin.bu ====================
void do_lasx_emu_xvmin_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
            uint8_t c = a < b ? a : b;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmin.hu ====================
void do_lasx_emu_xvmin_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint16_t c = a < b ? a : b;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmin.wu ====================
void do_lasx_emu_xvmin_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t b = (uint32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c = a < b ? a : b;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmin.du ====================
void do_lasx_emu_xvmin_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmin.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t c = xj_d[i] < xk_d[i] ? xj_d[i] : xk_d[i];
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvclo.b ====================
void do_lasx_emu_xvclo_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclo.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t cnt = 0;
            for (int bit = 7; bit >= 0 && (val & (1 << bit)); bit--) {
                cnt++;
            }
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvclo.h ====================
void do_lasx_emu_xvclo_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclo.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint8_t cnt = 0;
            for (int bit = 15; bit >= 0 && (val & (1 << bit)); bit--) {
                cnt++;
            }
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvclo.w ====================
void do_lasx_emu_xvclo_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclo.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint8_t cnt = 0;
            for (int bit = 31; bit >= 0 && (val & (1U << bit)); bit--) {
                cnt++;
            }
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvclo.d ====================
void do_lasx_emu_xvclo_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclo.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t val = xj_d[i];
        uint8_t cnt = 0;
        for (int bit = 63; bit >= 0 && (val & (1ULL << bit)); bit--) {
            cnt++;
        }
        vreg_write_64(uc, td, xd, i, (uint64_t)cnt);
    }
}

// ==================== xvclz.b ====================
void do_lasx_emu_xvclz_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclz.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t cnt = 0;
            for (int bit = 7; bit >= 0 && !(val & (1 << bit)); bit--) {
                cnt++;
            }
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvclz.h ====================
void do_lasx_emu_xvclz_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclz.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint8_t cnt = 0;
            for (int bit = 15; bit >= 0 && !(val & (1 << bit)); bit--) {
                cnt++;
            }
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvclz.w ====================
void do_lasx_emu_xvclz_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclz.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint8_t cnt = 0;
            for (int bit = 31; bit >= 0 && !(val & (1U << bit)); bit--) {
                cnt++;
            }
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvclz.d ====================
void do_lasx_emu_xvclz_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvclz.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t val = xj_d[i];
        uint8_t cnt = 0;
        for (int bit = 63; bit >= 0 && !(val & (1ULL << bit)); bit--) {
            cnt++;
        }
        vreg_write_64(uc, td, xd, i, (uint64_t)cnt);
    }
}

// ==================== xvpcnt.b ====================
void do_lasx_emu_xvpcnt_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpcnt.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t cnt = __builtin_popcount(val);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvpcnt.h ====================
void do_lasx_emu_xvpcnt_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpcnt.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint8_t cnt = __builtin_popcount(val);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvpcnt.w ====================
void do_lasx_emu_xvpcnt_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpcnt.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint8_t cnt = __builtin_popcount(val);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)cnt << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvpcnt.d ====================
void do_lasx_emu_xvpcnt_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpcnt.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint8_t cnt = __builtin_popcountll(xj_d[i]);
        vreg_write_64(uc, td, xd, i, (uint64_t)cnt);
    }

}

// ==================== xvneg.b ====================
void do_lasx_emu_xvneg_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvneg.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            int8_t res = -val;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvneg.h ====================
void do_lasx_emu_xvneg_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvneg.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            int16_t res = -val;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvneg.w ====================
void do_lasx_emu_xvneg_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvneg.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t res = -val;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvneg.d ====================
void do_lasx_emu_xvneg_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvneg.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        int64_t res = -xj_d[i];
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// ==================== xvmskltz.b ====================
void do_lasx_emu_xvmskltz_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmskltz.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src0 = vreg_read_64(uc, td, xj, 0);
    uint64_t src1 = vreg_read_64(uc, td, xj, 1);
    uint64_t src2 = vreg_read_64(uc, td, xj, 2);
    uint64_t src3 = vreg_read_64(uc, td, xj, 3);

    uint64_t dst0 = 0;
    uint64_t dst2 = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t val0 = (src0 >> (i * 8)) & 0xff;
        int8_t s0 = (int8_t)val0;
        if (s0 < 0)
            dst0 |= (1ULL << i);

        uint8_t val1 = (src1 >> (i * 8)) & 0xff;
        int8_t s1 = (int8_t)val1;
        if (s1 < 0)
            dst0 |= (1ULL << (i + 8));

        uint8_t val2 = (src2 >> (i * 8)) & 0xff;
        int8_t s2 = (int8_t)val2;
        if (s2 < 0)
            dst2 |= (1ULL << i);

        uint8_t val3 = (src3 >> (i * 8)) & 0xff;
        int8_t s3 = (int8_t)val3;
        if (s3 < 0)
            dst2 |= (1ULL << (i + 8));
    }

    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvmskltz.h ====================
void do_lasx_emu_xvmskltz_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmskltz.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src0 = vreg_read_64(uc, td, xj, 0);
    uint64_t src1 = vreg_read_64(uc, td, xj, 1);
    uint64_t src2 = vreg_read_64(uc, td, xj, 2);
    uint64_t src3 = vreg_read_64(uc, td, xj, 3);

    uint64_t dst0 = 0;
    uint64_t dst2 = 0;

    for (int i = 0; i < 4; i++) {
        uint16_t val0 = (src0 >> (i * 16)) & 0xffff;
        int16_t s0 = (int16_t)val0;
        if (s0 < 0)
            dst0 |= (1ULL << i);

        uint16_t val1 = (src1 >> (i * 16)) & 0xffff;
        int16_t s1 = (int16_t)val1;
        if (s1 < 0)
            dst0 |= (1ULL << (i + 4));

        uint16_t val2 = (src2 >> (i * 16)) & 0xffff;
        int16_t s2 = (int16_t)val2;
        if (s2 < 0)
            dst2 |= (1ULL << i);

        uint16_t val3 = (src3 >> (i * 16)) & 0xffff;
        int16_t s3 = (int16_t)val3;
        if (s3 < 0)
            dst2 |= (1ULL << (i + 4));
    }

    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvmskltz.w ====================
void do_lasx_emu_xvmskltz_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmskltz.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src0 = vreg_read_64(uc, td, xj, 0);
    uint64_t src1 = vreg_read_64(uc, td, xj, 1);
    uint64_t src2 = vreg_read_64(uc, td, xj, 2);
    uint64_t src3 = vreg_read_64(uc, td, xj, 3);

    uint64_t dst0 = 0;
    uint64_t dst2 = 0;

    for (int i = 0; i < 2; i++) {
        uint32_t val0 = (src0 >> (i * 32)) & 0xffffffff;
        int32_t s0 = (int32_t)val0;
        if (s0 < 0)
            dst0 |= (1ULL << i);

        uint32_t val1 = (src1 >> (i * 32)) & 0xffffffff;
        int32_t s1 = (int32_t)val1;
        if (s1 < 0)
            dst0 |= (1ULL << (i + 2));

        uint32_t val2 = (src2 >> (i * 32)) & 0xffffffff;
        int32_t s2 = (int32_t)val2;
        if (s2 < 0)
            dst2 |= (1ULL << i);

        uint32_t val3 = (src3 >> (i * 32)) & 0xffffffff;
        int32_t s3 = (int32_t)val3;
        if (s3 < 0)
            dst2 |= (1ULL << (i + 2));
    }

    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvmskltz.d ====================
void do_lasx_emu_xvmskltz_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmskltz.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src0 = vreg_read_64(uc, td, xj, 0);
    uint64_t src1 = vreg_read_64(uc, td, xj, 1);
    uint64_t src2 = vreg_read_64(uc, td, xj, 2);
    uint64_t src3 = vreg_read_64(uc, td, xj, 3);

    uint64_t dst0 = 0;
    uint64_t dst2 = 0;

    int64_t s0 = (int64_t)src0;
    int64_t s1 = (int64_t)src1;
    int64_t s2 = (int64_t)src2;
    int64_t s3 = (int64_t)src3;

    if (s0 < 0)
        dst0 |= 1ULL;
    if (s1 < 0)
        dst0 |= (1ULL << 1);
    if (s2 < 0)
        dst2 |= 1ULL;
    if (s3 < 0)
        dst2 |= (1ULL << 1);

    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvmskgez.b ====================
void do_lasx_emu_xvmskgez_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmskgez.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src0 = vreg_read_64(uc, td, xj, 0);
    uint64_t src1 = vreg_read_64(uc, td, xj, 1);
    uint64_t src2 = vreg_read_64(uc, td, xj, 2);
    uint64_t src3 = vreg_read_64(uc, td, xj, 3);

    uint64_t dst0 = 0;
    uint64_t dst2 = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t val0 = (src0 >> (i * 8)) & 0xff;
        int8_t s0 = (int8_t)val0;
        if (s0 >= 0)
            dst0 |= (1ULL << i);

        uint8_t val1 = (src1 >> (i * 8)) & 0xff;
        int8_t s1 = (int8_t)val1;
        if (s1 >= 0)
            dst0 |= (1ULL << (i + 8));

        uint8_t val2 = (src2 >> (i * 8)) & 0xff;
        int8_t s2 = (int8_t)val2;
        if (s2 >= 0)
            dst2 |= (1ULL << i);

        uint8_t val3 = (src3 >> (i * 8)) & 0xff;
        int8_t s3 = (int8_t)val3;
        if (s3 >= 0)
            dst2 |= (1ULL << (i + 8));
    }

    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvmsknz.b ====================
void do_lasx_emu_xvmsknz_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmsknz.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src0 = vreg_read_64(uc, td, xj, 0);
    uint64_t src1 = vreg_read_64(uc, td, xj, 1);
    uint64_t src2 = vreg_read_64(uc, td, xj, 2);
    uint64_t src3 = vreg_read_64(uc, td, xj, 3);

    uint64_t dst0 = 0;
    uint64_t dst2 = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t val0 = (src0 >> (i * 8)) & 0xff;
        if (val0 != 0)
            dst0 |= (1ULL << i);

        uint8_t val1 = (src1 >> (i * 8)) & 0xff;
        if (val1 != 0)
            dst0 |= (1ULL << (i + 8));

        uint8_t val2 = (src2 >> (i * 8)) & 0xff;
        if (val2 != 0)
            dst2 |= (1ULL << i);

        uint8_t val3 = (src3 >> (i * 8)) & 0xff;
        if (val3 != 0)
            dst2 |= (1ULL << (i + 8));
    }

    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvseteqz.v ====================
void do_lasx_emu_xvseteqz_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseteqz.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int all_zero = 1;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        if (val != 0) {
            all_zero = 0;
            break;
        }
    }

    if (all_zero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetnez.v ====================
void do_lasx_emu_xvsetnez_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetnez.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int has_nonzero = 0;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        if (val != 0) {
            has_nonzero = 1;
            break;
        }
    }

    if (has_nonzero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetanyeqz.b ====================
void do_lasx_emu_xvsetanyeqz_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetanyeqz.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int has_zero = 0;

    for (int i = 0; i < 32; i++) {
        int byte_idx = i / 8;
        int shift = (i % 8) * 8;
        uint64_t src_j = vreg_read_64(uc, td, xj, byte_idx);
        uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
        if (val == 0) {
            has_zero = 1;
            break;
        }
    }

    if (has_zero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetanyeqz.h ====================
void do_lasx_emu_xvsetanyeqz_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetanyeqz.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int has_zero = 0;

    for (int i = 0; i < 16; i++) {
        int word_idx = i / 4;
        int shift = (i % 4) * 16;
        uint64_t src_j = vreg_read_64(uc, td, xj, word_idx);
        uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
        if (val == 0) {
            has_zero = 1;
            break;
        }
    }

    if (has_zero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetanyeqz.w ====================
void do_lasx_emu_xvsetanyeqz_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetanyeqz.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int has_zero = 0;

    for (int i = 0; i < 8; i++) {
        int dword_idx = i / 2;
        int shift = (i % 2) * 32;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
        if (val == 0) {
            has_zero = 1;
            break;
        }
    }

    if (has_zero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetanyeqz.d ====================
void do_lasx_emu_xvsetanyeqz_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetanyeqz.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int has_zero = 0;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        if (val == 0) {
            has_zero = 1;
            break;
        }
    }

    if (has_zero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetallnez.b ====================
void do_lasx_emu_xvsetallnez_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetallnez.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int all_nonzero = 1;

    for (int i = 0; i < 32; i++) {
        int byte_idx = i / 8;
        int shift = (i % 8) * 8;
        uint64_t src_j = vreg_read_64(uc, td, xj, byte_idx);
        uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
        if (val == 0) {
            all_nonzero = 0;
            break;
        }
    }

    if (all_nonzero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetallnez.h ====================
void do_lasx_emu_xvsetallnez_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetallnez.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int all_nonzero = 1;

    for (int i = 0; i < 16; i++) {
        int word_idx = i / 4;
        int shift = (i % 4) * 16;
        uint64_t src_j = vreg_read_64(uc, td, xj, word_idx);
        uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
        if (val == 0) {
            all_nonzero = 0;
            break;
        }
    }

    if (all_nonzero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetallnez.w ====================
void do_lasx_emu_xvsetallnez_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetallnez.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int all_nonzero = 1;

    for (int i = 0; i < 8; i++) {
        int dword_idx = i / 2;
        int shift = (i % 2) * 32;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
        if (val == 0) {
            all_nonzero = 0;
            break;
        }
    }

    if (all_nonzero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvsetallnez.d ====================
void do_lasx_emu_xvsetallnez_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsetallnez.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int cd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int all_nonzero = 1;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        if (val == 0) {
            all_nonzero = 0;
            break;
        }
    }

    if (all_nonzero) {
        UC_SET_FCC(uc, cd, 1);
    } else {
        UC_SET_FCC(uc, cd, 0);
    }
}

// ==================== xvseqi.b ====================
void do_lasx_emu_xvseqi_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseqi.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            uint8_t res = val == simm ? 0xff : 0x00;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvseqi.h ====================
void do_lasx_emu_xvseqi_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseqi.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            uint16_t res = val == simm ? 0xffff : 0x0000;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvseqi.w ====================
void do_lasx_emu_xvseqi_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseqi.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            uint32_t res = val == simm ? 0xffffffff : 0x00000000;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvseqi.d ====================
void do_lasx_emu_xvseqi_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseqi.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t res = xj_d[i] == simm ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvslei.b ====================
void do_lasx_emu_xvslei_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            uint8_t res = val <= simm ? 0xff : 0x00;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslei.h ====================
void do_lasx_emu_xvslei_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            uint16_t res = val <= simm ? 0xffff : 0x0000;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslei.w ====================
void do_lasx_emu_xvslei_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            uint32_t res = val <= simm ? 0xffffffff : 0x00000000;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslei.d ====================
void do_lasx_emu_xvslei_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t res = xj_d[i] <= simm ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvslti.b ====================
void do_lasx_emu_xvslti_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            uint8_t res = val < simm ? 0xff : 0x00;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslti.h ====================
void do_lasx_emu_xvslti_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            uint16_t res = val < simm ? 0xffff : 0x0000;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslti.w ====================
void do_lasx_emu_xvslti_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            uint32_t res = val < simm ? 0xffffffff : 0x00000000;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslti.d ====================
void do_lasx_emu_xvslti_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t res = xj_d[i] < simm ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvslei.bu ====================
void do_lasx_emu_xvslei_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t res = val <= imm ? 0xff : 0x00;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslei.hu ====================
void do_lasx_emu_xvslei_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t res = val <= imm ? 0xffff : 0x0000;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslei.wu ====================
void do_lasx_emu_xvslei_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t res = val <= imm ? 0xffffffff : 0x00000000;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslei.du ====================
void do_lasx_emu_xvslei_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslei.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t res = xj_d[i] <= (uint64_t)imm ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvslti.bu ====================
void do_lasx_emu_xvslti_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t res = val < imm ? 0xff : 0x00;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslti.hu ====================
void do_lasx_emu_xvslti_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t res = val < imm ? 0xffff : 0x0000;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslti.wu ====================
void do_lasx_emu_xvslti_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t res = val < imm ? 0xffffffff : 0x00000000;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvslti.du ====================
void do_lasx_emu_xvslti_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslti.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t res = xj_d[i] < (uint64_t)imm ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvmaxi.b ====================
void do_lasx_emu_xvmaxi_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            int8_t c = val > simm ? val : simm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmaxi.h ====================
void do_lasx_emu_xvmaxi_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            int16_t c = val > simm ? val : simm;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmaxi.w ====================
void do_lasx_emu_xvmaxi_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t c = val > simm ? val : simm;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmaxi.d ====================
void do_lasx_emu_xvmaxi_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        int64_t c = xj_d[i] > simm ? xj_d[i] : simm;
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvmini.b ====================
void do_lasx_emu_xvmini_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            int8_t c = val < simm ? val : simm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmini.h ====================
void do_lasx_emu_xvmini_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            int16_t c = val < simm ? val : simm;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmini.w ====================
void do_lasx_emu_xvmini_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t c = val < simm ? val : simm;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmini.d ====================
void do_lasx_emu_xvmini_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int imm = (instr >> 10) & 0x1f;
    int simm = (imm & 0x10) ? (imm | ~0x1f) : imm;

    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = (int64_t)vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        int64_t c = xj_d[i] < simm ? xj_d[i] : simm;
        vreg_write_64(uc, td, xd, i, (uint64_t)c);
    }
}

// ==================== xvmaxi.bu ====================
void do_lasx_emu_xvmaxi_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t c = val > imm ? val : imm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmaxi.hu ====================
void do_lasx_emu_xvmaxi_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t c = val > imm ? val : imm;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmaxi.wu ====================
void do_lasx_emu_xvmaxi_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t c = val > imm ? val : imm;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmaxi.du ====================
void do_lasx_emu_xvmaxi_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmaxi.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t c = xj_d[i] > (uint64_t)imm ? xj_d[i] : (uint64_t)imm;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvmini.bu ====================
void do_lasx_emu_xvmini_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t c = val < imm ? val : imm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmini.hu ====================
void do_lasx_emu_xvmini_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t c = val < imm ? val : imm;
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmini.wu ====================
void do_lasx_emu_xvmini_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t c = val < imm ? val : imm;
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)c << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvmini.du ====================
void do_lasx_emu_xvmini_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvmini.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        uint64_t c = xj_d[i] < (uint64_t)imm ? xj_d[i] : (uint64_t)imm;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvfrstpi.b ====================
// m = ui5%16
// XR[xd].B[m] = count of non-negative bytes in xj.B[0:15]
// XR[xd].B[m] = count of non-negative bytes in xj.B[16:31]
void do_lasx_emu_xvfrstpi_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrstpi.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int m = (instr >> 10) & 0x1f;

    int8_t xj_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }

    int idx0 = 0;
    for (int i = 0; i < 16; i++) {
        if (xj_b[i] < 0)
            break;
        idx0++;
    }
    int idx1 = 0;
    for (int i = 16; i < 32; i++) {
        if (xj_b[i] < 0)
            break;
        idx1++;
    }

    vreg_write_8(uc, td, xd, m & 0xf, idx0);
    vreg_write_8(uc, td, xd, (m & 0xf) + 16, idx1);
}

// ==================== xvfrstpi.h ====================
// m = ui5%8
// XR[xd].H[m] = count of non-negative halfwords in xj.H[0:7]
// XR[xd].H[m] = count of non-negative halfwords in xj.H[8:15]
void do_lasx_emu_xvfrstpi_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrstpi.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int m = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }

    int idx0 = 0;
    for (int i = 0; i < 8; i++) {
        if (xj_h[i] < 0)
            break;
        idx0++;
    }
    int idx1 = 0;
    for (int i = 8; i < 16; i++) {
        if (xj_h[i] < 0)
            break;
        idx1++;
    }

    vreg_write_16(uc, td, xd, m & 0x7, idx0);
    vreg_write_16(uc, td, xd, (m & 0x7) + 8, idx1);
}
