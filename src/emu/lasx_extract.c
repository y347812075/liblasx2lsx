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

#include "lasx_extract.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== Extract Half ====================

// XVEXTH.H.B: for i in range(8): XR[xd].H[i] = SignExtend(XR[xj].B[i+8], 16), XR[xd].H[i+8] = SignExtend(XR[xj].B[i+24], 16)
void do_lasx_emu_xvexth_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int8_t src_low[8], src_high[8];
    for (int i = 0; i < 8; i++) {
        src_low[i] = vreg_read_8(uc, td, xj, i + 8);
    }
    for (int i = 0; i < 8; i++) {
        src_high[i] = vreg_read_8(uc, td, xj, i + 24);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, (uint16_t)(int16_t)src_low[i]);
    }
    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i + 8, (uint16_t)(int16_t)src_high[i]);
    }
}

// XVEXTH.W.H: for i in range(4): XR[xd].W[i] = SignExtend(XR[xj].H[i+4], 32), XR[xd].W[i+4] = SignExtend(XR[xj].H[i+12], 32)
void do_lasx_emu_xvexth_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int16_t src_low[4], src_high[4];
    for (int i = 0; i < 4; i++) {
        src_low[i] = vreg_read_16(uc, td, xj, i + 4);
    }
    for (int i = 0; i < 4; i++) {
        src_high[i] = vreg_read_16(uc, td, xj, i + 12);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)(int32_t)src_low[i]);
    }
    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i + 4, (uint32_t)(int32_t)src_high[i]);
    }
}

// XVEXTH.D.W: for i in range(2): XR[xd].D[i] = SignExtend(XR[xj].W[i+2], 64), XR[xd].D[i+2] = SignExtend(XR[xj].W[i+6], 64)
void do_lasx_emu_xvexth_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int32_t src_low[2], src_high[2];
    for (int i = 0; i < 2; i++) {
        src_low[i] = vreg_read_32(uc, td, xj, i + 2);
    }
    for (int i = 0; i < 2; i++) {
        src_high[i] = vreg_read_32(uc, td, xj, i + 6);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)src_low[i]);
    }
    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i + 2, (uint64_t)(int64_t)src_high[i]);
    }
}

// XVEXTH.Q.D: XR[xd].Q[0] = SignExtend(XR[xj].D[1], 128), XR[xd].Q[1] = SignExtend(XR[xj].D[3], 128)
void do_lasx_emu_xvexth_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src[2];
    src[0] = vreg_read_64(uc, td, xj, 1);
    src[1] = vreg_read_64(uc, td, xj, 3);

    signed __int128 res0 = (signed __int128)(int64_t)src[0];
    vreg_write_128(uc, td, xd, 0, (uint64_t)res0, (uint64_t)(res0 >> 64));

    signed __int128 res1 = (signed __int128)(int64_t)src[1];
    vreg_write_128(uc, td, xd, 1, (uint64_t)res1, (uint64_t)(res1 >> 64));
}

// XVEXTH.HU.BU: for i in range(8): XR[xd].H[i] = ZeroExtend(XR[xj].B[i+8], 16), XR[xd].H[i+8] = ZeroExtend(XR[xj].B[i+24], 16)
void do_lasx_emu_xvexth_hu_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.hu.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint8_t src_low[8], src_high[8];
    for (int i = 0; i < 8; i++) {
        src_low[i] = vreg_read_u8(uc, td, xj, i + 8);
    }
    for (int i = 0; i < 8; i++) {
        src_high[i] = vreg_read_u8(uc, td, xj, i + 24);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, (uint16_t)src_low[i]);
    }
    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i + 8, (uint16_t)src_high[i]);
    }
}

// XVEXTH.WU.HU: for i in range(4): XR[xd].W[i] = ZeroExtend(XR[xj].H[i+4], 32), XR[xd].W[i+4] = ZeroExtend(XR[xj].H[i+12], 32)
void do_lasx_emu_xvexth_wu_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.wu.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint16_t src_low[4], src_high[4];
    for (int i = 0; i < 4; i++) {
        src_low[i] = vreg_read_u16(uc, td, xj, i + 4);
    }
    for (int i = 0; i < 4; i++) {
        src_high[i] = vreg_read_u16(uc, td, xj, i + 12);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)src_low[i]);
    }
    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i + 4, (uint32_t)src_high[i]);
    }
}

// XVEXTH.DU.WU: for i in range(2): XR[xd].D[i] = ZeroExtend(XR[xj].W[i+2], 64), XR[xd].D[i+2] = ZeroExtend(XR[xj].W[i+6], 64)
void do_lasx_emu_xvexth_du_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.du.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint32_t src_low[2], src_high[2];
    for (int i = 0; i < 2; i++) {
        src_low[i] = vreg_read_u32(uc, td, xj, i + 2);
    }
    for (int i = 0; i < 2; i++) {
        src_high[i] = vreg_read_u32(uc, td, xj, i + 6);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)src_low[i]);
    }
    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i + 2, (uint64_t)src_high[i]);
    }
}

// XVEXTH.QU.DU: XR[xd].Q[0] = ZeroExtend(XR[xj].D[1], 128), XR[xd].Q[1] = ZeroExtend(XR[xj].D[3], 128)
void do_lasx_emu_xvexth_qu_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvexth.qu.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src[2];
    src[0] = vreg_read_64(uc, td, xj, 1);
    src[1] = vreg_read_64(uc, td, xj, 3);

    vreg_write_128(uc, td, xd, 0, src[0], 0);
    vreg_write_128(uc, td, xd, 1, src[1], 0);
}

// ==================== Extract Lower ====================

// xvextl.q.d: extract lower 2 dwords from source, sign extend to 2 quadwords
void do_lasx_emu_xvextl_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvextl.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src[2];
    src[0] = vreg_read_64(uc, td, xj, 0);
    src[1] = vreg_read_64(uc, td, xj, 2);

    signed __int128 res0 = (signed __int128)(int64_t)src[0];
    vreg_write_128(uc, td, xd, 0, (uint64_t)res0, (uint64_t)(res0 >> 64));

    signed __int128 res1 = (signed __int128)(int64_t)src[1];
    vreg_write_128(uc, td, xd, 1, (uint64_t)res1, (uint64_t)(res1 >> 64));
}

// xvextl.qu.du: extract lower 2 dwords from source, zero extend to 2 quadwords
void do_lasx_emu_xvextl_qu_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvextl.qu.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t src[2];
    src[0] = vreg_read_64(uc, td, xj, 0);
    src[1] = vreg_read_64(uc, td, xj, 2);

    vreg_write_128(uc, td, xd, 0, src[0], 0);
    vreg_write_128(uc, td, xd, 1, src[1], 0);
}

// ==================== Extract Insert ====================

void do_lasx_emu_xvextrins_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvextrins.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int lsb = (instr >> 10) & 0xff;

    int src_idx = lsb & 0x1;
    int dst_idx = (lsb >> 4) & 0x1;

    // Prefetch source values
    uint64_t src_vals[2];
    src_vals[0] = vreg_read_64(uc, td, xj, src_idx);
    src_vals[1] = vreg_read_64(uc, td, xj, src_idx + 2);

    // Write to destination
    vreg_write_64(uc, td, xd, dst_idx, src_vals[0]);
    vreg_write_64(uc, td, xd, dst_idx + 2, src_vals[1]);
}

void do_lasx_emu_xvextrins_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvextrins.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int lsb = (instr >> 10) & 0xff;

    int src_idx = lsb & 0x3;
    int dst_idx = (lsb >> 4) & 0x3;

    uint32_t src_vals[2];
    src_vals[0] = vreg_read_u32(uc, td, xj, src_idx);
    src_vals[1] = vreg_read_u32(uc, td, xj, src_idx + 4);

    vreg_write_32(uc, td, xd, dst_idx, src_vals[0]);
    vreg_write_32(uc, td, xd, dst_idx + 4, src_vals[1]);
}

void do_lasx_emu_xvextrins_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvextrins.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int lsb = (instr >> 10) & 0xff;

    int src_idx = lsb & 0x7;
    int dst_idx = (lsb >> 4) & 0x7;

    uint16_t src_vals[2];
    src_vals[0] = vreg_read_u16(uc, td, xj, src_idx);
    src_vals[1] = vreg_read_u16(uc, td, xj, src_idx + 8);

    vreg_write_16(uc, td, xd, dst_idx, src_vals[0]);
    vreg_write_16(uc, td, xd, dst_idx + 8, src_vals[1]);
}

// xvextrins.b: XR[xd].B[ui8[7:4]] = XR[xj].B[ui8[3:0]] + XR[xd].B[ui8[7:4]+16] = XR[xj].B[ui8[3:0]+16]
void do_lasx_emu_xvextrins_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvextrins.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int lsb = (instr >> 10) & 0xff;

    int src_idx = lsb & 0xf;
    int dst_idx = (lsb >> 4) & 0xf;

    uint8_t src_vals[2];
    src_vals[0] = vreg_read_u8(uc, td, xj, src_idx);
    src_vals[1] = vreg_read_u8(uc, td, xj, src_idx + 16);

    vreg_write_8(uc, td, xd, dst_idx, src_vals[0]);
    vreg_write_8(uc, td, xd, dst_idx + 16, src_vals[1]);
}

// ==================== Extract from LASX to LASX (sign/zero extend) ====================

// VEXT2XV.H.B: XR[xd].H[i] = SignExtend(XR[xj].B[i], 16) for i in 0..15
void do_lasx_emu_vext2xv_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int8_t xj_b[16];
    for (int i = 0; i < 16; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, (uint16_t)(int16_t)xj_b[i]);
    }

}

// VEXT2XV.W.B: XR[xd].W[i] = SignExtend(XR[xj].B[i], 32) for i in 0..7
void do_lasx_emu_vext2xv_w_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.w.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int8_t xj_b[8];
    for (int i = 0; i < 8; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)(int32_t)xj_b[i]);
    }

}

// VEXT2XV.D.B: XR[xd].D[i] = SignExtend(XR[xj].B[i], 64) for i in 0..3
void do_lasx_emu_vext2xv_d_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.d.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int8_t xj_b[4];
    for (int i = 0; i < 4; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)xj_b[i]);
    }

}

// VEXT2XV.W.H: XR[xd].W[i] = SignExtend(XR[xj].H[i], 32) for i in 0..7
void do_lasx_emu_vext2xv_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int16_t xj_h[8];
    for (int i = 0; i < 8; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)(int32_t)xj_h[i]);
    }

}

// VEXT2XV.D.H: XR[xd].D[i] = SignExtend(XR[xj].H[i], 64) for i in 0..3
void do_lasx_emu_vext2xv_d_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.d.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int16_t xj_h[4];
    for (int i = 0; i < 4; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)xj_h[i]);
    }

}

// VEXT2XV.D.W: XR[xd].D[i] = SignExtend(XR[xj].W[i], 64) for i in 0..3
void do_lasx_emu_vext2xv_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    int32_t xj_w[4];
    for (int i = 0; i < 4; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)xj_w[i]);
    }

}

// VEXT2XV.HU.BU: XR[xd].H[i] = ZeroExtend(XR[xj].B[i], 16) for i in 0..15
void do_lasx_emu_vext2xv_hu_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.hu.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint8_t xj_b[16];
    for (int i = 0; i < 16; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, (uint16_t)xj_b[i]);
    }

}

// VEXT2XV.WU.BU: XR[xd].W[i] = ZeroExtend(XR[xj].B[i], 32) for i in 0..7
void do_lasx_emu_vext2xv_wu_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.wu.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint8_t xj_b[8];
    for (int i = 0; i < 8; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)xj_b[i]);
    }

}

// VEXT2XV.DU.BU: XR[xd].D[i] = ZeroExtend(XR[xj].B[i], 64) for i in 0..3
void do_lasx_emu_vext2xv_du_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.du.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint8_t xj_b[4];
    for (int i = 0; i < 4; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)xj_b[i]);
    }

}

// VEXT2XV.WU.HU: XR[xd].W[i] = ZeroExtend(XR[xj].H[i], 32) for i in 0..7
void do_lasx_emu_vext2xv_wu_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.wu.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint16_t xj_h[8];
    for (int i = 0; i < 8; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)xj_h[i]);
    }

}

// VEXT2XV.DU.HU: XR[xd].D[i] = ZeroExtend(XR[xj].H[i], 64) for i in 0..3
void do_lasx_emu_vext2xv_du_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.du.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint16_t xj_h[4];
    for (int i = 0; i < 4; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)xj_h[i]);
    }

}

// VEXT2XV.DU.WU: XR[xd].D[i] = ZeroExtend(XR[xj].W[i], 64) for i in 0..3
void do_lasx_emu_vext2xv_du_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate vext2xv.du.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint32_t xj_w[4];
    for (int i = 0; i < 4; i++) {
        xj_w[i] = vreg_read_u32(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)xj_w[i]);
    }

}

// ==================== Move GPR to LASX - replicate to all elements ====================

// xvreplgr2vr.b: replicate GPR byte to all bytes of vector
void do_lasx_emu_xvreplgr2vr_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplgr2vr.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int rj = (instr >> 5) & 0x1f;

    uint8_t val = (uint8_t)UC_GPR(uc, rj);
    uint64_t replicated = 0;
    for (int i = 0; i < 8; i++) {
        replicated |= ((uint64_t)val << (i * 8));
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, replicated);
    }
}

// xvreplgr2vr.h: replicate GPR halfword to all halfwords of vector
void do_lasx_emu_xvreplgr2vr_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplgr2vr.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int rj = (instr >> 5) & 0x1f;

    uint16_t val = (uint16_t)UC_GPR(uc, rj);
    uint64_t replicated = 0;
    for (int i = 0; i < 4; i++) {
        replicated |= ((uint64_t)val << (i * 16));
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, replicated);
    }
}

// xvreplgr2vr.w: replicate GPR word to all words of vector
void do_lasx_emu_xvreplgr2vr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplgr2vr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int rj = (instr >> 5) & 0x1f;

    uint32_t val = (uint32_t)UC_GPR(uc, rj);
    uint64_t replicated = 0;
    for (int i = 0; i < 2; i++) {
        replicated |= ((uint64_t)val << (i * 32));
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, replicated);
    }
}

// xvreplgr2vr.d: replicate GPR doubleword to all doublewords of vector
void do_lasx_emu_xvreplgr2vr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplgr2vr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int rj = (instr >> 5) & 0x1f;

    uint64_t val = (uint64_t)UC_GPR(uc, rj);

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, val);
    }
}

// ==================== Replicate even elements ====================
// XVREPLVE.B: replicateXR[xj].B[t] to all B[0-15], XR[xj].B[t+16] to all B[16-31]
void do_lasx_emu_xvreplve_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int rk = (instr >> 10) & 0x1f;
    int t = (int)UC_GPR(uc, rk) & 0xf;

    uint64_t src_j1 = vreg_read_64(uc, td, xj, t / 8);
    int shift1 = (t % 8) * 8;
    uint8_t val1 = (src_j1 >> shift1) & 0xff;
    uint64_t dst1 = 0;
    for (int i = 0; i < 8; i++) {
        dst1 |= ((uint64_t)val1 << (i * 8));
    }

    uint64_t src_j2 = vreg_read_64(uc, td, xj, (t + 16) / 8);
    int shift2 = ((t + 16) % 8) * 8;
    uint8_t val2 = (src_j2 >> shift2) & 0xff;
    uint64_t dst2 = 0;
    for (int i = 0; i < 8; i++) {
        dst2 |= ((uint64_t)val2 << (i * 8));
    }

    vreg_write_64(uc, td, xd, 0, dst1);
    vreg_write_64(uc, td, xd, 1, dst1);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, dst2);
}

// XVREPLVE.H: replicate XR[xj].H[t] to all H[0-7], XR[xj].H[t+8] to all H[8-15]
void do_lasx_emu_xvreplve_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int rk = (instr >> 10) & 0x1f;
    int t = (int)UC_GPR(uc, rk) & 0x7;

    int16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, xj_h[t]);
        vreg_write_16(uc, td, xd, i + 8, xj_h[t + 8]);
    }
}

// XVREPLVE.W: replicate XR[xj].W[t] to all W[0-3], XR[xj].W[t+4] to all W[4-7]
void do_lasx_emu_xvreplve_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int rk = (instr >> 10) & 0x1f;
    int t = (int)UC_GPR(uc, rk) & 0x3;

    int32_t xj_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, xj_w[t]);
        vreg_write_32(uc, td, xd, i + 4, xj_w[t + 4]);
    }
}

// XVREPLVE.D: replicate XR[xj].D[t] to D[0-1], XR[xj].D[t+2] to D[2-3]
void do_lasx_emu_xvreplve_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int rk = (instr >> 10) & 0x1f;
    int t = (int)UC_GPR(uc, rk) & 0x1;

    //// D[0-1]: replicate xj.D[t]
    // uint64_t val0 = vreg_read_64(uc, td, xj, t);
    // vreg_write_64(uc, td, xd, 0, val0);
    // vreg_write_64(uc, td, xd, 1, val0);

    //// D[2-3]: replicate xj.D[t+2]
    // uint64_t val1 = vreg_read_64(uc, td, xj, t + 2);
    // vreg_write_64(uc, td, xd, 2, val1);
    // vreg_write_64(uc, td, xd, 3, val1);
    int64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i, xj_d[t]);
        vreg_write_64(uc, td, xd, i + 2, xj_d[t + 2]);
    }
}

// ==================== Pick from LASX to GPR ====================

// xvpickve2gr.w: pick vector element to GPR
void do_lasx_emu_xvpickve2gr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickve2gr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int rd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x7;

    int qw_idx = uk / 2;
    int shift = (uk % 2) * 32;
    uint64_t src = vreg_read_64(uc, td, xj, qw_idx);
    int32_t val = (int32_t)((src >> shift) & 0xffffffff);
    UC_GPR(uc, rd) = (uint64_t)(int64_t)val;

}

// xvpickve2gr.d: pick vector element to GPR
void do_lasx_emu_xvpickve2gr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickve2gr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int rd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x3;

    uint64_t val = vreg_read_64(uc, td, xj, uk);
    UC_GPR(uc, rd) = val;

}

// xvpickve2gr.wu: pick vector element to GPR unsigned
void do_lasx_emu_xvpickve2gr_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickve2gr.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int rd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x7;

    int qw_idx = uk / 2;
    int shift = (uk % 2) * 32;
    uint64_t src = vreg_read_64(uc, td, xj, qw_idx);
    uint32_t val = (uint32_t)((src >> shift) & 0xffffffff);
    UC_GPR(uc, rd) = (uint64_t)val;

}

// xvpickve2gr.du: pick vector element to GPR unsigned
void do_lasx_emu_xvpickve2gr_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickve2gr.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int rd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x3;

    uint64_t val = vreg_read_64(uc, td, xj, uk);
    UC_GPR(uc, rd) = val;

}

// ==================== Pick from LASX vector ====================

// xvpickve.w: pick vector element to other vector position
void do_lasx_emu_xvpickve_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickve.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x7;

    int qw_idx = uk / 2;
    int shift = (uk % 2) * 32;
    uint64_t src = vreg_read_64(uc, td, xj, qw_idx);
    uint32_t val = (uint32_t)((src >> shift) & 0xffffffff);

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, 0);
    }

    vreg_write_64(uc, td, xd, 0, (uint64_t)val);
}

// xvpickve.d: pick vector element to other vector position
void do_lasx_emu_xvpickve_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickve.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x3;

    uint64_t val = vreg_read_64(uc, td, xj, uk);

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, 0);
    }

    vreg_write_64(uc, td, xd, 0, val);
}

// ==================== Insert GPR with zero extension ====================

void do_lasx_emu_xvinsve0_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvinsve0.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x7;

    uint64_t src = vreg_read_64(uc, td, xj, 0);
    uint32_t val = (uint32_t)(src >> 0);

    uint64_t dst = vreg_read_64(uc, td, xd, uk / 2);
    int dst_shift = (uk % 2) * 32;
    dst &= ~((uint64_t)0xffffffff << dst_shift);
    dst |= ((uint64_t)val << dst_shift);
    vreg_write_64(uc, td, xd, uk / 2, dst);
}

void do_lasx_emu_xvinsve0_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvinsve0.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x3;

    uint64_t val = vreg_read_64(uc, td, xj, 0);
    vreg_write_64(uc, td, xd, uk, val);
}

// ==================== Shift left widening ====================

void do_lasx_emu_xvsllwil_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsllwil.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x7;

    int8_t xj_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, (int16_t)xj_b[i] << uk);
        vreg_write_16(uc, td, xd, i + 8, (int16_t)xj_b[i + 16] << uk);
    }
}

void do_lasx_emu_xvsllwil_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsllwil.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0xf;

    int16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, (int32_t)xj_h[i] << uk);
        vreg_write_32(uc, td, xd, i + 4, (int32_t)xj_h[i + 8] << uk);
    }
}

void do_lasx_emu_xvsllwil_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsllwil.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x1f;

    int32_t xj_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i, (int64_t)xj_w[i] << uk);
        vreg_write_64(uc, td, xd, i + 2, (int64_t)xj_w[i + 4] << uk);
    }
}

void do_lasx_emu_xvsllwil_hu_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsllwil.hu.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x7;

    uint8_t xj_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, (uint16_t)xj_b[i] << uk);
        vreg_write_16(uc, td, xd, i + 8, (uint16_t)xj_b[i + 16] << uk);
    }
}

void do_lasx_emu_xvsllwil_wu_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsllwil.wu.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0xf;

    uint16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, (uint32_t)xj_h[i] << uk);
        vreg_write_32(uc, td, xd, i + 4, (uint32_t)xj_h[i + 8] << uk);
    }
}

void do_lasx_emu_xvsllwil_du_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsllwil.du.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_u32(uc, td, xj, i);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_64(uc, td, xd, i, (uint64_t)xj_w[i] << uk);
        vreg_write_64(uc, td, xd, i + 2, (uint64_t)xj_w[i + 4] << uk);
    }
}

// ==================== Insert GPR into vector element ====================

// xvinsgr2vr.w: insert GPR word into vector word at index
void do_lasx_emu_xvinsgr2vr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvinsgr2vr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int rj = (instr >> 5) & 0x1f;
    int ik = (instr >> 10) & 0xf;

    int qw_idx = ik / 2;
    int shift = (ik % 2) * 32;
    uint32_t val = (uint32_t)UC_GPR(uc, rj);
    uint64_t dst = vreg_read_64(uc, td, xd, qw_idx);
    dst &= ~((uint64_t)0xffffffff << shift);
    dst |= ((uint64_t)val << shift);
    vreg_write_64(uc, td, xd, qw_idx, dst);
}

// xvinsgr2vr.d: insert GPR doubleword into vector doubleword at index
void do_lasx_emu_xvinsgr2vr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvinsgr2vr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int rj = (instr >> 5) & 0x1f;
    int ik = (instr >> 10) & 0x7;

    uint64_t val = (uint64_t)UC_GPR(uc, rj);
    vreg_write_64(uc, td, xd, ik, val);
}

// ==================== Replicate 128-bit element by immediate index ====================

void do_lasx_emu_xvrepl128vei_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrepl128vei.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int t = (instr >> 10) & 0xf;

    int src_byte_idx = t / 8;
    int src_shift = (t % 8) * 8;
    uint64_t src0 = vreg_read_64(uc, td, xj, src_byte_idx);
    uint8_t val = (uint8_t)((src0 >> src_shift) & 0xff);

    for (int i = 0; i < 16; i++) {
        vreg_write_8(uc, td, xd, i, val);
    }

    int src_byte_idx2 = (t + 16) / 8;
    int src_shift2 = ((t + 16) % 8) * 8;
    uint64_t src1 = vreg_read_64(uc, td, xj, src_byte_idx2);
    uint8_t val2 = (uint8_t)((src1 >> src_shift2) & 0xff);

    for (int i = 0; i < 16; i++) {
        vreg_write_8(uc, td, xd, i + 16, val2);
    }
}

void do_lasx_emu_xvrepl128vei_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrepl128vei.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int t = (instr >> 10) & 0x7;

    int src_half_idx = t / 4;
    int src_shift = (t % 4) * 16;
    uint64_t src0 = vreg_read_64(uc, td, xj, src_half_idx);
    uint16_t val = (uint16_t)((src0 >> src_shift) & 0xffff);

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, val);
    }

    int src_half_idx2 = (t + 8) / 4;
    int src_shift2 = ((t + 8) % 4) * 16;
    uint64_t src1 = vreg_read_64(uc, td, xj, src_half_idx2);
    uint16_t val2 = (uint16_t)((src1 >> src_shift2) & 0xffff);

    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i + 8, val2);
    }
}

void do_lasx_emu_xvrepl128vei_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrepl128vei.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int t = (instr >> 10) & 0x3;

    int src_word_idx = t / 2;
    int src_shift = (t % 2) * 32;
    uint64_t src = vreg_read_64(uc, td, xj, src_word_idx);
    uint32_t val = (uint32_t)(src >> src_shift);

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, val);
    }

    int src_word_idx2 = (t + 4) / 2;
    int src_shift2 = ((t + 4) % 2) * 32;
    uint64_t src2 = vreg_read_64(uc, td, xj, src_word_idx2);
    uint32_t val2 = (uint32_t)(src2 >> src_shift2);

    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i + 4, val2);
    }
}

void do_lasx_emu_xvrepl128vei_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvrepl128vei.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int t = (instr >> 10) & 0x1;

    uint64_t val0 = vreg_read_64(uc, td, xj, t);
    uint64_t val1 = vreg_read_64(uc, td, xj, t + 2);

    vreg_write_64(uc, td, xd, 0, val0);
    vreg_write_64(uc, td, xd, 1, val0);
    vreg_write_64(uc, td, xd, 2, val1);
    vreg_write_64(uc, td, xd, 3, val1);
}

// ==================== Replicate element 0 to entire vector ====================

// xvreplve0.b: replicate byte 0 to all bytes
void do_lasx_emu_xvreplve0_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve0.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint8_t val = (uint8_t)(vreg_read_64(uc, td, xj, 0) >> 0);
    uint64_t replicated = 0;
    for (int i = 0; i < 8; i++) {
        replicated |= ((uint64_t)val << (i * 8));
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, replicated);
    }
}

// xvreplve0.h: replicate halfword 0 to all halfwords
void do_lasx_emu_xvreplve0_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve0.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint16_t val = (uint16_t)(vreg_read_64(uc, td, xj, 0) >> 0);
    uint64_t replicated = 0;
    for (int i = 0; i < 4; i++) {
        replicated |= ((uint64_t)val << (i * 16));
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, replicated);
    }
}

// xvreplve0.w: replicate word 0 to all words
void do_lasx_emu_xvreplve0_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve0.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint32_t val = (uint32_t)(vreg_read_64(uc, td, xj, 0) >> 0);
    uint64_t replicated = 0;
    for (int i = 0; i < 2; i++) {
        replicated |= ((uint64_t)val << (i * 32));
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, replicated);
    }
}

// xvreplve0.d: replicate doubleword 0 to all doublewords
void do_lasx_emu_xvreplve0_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve0.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t val = vreg_read_64(uc, td, xj, 0);

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, val);
    }
}

// xvreplve0.q: replicate quadword 0 to all quadwords
void do_lasx_emu_xvreplve0_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvreplve0.q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;

    uint64_t lo0 = vreg_read_64(uc, td, xj, 0);
    uint64_t hi0 = vreg_read_64(uc, td, xj, 1);

    vreg_write_64(uc, td, xd, 0, lo0);
    vreg_write_64(uc, td, xd, 1, hi0);
    vreg_write_64(uc, td, xd, 2, lo0);
    vreg_write_64(uc, td, xd, 3, hi0);
}
