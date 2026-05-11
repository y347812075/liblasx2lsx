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

#include "debug.h"
#include "lasx_emu_private.h"
#include "lasx_shift.h"
#include "thread_data.h"

// ==================== xvsrln.b.h ====================
// B[0-7] from xj.H[0-7], B[8-15] zero, B[16-23] from xj.H[8-15], B[24-31] zero
// xk.H[i] contains the shift amount (low 4 bits)
void do_lasx_emu_xvsrln_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrln.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, srln_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, srln_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrln.h.w ====================
// H[0-7] from xj.W[0-7], H[8-15] from xj.W[4-7]
// xk.W[i] contains the shift amount (low 5 bits)
void do_lasx_emu_xvsrln_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrln.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, srln_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, srln_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrln.w.d ====================
// W[0-3] from xj.D[0-1], W[4-7] from xj.D[2-3]
// xk.D[i] contains the shift amount (low 6 bits)
void do_lasx_emu_xvsrln_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrln.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, srln_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, srln_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsran.b.h ====================
void do_lasx_emu_xvsran_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsran.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, sran_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, sran_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsran.h.w ====================
void do_lasx_emu_xvsran_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsran.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, sran_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, sran_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsran.w.d ====================
void do_lasx_emu_xvsran_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsran.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, sran_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, sran_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrlrn.b.h ====================
void do_lasx_emu_xvsrlrn_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrn.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, srlrn_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, srlrn_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrlrn.h.w ====================
void do_lasx_emu_xvsrlrn_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrn.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, srlrn_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, srlrn_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrlrn.w.d ====================
void do_lasx_emu_xvsrlrn_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrn.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, srlrn_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, srlrn_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrarn.b.h ====================
void do_lasx_emu_xvsrarn_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarn.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, srarn_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, srarn_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrarn.h.w ====================
void do_lasx_emu_xvsrarn_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarn.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, srarn_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, srarn_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvsrarn.w.d ====================
void do_lasx_emu_xvsrarn_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarn.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, srarn_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, srarn_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrln.b.h ====================
void do_lasx_emu_xvssrln_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrln.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlns_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrlns_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrln.h.w ====================
void do_lasx_emu_xvssrln_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrln.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlns_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrlns_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrln.w.d ====================
void do_lasx_emu_xvssrln_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrln.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlns_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrlns_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssran.b.h ====================
void do_lasx_emu_xvssran_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssran.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrans_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrans_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssran.h.w ====================
void do_lasx_emu_xvssran_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssran.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrans_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrans_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssran.w.d ====================
void do_lasx_emu_xvssran_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssran.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrans_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrans_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrlrn.b.h ====================
void do_lasx_emu_xvssrlrn_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrn.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlrns_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrlrns_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrlrn.h.w ====================
void do_lasx_emu_xvssrlrn_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrn.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlrns_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrlrns_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrlrn.w.d ====================
void do_lasx_emu_xvssrlrn_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrn.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlrns_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrlrns_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrarn.b.h ====================
void do_lasx_emu_xvssrarn_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarn.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrarns_b_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrarns_b_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrarn.h.w ====================
void do_lasx_emu_xvssrarn_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarn.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrarns_h_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrarns_h_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrarn.w.d ====================
void do_lasx_emu_xvssrarn_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarn.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrarns_w_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrarns_w_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrln_bu_h ====================
void do_lasx_emu_xvssrln_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrln_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlnu_bu_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrlnu_bu_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrln_hu_w ====================
void do_lasx_emu_xvssrln_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrln_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlnu_hu_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrlnu_hu_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrln_wu_d ====================
void do_lasx_emu_xvssrln_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrln_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlnu_wu_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrlnu_wu_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssran_bu_h ====================
void do_lasx_emu_xvssran_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssran_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssranu_bu_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssranu_bu_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssran_hu_w ====================
void do_lasx_emu_xvssran_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssran_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssranu_hu_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssranu_hu_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssran_wu_d ====================
void do_lasx_emu_xvssran_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssran_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssranu_wu_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssranu_wu_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrlrn_bu_h ====================
void do_lasx_emu_xvssrlrn_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrn_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlrnu_bu_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrlrnu_bu_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrlrn_hu_w ====================
void do_lasx_emu_xvssrlrn_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrn_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlrnu_hu_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrlrnu_hu_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrlrn_wu_d ====================
void do_lasx_emu_xvssrlrn_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrn_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlrnu_wu_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrlrnu_wu_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrarn_bu_h ====================
void do_lasx_emu_xvssrarn_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarn_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    uint16_t xk_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xk_h[i] = vreg_read_u16(uc, td, xk, i) & 0xf;
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrarnu_bu_h(xj_h[i], xk_h[i]));
        vreg_write_8(uc, td, xd, i + 16, ssrarnu_bu_h(xj_h[i + 8], xk_h[i + 8]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrarn_hu_w ====================
void do_lasx_emu_xvssrarn_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarn_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    uint32_t xk_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i) & 0x1f;
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrarnu_hu_w(xj_w[i], xk_w[i]));
        vreg_write_16(uc, td, xd, i + 8, ssrarnu_hu_w(xj_w[i + 4], xk_w[i + 4]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}

// ==================== xvssrarn_wu_d ====================
void do_lasx_emu_xvssrarn_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarn_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int64_t xj_d[4];
    uint64_t xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i) & 0x3f;
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrarnu_wu_d(xj_d[i], xk_d[i]));
        vreg_write_32(uc, td, xd, i + 4, ssrarnu_wu_d(xj_d[i + 2], xk_d[i + 2]));
    }
    vreg_write_64(uc, td, xd, 1, 0);
    vreg_write_64(uc, td, xd, 3, 0);
}
