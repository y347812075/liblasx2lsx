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

#include "lasx_imm.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"

// ==================== xvaddi.bu - Xd[i] = Xj[i] + imm ====================
void do_lasx_emu_xvaddi_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddi.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    uint8_t xj_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
    }

    for (int i = 0; i < 32; i++) {
        vreg_write_8(uc, td, xd, i, xj_b[i] + imm);
    }
}

// ==================== xvaddi.hu - Xd[i] = Xj[i] + imm ====================
void do_lasx_emu_xvaddi_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddi.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint16_t imm = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, xj_h[i] + imm);
    }
}

// ==================== xvaddi.wu - Xd[i] = Xj[i] + imm ====================
void do_lasx_emu_xvaddi_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddi.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint32_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t lo_j = xj_d[slot] & 0xffffffff;
        uint64_t hi_j = (xj_d[slot] >> 32) & 0xffffffff;
        uint64_t result = (hi_j + imm) << 32 | ((lo_j + imm) & 0xffffffff);
        vreg_write_64(uc, td, xd, slot, result);
    }
}

// ==================== xvaddi.du - Xd[i] = Xj[i] + imm ====================
void do_lasx_emu_xvaddi_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvaddi.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint64_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, xj_d[i] + imm);
    }
}

// ==================== xvsubi.bu - Xd[i] = Xj[i] - imm ====================
void do_lasx_emu_xvsubi_bu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubi.bu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint8_t imm = (instr >> 10) & 0x1f;

    uint8_t xj_b[32];
    for (int i = 0; i < 32; i++) {
        xj_b[i] = vreg_read_u8(uc, td, xj, i);
    }

    for (int i = 0; i < 32; i++) {
        vreg_write_8(uc, td, xd, i, xj_b[i] - imm);
    }
}

// ==================== xvsubi.hu - Xd[i] = Xj[i] - imm ====================
void do_lasx_emu_xvsubi_hu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubi.hu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint16_t imm = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_u16(uc, td, xj, i);
    }

    for (int i = 0; i < 16; i++) {
        vreg_write_16(uc, td, xd, i, xj_h[i] - imm);
    }
}

// ==================== xvsubi.wu - Xd[i] = Xj[i] - imm ====================
void do_lasx_emu_xvsubi_wu(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubi.wu %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint32_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int slot = 0; slot < 4; slot++) {
        uint64_t lo_j = xj_d[slot] & 0xffffffff;
        uint64_t hi_j = (xj_d[slot] >> 32) & 0xffffffff;
        uint64_t result = (hi_j - imm) << 32 | ((lo_j - imm) & 0xffffffff);
        vreg_write_64(uc, td, xd, slot, result);
    }
}

// ==================== xvsubi.du - Xd[i] = Xj[i] - imm ====================
void do_lasx_emu_xvsubi_du(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvsubi.du %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    uint64_t imm = (instr >> 10) & 0x1f;

    uint64_t xj_d[4];
    for (int i = 0; i < 4; i++) {
        xj_d[i] = vreg_read_64(uc, td, xj, i);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, xj_d[i] - imm);
    }
}

static uint64_t set_value(uint16_t x)
{
    uint64_t data = 0;
    uint8_t x7 = (x >> 7) & 1;
    uint8_t x6 = (x >> 6) & 1;

    if ((x & 0x1000) == 0) {
        switch ((x >> 10) & 0x3) {
        case 0:
            data = (uint64_t)(x & 0xff);
            data |= data << 8;
            data |= data << 16;
            data |= data << 32;
            break;
        case 1: {
            int16_t v = (int16_t)(x & 0x3ff);
            if (v & 0x200)
                v |= 0xfc00;
            data = (uint64_t)(uint16_t)v;
            data |= ((uint64_t)(uint16_t)v) << 16;
            data |= ((uint64_t)(uint16_t)v) << 32;
            data |= ((uint64_t)(uint16_t)v) << 48;
            break;
        }
        case 2: {
            int32_t v = (int32_t)(x & 0x3ff);
            if (v & 0x200)
                v |= 0xfffffc00;
            data = (uint64_t)(uint32_t)v;
            data |= ((uint64_t)(uint32_t)v) << 32;
            break;
        }
        case 3: {
            int64_t v = (int64_t)(x & 0x3ff);
            if (v & 0x200)
                v |= ~0x3ff;
            data = (uint64_t)v;
            break;
        }
        }
    } else {
        switch ((x >> 8) & 0xf) {
        case 0:
            data = ((uint64_t)(x & 0xff)) << 48;
            data |= ((uint64_t)(x & 0xff)) << 16;
            break;
        case 1:
            data = ((uint64_t)(x & 0xff)) << 40;
            data |= ((uint64_t)(x & 0xff)) << 8;
            break;
        case 2:
            data = ((uint64_t)(x & 0xff)) << 32;
            data |= ((uint64_t)(x & 0xff)) << 16;
            break;
        case 3:
            data = ((uint64_t)(x & 0xff)) << 24;
            data |= ((uint64_t)(x & 0xff));
            break;
        case 4:
            data = ((uint64_t)(x & 0xff)) << 32;
            data |= ((uint64_t)(x & 0xff)) << 24;
            data |= ((uint64_t)(x & 0xff)) << 16;
            data |= ((uint64_t)(x & 0xff)) << 8;
            break;
        case 5:
            data = ((uint64_t)(x & 0xff)) << 28;
            data |= ((uint64_t)(x & 0xff)) << 20;
            data |= ((uint64_t)(x & 0xff)) << 12;
            data |= ((uint64_t)(x & 0xff)) << 4;
            break;
        case 6:
            data = ((uint64_t)(x & 0xff)) << 40;
            data |= ((uint64_t)(x & 0xff)) << 16;
            data |= 0x000000ff00000000ULL;
            data |= 0xff00000000000000ULL;
            break;
        case 7:
            data = ((uint64_t)(x & 0xff)) << 48;
            data |= ((uint64_t)(x & 0xff)) << 16;
            data |= 0x000000000000ffffULL;
            data |= 0xffff000000000000ULL;
            break;
        case 8:
            data = (uint64_t)(x & 0xff);
            data |= data << 8;
            data |= data << 16;
            data |= data << 32;
            break;
        case 9: {
            for (int i = 0; i < 8; i++) {
                uint8_t bit = (x >> (7 - i)) & 1;
                data |= ((uint64_t)bit) << (i * 8);
            }
            break;
        }
        case 10: {
            uint64_t v50 = ((uint64_t)x7) | ((uint64_t)(x6 ^ 1) << 1) | ((uint64_t)(x6 ? 0x1f : 0) << 2) | ((uint64_t)(x & 0x3f) << 7);
            data = v50 | (v50 << 32);
            break;
        }
        case 11: {
            uint64_t v50 = ((uint64_t)x7) | ((uint64_t)(x6 ^ 1) << 1) | ((uint64_t)(x6 ? 0x1f : 0) << 2) | ((uint64_t)(x & 0x3f) << 7);
            data = v50 | 0x8000000000000000ULL;
            break;
        }
        case 12: {
            data = ((uint64_t)x7) | ((uint64_t)(x6 ^ 1) << 1) | ((uint64_t)(x6 ? 0xff : 0) << 2) | ((uint64_t)(x & 0x3f) << 10);
            break;
        }
        default: break;
        }
    }
    return data;
}

void do_lasx_emu_xvldi(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvldi %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    uint16_t raw_imm = (instr >> 5) & 0x1fff;

    uint64_t data = set_value(raw_imm);

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, data);
    }
}
