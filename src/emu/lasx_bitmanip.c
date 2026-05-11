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

#include "lasx_bitmanip.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== xvbitclr - register variant ====================
void do_lasx_emu_xvbitclr_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclr.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        // Process 8 bytes per slot
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t pos = (src_k >> shift) & 0x7;
            val &= ~(1U << pos);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitclr_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclr.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (src_j >> shift) & 0xffff;
            uint16_t pos = (src_k >> shift) & 0xf;
            val &= ~(1U << pos);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitclr_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclr.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (src_j >> shift) & 0xffffffff;
            uint32_t pos = (src_k >> shift) & 0x1f;
            val &= ~(1U << pos);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitclr_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclr.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        uint64_t pos = vreg_read_64(uc, td, xk, i) & 0x3f;
        val &= ~(1ULL << pos);
        vreg_write_64(uc, td, xd, i, val);
    }
}

// ==================== xvbitset - register variant ====================
void do_lasx_emu_xvbitset_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitset.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t pos = (src_k >> shift) & 0x7;
            val |= (1U << pos);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitset_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitset.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (src_j >> shift) & 0xffff;
            uint16_t pos = (src_k >> shift) & 0xf;
            val |= (1U << pos);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitset_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitset.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (src_j >> shift) & 0xffffffff;
            uint32_t pos = (src_k >> shift) & 0x1f;
            val |= (1U << pos);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitset_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitset.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        uint64_t pos = vreg_read_64(uc, td, xk, i) & 0x3f;
        val |= (1ULL << pos);
        vreg_write_64(uc, td, xd, i, val);
    }
}

// ==================== xvbitrev - register variant ====================
void do_lasx_emu_xvbitrev_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrev.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t pos = (src_k >> shift) & 0x7;
            uint8_t res = val ^ (1U << pos);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitrev_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrev.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (src_j >> shift) & 0xffff;
            uint16_t pos = (src_k >> shift) & 0xf;
            uint16_t res = val ^ (1U << pos);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitrev_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrev.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (src_j >> shift) & 0xffffffff;
            uint32_t pos = (src_k >> shift) & 0x1f;
            uint32_t res = val ^ (1U << pos);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitrev_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrev.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        uint64_t pos = vreg_read_64(uc, td, xk, i) & 0x3f;
        uint64_t res = val ^ (1ULL << pos);
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvbitclri - immediate variant ====================
void do_lasx_emu_xvbitclri_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclri.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            val &= ~(1U << pos);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitclri_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclri.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (src_j >> shift) & 0xffff;
            val &= ~(1U << pos);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitclri_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclri.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (src_j >> shift) & 0xffffffff;
            val &= ~(1U << pos);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitclri_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitclri.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        val &= ~(1ULL << pos);
        vreg_write_64(uc, td, xd, i, val);
    }
}

// ==================== xvbitseti - immediate variant ====================
void do_lasx_emu_xvbitseti_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitseti.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            val |= (1U << pos);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitseti_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitseti.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (src_j >> shift) & 0xffff;
            val |= (1U << pos);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitseti_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitseti.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (src_j >> shift) & 0xffffffff;
            val |= (1U << pos);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)val << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitseti_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitseti.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        val |= (1ULL << pos);
        vreg_write_64(uc, td, xd, i, val);
    }
}

// ==================== xvbitrevi - immediate variant ====================
void do_lasx_emu_xvbitrevi_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrevi.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t res = val ^ (1U << pos);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitrevi_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrevi.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (src_j >> shift) & 0xffff;
            uint16_t res = val ^ (1U << pos);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitrevi_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrevi.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (src_j >> shift) & 0xffffffff;
            uint32_t res = val ^ (1U << pos);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvbitrevi_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitrevi.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int pos = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t val = vreg_read_64(uc, td, xj, i);
        uint64_t res = val ^ (1ULL << pos);
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvsigncov ====================
void do_lasx_emu_xvsigncov_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsigncov.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);

        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t x = (src_j >> shift) & 0xff;
            int8_t y = (int8_t)((src_k >> shift) & 0xff);
            uint8_t res;
            if (x == 0)
                res = 0;
            else if ((int8_t)x < 0)
                res = (uint8_t)(-y);
            else
                res = y;
            vreg_write_8(uc, td, xd, slot * 8 + i, res);
        }
    }
}

void do_lasx_emu_xvsigncov_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsigncov.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);

        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t x = (src_j >> shift) & 0xffff;
            int16_t y = (int16_t)((src_k >> shift) & 0xffff);
            uint16_t res;
            if (x == 0)
                res = 0;
            else if ((int16_t)x < 0)
                res = (uint16_t)(-y);
            else
                res = y;
            vreg_write_16(uc, td, xd, slot * 4 + i, res);
        }
    }
}

void do_lasx_emu_xvsigncov_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsigncov.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t src_k = vreg_read_64(uc, td, xk, slot);

        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t x = (src_j >> shift) & 0xffffffff;
            int32_t y = (int32_t)((src_k >> shift) & 0xffffffff);
            uint32_t res;
            if (x == 0)
                res = 0;
            else if ((int32_t)x < 0)
                res = (uint32_t)(-y);
            else
                res = y;
            vreg_write_32(uc, td, xd, slot * 2 + i, res);
        }
    }
}

void do_lasx_emu_xvsigncov_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsigncov.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 4; i++) {
        uint64_t x = vreg_read_64(uc, td, xj, i);
        int64_t y = (int64_t)vreg_read_64(uc, td, xk, i);
        uint64_t res;
        if (x == 0)
            res = 0;
        else if ((int64_t)x < 0)
            res = (uint64_t)(-y);
        else
            res = (uint64_t)y;
        vreg_write_64(uc, td, xd, i, res);
    }
}

// ==================== xvfrstp.b - find first set bit ====================
// XR[xd].B[xk.B[0]%16] = count of non-negative bytes in xj.B[0:15]
// XR[xd].B[xk.B[16]%16] = count of non-negative bytes in xj.B[16:31]
void do_lasx_emu_xvfrstp_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrstp.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int8_t xj_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_8(uc, td, xj, i);
    }
    uint8_t xk_b0 = vreg_read_8(uc, td, xk, 0) & 0xf;
    uint8_t xk_b16 = vreg_read_8(uc, td, xk, 16) & 0xf;

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

    vreg_write_8(uc, td, xd, xk_b0, idx0);
    vreg_write_8(uc, td, xd, xk_b16 + 16, idx1);
}

// ==================== xvfrstp.h - find first set bit ====================
// XR[xd].H[xk.H[0]%8] = count of non-negative halfwords in xj.H[0:7]
// XR[xd].H[xk.H[8]%8] = count of non-negative halfwords in xj.H[8:15]
void do_lasx_emu_xvfrstp_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvfrstp.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }
    uint8_t xk_h0 = vreg_read_16(uc, td, xk, 0) & 0x7;
    uint8_t xk_h8 = vreg_read_16(uc, td, xk, 8) & 0x7;

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

    vreg_write_16(uc, td, xd, xk_h0, idx0);
    vreg_write_16(uc, td, xd, xk_h8 + 8, idx1);
}

// ==================== xvbitseci.b - immediate bit select ====================

// ==================== xvbitseli.b - immediate bit select to all bits ====================
void do_lasx_emu_xvbitseli_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitseli.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0xff;

    uint64_t src_j[4];
    uint64_t src_xd[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j[slot] = vreg_read_64(uc, td, xj, slot);
        src_xd[slot] = vreg_read_64(uc, td, xd, slot);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t xd_byte = (src_xd[slot] >> shift) & 0xff;
            uint8_t xj_byte = (src_j[slot] >> shift) & 0xff;
            uint8_t res = ((~xd_byte) & xj_byte) | (xd_byte & imm);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

// ==================== xvandi.b/xvori.b/xvxori.b/xvnori.b - logical immediate ====================
void do_lasx_emu_xvandi_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvandi.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0xff;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t res = val & imm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

void do_lasx_emu_xvori_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvori.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0xff;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t res = val | imm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

void do_lasx_emu_xvxori_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvxori.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0xff;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t res = val ^ imm;
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

void do_lasx_emu_xvnori_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvnori.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0xff;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (src_j >> shift) & 0xff;
            uint8_t res = ~(val | imm);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }

}

// ==================== xvmepatmsk.v - pattern mask ====================

// ==================== xvsat - saturate ====================
// xvsat.b/h/w/d (signed): clamp to range [-2^n, 2^n-1]
// xvsat.bu/hu/wu/du (unsigned): saturate to [0, 2^(n+1)-1]
// n is the immediate (0-7 for byte, 0-15 for half, 0-31 for word, 0-63 for dword)
static int8_t sat_b(int val, int n)
{
    if (n >= 7)
        return (int8_t)val;
    int8_t max = (1 << n) - 1;
    int8_t min = -(1 << n);
    if (val > max)
        return max;
    if (val < min)
        return min;
    return (int8_t)val;
}

static uint8_t sat_bu(int val, int n)
{
    if (n >= 7)
        return (uint8_t)val;
    uint8_t max = (1 << (n + 1)) - 1;
    return (uint8_t)(val > (int)max ? max : val);
}

static int16_t sat_h(int val, int n)
{
    if (n >= 15)
        return (int16_t)val;
    int16_t max = (1 << n) - 1;
    int16_t min = -(1 << n);
    if (val > max)
        return max;
    if (val < min)
        return min;
    return (int16_t)val;
}

static uint16_t sat_hu(int val, int n)
{
    if (n >= 15)
        return (uint16_t)val;
    uint16_t max = (1 << (n + 1)) - 1;
    return (uint16_t)(val > (int)max ? max : val);
}

static int32_t sat_w(int val, int n)
{
    if (n >= 31)
        return (int32_t)val;
    int32_t max = (1L << n) - 1;
    int32_t min = -(1L << n);
    if (val > max)
        return max;
    if (val < min)
        return min;
    return (int32_t)val;
}

static uint32_t sat_wu(int val, int n)
{
    if (n >= 31)
        return (uint32_t)val;
    uint32_t max = (1U << (n + 1)) - 1;
    uint32_t uval = (uint32_t)val;
    return uval > max ? max : uval;
}

static int64_t sat_d(long long val, int n)
{
    if (n >= 63)
        return (int64_t)val;
    int64_t max = (1LL << n) - 1;
    int64_t min = -(1LL << n);
    if (val > max)
        return max;
    if (val < min)
        return min;
    return (int64_t)val;
}

static uint64_t sat_du(unsigned long long val, int n)
{
    if (n >= 63)
        return (uint64_t)val;
    uint64_t max = (1ULL << (n + 1)) - 1;
    return val > max ? max : val;
}

void do_lasx_emu_xvsat_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0x7;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            int8_t val = (int8_t)((src_j >> shift) & 0xff);
            int8_t res = sat_b(val, n);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)(uint8_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvsat_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            int16_t val = (int16_t)((src_j >> shift) & 0xffff);
            int16_t res = sat_h(val, n);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)(uint16_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvsat_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0x1f;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            int32_t val = (int32_t)((src_j >> shift) & 0xffffffff);
            int32_t res = sat_w(val, n);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)(uint32_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvsat_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, i);
        int64_t val = (int64_t)src_j;
        int64_t res = sat_d(val, n);
        vreg_write_64(uc, td, xd, i, (uint64_t)res);
    }
}

void do_lasx_emu_xvsat_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0x7; // 3-bit immediate for @vv_ui3

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            uint8_t val = (uint8_t)((src_j >> shift) & 0xff);
            uint8_t res = sat_bu(val, n);
            dst &= ~(0xffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvsat_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0xf;

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 4; i++) {
            int shift = i * 16;
            uint16_t val = (uint16_t)((src_j >> shift) & 0xffff);
            uint16_t res = sat_hu(val, n);
            dst &= ~(0xffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvsat_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0x1f; // 5-bit immediate for @vv_ui5

    for (int slot = 0; slot < 4; slot++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, slot);
        uint64_t dst = 0;
        for (int i = 0; i < 2; i++) {
            int shift = i * 32;
            uint32_t val = (uint32_t)((src_j >> shift) & 0xffffffff);
            uint32_t res = sat_wu(val, n);
            dst &= ~(0xffffffffULL << shift);
            dst |= ((uint64_t)res << shift);
        }
        vreg_write_64(uc, td, xd, slot, dst);
    }
}

void do_lasx_emu_xvsat_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsat.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int n = (instr >> 10) & 0x3f;

    for (int i = 0; i < 4; i++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, i);
        uint64_t val = src_j;
        uint64_t res = sat_du(val, n);
        vreg_write_64(uc, td, xd, i, res);
    }
}
