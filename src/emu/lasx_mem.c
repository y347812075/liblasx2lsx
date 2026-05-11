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

#include "lasx_mem.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

void do_lasx_emu_xvld(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvld %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int off = (instr >> 10) & 0xfff;
    off = (off << 20) >> 20;
    unsigned long long mem = UC_GPR(uc, rj);
    long long* p = (void*)(mem + off);

    vreg_write_64(uc, td, xd, 0, p[0]);
    vreg_write_64(uc, td, xd, 1, p[1]);
    vreg_write_64(uc, td, xd, 2, p[2]);
    vreg_write_64(uc, td, xd, 3, p[3]);

}

void do_lasx_emu_xvst(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvst %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int off = (instr >> 10) & 0xfff;
    off = (off << 20) >> 20;
    unsigned long long mem = UC_GPR(uc, rj);
    long long* p = (void*)(mem + off);

    p[0] = vreg_read_64(uc, td, xd, 0);
    p[1] = vreg_read_64(uc, td, xd, 1);
    p[2] = vreg_read_64(uc, td, xd, 2);
    p[3] = vreg_read_64(uc, td, xd, 3);

}

void do_lasx_emu_xvldrepl_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvldrepl.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int off = (instr >> 10) & 0xffc;
    off = (off << 20) >> 20;
    unsigned long long mem = UC_GPR(uc, rj) + off;
    uint8_t val = *(uint8_t*)mem;

    for (int i = 0; i < 32; i++) {
        vreg_write_8(uc, td, xd, i, val);
    }

}

void do_lasx_emu_xvldrepl_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvldrepl.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int off = (instr >> 10) & 0x7ff;
    off = (off << 21) >> 21;
    unsigned long long mem = UC_GPR(uc, rj) + (off << 1);
    uint16_t val = *(uint16_t*)mem;

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, val);
    }

}

void do_lasx_emu_xvldrepl_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvldrepl.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int off = (instr >> 10) & 0x3ff;
    off = (off << 22) >> 22;
    unsigned long long mem = UC_GPR(uc, rj) + (off << 2);
    uint32_t val = *(uint32_t*)mem;

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, val);
    }

}

void do_lasx_emu_xvldrepl_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvldrepl.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int off = (instr >> 10) & 0x1ff;
    off = (off << 23) >> 23;
    unsigned long long mem = UC_GPR(uc, rj) + (off << 3);
    uint64_t val = *(uint64_t*)mem;

    vreg_write_64(uc, td, xd, 0, val);
    vreg_write_64(uc, td, xd, 1, val);
    vreg_write_64(uc, td, xd, 2, val);
    vreg_write_64(uc, td, xd, 3, val);

}

void do_lasx_emu_xvstelm_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvstelm.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int si8 = (int8_t)(instr >> 10);
    int idx = (instr >> 18) & 0x1f;
    unsigned long long mem = UC_GPR(uc, rj) + si8;
    uint8_t* p = (void*)(mem);
    uint64_t val = vreg_read_64(uc, td, xd, idx / 8);
    int shift = (idx % 8) * 8;
    *p = (uint8_t)(val >> shift);
}

void do_lasx_emu_xvstelm_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvstelm.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int si8 = (int8_t)(instr >> 10);
    int idx = (instr >> 18) & 0xf;
    unsigned long long mem = UC_GPR(uc, rj) + (si8 << 1);
    uint16_t* p = (void*)(mem);
    uint64_t val = vreg_read_64(uc, td, xd, idx / 4);
    int shift = (idx % 4) * 16;
    *p = (uint16_t)(val >> shift);
}

void do_lasx_emu_xvstelm_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvstelm.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int si8 = (int8_t)(instr >> 10);
    int idx = (instr >> 18) & 0x7;
    unsigned long long mem = UC_GPR(uc, rj) + (si8 << 2);
    uint32_t* p = (void*)(mem);
    uint64_t val = vreg_read_64(uc, td, xd, idx / 2);
    int shift = (idx % 2) * 32;
    *p = (uint32_t)(val >> shift);
}

void do_lasx_emu_xvstelm_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvstelm.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    int si8 = (int8_t)(instr >> 10);
    int idx = (instr >> 18) & 0x3;
    unsigned long long mem = UC_GPR(uc, rj) + (si8 << 3);
    uint64_t* p = (void*)(mem);
    *p = vreg_read_64(uc, td, xd, idx);
}

void do_lasx_emu_xvldx(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvldx %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int rk = (instr >> 10) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    unsigned long long mem = UC_GPR(uc, rj) + UC_GPR(uc, rk);
    uint64_t* p = (void*)mem;

    vreg_write_64(uc, td, xd, 0, p[0]);
    vreg_write_64(uc, td, xd, 1, p[1]);
    vreg_write_64(uc, td, xd, 2, p[2]);
    vreg_write_64(uc, td, xd, 3, p[3]);

}

void do_lasx_emu_xvstx(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvstx %08x\n", (unsigned long)UC_PC(uc), instr);

    int rj = (instr >> 5) & 0x1f;
    int rk = (instr >> 10) & 0x1f;
    int xd = (instr >> 0) & 0x1f;
    unsigned long long mem = UC_GPR(uc, rj) + UC_GPR(uc, rk);
    uint64_t* p = (void*)mem;

    p[0] = vreg_read_64(uc, td, xd, 0);
    p[1] = vreg_read_64(uc, td, xd, 1);
    p[2] = vreg_read_64(uc, td, xd, 2);
    p[3] = vreg_read_64(uc, td, xd, 3);

}
