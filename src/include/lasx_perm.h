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

#ifndef LASX_PERM_H
#define LASX_PERM_H

#include <ucontext.h>

// Bit permutation and selection instructions

// Bit selection
void do_lasx_emu_xvbitsel_v(ucontext_t *uc, unsigned int instr);

// Packing
void do_lasx_emu_xvpackev_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpackev_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpackev_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpackev_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvpackod_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpackod_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpackod_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpackod_d(ucontext_t *uc, unsigned int instr);

// Shuffle
void do_lasx_emu_xvshuf_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvshuf_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvshuf_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvshuf_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvperm_w(ucontext_t *uc, unsigned int instr);

// Permute immediate
void do_lasx_emu_xvpermi_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpermi_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpermi_q(ucontext_t *uc, unsigned int instr);

// Pick/replace
void do_lasx_emu_xvpickev_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickev_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickev_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickev_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvpickod_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickod_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickod_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickod_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvilvl_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvilvl_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvilvl_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvilvl_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvilvh_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvilvh_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvilvh_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvilvh_d(ucontext_t *uc, unsigned int instr);

// Shuffle with immediate indices
void do_lasx_emu_xvshuf4i_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvshuf4i_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvshuf4i_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvshuf4i_d(ucontext_t *uc, unsigned int instr);

#endif // LASX_PERM_H
