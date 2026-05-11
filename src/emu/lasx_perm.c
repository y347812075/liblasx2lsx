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

#include "lasx_perm.h"
#include "debug.h"
#include "lasx_emu_private.h"
#include "thread_data.h"
#include "lasx_interpret.h"

// ==================== xvbitsel.v ====================
void do_lasx_emu_xvbitsel_v(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvbitsel.v %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    uint64_t res[4] = {0};
    for (int bit = 0; bit < 256; bit++) {
        int elem = bit / 64;
        int bit_in_elem = bit % 64;

        // Get the select bit from Xa
        uint64_t xa_elem = vreg_read_64(uc, td, xa, elem);
        int sel = (xa_elem >> bit_in_elem) & 1;

        // Get the source bit from Xj or Xk
        uint64_t src_elem;
        if (sel)
            src_elem = vreg_read_64(uc, td, xk, elem);
        else
            src_elem = vreg_read_64(uc, td, xj, elem);
        int src_bit = (src_elem >> bit_in_elem) & 1;

        // Set the result bit
        if (src_bit)
            res[elem] |= (1ULL << bit_in_elem);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, res[i]);
    }
}

// ==================== xvpackev.b ====================
void do_lasx_emu_xvpackev_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackev.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t dst[4] = {0, 0, 0, 0};
    for (int i = 0; i < 16; i++) {
        int src_byte = i * 2;
        int dst_byte_idx_even = (i * 2) / 8;
        int dst_shift_even = (i * 2) % 8 * 8;
        int src_byte_idx_even = src_byte / 8;
        int src_shift_even = (src_byte % 8) * 8;

        uint64_t src_k_even = vreg_read_64(uc, td, xk, src_byte_idx_even);
        uint8_t b_even = (src_k_even >> src_shift_even) & 0xff;

        dst[dst_byte_idx_even] &= ~(0xffULL << dst_shift_even);
        dst[dst_byte_idx_even] |= ((uint64_t)b_even << dst_shift_even);

        int dst_byte_idx_odd = (i * 2 + 1) / 8;
        int dst_shift_odd = (i * 2 + 1) % 8 * 8;
        int src_byte_idx_odd = src_byte / 8;
        int src_shift_odd = (src_byte % 8) * 8;

        uint64_t src_j_odd = vreg_read_64(uc, td, xj, src_byte_idx_odd);
        uint8_t b_odd = (src_j_odd >> src_shift_odd) & 0xff;

        dst[dst_byte_idx_odd] &= ~(0xffULL << dst_shift_odd);
        dst[dst_byte_idx_odd] |= ((uint64_t)b_odd << dst_shift_odd);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvpackev.h ====================
void do_lasx_emu_xvpackev_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackev.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int src_half = i * 2;
        int dst_half_idx_even = i * 2;
        int dst_half_idx_odd = i * 2 + 1;
        int src_half_idx = src_half / 4;
        int src_shift = (src_half % 4) * 16;

        uint64_t src_k = vreg_read_64(uc, td, xk, src_half_idx);
        uint64_t src_j = vreg_read_64(uc, td, xj, src_half_idx);
        uint16_t h_even = (src_k >> src_shift) & 0xffff;
        uint16_t h_odd = (src_j >> src_shift) & 0xffff;

        vreg_write_16(uc, td, xd, dst_half_idx_even, h_even);
        vreg_write_16(uc, td, xd, dst_half_idx_odd, h_odd);
    }
}

// ==================== xvpackev.w ====================
void do_lasx_emu_xvpackev_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackev.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 4; i++) {
        int src_word = i * 2;
        int dst_word_idx_even = i * 2;
        int dst_word_idx_odd = i * 2 + 1;
        int src_word_idx = src_word / 2;
        int src_shift = (src_word % 2) * 32;

        uint64_t src_k = vreg_read_64(uc, td, xk, src_word_idx);
        uint64_t src_j = vreg_read_64(uc, td, xj, src_word_idx);
        uint32_t w_even = (src_k >> src_shift) & 0xffffffff;
        uint32_t w_odd = (src_j >> src_shift) & 0xffffffff;

        vreg_write_32(uc, td, xd, dst_word_idx_even, w_even);
        vreg_write_32(uc, td, xd, dst_word_idx_odd, w_odd);
    }
}

// ==================== xvpackev.d ====================
void do_lasx_emu_xvpackev_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackev.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    vreg_write_64(uc, td, xd, 0, vreg_read_64(uc, td, xk, 0));
    vreg_write_64(uc, td, xd, 1, vreg_read_64(uc, td, xj, 0));
    vreg_write_64(uc, td, xd, 2, vreg_read_64(uc, td, xk, 2));
    vreg_write_64(uc, td, xd, 3, vreg_read_64(uc, td, xj, 2));
}

// ==================== xvpackod.b ====================
void do_lasx_emu_xvpackod_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackod.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t dst[4] = {vreg_read_64(uc, td, xd, 0), vreg_read_64(uc, td, xd, 1), vreg_read_64(uc, td, xd, 2), vreg_read_64(uc, td, xd, 3)};

    for (int i = 0; i < 16; i++) {
        int src_byte = i * 2 + 1;
        int dst_byte_idx_even = (i * 2) / 8;
        int dst_shift_even = (i * 2) % 8 * 8;
        int src_byte_idx_even = src_byte / 8;
        int src_shift_even = (src_byte % 8) * 8;

        uint64_t src_k_even = vreg_read_64(uc, td, xk, src_byte_idx_even);
        uint8_t b_even = (src_k_even >> src_shift_even) & 0xff;

        dst[dst_byte_idx_even] &= ~(0xffULL << dst_shift_even);
        dst[dst_byte_idx_even] |= ((uint64_t)b_even << dst_shift_even);

        int dst_byte_idx_odd = (i * 2 + 1) / 8;
        int dst_shift_odd = (i * 2 + 1) % 8 * 8;
        int src_byte_idx_odd = src_byte / 8;
        int src_shift_odd = (src_byte % 8) * 8;

        uint64_t src_j_odd = vreg_read_64(uc, td, xj, src_byte_idx_odd);
        uint8_t b_odd = (src_j_odd >> src_shift_odd) & 0xff;

        dst[dst_byte_idx_odd] &= ~(0xffULL << dst_shift_odd);
        dst[dst_byte_idx_odd] |= ((uint64_t)b_odd << dst_shift_odd);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, dst[i]);
    }
}

// ==================== xvpackod.h ====================
void do_lasx_emu_xvpackod_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackod.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 8; i++) {
        int src_half = i * 2 + 1;
        int dst_half_idx_even = i * 2;
        int dst_half_idx_odd = i * 2 + 1;
        int src_half_idx = src_half / 4;
        int src_shift = (src_half % 4) * 16;

        uint64_t src_k = vreg_read_64(uc, td, xk, src_half_idx);
        uint64_t src_j = vreg_read_64(uc, td, xj, src_half_idx);
        uint16_t h_even = (src_k >> src_shift) & 0xffff;
        uint16_t h_odd = (src_j >> src_shift) & 0xffff;

        vreg_write_16(uc, td, xd, dst_half_idx_even, h_even);
        vreg_write_16(uc, td, xd, dst_half_idx_odd, h_odd);
    }
}

// ==================== xvpackod.w ====================
void do_lasx_emu_xvpackod_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackod.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    for (int i = 0; i < 4; i++) {
        int src_word = i * 2 + 1;
        int dst_word_idx_even = i * 2;
        int dst_word_idx_odd = i * 2 + 1;
        int src_word_idx = src_word / 2;
        int src_shift = (src_word % 2) * 32;

        uint64_t src_k = vreg_read_64(uc, td, xk, src_word_idx);
        uint64_t src_j = vreg_read_64(uc, td, xj, src_word_idx);
        uint32_t w_even = (src_k >> src_shift) & 0xffffffff;
        uint32_t w_odd = (src_j >> src_shift) & 0xffffffff;

        vreg_write_32(uc, td, xd, dst_word_idx_even, w_even);
        vreg_write_32(uc, td, xd, dst_word_idx_odd, w_odd);
    }
}

// ==================== xvpackod.d ====================
void do_lasx_emu_xvpackod_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpackod.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    vreg_write_64(uc, td, xd, 0, vreg_read_64(uc, td, xk, 1));
    vreg_write_64(uc, td, xd, 1, vreg_read_64(uc, td, xj, 1));
    vreg_write_64(uc, td, xd, 2, vreg_read_64(uc, td, xk, 3));
    vreg_write_64(uc, td, xd, 3, vreg_read_64(uc, td, xj, 3));
}

// ==================== xvilvl.b ====================
void do_lasx_emu_xvilvl_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvl.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int dst_idx = i * 2;
        int slot = i / 8;
        uint64_t val_j = (src_j_cache[slot] >> ((i % 8) * 8)) & 0xff;
        uint64_t val_k = (src_k_cache[slot] >> ((i % 8) * 8)) & 0xff;

        vreg_write_8(uc, td, xd, dst_idx, val_k);
        vreg_write_8(uc, td, xd, dst_idx + 1, val_j);
    }

    for (int i = 0; i < 8; i++) {
        int dst_idx = 16 + i * 2;
        int slot = (i + 16) / 8;
        uint64_t val_j = (src_j_cache[slot] >> (((i + 16) % 8) * 8)) & 0xff;
        uint64_t val_k = (src_k_cache[slot] >> (((i + 16) % 8) * 8)) & 0xff;

        vreg_write_8(uc, td, xd, dst_idx, val_k);
        vreg_write_8(uc, td, xd, dst_idx + 1, val_j);
    }
}

// ==================== xvilvl.h ====================
void do_lasx_emu_xvilvl_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvl.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int dst_idx = i * 2;
        int slot = i / 4;
        uint16_t val_j = (src_j_cache[slot] >> ((i % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> ((i % 4) * 16)) & 0xffff;

        vreg_write_16(uc, td, xd, dst_idx, val_k);
        vreg_write_16(uc, td, xd, dst_idx + 1, val_j);
    }

    for (int i = 0; i < 4; i++) {
        int dst_idx = 8 + i * 2;
        int slot = (i + 8) / 4;
        uint16_t val_j = (src_j_cache[slot] >> (((i + 8) % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> (((i + 8) % 4) * 16)) & 0xffff;

        vreg_write_16(uc, td, xd, dst_idx, val_k);
        vreg_write_16(uc, td, xd, dst_idx + 1, val_j);
    }
}

// ==================== xvilvl.w ====================
void do_lasx_emu_xvilvl_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvl.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 2; i++) {
        int dst_idx = i * 2;
        int slot = i / 2;
        uint32_t val_j = (src_j_cache[slot] >> ((i % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> ((i % 2) * 32)) & 0xffffffff;

        vreg_write_32(uc, td, xd, dst_idx, val_k);
        vreg_write_32(uc, td, xd, dst_idx + 1, val_j);
    }

    for (int i = 0; i < 2; i++) {
        int dst_idx = 4 + i * 2;
        int slot = (i + 4) / 2;
        uint32_t val_j = (src_j_cache[slot] >> (((i + 4) % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> (((i + 4) % 2) * 32)) & 0xffffffff;

        vreg_write_32(uc, td, xd, dst_idx, val_k);
        vreg_write_32(uc, td, xd, dst_idx + 1, val_j);
    }
}

// ==================== xvilvl.d ====================
void do_lasx_emu_xvilvl_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvl.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    vreg_write_64(uc, td, xd, 0, vreg_read_64(uc, td, xk, 0));
    vreg_write_64(uc, td, xd, 1, vreg_read_64(uc, td, xj, 0));
    vreg_write_64(uc, td, xd, 2, vreg_read_64(uc, td, xk, 2));
    vreg_write_64(uc, td, xd, 3, vreg_read_64(uc, td, xj, 2));
}

// ==================== xvilvh.b ====================
void do_lasx_emu_xvilvh_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvh.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int dst_idx = i * 2;
        int slot = (i + 8) / 8;
        uint8_t val_j = (src_j_cache[slot] >> (((i + 8) % 8) * 8)) & 0xff;
        uint8_t val_k = (src_k_cache[slot] >> (((i + 8) % 8) * 8)) & 0xff;

        vreg_write_8(uc, td, xd, dst_idx, val_k);
        vreg_write_8(uc, td, xd, dst_idx + 1, val_j);
    }

    for (int i = 0; i < 8; i++) {
        int dst_idx = 16 + i * 2;
        int slot = (i + 24) / 8;
        uint8_t val_j = (src_j_cache[slot] >> (((i + 24) % 8) * 8)) & 0xff;
        uint8_t val_k = (src_k_cache[slot] >> (((i + 24) % 8) * 8)) & 0xff;

        vreg_write_8(uc, td, xd, dst_idx, val_k);
        vreg_write_8(uc, td, xd, dst_idx + 1, val_j);
    }
}

// ==================== xvilvh.h ====================
void do_lasx_emu_xvilvh_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvh.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int dst_idx = i * 2;
        int slot = (i + 4) / 4;
        uint16_t val_j = (src_j_cache[slot] >> (((i + 4) % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> (((i + 4) % 4) * 16)) & 0xffff;

        vreg_write_16(uc, td, xd, dst_idx, val_k);
        vreg_write_16(uc, td, xd, dst_idx + 1, val_j);
    }

    for (int i = 0; i < 4; i++) {
        int dst_idx = 8 + i * 2;
        int slot = (i + 12) / 4;
        uint16_t val_j = (src_j_cache[slot] >> (((i + 12) % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> (((i + 12) % 4) * 16)) & 0xffff;

        vreg_write_16(uc, td, xd, dst_idx, val_k);
        vreg_write_16(uc, td, xd, dst_idx + 1, val_j);
    }
}

// ==================== xvilvh.w ====================
void do_lasx_emu_xvilvh_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvh.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 2; i++) {
        int dst_idx = i * 2;
        int slot = (i + 2) / 2;
        uint32_t val_j = (src_j_cache[slot] >> (((i + 2) % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> (((i + 2) % 2) * 32)) & 0xffffffff;

        vreg_write_32(uc, td, xd, dst_idx, val_k);
        vreg_write_32(uc, td, xd, dst_idx + 1, val_j);
    }

    for (int i = 0; i < 2; i++) {
        int dst_idx = 4 + i * 2;
        int slot = (i + 6) / 2;
        uint32_t val_j = (src_j_cache[slot] >> (((i + 6) % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> (((i + 6) % 2) * 32)) & 0xffffffff;

        vreg_write_32(uc, td, xd, dst_idx, val_k);
        vreg_write_32(uc, td, xd, dst_idx + 1, val_j);
    }
}

// ==================== xvilvh.d ====================
void do_lasx_emu_xvilvh_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvilvh.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    vreg_write_64(uc, td, xd, 0, vreg_read_64(uc, td, xk, 1));
    vreg_write_64(uc, td, xd, 1, vreg_read_64(uc, td, xj, 1));
    vreg_write_64(uc, td, xd, 2, vreg_read_64(uc, td, xk, 3));
    vreg_write_64(uc, td, xd, 3, vreg_read_64(uc, td, xj, 3));
}

// ==================== xvpickev.b ====================
void do_lasx_emu_xvpickev_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickev.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int src_byte = i * 2;
        int slot = src_byte / 8;
        uint8_t val_j = (src_j_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        uint8_t val_k = (src_k_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        vreg_write_8(uc, td, xd, i + 8, val_j);
        vreg_write_8(uc, td, xd, i, val_k);
    }

    for (int i = 0; i < 8; i++) {
        int src_byte = i * 2 + 16;
        int slot = src_byte / 8;
        uint8_t val_j = (src_j_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        uint8_t val_k = (src_k_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        vreg_write_8(uc, td, xd, i + 24, val_j);
        vreg_write_8(uc, td, xd, i + 16, val_k);
    }
}

// ==================== xvpickev.h ====================
void do_lasx_emu_xvpickev_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickev.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int src_half = i * 2;
        int slot = src_half / 4;
        uint16_t val_j = (src_j_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        vreg_write_16(uc, td, xd, i + 4, val_j);
        vreg_write_16(uc, td, xd, i, val_k);
    }

    for (int i = 0; i < 4; i++) {
        int src_half = i * 2 + 8;
        int slot = src_half / 4;
        uint16_t val_j = (src_j_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        vreg_write_16(uc, td, xd, i + 12, val_j);
        vreg_write_16(uc, td, xd, i + 8, val_k);
    }
}

// ==================== xvpickev.w ====================
void do_lasx_emu_xvpickev_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickev.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 2; i++) {
        int src_word = i * 2;
        int slot = src_word / 2;
        uint32_t val_j = (src_j_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        vreg_write_32(uc, td, xd, i + 2, val_j);
        vreg_write_32(uc, td, xd, i, val_k);
    }

    for (int i = 0; i < 2; i++) {
        int src_word = i * 2 + 4;
        int slot = src_word / 2;
        uint32_t val_j = (src_j_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        vreg_write_32(uc, td, xd, i + 6, val_j);
        vreg_write_32(uc, td, xd, i + 4, val_k);
    }
}

// ==================== xvpickev.d ====================
void do_lasx_emu_xvpickev_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickev.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    vreg_write_64(uc, td, xd, 0, vreg_read_64(uc, td, xk, 0));
    vreg_write_64(uc, td, xd, 1, vreg_read_64(uc, td, xj, 0));
    vreg_write_64(uc, td, xd, 2, vreg_read_64(uc, td, xk, 2));
    vreg_write_64(uc, td, xd, 3, vreg_read_64(uc, td, xj, 2));
}

// ==================== xvpickod.b ====================
void do_lasx_emu_xvpickod_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickod.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 8; i++) {
        int src_byte = i * 2 + 1;
        int slot = src_byte / 8;
        uint8_t val_j = (src_j_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        uint8_t val_k = (src_k_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        vreg_write_8(uc, td, xd, i + 8, val_j);
        vreg_write_8(uc, td, xd, i, val_k);
    }

    for (int i = 0; i < 8; i++) {
        int src_byte = i * 2 + 17;
        int slot = src_byte / 8;
        uint8_t val_j = (src_j_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        uint8_t val_k = (src_k_cache[slot] >> ((src_byte % 8) * 8)) & 0xff;
        vreg_write_8(uc, td, xd, i + 24, val_j);
        vreg_write_8(uc, td, xd, i + 16, val_k);
    }
}

// ==================== xvpickod.h ====================
void do_lasx_emu_xvpickod_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickod.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 4; i++) {
        int src_half = i * 2 + 1;
        int slot = src_half / 4;
        uint16_t val_j = (src_j_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        vreg_write_16(uc, td, xd, i + 4, val_j);
        vreg_write_16(uc, td, xd, i, val_k);
    }

    for (int i = 0; i < 4; i++) {
        int src_half = i * 2 + 9;
        int slot = src_half / 4;
        uint16_t val_j = (src_j_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        uint16_t val_k = (src_k_cache[slot] >> ((src_half % 4) * 16)) & 0xffff;
        vreg_write_16(uc, td, xd, i + 12, val_j);
        vreg_write_16(uc, td, xd, i + 8, val_k);
    }
}

// ==================== xvpickod.w ====================
void do_lasx_emu_xvpickod_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickod.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t src_j_cache[4];
    uint64_t src_k_cache[4];
    for (int slot = 0; slot < 4; slot++) {
        src_j_cache[slot] = vreg_read_64(uc, td, xj, slot);
        src_k_cache[slot] = vreg_read_64(uc, td, xk, slot);
    }

    for (int i = 0; i < 2; i++) {
        int src_word = i * 2 + 1;
        int slot = src_word / 2;
        uint32_t val_j = (src_j_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        vreg_write_32(uc, td, xd, i + 2, val_j);
        vreg_write_32(uc, td, xd, i, val_k);
    }

    for (int i = 0; i < 2; i++) {
        int src_word = i * 2 + 5;
        int slot = src_word / 2;
        uint32_t val_j = (src_j_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        uint32_t val_k = (src_k_cache[slot] >> ((src_word % 2) * 32)) & 0xffffffff;
        vreg_write_32(uc, td, xd, i + 6, val_j);
        vreg_write_32(uc, td, xd, i + 4, val_k);
    }
}

// ==================== xvpickod.d ====================
void do_lasx_emu_xvpickod_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpickod.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    vreg_write_64(uc, td, xd, 0, vreg_read_64(uc, td, xk, 1));
    vreg_write_64(uc, td, xd, 1, vreg_read_64(uc, td, xj, 1));
    vreg_write_64(uc, td, xd, 2, vreg_read_64(uc, td, xk, 3));
    vreg_write_64(uc, td, xd, 3, vreg_read_64(uc, td, xj, 3));
}

// ==================== xvshuf.b ====================
void do_lasx_emu_xvshuf_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    uint64_t xj_d[4] = {vreg_read_64(uc, td, xj, 0), vreg_read_64(uc, td, xj, 1), vreg_read_64(uc, td, xj, 2), vreg_read_64(uc, td, xj, 3)};
    uint64_t xk_d[4] = {vreg_read_64(uc, td, xk, 0), vreg_read_64(uc, td, xk, 1), vreg_read_64(uc, td, xk, 2), vreg_read_64(uc, td, xk, 3)};
    uint64_t xa_d[4] = {vreg_read_64(uc, td, xa, 0), vreg_read_64(uc, td, xa, 1), vreg_read_64(uc, td, xa, 2), vreg_read_64(uc, td, xa, 3)};

    for (int i = 0; i < 32; i++) {
        int half = i < 16 ? 0 : 1;
        int sel_word_idx = half * 2 + ((i % 16) / 8);
        int sel_byte_offset = (i % 8) * 8;
        int sel = (xa_d[sel_word_idx] >> sel_byte_offset) & 0xff;
        int k = sel % 32;
        uint8_t src_byte_val;

        if (k < 16) {
            int src_idx = k + half * 16;
            int src_dword = src_idx / 8;
            int bit_shift = (src_idx % 8) * 8;
            src_byte_val = (xk_d[src_dword] >> bit_shift) & 0xff;
        } else {
            int src_idx = k - 16 + half * 16;
            int src_dword = src_idx / 8;
            int bit_shift = (src_idx % 8) * 8;
            src_byte_val = (xj_d[src_dword] >> bit_shift) & 0xff;
        }
        vreg_write_8(uc, td, xd, i, src_byte_val);
    }
}

// ==================== xvshuf.h ====================
void do_lasx_emu_xvshuf_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint16_t xj_h[16];
    uint16_t xk_h[16];
    uint16_t xd_h[16];
    uint16_t vec0[16];
    uint16_t vec1[16];

    for (int i = 0; i < 16; i++) {
        xd_h[i] = vreg_read_16(uc, td, xd, i);
    }
    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
        xk_h[i] = vreg_read_16(uc, td, xk, i);
    }
    for (int i = 0; i < 8; i++) {
        vec0[i] = xk_h[i];
        vec0[i + 8] = xj_h[i];
        vec1[i] = xk_h[i + 8];
        vec1[i + 8] = xj_h[i + 8];
    }
    for (int i = 0; i < 8; i++) {
        vreg_write_16(uc, td, xd, i, vec0[(xd_h[i] & 0x0f)]);
        vreg_write_16(uc, td, xd, i + 8, vec1[(xd_h[i + 8] & 0x0f)]);
    }
}

// ==================== xvshuf.w ====================
void do_lasx_emu_xvshuf_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t xj_w[8];
    uint32_t xk_w[8];
    uint32_t xd_w[8];
    uint32_t vec0[8];
    uint32_t vec1[8];

    for (int i = 0; i < 8; i++) {
        xd_w[i] = vreg_read_32(uc, td, xd, i);
    }
    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
        xk_w[i] = vreg_read_32(uc, td, xk, i);
    }
    for (int i = 0; i < 4; i++) {
        vec0[i] = xk_w[i];
        vec0[i + 4] = xj_w[i];
        vec1[i] = xk_w[i + 4];
        vec1[i + 4] = xj_w[i + 4];
    }
    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, vec0[(xd_w[i] & 0x07)]);
        vreg_write_32(uc, td, xd, i + 4, vec1[(xd_w[i + 4] & 0x07)]);
    }
}

// ==================== xvshuf.d ====================
void do_lasx_emu_xvshuf_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    if (!td)
        return;

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint64_t xj_d[4] = {vreg_read_64(uc, td, xj, 0), vreg_read_64(uc, td, xj, 1), vreg_read_64(uc, td, xj, 2), vreg_read_64(uc, td, xj, 3)};
    uint64_t xk_d[4] = {vreg_read_64(uc, td, xk, 0), vreg_read_64(uc, td, xk, 1), vreg_read_64(uc, td, xk, 2), vreg_read_64(uc, td, xk, 3)};
    uint64_t xd_d[4] = {vreg_read_64(uc, td, xd, 0), vreg_read_64(uc, td, xd, 1), vreg_read_64(uc, td, xd, 2), vreg_read_64(uc, td, xd, 3)};

    uint64_t vec0[4] = {xk_d[0], xk_d[1], xj_d[0], xj_d[1]};
    uint64_t vec1[4] = {xk_d[2], xk_d[3], xj_d[2], xj_d[3]};

    vreg_write_64(uc, td, xd, 0, vec0[(xd_d[0] & 0x03)]);
    vreg_write_64(uc, td, xd, 1, vec0[(xd_d[1] & 0x03)]);
    vreg_write_64(uc, td, xd, 2, vec1[(xd_d[2] & 0x03)]);
    vreg_write_64(uc, td, xd, 3, vec1[(xd_d[3] & 0x03)]);
}

// ==================== xvperm.w ====================
void do_lasx_emu_xvperm_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvperm.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    uint32_t src_val[8];
    for (int i = 0; i < 8; i++) {
        uint32_t sel = vreg_read_32(uc, td, xk, i);
        uint8_t sel_val = sel & 0x7;

        src_val[i] = vreg_read_32(uc, td, xj, sel_val);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, src_val[i]);
    }
}

// ==================== xvshuf4i.b ====================
void do_lasx_emu_xvshuf4i_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf4i.b %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int ui8 = (instr >> 10) & 0xff;

    uint8_t vec[8][4];
    uint32_t sel[4];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            vec[i][j] = vreg_read_8(uc, td, xj, i * 4 + j);
        }
    }
    for (int i = 0; i < 4; i++) {
        sel[i] = (ui8 >> (2 * i)) & 0x3;
    }
    for (int i = 0; i < 4; i++) {
        vreg_write_8(uc, td, xd, i, vec[0][sel[i]]);
        vreg_write_8(uc, td, xd, i + 4, vec[1][sel[i]]);
        vreg_write_8(uc, td, xd, i + 8, vec[2][sel[i]]);
        vreg_write_8(uc, td, xd, i + 12, vec[3][sel[i]]);
        vreg_write_8(uc, td, xd, i + 16, vec[4][sel[i]]);
        vreg_write_8(uc, td, xd, i + 20, vec[5][sel[i]]);
        vreg_write_8(uc, td, xd, i + 24, vec[6][sel[i]]);
        vreg_write_8(uc, td, xd, i + 28, vec[7][sel[i]]);
    }
}

// ==================== xvshuf4i.h ====================
void do_lasx_emu_xvshuf4i_h(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf4i.h %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int ui8 = (instr >> 10) & 0xff;

    uint16_t xj_h[16];
    uint16_t vec0[4];
    uint16_t vec1[4];
    uint16_t vec2[4];
    uint16_t vec3[4];
    uint32_t sel[4];

    for (int i = 0; i < 16; i++) {
        xj_h[i] = vreg_read_16(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        vec0[i] = xj_h[i];
        vec1[i] = xj_h[i + 4];
        vec2[i] = xj_h[i + 8];
        vec3[i] = xj_h[i + 12];
        sel[i] = (ui8 >> (2 * i)) & 0x3;
    }
    for (int i = 0; i < 4; i++) {
        vreg_write_16(uc, td, xd, i, vec0[sel[i]]);
        vreg_write_16(uc, td, xd, i + 4, vec1[sel[i]]);
        vreg_write_16(uc, td, xd, i + 8, vec2[sel[i]]);
        vreg_write_16(uc, td, xd, i + 12, vec3[sel[i]]);
    }
}

// ==================== xvshuf4i.w ====================
void do_lasx_emu_xvshuf4i_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf4i.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int ui8 = (instr >> 10) & 0xff;

    uint32_t xj_w[8];
    uint32_t vec0[4];
    uint32_t vec1[4];
    uint32_t sel[4];

    for (int i = 0; i < 8; i++) {
        xj_w[i] = vreg_read_32(uc, td, xj, i);
    }
    for (int i = 0; i < 4; i++) {
        vec0[i] = xj_w[i];
        vec1[i] = xj_w[i + 4];
        sel[i] = (ui8 >> (2 * i)) & 0x3;
    }
    for (int i = 0; i < 4; i++) {
        vreg_write_32(uc, td, xd, i, vec0[sel[i]]);
        vreg_write_32(uc, td, xd, i + 4, vec1[sel[i]]);
    }
}

// ==================== xvshuf4i.d ====================
void do_lasx_emu_xvshuf4i_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvshuf4i.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int ui8 = (instr >> 10) & 0xff;

    uint64_t xj_d[4] = {vreg_read_64(uc, td, xj, 0), vreg_read_64(uc, td, xj, 1), vreg_read_64(uc, td, xj, 2), vreg_read_64(uc, td, xj, 3)};
    uint64_t xd_d[4] = {vreg_read_64(uc, td, xd, 0), vreg_read_64(uc, td, xd, 1), vreg_read_64(uc, td, xd, 2), vreg_read_64(uc, td, xd, 3)};

    uint64_t vec0[4] = {xd_d[0], xd_d[1], xj_d[0], xj_d[1]};
    uint64_t vec1[4] = {xd_d[2], xd_d[3], xj_d[2], xj_d[3]};
    int sel0 = ui8 & 0x3;
    int sel1 = (ui8 >> 2) & 0x3;

    vreg_write_64(uc, td, xd, 0, vec0[sel0]);
    vreg_write_64(uc, td, xd, 1, vec0[sel1]);
    vreg_write_64(uc, td, xd, 2, vec1[sel0]);
    vreg_write_64(uc, td, xd, 3, vec1[sel1]);
}

// ==================== xvpermi.w ====================
void do_lasx_emu_xvpermi_w(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpermi.w %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk8 = (instr >> 10) & 0xff;

    uint8_t sel[4];
    uint32_t temp[8];

    for (int i = 0; i < 4; i++) {
        sel[i] = (uk8 >> (i * 2)) & 0x3;
    }

    for (int i = 0; i < 4; i++) {
        temp[i] = vreg_read_32(uc, td, (i < 2) ? xj : xd, sel[i]);
    }

    for (int i = 0; i < 4; i++) {
        temp[i + 4] = vreg_read_32(uc, td, (i < 2) ? xj : xd, sel[i] + 4);
    }

    for (int i = 0; i < 8; i++) {
        vreg_write_32(uc, td, xd, i, temp[i]);
    }

}

// ==================== xvpermi.d ====================
void do_lasx_emu_xvpermi_d(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpermi.d %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk8 = (instr >> 10) & 0xff;

    uint8_t sel[4];
    uint64_t temp[4];
    for (int i = 0; i < 4; i++) {
        sel[i] = (uk8 >> (i * 2)) & 0x3;
    }

    for (int i = 0; i < 4; i++) {
        temp[i] = vreg_read_64(uc, td, xj, sel[i]);
    }

    for (int i = 0; i < 4; i++) {
        vreg_write_64(uc, td, xd, i, temp[i]);
    }

}

// ==================== xvpermi.q ====================
void do_lasx_emu_xvpermi_q(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();
    tdlog("%16lx : emulate xvpermi.q %08x\n", (unsigned long)UC_PC(uc), instr);

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int uk8 = (instr >> 10) & 0xff;

    uint8_t sel[2];
    uint64_t temp[4];
    for (int i = 0; i < 2; i++) {
        sel[i] = (uk8 >> (i * 4)) & 0x3;
    }

    for (int i = 0; i < 2; i++) {
        if (sel[i] < 2)
            vreg_read_128(uc, td, xj, sel[i] & 1, &temp[2 * i], &temp[2 * i + 1]);
        else
            vreg_read_128(uc, td, xd, sel[i] & 1, &temp[2 * i], &temp[2 * i + 1]);
    }

    for (int i = 0; i < 2; i++) {
        vreg_write_128(uc, td, xd, i, temp[2 * i], temp[2 * i + 1]);
    }

}
