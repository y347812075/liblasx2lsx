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

// ==================== xvsrlni.b.h ====================
// for i in range(8):
//   XR[xd].B[i] = SRLN(XR[xj].H[i], ui4)       # B[0-7] from xj.H[0-7]
//   XR[xd].B[i+8] = SRLN(XR[xd].H[i], ui4)     # B[8-15] from xd.H[0-7] (OLD!)
//   XR[xd].B[i+16] = SRLN(XR[xj].H[i+8], ui4)  # B[16-23] from xj.H[8-15]
//   XR[xd].B[i+24] = SRLN(XR[xd].H[i+8], ui4)  # B[24-31] from xd.H[8-15] (OLD!)
void do_lasx_emu_xvsrlni_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlni.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    uint16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, srln_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, srln_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, srln_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, srln_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvsrlni.h.w ====================
// for i in range(4):
//   XR[xd].H[i] = SRLN(XR[xj].W[i], ui5)       # Lower half: from xj
//   XR[xd].H[i+4] = SRLN(XR[xd].W[i], ui5)     # Upper half: from xd OLD!
//   XR[xd].H[i+8] = SRLN(XR[xj].W[i+4], ui5)   # Lower half: from xj
//   XR[xd].H[i+12] = SRLN(XR[xd].W[i+4], ui5)  # Upper half: from xd OLD!
void do_lasx_emu_xvsrlni_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlni.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, srln_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, srln_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, srln_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, srln_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvsrlni.w.d ====================
// for i in range(2):
//   XR[xd].W[i] = SRLN(XR[xj].D[i], ui6)       # Lower half: from xj
//   XR[xd].W[i+2] = SRLN(XR[xd].D[i], ui6)     # Upper half: from xd OLD!
//   XR[xd].W[i+4] = SRLN(XR[xj].D[i+2], ui6)   # Lower half: from xj
//   XR[xd].W[i+6] = SRLN(XR[xd].D[i+2], ui6)   # Upper half: from xd OLD!
void do_lasx_emu_xvsrlni_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlni.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    uint64_t xj_d[4];
    uint64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, srln_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, srln_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, srln_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, srln_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvsrlrni.b.h ====================
// for i in range(8):
//   XR[xd].B[i] = SRLRN(XR[xj].H[i], ui4)      # Lower half: from xj
//   XR[xd].B[i+8] = SRLRN(XR[xd].H[i], ui4)    # Upper half: from xd OLD!
//   XR[xd].B[i+16] = SRLRN(XR[xj].H[i+8], ui4) # Lower half: from xj
//   XR[xd].B[i+24] = SRLRN(XR[xd].H[i+8], ui4) # Upper half: from xd OLD!
void do_lasx_emu_xvsrlrni_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrni.b.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    uint16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, srlrn_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, srlrn_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, srlrn_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, srlrn_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvsrlrni.h.w ====================
// for i in range(4):
//   XR[xd].H[i] = SRLRN(XR[xj].W[i], ui5)      # Lower half: from xj
//   XR[xd].H[i+4] = SRLRN(XR[xd].W[i], ui5)    # Upper half: from xd OLD!
//   XR[xd].H[i+8] = SRLRN(XR[xj].W[i+4], ui5)  # Lower half: from xj
//   XR[xd].H[i+12] = SRLRN(XR[xd].W[i+4], ui5) # Upper half: from xd OLD!
void do_lasx_emu_xvsrlrni_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrni.h.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, srlrn_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, srlrn_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, srlrn_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, srlrn_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvsrlrni.w.d ====================
// for i in range(2):
//   XR[xd].W[i] = SRLRN(XR[xj].D[i], ui6)      # Lower half: from xj
//   XR[xd].W[i+2] = SRLRN(XR[xd].D[i], ui6)    # Upper half: from xd OLD!
//   XR[xd].W[i+4] = SRLRN(XR[xj].D[i+2], ui6)  # Lower half: from xj
//   XR[xd].W[i+6] = SRLRN(XR[xd].D[i+2], ui6)  # Upper half: from xd OLD!
void do_lasx_emu_xvsrlrni_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrni.w.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    uint64_t xj_d[4];
    uint64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, srlrn_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, srlrn_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, srlrn_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, srlrn_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrlni_b_h ====================
// Signed saturated + logic: B <- H
// For i in range(8):
//   XR[xd].B[i] = SSRLNS(XR[xj].H[i], ui4)
//   XR[xd].B[i+8] = SSRLNS(XR[xd].H[i], ui4)
//   XR[xd].B[i+16] = SSRLNS(XR[xj].H[i+8], ui4)
//   XR[xd].B[i+24] = SSRLNS(XR[xd].H[i+8], ui4)
void do_lasx_emu_xvssrlni_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_b_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    int16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlns_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrlns_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrlns_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrlns_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrlni_h_w ====================
// Signed saturated + logic: H <- W
// For i in range(4):
//   XR[xd].H[i] = SSRLNS(XR[xj].W[i], ui5)
//   XR[xd].H[i+4] = SSRLNS(XR[xd].W[i], ui5)
//   XR[xd].H[i+8] = SSRLNS(XR[xj].W[i+4], ui5)
//   XR[xd].H[i+12] = SSRLNS(XR[xd].W[i+4], ui5)
void do_lasx_emu_xvssrlni_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_h_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    int32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlns_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrlns_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrlns_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrlns_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrlni_w_d ====================
// Signed saturated + logic: W <- D
// For i in range(2):
//   XR[xd].W[i] = SSRLNS(XR[xj].D[i], ui6)
//   XR[xd].W[i+2] = SSRLNS(XR[xd].D[i], ui6)
//   XR[xd].W[i+4] = SSRLNS(XR[xj].D[i+2], ui6)
//   XR[xd].W[i+6] = SSRLNS(XR[xd].D[i+2], ui6)
void do_lasx_emu_xvssrlni_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_w_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    int64_t xj_d[4];
    int64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlns_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrlns_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrlns_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrlns_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrlrni.b.h ====================
// for i in range(8):
//   XR[xd].B[i] = SSRLNS(XR[xj].H[i], ui4) + rounding
//   XR[xd].B[i+8] = SSRLNS(XR[xd].H[i], ui4) + rounding (from xd OLD!)
//   XR[xd].B[i+16] = SSRLNS(XR[xj].H[i+8], ui4) + rounding
//   XR[xd].B[i+24] = SSRLNS(XR[xd].H[i+8], ui4) + rounding (from xd OLD!)
void do_lasx_emu_xvssrlrni_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_b_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    int16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlrns_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrlrns_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrlrns_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrlrns_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrlrni_bu_h ====================
void do_lasx_emu_xvssrlrni_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    uint16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlrnu_bu_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrlrnu_bu_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrlrnu_bu_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrlrnu_bu_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrlrni_hu_w ====================
// Unsigned version
void do_lasx_emu_xvssrlrni_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlrnu_hu_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrlrnu_hu_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrlrnu_hu_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrlrnu_hu_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrlrni_h_w ====================
void do_lasx_emu_xvssrlrni_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_h_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    int32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlrns_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrlrns_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrlrns_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrlrns_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrlrni_wu_d ====================
// Unsigned version
void do_lasx_emu_xvssrlrni_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    uint64_t xj_d[4];
    uint64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlrnu_wu_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrlrnu_wu_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrlrnu_wu_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrlrnu_wu_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrlrni_w_d ====================
void do_lasx_emu_xvssrlrni_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_w_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    int64_t xj_d[4];
    int64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlrns_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrlrns_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrlrns_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrlrns_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvsrani_b_h ====================
// Arithmetic: B <- H
// For i in range(8):
//   XR[xd].B[i] = SRAN(XR[xj].H[i], ui4)
//   XR[xd].B[i+8] = SRAN(XR[xd].H[i], ui4)
//   XR[xd].B[i+16] = SRAN(XR[xj].H[i+8], ui4)
//   XR[xd].B[i+24] = SRAN(XR[xd].H[i+8], ui4)
void do_lasx_emu_xvsrani_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrani_b_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    int16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, sran_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, sran_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, sran_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, sran_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvsrani_h_w ====================
// Arithmetic: H <- W
// For i in range(4):
//   XR[xd].H[i] = SRAN(XR[xj].W[i], ui5)
//   XR[xd].H[i+4] = SRAN(XR[xd].W[i], ui5)
//   XR[xd].H[i+8] = SRAN(XR[xj].W[i+4], ui5)
//   XR[xd].H[i+12] = SRAN(XR[xd].W[i+4], ui5)
void do_lasx_emu_xvsrani_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrani_h_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    int32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, sran_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, sran_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, sran_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, sran_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvsrani_w_d ====================
// Arithmetic: W <- D
// For i in range(2):
//   XR[xd].W[i] = SRAN(XR[xj].D[i], ui6)
//   XR[xd].W[i+2] = SRAN(XR[xd].D[i], ui6)
//   XR[xd].W[i+4] = SRAN(XR[xj].D[i+2], ui6)
//   XR[xd].W[i+6] = SRAN(XR[xd].D[i+2], ui6)
void do_lasx_emu_xvsrani_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrani_w_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    int64_t xj_d[4];
    int64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, sran_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, sran_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, sran_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, sran_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvsrarni_b_h ====================
// Arithmetic + rounding: B <- H
// For i in range(8):
//   XR[xd].B[i] = SRANR(XR[xj].H[i], ui4)
//   XR[xd].B[i+8] = SRANR(XR[xd].H[i], ui4)
//   XR[xd].B[i+16] = SRANR(XR[xj].H[i+8], ui4)
//   XR[xd].B[i+24] = SRANR(XR[xd].H[i+8], ui4)
void do_lasx_emu_xvsrarni_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarni_b_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    int16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, srarn_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, srarn_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, srarn_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, srarn_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvsrarni_h_w ====================
// Arithmetic + rounding: H <- W
// For i in range(4):
//   XR[xd].H[i] = SRANR(XR[xj].W[i], ui5)
//   XR[xd].H[i+4] = SRANR(XR[xd].W[i], ui5)
//   XR[xd].H[i+8] = SRANR(XR[xj].W[i+4], ui5)
//   XR[xd].H[i+12] = SRANR(XR[xd].W[i+4], ui5)
void do_lasx_emu_xvsrarni_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarni_h_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    int32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, srarn_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, srarn_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, srarn_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, srarn_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvsrarni_w_d ====================
// Arithmetic narrow + rounding: D -> W (64->32 bit)
// D[0-1] -> W[0-3], D[2-3] -> W[4-7]
void do_lasx_emu_xvsrarni_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarni_w_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    int64_t xj_d[4];
    int64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, srarn_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, srarn_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, srarn_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, srarn_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrani_b_h ====================
// Signed saturated + arithmetic: B[0-7] from xj, B[8-15] from xd, B[16-23] from xj, B[24-31] from xd
// For i in range(8):
//   XR[xd].B[i] = SSRANS(XR[xj].H[i], ui4)
//   XR[xd].B[i+8] = SSRANS(XR[xd].H[i], ui4)
//   XR[xd].B[i+16] = SSRANS(XR[xj].H[i+8], ui4)
//   XR[xd].B[i+24] = SSRANS(XR[xd].H[i+8], ui4)
void do_lasx_emu_xvssrani_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_b_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    int16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrans_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrans_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrans_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrans_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrani_h_w ====================
// Signed saturated + arithmetic: H <- W
// For i in range(4):
//   XR[xd].H[i] = SSRANS(XR[xj].W[i], ui5)
//   XR[xd].H[i+4] = SSRANS(XR[xd].W[i], ui5)
//   XR[xd].H[i+8] = SSRANS(XR[xj].W[i+4], ui5)
//   XR[xd].H[i+12] = SSRANS(XR[xd].W[i+4], ui5)
void do_lasx_emu_xvssrani_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_h_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    int32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrans_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrans_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrans_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrans_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrani_w_d ====================
// Signed saturated + arithmetic: D -> W (64->32 bit) with immediate shift
// For i in range(2):
//   XR[xd].W[i] = SSRANS(XR[xj].D[i], ui6)
//   XR[xd].W[i+2] = SSRANS(XR[xd].D[i], ui6)
//   XR[xd].W[i+4] = SSRANS(XR[xj].D[i+2], ui6)
//   XR[xd].W[i+6] = SSRANS(XR[xd].D[i+2], ui6)
void do_lasx_emu_xvssrani_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_w_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    int64_t xj_d[4];
    int64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrans_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrans_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrans_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrans_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrani_d_q ====================
// Signed saturated: D <- Q
// For i in range(1):
//   XR[xd].D[i] = SSRANS(XR[xj].Q[i], ui7)
//   XR[xd].D[i+1] = SSRANS(XR[xd].Q[i], ui7)
//   XR[xd].D[i+2] = SSRANS(XR[xj].Q[i+1], ui7)
//   XR[xd].D[i+3] = SSRANS(XR[xd].Q[i+1], ui7)
void do_lasx_emu_xvssrani_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __int128_t xj_q[2];
    __int128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrans_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrans_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrans_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrans_d_q(xd_q[1], shamt));
}

// ==================== xvssrani_bu_h ====================
// Unsigned saturated: B <- H
// For i in range(8):
//   XR[xd].B[i] = SSRANU(XR[xj].H[i], ui4)
//   XR[xd].B[i+8] = SSRANU(XR[xd].H[i], ui4)
//   XR[xd].B[i+16] = SSRANU(XR[xj].H[i+8], ui4)
//   XR[xd].B[i+24] = SSRANU(XR[xd].H[i+8], ui4)
void do_lasx_emu_xvssrani_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    uint16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssranu_bu_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssranu_bu_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssranu_bu_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssranu_bu_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrani_hu_w ====================
// Unsigned saturated: H <- W
// For i in range(4):
//   XR[xd].H[i] = SSRANU(XR[xj].W[i], ui5)
//   XR[xd].H[i+4] = SSRANU(XR[xd].W[i], ui5)
//   XR[xd].H[i+8] = SSRANU(XR[xj].W[i+4], ui5)
//   XR[xd].H[i+12] = SSRANU(XR[xd].W[i+4], ui5)
void do_lasx_emu_xvssrani_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssranu_hu_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssranu_hu_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssranu_hu_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssranu_hu_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrani_wu_d ====================
// Unsigned saturated: W <- D
// For i in range(2):
//   XR[xd].W[i] = SSRANU(XR[xj].D[i], ui6)
//   XR[xd].W[i+2] = SSRANU(XR[xd].D[i], ui6)
//   XR[xd].W[i+4] = SSRANU(XR[xj].D[i+2], ui6)
//   XR[xd].W[i+6] = SSRANU(XR[xd].D[i+2], ui6)
void do_lasx_emu_xvssrani_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    uint64_t xj_d[4];
    uint64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssranu_wu_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssranu_wu_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssranu_wu_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssranu_wu_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrani_du_q ====================
// Unsigned saturated: D <- Q
// For i in range(1):
//   XR[xd].D[i] = SSRANU(XR[xj].Q[i], ui7)
//   XR[xd].D[i+1] = SSRANU(XR[xd].Q[i], ui7)
//   XR[xd].D[i+2] = SSRANU(XR[xj].Q[i+1], ui7)
//   XR[xd].D[i+3] = SSRANU(XR[xd].Q[i+1], ui7)
void do_lasx_emu_xvssrani_du_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrani_du_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __uint128_t xj_q[2];
    __uint128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssranu_du_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssranu_du_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssranu_du_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssranu_du_q(xd_q[1], shamt));
}

// ==================== xvssrarni_b_h ====================
// Signed saturated + arithmetic + rounding: B[0-7] from xj, B[8-15] from xd, B[16-23] from xj, B[24-31] from xd
void do_lasx_emu_xvssrarni_b_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_b_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    int16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrarns_b_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrarns_b_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrarns_b_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrarns_b_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrarni_h_w ====================
// Signed saturated + arithmetic + rounding: H[0-3] from xj, H[4-7] from xd, H[8-11] from xj, H[12-15] from xd
void do_lasx_emu_xvssrarni_h_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_h_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    int32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrarns_h_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrarns_h_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrarns_h_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrarns_h_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrarni_w_d ====================
// Signed saturated + arithmetic + rounding: D -> W (64->32 bit)
// D[0-1] -> W[0-3], D[2-3] -> W[4-7]
void do_lasx_emu_xvssrarni_w_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_w_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    int64_t xj_d[4];
    int64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrarns_w_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrarns_w_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrarns_w_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrarns_w_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrarni_d_q ====================
// Signed saturated + rounding: D[0] from xj, D[1] from xd, D[2] from xj, D[3] from xd
void do_lasx_emu_xvssrarni_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __int128_t xj_q[2];
    __int128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrarns_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrarns_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrarns_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrarns_d_q(xd_q[1], shamt));
}

// ==================== xvssrarni_bu_h ====================
// Unsigned saturated + rounding: B[0-7] from xj, B[8-15] from xd, B[16-23] from xj, B[24-31] from xd
void do_lasx_emu_xvssrarni_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    uint16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrarnu_bu_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrarnu_bu_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrarnu_bu_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrarnu_bu_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrarni_hu_w ====================
// Unsigned saturated + rounding: H[0-3] from xj, H[4-7] from xd, H[8-11] from xj, H[12-15] from xd
void do_lasx_emu_xvssrarni_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrarnu_hu_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrarnu_hu_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrarnu_hu_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrarnu_hu_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrarni_wu_d ====================
// Unsigned saturated + rounding: D -> W (64->32 bit)
// D[0-1] -> W[0-3], D[2-3] -> W[4-7]
void do_lasx_emu_xvssrarni_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    uint64_t xj_d[4];
    uint64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrarnu_wu_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrarnu_wu_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrarnu_wu_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrarnu_wu_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrarni_du_q ====================
// Unsigned saturated + rounding: D[0] from xj, D[1] from xd, D[2] from xj, D[3] from xd
void do_lasx_emu_xvssrarni_du_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrarni_du_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __uint128_t xj_q[2];
    __uint128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrarnu_du_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrarnu_du_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrarnu_du_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrarnu_du_q(xd_q[1], shamt));
}

// ==================== xvsrlni_d_q ====================
// Logicals: D[0] from xj, D[1] from xd, D[2] from xj, D[3] from xd
void do_lasx_emu_xvsrlni_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlni_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __uint128_t xj_q[2];
    __uint128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, srln_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, srln_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, srln_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, srln_d_q(xd_q[1], shamt));
}

// ==================== xvsrlrni_d_q ====================
// Logicals + rounding: D[0] from xj, D[1] from xd, D[2] from xj, D[3] from xd
void do_lasx_emu_xvsrlrni_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrlrni_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __uint128_t xj_q[2];
    __uint128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, srlrn_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, srlrn_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, srlrn_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, srlrn_d_q(xd_q[1], shamt));
}

// ==================== xvssrlni_d_q ====================
// Signed saturated: D <- Q (128 -> 64 bit)
// Q[0] -> D[0], D[1]; Q[1] -> D[2], D[3]
void do_lasx_emu_xvssrlni_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __int128_t xj_q[2];
    __int128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrlns_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrlns_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrlns_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrlns_d_q(xd_q[1], shamt));
}

// ==================== xvssrlni_bu_h ====================
// Unsigned saturated + logic: B <- H
// For i in range(8):
//   XR[xd].B[i] = SSRLNU(XR[xj].H[i], ui4)
//   XR[xd].B[i+8] = SSRLNU(XR[xd].H[i], ui4)
//   XR[xd].B[i+16] = SSRLNU(XR[xj].H[i+8], ui4)
//   XR[xd].B[i+24] = SSRLNU(XR[xd].H[i+8], ui4)
void do_lasx_emu_xvssrlni_bu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_bu_h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    uint16_t xd_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
        xd_h[i] = vreg_read_u16(uc, td, xd, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_8(uc, td, xd, i, ssrlnu_bu_h(xj_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 8, ssrlnu_bu_h(xd_h[i], shamt));
        vreg_write_8(uc, td, xd, i + 16, ssrlnu_bu_h(xj_h[i + 8], shamt));
        vreg_write_8(uc, td, xd, i + 24, ssrlnu_bu_h(xd_h[i + 8], shamt));
    }
}

// ==================== xvssrlni_hu_w ====================
// Unsigned saturated + logic: H <- W
// For i in range(4):
//   XR[xd].H[i] = SSRLNU(XR[xj].W[i], ui5)
//   XR[xd].H[i+4] = SSRLNU(XR[xd].W[i], ui5)
//   XR[xd].H[i+8] = SSRLNU(XR[xj].W[i+4], ui5)
//   XR[xd].H[i+12] = SSRLNU(XR[xd].W[i+4], ui5)
void do_lasx_emu_xvssrlni_hu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_hu_w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xd_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, ssrlnu_hu_w(xj_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 4, ssrlnu_hu_w(xd_w[i], shamt));
        vreg_write_16(uc, td, xd, i + 8, ssrlnu_hu_w(xj_w[i + 4], shamt));
        vreg_write_16(uc, td, xd, i + 12, ssrlnu_hu_w(xd_w[i + 4], shamt));
    }
}

// ==================== xvssrlni_wu_d ====================
// Unsigned saturated + logic: W <- D
// For i in range(2):
//   XR[xd].W[i] = SSRLNU(XR[xj].D[i], ui6)
//   XR[xd].W[i+2] = SSRLNU(XR[xd].D[i], ui6)
//   XR[xd].W[i+4] = SSRLNU(XR[xj].D[i+2], ui6)
//   XR[xd].W[i+6] = SSRLNU(XR[xd].D[i+2], ui6)
void do_lasx_emu_xvssrlni_wu_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_wu_d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x3f;

    uint64_t xj_d[4];
    uint64_t xd_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xd_d[i] = vreg_read_64(uc, td, xd, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_32(uc, td, xd, i, ssrlnu_wu_d(xj_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 2, ssrlnu_wu_d(xd_d[i], shamt));
        vreg_write_32(uc, td, xd, i + 4, ssrlnu_wu_d(xj_d[i + 2], shamt));
        vreg_write_32(uc, td, xd, i + 6, ssrlnu_wu_d(xd_d[i + 2], shamt));
    }
}

// ==================== xvssrlni_du_q ====================
// Unsigned saturated: D <- Q (128 -> 64 bit)
// Q[0] -> D[0], D[1]; Q[1] -> D[2], D[3]
void do_lasx_emu_xvssrlni_du_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlni_du_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __uint128_t xj_q[2];
    __uint128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrlnu_du_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrlnu_du_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrlnu_du_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrlnu_du_q(xd_q[1], shamt));
}

// ==================== xvssrlrni_d_q ====================
// Signed saturated + rounding: D[0] from xj, D[1] from xd, D[2] from xj, D[3] from xd
void do_lasx_emu_xvssrlrni_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __int128_t xj_q[2];
    __int128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrlrns_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrlrns_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrlrns_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrlrns_d_q(xd_q[1], shamt));
}

// ==================== xvssrlrni_du_q ====================
// Unsigned saturated + rounding: D[0] from xj, D[1] from xd, D[2] from xj, D[3] from xd
void do_lasx_emu_xvssrlrni_du_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssrlrni_du_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __uint128_t xj_q[2];
    __uint128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, ssrlrnu_du_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, ssrlrnu_du_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, ssrlrnu_du_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, ssrlrnu_du_q(xd_q[1], shamt));
}

// ==================== xvsrani_d_q ====================
// Arithmetic: D <- Q
// Q[0] -> D[0], D[1]; Q[1] -> D[2], D[3]
void do_lasx_emu_xvsrani_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrani_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __int128_t xj_q[2];
    __int128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, sran_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, sran_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, sran_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, sran_d_q(xd_q[1], shamt));
}

// ==================== xvsrarni_d_q ====================
// Arithmetic + rounding: D <- Q
// Q[0] -> D[0], D[1]; Q[1] -> D[2], D[3]
void do_lasx_emu_xvsrarni_d_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsrarni_d_q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    unsigned int shamt = (instr >> 10) & 0x7f;

    __int128_t xj_q[2];
    __int128_t xd_q[2];
    for (int i = 0; i < 2; i++) {
        xj_q[i] = vreg_read_w128(uc, td, xj, i);
        xd_q[i] = vreg_read_w128(uc, td, xd, i);
    }

    vreg_write_64(uc, td, xd, 0, srarn_d_q(xj_q[0], shamt));
    vreg_write_64(uc, td, xd, 1, srarn_d_q(xd_q[0], shamt));
    vreg_write_64(uc, td, xd, 2, srarn_d_q(xj_q[1], shamt));
    vreg_write_64(uc, td, xd, 3, srarn_d_q(xd_q[1], shamt));
}
