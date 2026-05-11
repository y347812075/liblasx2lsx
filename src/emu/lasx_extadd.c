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

#include "lasx_extadd.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== Compare-predicate: xvseq ====================
// xvseq.b/h/w/d
void do_lasx_emu_xvseq_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseq.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            uint8_t c = (a == b) ? 0xff : 0x00;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvseq_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseq.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            uint16_t c = (a == b) ? 0xffff : 0x0000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvseq_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseq.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c = (a == b) ? 0xffffffff : 0x00000000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvseq_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvseq.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        uint64_t c = (a == b) ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// xvseq.bu/hu/wu/du

// ==================== xvsle signed ====================
void do_lasx_emu_xvsle_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            uint8_t c = (a <= b) ? 0xff : 0x00;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsle_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            uint16_t c = (a <= b) ? 0xffff : 0x0000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsle_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c = (a <= b) ? 0xffffffff : 0x00000000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsle_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        uint64_t c = (a <= b) ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvslt signed ====================
void do_lasx_emu_xvslt_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            uint8_t c = (a < b) ? 0xff : 0x00;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvslt_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            uint16_t c = (a < b) ? 0xffff : 0x0000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvslt_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            uint32_t c = (a < b) ? 0xffffffff : 0x00000000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvslt_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        uint64_t c = (a < b) ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvsle unsigned ====================
void do_lasx_emu_xvsle_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t b = (src_k >> shift) & 0xff;
            uint8_t c = (a <= b) ? 0xff : 0x00;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsle_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t b = (src_k >> shift) & 0xffff;
            uint16_t c = (a <= b) ? 0xffff : 0x0000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsle_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t b = (src_k >> shift) & 0xffffffff;
            uint32_t c = (a <= b) ? 0xffffffff : 0x00000000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsle_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsle.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t c = (xj_d[i] <= xk_d[i]) ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== xvslt unsigned ====================
void do_lasx_emu_xvslt_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t b = (src_k >> shift) & 0xff;
            uint8_t c = (a < b) ? 0xff : 0x00;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvslt_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t b = (src_k >> shift) & 0xffff;
            uint16_t c = (a < b) ? 0xffff : 0x0000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvslt_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t b = (src_k >> shift) & 0xffffffff;
            uint32_t c = (a < b) ? 0xffffffff : 0x00000000;
            dst |= ((uint64_t)c << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvslt_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvslt.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint64_t c = (xj_d[i] < xk_d[i]) ? ~0ULL : 0ULL;
        vreg_write_64(uc, td, xd, i, c);
    }
}

// ==================== Add/sub with writeback ====================
void do_lasx_emu_xvaddwev_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int dword_idx = i / 4;
        int half_in_dword = i % 4;
        int shift = half_in_dword * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        int8_t a = (int8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        int16_t res = (int16_t)a + (int16_t)b;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvaddwev_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2;
            int shift = half_idx * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t res = a + b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, (uint32_t)res);
        }
    }
}

void do_lasx_emu_xvaddwev_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int32_t a = (int32_t)(xj_d[i] & 0xffffffff);
        int32_t b = (int32_t)(xk_d[i] & 0xffffffff);
        int64_t res = (int64_t)a + b;

        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvaddwev_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2);
    }

    for (int i = 0; i < 2; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a + b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvsubwev
void do_lasx_emu_xvsubwev_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int dword_idx = i / 4;
        int half_in_dword = i % 4;
        int shift = half_in_dword * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        int8_t a = (int8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        int16_t res = (int16_t)a - (int16_t)b;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvsubwev_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2;
            int shift = half_idx * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t res = a - b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, (uint32_t)res);
        }
    }
}

void do_lasx_emu_xvsubwev_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int32_t a = (int32_t)(xj_d[i] & 0xffffffff);
        int32_t b = (int32_t)(xk_d[i] & 0xffffffff);
        int64_t res = (int64_t)a - b;

        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvsubwev_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2);
    }

    for (int i = 0; i < 2; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a - b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvaddwod
void do_lasx_emu_xvaddwod_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int shift = (byte_idx % 8) * 8;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        int8_t a = (int8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        int16_t res = (int16_t)a + (int16_t)b;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvaddwod_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2 + 1;
            int shift = half_idx * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t res = a + b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, (uint32_t)res);
        }
    }
}

void do_lasx_emu_xvaddwod_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int32_t a = (int32_t)((xj_d[i] >> 32) & 0xffffffff);
        int32_t b = (int32_t)((xk_d[i] >> 32) & 0xffffffff);
        int64_t res = (int64_t)a + b;

        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvaddwod_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2 + 1);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a + b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvsubwod
void do_lasx_emu_xvsubwod_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int shift = (byte_idx % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        int8_t a = (int8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        int16_t res = (int16_t)a - (int16_t)b;

        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvsubwod_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2 + 1;
        int dword_idx = half_idx / 4;
        int shift = (half_idx % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        int16_t a = (int16_t)((src_j >> shift) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift) & 0xffff);
        int32_t res = a - b;

        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }
}

void do_lasx_emu_xvsubwod_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int32_t a = (int32_t)((xj_d[i] >> 32) & 0xffffffff);
        int32_t b = (int32_t)((xk_d[i] >> 32) & 0xffffffff);
        int64_t res = (int64_t)a - b;

        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvsubwod_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2 + 1);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a - b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// ==================== Add/sub with writeback and unsigned ====================
// xvaddwev unsigned
void do_lasx_emu_xvaddwev_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int dword_idx = i / 4;
        int half_in_dword = i % 4;
        int shift = half_in_dword * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
        uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
        uint16_t res = (uint16_t)a + b;
        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwev_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2;
        int dword_idx = half_idx / 4;
        int shift = (half_idx % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
        uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
        uint32_t res = (uint32_t)a + b;

        vreg_write_32(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwev_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = (uint32_t)(xj_d[i] & 0xffffffff);
        uint32_t b = (uint32_t)(xk_d[i] & 0xffffffff);
        uint64_t res = (uint64_t)a + b;

        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwev_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = (unsigned __int128)xj_d[i];
        unsigned __int128 b = (unsigned __int128)xk_d[i];
        unsigned __int128 res = a + b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvsubwev unsigned
void do_lasx_emu_xvsubwev_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int dword_idx = i / 4;
        int half_in_dword = i % 4;
        int shift = half_in_dword * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
        uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
        uint16_t res = (uint16_t)a - b;
        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvsubwev_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2;
        int dword_idx = half_idx / 4;
        int shift = (half_idx % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
        uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
        uint32_t res = (uint32_t)a - b;

        vreg_write_32(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvsubwev_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = (uint32_t)(xj_d[i] & 0xffffffff);
        uint32_t b = (uint32_t)(xk_d[i] & 0xffffffff);
        uint64_t res = (uint64_t)a - b;

        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvsubwev_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwev.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = (unsigned __int128)xj_d[i];
        unsigned __int128 b = (unsigned __int128)xk_d[i];
        unsigned __int128 res = a - b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}
// xvaddwod unsigned
void do_lasx_emu_xvaddwod_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int shift = (byte_idx % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
        uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
        uint16_t res = (uint16_t)a + b;

        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwod_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int half_idx = i * 2 + 1;
            int shift = half_idx * 16;
            uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
            uint32_t res = (uint32_t)a + b;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, res);
        }
    }
}

void do_lasx_emu_xvaddwod_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = (uint32_t)((xj_d[i] >> 32) & 0xffffffff);
        uint32_t b = (uint32_t)((xk_d[i] >> 32) & 0xffffffff);
        uint64_t res = (uint64_t)a + b;

        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwod_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2 + 1);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = (unsigned __int128)xj_d[i];
        unsigned __int128 b = (unsigned __int128)xk_d[i];
        unsigned __int128 res = a + b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvsubwod unsigned
void do_lasx_emu_xvsubwod_h_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.h.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int shift = (byte_idx % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
        uint8_t b = (uint8_t)((src_k >> shift) & 0xff);
        uint16_t res = (uint16_t)a - b;

        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvsubwod_w_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.w.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2 + 1;
        int dword_idx = half_idx / 4;
        int shift = (half_idx % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
        uint16_t b = (uint16_t)((src_k >> shift) & 0xffff);
        uint32_t res = (uint32_t)a - b;

        vreg_write_32(uc, td, xd, i, res);
    }
}
void do_lasx_emu_xvsubwod_d_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.d.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = (uint32_t)((xj_d[i] >> 32) & 0xffffffff);
        uint32_t b = (uint32_t)((xk_d[i] >> 32) & 0xffffffff);
        uint64_t res = (uint64_t)a - b;

        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvsubwod_q_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubwod.q.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2 + 1);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = (unsigned __int128)xj_d[i];
        unsigned __int128 b = (unsigned __int128)xk_d[i];
        unsigned __int128 res = a - b;

        vreg_write_64(uc, td, xd, i * 2, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// ==================== Add/sub with writeback and bundling ====================
void do_lasx_emu_xvaddwev_h_bu_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.h.bu.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int dword_idx = i / 4;
        int half_in_dword = i % 4;
        int shift = half_in_dword * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        uint16_t res = (uint16_t)a + (uint16_t)b;
        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwev_w_hu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.w.hu.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2;
        int dword_idx = half_idx / 4;
        int shift = (half_idx % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift) & 0xffff);
        uint32_t res = (uint32_t)a + (uint32_t)b;

        vreg_write_32(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwev_d_wu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.d.wu.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = (uint32_t)(xj_d[i] & 0xffffffff);
        int32_t b = (int32_t)(xk_d[i] & 0xffffffff);
        uint64_t res = (uint64_t)a + (uint64_t)b;

        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwev_q_du_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwev.q.du.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = (unsigned __int128)xj_d[i];
        __int128 b = (__int128)(int64_t)xk_d[i];
        unsigned __int128 res = a + (unsigned __int128)b;

        vreg_write_64(uc, td, xd, i * 2 + 0, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

void do_lasx_emu_xvaddwod_h_bu_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.h.bu.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx = i * 2 + 1;
        int dword_idx = byte_idx / 8;
        int shift = (byte_idx % 8) * 8;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint8_t a = (uint8_t)((src_j >> shift) & 0xff);
        int8_t b = (int8_t)((src_k >> shift) & 0xff);
        uint16_t res = (uint16_t)a + (uint16_t)b;
        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwod_w_hu_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.w.hu.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx = i * 2 + 1;
        int dword_idx = half_idx / 4;
        int shift = (half_idx % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        uint16_t a = (uint16_t)((src_j >> shift) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift) & 0xffff);
        uint32_t res = (uint32_t)a + (uint32_t)b;

        vreg_write_32(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwod_d_wu_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.d.wu.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        uint32_t a = (uint32_t)(xj_d[i] >> 32);
        int32_t b = (int32_t)(xk_d[i] >> 32);
        uint64_t res = (uint64_t)a + (uint64_t)b;

        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvaddwod_q_du_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddwod.q.du.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i * 2 + 1);
        xk_d[i] = vreg_read_64(uc, td, xk, i * 2 + 1);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 a = (unsigned __int128)xj_d[i];
        __int128 b = (__int128)(int64_t)xk_d[i];
        unsigned __int128 res = a + (unsigned __int128)b;

        vreg_write_64(uc, td, xd, i * 2 + 0, (uint64_t)(res >> 0));
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// ==================== Saturating add/sub ====================
void do_lasx_emu_xvsadd_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int16_t res = (int16_t)a + (int16_t)b;
            if (res > INT8_MAX)
                res = INT8_MAX;
            if (res < INT8_MIN)
                res = INT8_MIN;
            vreg_write_8(uc, td, xd, dword_idx * 8 + i, (uint8_t)res);
        }
    }
}

void do_lasx_emu_xvsadd_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t res = (int32_t)a + (int32_t)b;
            if (res > INT16_MAX)
                res = INT16_MAX;
            if (res < INT16_MIN)
                res = INT16_MIN;
            vreg_write_16(uc, td, xd, dword_idx * 4 + i, (uint16_t)res);
        }
    }
}

void do_lasx_emu_xvsadd_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int64_t res = (int64_t)a + (int64_t)b;
            if (res > INT32_MAX)
                res = INT32_MAX;
            if (res < INT32_MIN)
                res = INT32_MIN;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, (uint32_t)res);
        }
    }
}

void do_lasx_emu_xvsadd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a + (__int128)b;
        if (res > INT64_MAX)
            res = INT64_MAX;
        if (res < INT64_MIN)
            res = INT64_MIN;
        vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)res);
    }
}

// xvssub
void do_lasx_emu_xvssub_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t a = (int8_t)((src_j >> shift) & 0xff);
            int8_t b = (int8_t)((src_k >> shift) & 0xff);
            int16_t res = (int16_t)a - (int16_t)b;
            if (res > INT8_MAX)
                res = INT8_MAX;
            if (res < INT8_MIN)
                res = INT8_MIN;
            dst |= (uint64_t)(uint8_t)res << shift;
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvssub_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t a = (int16_t)((src_j >> shift) & 0xffff);
            int16_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t res = (int32_t)a - (int32_t)b;
            if (res > INT16_MAX)
                res = INT16_MAX;
            if (res < INT16_MIN)
                res = INT16_MIN;
            dst |= (uint64_t)(uint16_t)res << shift;
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvssub_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int64_t res = (int64_t)a - (int64_t)b;
            if (res > INT32_MAX)
                res = INT32_MAX;
            if (res < INT32_MIN)
                res = INT32_MIN;
            dst |= (uint64_t)(uint32_t)res << shift;
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvssub_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a - (__int128)b;
        if (res > INT64_MAX)
            res = INT64_MAX;
        if (res < INT64_MIN)
            res = INT64_MIN;
        vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)res);
    }
}
// Unsigned saturating add/sub
void do_lasx_emu_xvsadd_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t b = (src_k >> shift) & 0xff;
            uint16_t res = (uint16_t)a + (uint16_t)b;
            if (res > UINT8_MAX)
                res = UINT8_MAX;
            dst |= ((uint64_t)res << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsadd_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t b = (src_k >> shift) & 0xffff;
            uint32_t res = (uint32_t)a + (uint32_t)b;
            if (res > UINT16_MAX)
                res = UINT16_MAX;
            dst |= ((uint64_t)(uint16_t)res << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsadd_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t b = (src_k >> shift) & 0xffffffff;
            uint64_t res = (uint64_t)a + (uint64_t)b;
            if (res > UINT32_MAX)
                res = UINT32_MAX;
            dst |= ((uint64_t)(uint32_t)res << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvsadd_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsadd.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        unsigned __int128 res = (unsigned __int128)xj_d[i] + (unsigned __int128)xk_d[i];
        if (res > UINT64_MAX)
            res = UINT64_MAX;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

// xvssub unsigned
void do_lasx_emu_xvssub_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t a = (src_j >> shift) & 0xff;
            uint8_t b = (src_k >> shift) & 0xff;
            int16_t res = (int16_t)a - (int16_t)b;
            if (res < 0)
                res = 0;
            dst |= (uint64_t)(uint8_t)res << shift;
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvssub_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t a = (src_j >> shift) & 0xffff;
            uint16_t b = (src_k >> shift) & 0xffff;
            int32_t res = (int32_t)a - (int32_t)b;
            if (res < 0)
                res = 0;
            dst |= ((uint64_t)(uint16_t)res << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvssub_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        uint64_t dst = 0;

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t a = (src_j >> shift) & 0xffffffff;
            uint32_t b = (src_k >> shift) & 0xffffffff;
            int64_t res = (int64_t)a - (int64_t)b;
            if (res < 0)
                res = 0;
            dst |= ((uint64_t)(uint32_t)res << shift);
        }

        vreg_write_64(uc, td, xd, dword_idx, dst);
    }
}

void do_lasx_emu_xvssub_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvssub.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        if (xj_d[i] < xk_d[i]) {
            vreg_write_64(uc, td, xd, i, 0);
        } else {
            vreg_write_64(uc, td, xd, i, xj_d[i] - xk_d[i]);
        }
    }
}

// ==================== Half extension add/sub ====================
void do_lasx_emu_xvhaddw_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx_j = i * 2 + 1;
        int byte_idx_k = i * 2;
        int dword_idx_j = byte_idx_j / 8;
        int dword_idx_k = byte_idx_k / 8;
        int shift_j = (byte_idx_j % 8) * 8;
        int shift_k = (byte_idx_k % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        int8_t a = (int8_t)((src_j >> shift_j) & 0xff);
        int8_t b = (int8_t)((src_k >> shift_k) & 0xff);
        int16_t res = (int16_t)a + (int16_t)b;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvhaddw_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx_j = i * 2 + 1;
        int half_idx_k = i * 2;
        int dword_idx_j = half_idx_j / 4;
        int dword_idx_k = half_idx_k / 4;
        int shift_j = (half_idx_j % 4) * 16;
        int shift_k = (half_idx_k % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        int16_t a = (int16_t)((src_j >> shift_j) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift_k) & 0xffff);
        int32_t res = (int32_t)a + (int32_t)b;
        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }
}
void do_lasx_emu_xvhaddw_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int dword_idx_j = word_idx_j / 2;
        int dword_idx_k = word_idx_k / 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int shift_j = (word_idx_j % 2) * 32;
        int shift_k = (word_idx_k % 2) * 32;

        int32_t a = (int32_t)((xj_d[i] >> shift_j) & 0xffffffff);
        int32_t b = (int32_t)((xk_d[i] >> shift_k) & 0xffffffff);
        int64_t res = (int64_t)a + (int64_t)b;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvhaddw_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        int dword_idx_j = i * 2 + 1;
        int dword_idx_k = i * 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 2; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a + (__int128)b;
        vreg_write_64(uc, td, xd, i * 2, (uint64_t)res);
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvhsubw
void do_lasx_emu_xvhsubw_h_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.h.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx_j = i * 2 + 1;
        int byte_idx_k = i * 2;
        int dword_idx_j = byte_idx_j / 8;
        int dword_idx_k = byte_idx_k / 8;
        int shift_j = (byte_idx_j % 8) * 8;
        int shift_k = (byte_idx_k % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        int8_t a = (int8_t)((src_j >> shift_j) & 0xff);
        int8_t b = (int8_t)((src_k >> shift_k) & 0xff);
        int16_t res = (int16_t)a - (int16_t)b;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvhsubw_w_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.w.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx_j = i * 2 + 1;
        int half_idx_k = i * 2;
        int dword_idx_j = half_idx_j / 4;
        int dword_idx_k = half_idx_k / 4;
        int shift_j = (half_idx_j % 4) * 16;
        int shift_k = (half_idx_k % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        int16_t a = (int16_t)((src_j >> shift_j) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift_k) & 0xffff);
        int32_t res = (int32_t)a - (int32_t)b;
        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }
}

void do_lasx_emu_xvhsubw_d_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.d.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int dword_idx_j = word_idx_j / 2;
        int dword_idx_k = word_idx_k / 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int shift_j = (word_idx_j % 2) * 32;
        int shift_k = (word_idx_k % 2) * 32;

        int32_t a = (int32_t)((xj_d[i] >> shift_j) & 0xffffffff);
        int32_t b = (int32_t)((xk_d[i] >> shift_k) & 0xffffffff);
        int64_t res = (int64_t)a - (int64_t)b;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvhsubw_q_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.q.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        int dword_idx_j = i * 2 + 1;
        int dword_idx_k = i * 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 2; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 res = (__int128)a - (__int128)b;
        vreg_write_64(uc, td, xd, i * 2, (uint64_t)res);
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}
// Half extension add/sub unsigned
void do_lasx_emu_xvhaddw_hu_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.hu.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx_j = i * 2 + 1;
        int byte_idx_k = i * 2;
        int dword_idx_j = byte_idx_j / 8;
        int dword_idx_k = byte_idx_k / 8;
        int shift_j = (byte_idx_j % 8) * 8;
        int shift_k = (byte_idx_k % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        uint8_t a = (src_j >> shift_j) & 0xff;
        uint8_t b = (src_k >> shift_k) & 0xff;
        uint16_t res = (uint16_t)a + (uint16_t)b;
        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvhaddw_wu_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.wu.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx_j = i * 2 + 1;
        int half_idx_k = i * 2;
        int dword_idx_j = half_idx_j / 4;
        int dword_idx_k = half_idx_k / 4;
        int shift_j = (half_idx_j % 4) * 16;
        int shift_k = (half_idx_k % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        uint16_t a = (src_j >> shift_j) & 0xffff;
        uint16_t b = (src_k >> shift_k) & 0xffff;
        uint32_t res = (uint32_t)a + (uint32_t)b;
        vreg_write_32(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvhaddw_du_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.du.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int dword_idx_j = word_idx_j / 2;
        int dword_idx_k = word_idx_k / 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int shift_j = (word_idx_j % 2) * 32;
        int shift_k = (word_idx_k % 2) * 32;

        uint32_t a = (xj_d[i] >> shift_j) & 0xffffffff;
        uint32_t b = (xk_d[i] >> shift_k) & 0xffffffff;
        uint64_t res = (uint64_t)a + (uint64_t)b;
        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvhaddw_qu_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhaddw.qu.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        int dword_idx_j = i * 2 + 1;
        int dword_idx_k = i * 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 res = (unsigned __int128)xj_d[i] + (unsigned __int128)xk_d[i];
        vreg_write_64(uc, td, xd, i * 2, (uint64_t)res);
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}

// xvhsubw unsigned
void do_lasx_emu_xvhsubw_hu_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.hu.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int byte_idx_j = i * 2 + 1;
        int byte_idx_k = i * 2;
        int dword_idx_j = byte_idx_j / 8;
        int dword_idx_k = byte_idx_k / 8;
        int shift_j = (byte_idx_j % 8) * 8;
        int shift_k = (byte_idx_k % 8) * 8;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        uint8_t a = (src_j >> shift_j) & 0xff;
        uint8_t b = (src_k >> shift_k) & 0xff;
        uint16_t res = (uint16_t)a - (uint16_t)b;
        vreg_write_16(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvhsubw_wu_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.wu.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int half_idx_j = i * 2 + 1;
        int half_idx_k = i * 2;
        int dword_idx_j = half_idx_j / 4;
        int dword_idx_k = half_idx_k / 4;
        int shift_j = (half_idx_j % 4) * 16;
        int shift_k = (half_idx_k % 4) * 16;

        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx_j);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx_k);

        uint16_t a = (src_j >> shift_j) & 0xffff;
        uint16_t b = (src_k >> shift_k) & 0xffff;
        uint32_t res = (uint32_t)a - (uint32_t)b;
        vreg_write_32(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvhsubw_du_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.du.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int dword_idx_j = word_idx_j / 2;
        int dword_idx_k = word_idx_k / 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 4; i++) {
        int word_idx_j = i * 2 + 1;
        int word_idx_k = i * 2;
        int shift_j = (word_idx_j % 2) * 32;
        int shift_k = (word_idx_k % 2) * 32;

        uint32_t a = (xj_d[i] >> shift_j) & 0xffffffff;
        uint32_t b = (xk_d[i] >> shift_k) & 0xffffffff;
        uint64_t res = (uint64_t)a - (uint64_t)b;
        vreg_write_64(uc, td, xd, i, res);
    }
}

void do_lasx_emu_xvhsubw_qu_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvhsubw.qu.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[2], xk_d[2];
    for (int i = 0; i < 2; i++) {
        int dword_idx_j = i * 2 + 1;
        int dword_idx_k = i * 2;
        xj_d[i] = vreg_read_64(uc, td, xj, dword_idx_j);
        xk_d[i] = vreg_read_64(uc, td, xk, dword_idx_k);
    }

    for (int i = 0; i < 2; i++) {
        unsigned __int128 res = (unsigned __int128)xj_d[i] - (unsigned __int128)xk_d[i];
        vreg_write_64(uc, td, xd, i * 2, (uint64_t)res);
        vreg_write_64(uc, td, xd, i * 2 + 1, (uint64_t)(res >> 64));
    }
}
// ==================== Accumulate ====================
void do_lasx_emu_xvadda_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadda.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t result[4];
    uint64_t src_j[4];
    uint64_t src_k[4];
    for (int i = 0; i < 4; i++) {
        result[i] = vreg_read_64(uc, td, xd, i);
        src_j[i] = vreg_read_64(uc, td, xj, i);
        src_k[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            int shift = j * 8;
            int8_t a = (int8_t)((src_j[i] >> shift) & 0xff);
            int8_t b = (int8_t)((src_k[i] >> shift) & 0xff);
            int16_t sum = (int16_t)((a < 0 ? -a : a) + (b < 0 ? -b : b));
            result[i] &= ~(0xffULL << shift);
            result[i] |= (uint64_t)(uint8_t)sum << shift;
        }
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, result[i]);
    }
}

void do_lasx_emu_xvadda_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadda.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int word_idx = i / 4;
        int shift = (i % 4) * 16;
        uint64_t src_j = vreg_read_64(uc, td, xj, word_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, word_idx);
        int16_t a = (int16_t)((src_j >> shift) & 0xffff);
        int16_t b = (int16_t)((src_k >> shift) & 0xffff);
        int16_t sum = (a < 0 ? -a : a) + (b < 0 ? -b : b);
        vreg_write_16(uc, td, xd, i, (uint16_t)sum);
    }
}

void do_lasx_emu_xvadda_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadda.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int dword_idx = i / 2;
        int shift = (i % 2) * 32;
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);
        int32_t a = (int32_t)((src_j >> shift) & 0xffffffff);
        int32_t b = (int32_t)((src_k >> shift) & 0xffffffff);
        int32_t sum = (a < 0 ? -a : a) + (b < 0 ? -b : b);
        vreg_write_32(uc, td, xd, i, (uint32_t)sum);
    }
}

void do_lasx_emu_xvadda_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvadda.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        uint64_t result = (a < 0 ? -a : a) + (b < 0 ? -b : b);
        vreg_write_64(uc, td, xd, i, result);
    }
}

// ==================== Absolute difference ====================
void do_lasx_emu_xvabsd_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int16_t a = (int8_t)((src_j >> shift) & 0xff);
            int16_t b = (int8_t)((src_k >> shift) & 0xff);
            int16_t diff = a - b;
            uint8_t res = diff < 0 ? -diff : diff;
            vreg_write_8(uc, td, xd, dword_idx * 8 + i, res);
        }
    }
}

void do_lasx_emu_xvabsd_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int32_t a = (int16_t)((src_j >> shift) & 0xffff);
            int32_t b = (int16_t)((src_k >> shift) & 0xffff);
            int32_t diff = a - b;
            uint16_t res = diff < 0 ? -diff : diff;
            vreg_write_16(uc, td, xd, dword_idx * 4 + i, res);
        }
    }
}

void do_lasx_emu_xvabsd_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int64_t a = (int32_t)((src_j >> shift) & 0xffffffff);
            int64_t b = (int32_t)((src_k >> shift) & 0xffffffff);
            int64_t diff = a - b;
            uint32_t res = diff < 0 ? -diff : diff;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, res);
        }
    }
}

void do_lasx_emu_xvabsd_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        int64_t a = (int64_t)xj_d[i];
        int64_t b = (int64_t)xk_d[i];
        __int128 diff = (__int128)a - (__int128)b;
        uint64_t res;
        if (diff < 0) {
            res = (uint64_t)(-(__int128)diff);
        } else {
            res = (uint64_t)diff;
        }
        vreg_write_64(uc, td, xd, i, res);
    }
}
// Absolute difference unsigned
void do_lasx_emu_xvabsd_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint16_t a = (src_j >> shift) & 0xff;
            uint16_t b = (src_k >> shift) & 0xff;
            uint16_t diff = a > b ? a - b : b - a;
            uint8_t res = (uint8_t)diff;
            vreg_write_8(uc, td, xd, dword_idx * 8 + i, res);
        }
    }
}

void do_lasx_emu_xvabsd_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint32_t a = (src_j >> shift) & 0xffff;
            uint32_t b = (src_k >> shift) & 0xffff;
            uint32_t diff = a > b ? a - b : b - a;
            uint16_t res = (uint16_t)diff;
            vreg_write_16(uc, td, xd, dword_idx * 4 + i, res);
        }
    }
}

void do_lasx_emu_xvabsd_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int dword_idx = 0; dword_idx < 4; dword_idx++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, dword_idx);
        uint64_t src_k = vreg_read_64(uc, td, xk, dword_idx);

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint64_t a = (src_j >> shift) & 0xffffffff;
            uint64_t b = (src_k >> shift) & 0xffffffff;
            uint64_t diff = a > b ? a - b : b - a;
            uint32_t res = (uint32_t)diff;
            vreg_write_32(uc, td, xd, dword_idx * 2 + i, res);
        }
    }
}

void do_lasx_emu_xvabsd_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvabsd.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        unsigned __int128 diff =
            (unsigned __int128)xj_d[i] > (unsigned __int128)xk_d[i] ? (unsigned __int128)xj_d[i] - (unsigned __int128)xk_d[i] : (unsigned __int128)xk_d[i] - (unsigned __int128)xj_d[i];
        uint64_t res = (uint64_t)diff;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== Average ====================
void do_lasx_emu_xvavg_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 32; i++) {
        int8_t src_j = vreg_read_8(uc, td, xj, i);
        int8_t src_k = vreg_read_8(uc, td, xk, i);
        int16_t a = (int16_t)src_j;
        int16_t b = (int16_t)src_k;
        int16_t res = (a + b) >> 1;
        vreg_write_8(uc, td, xd, i, (int8_t)res);
    }
}

void do_lasx_emu_xvavg_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int16_t src_j = vreg_read_16(uc, td, xj, i);
        int16_t src_k = vreg_read_16(uc, td, xk, i);
        int32_t a = (int32_t)src_j;
        int32_t b = (int32_t)src_k;
        int32_t res = (a + b) >> 1;
        vreg_write_16(uc, td, xd, i, (int16_t)res);
    }
}

void do_lasx_emu_xvavg_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int32_t src_j = vreg_read_32(uc, td, xj, i);
        int32_t src_k = vreg_read_32(uc, td, xk, i);
        int64_t a = (int64_t)src_j;
        int64_t b = (int64_t)src_k;
        int64_t res = (a + b) >> 1;
        vreg_write_32(uc, td, xd, i, (int32_t)res);
    }
}

void do_lasx_emu_xvavg_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        __int128_t a = (__int128_t)(int64_t)xj_d[i];
        __int128_t b = (__int128_t)(int64_t)xk_d[i];
        __int128_t res = (a + b) >> 1;
        vreg_write_64(uc, td, xd, i, (int64_t)res);
    }
}

void do_lasx_emu_xvavg_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        __uint128_t a = (__uint128_t)xj_d[i];
        __uint128_t b = (__uint128_t)xk_d[i];
        __uint128_t res = (a + b) >> 1;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}
// Average unsigned
void do_lasx_emu_xvavg_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 32; i++) {
        uint8_t src_j = vreg_read_u8(uc, td, xj, i);
        uint8_t src_k = vreg_read_u8(uc, td, xk, i);
        uint16_t a = (uint16_t)src_j;
        uint16_t b = (uint16_t)src_k;
        uint16_t res = (a + b) >> 1;
        vreg_write_8(uc, td, xd, i, (uint8_t)res);
    }
}

void do_lasx_emu_xvavg_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        uint16_t src_j = vreg_read_u16(uc, td, xj, i);
        uint16_t src_k = vreg_read_u16(uc, td, xk, i);
        uint32_t a = (uint32_t)src_j;
        uint32_t b = (uint32_t)src_k;
        uint32_t res = (a + b) >> 1;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }
}

void do_lasx_emu_xvavg_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavg.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        uint32_t src_j = vreg_read_u32(uc, td, xj, i);
        uint32_t src_k = vreg_read_u32(uc, td, xk, i);
        uint64_t a = (uint64_t)src_j;
        uint64_t b = (uint64_t)src_k;
        uint64_t res = (a + b) >> 1;
        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }
}

// Average rounded
void do_lasx_emu_xvavgr_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 32; i++) {
        int8_t src_j = vreg_read_8(uc, td, xj, i);
        int8_t src_k = vreg_read_8(uc, td, xk, i);
        int16_t a = (int16_t)src_j;
        int16_t b = (int16_t)src_k;
        int16_t res = (a + b + 1) >> 1;
        vreg_write_8(uc, td, xd, i, (int8_t)res);
    }

}

void do_lasx_emu_xvavgr_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        int16_t src_j = vreg_read_16(uc, td, xj, i);
        int16_t src_k = vreg_read_16(uc, td, xk, i);
        int32_t a = (int32_t)src_j;
        int32_t b = (int32_t)src_k;
        int32_t res = (a + b + 1) >> 1;
        vreg_write_16(uc, td, xd, i, (int16_t)res);
    }

}

void do_lasx_emu_xvavgr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int32_t src_j = vreg_read_32(uc, td, xj, i);
        int32_t src_k = vreg_read_32(uc, td, xk, i);
        int64_t a = (int64_t)src_j;
        int64_t b = (int64_t)src_k;
        int64_t res = (a + b + 1) >> 1;
        vreg_write_32(uc, td, xd, i, (int32_t)res);
    }

}

void do_lasx_emu_xvavgr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        __int128_t a = (__int128_t)(int64_t)xj_d[i];
        __int128_t b = (__int128_t)(int64_t)xk_d[i];
        __int128_t res = (a + b + 1) >> 1;
        vreg_write_64(uc, td, xd, i, (int64_t)res);
    }

}

void do_lasx_emu_xvavgr_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 32; i++) {
        uint8_t src_j = vreg_read_u8(uc, td, xj, i);
        uint8_t src_k = vreg_read_u8(uc, td, xk, i);
        uint16_t a = (uint16_t)src_j;
        uint16_t b = (uint16_t)src_k;
        uint16_t res = (a + b + 1) >> 1;
        vreg_write_8(uc, td, xd, i, (uint8_t)res);
    }

}

void do_lasx_emu_xvavgr_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 16; i++) {
        uint16_t src_j = vreg_read_u16(uc, td, xj, i);
        uint16_t src_k = vreg_read_u16(uc, td, xk, i);
        uint32_t a = (uint32_t)src_j;
        uint32_t b = (uint32_t)src_k;
        uint32_t res = (a + b + 1) >> 1;
        vreg_write_16(uc, td, xd, i, (uint16_t)res);
    }

}

void do_lasx_emu_xvavgr_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        uint32_t src_j = vreg_read_u32(uc, td, xj, i);
        uint32_t src_k = vreg_read_u32(uc, td, xk, i);
        uint64_t a = (uint64_t)src_j;
        uint64_t b = (uint64_t)src_k;
        uint64_t res = (a + b + 1) >> 1;
        vreg_write_32(uc, td, xd, i, (uint32_t)res);
    }

}

void do_lasx_emu_xvavgr_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvavgr.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4], xk_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
        xk_d[i] = vreg_read_64(uc, td, xk, i);
    }

    for (int i = 0; i < 4; i++) {
        __uint128_t a = (__uint128_t)xj_d[i];
        __uint128_t b = (__uint128_t)xk_d[i];
        __uint128_t res = (a + b + 1) >> 1;
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }

}
