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

#ifndef LASX_BITMANIP_H
#define LASX_BITMANIP_H

#include <ucontext.h>

// Bit clear/set/reverse by position
void do_lasx_emu_xvbitclr_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitclr_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitclr_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitclr_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvbitset_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitset_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitset_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitset_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvbitrev_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitrev_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitrev_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitrev_d(ucontext_t *uc, unsigned int instr);

// Bit clear/set/reverse by immediate position
void do_lasx_emu_xvbitclri_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitclri_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitclri_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitclri_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvbitseti_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitseti_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitseti_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitseti_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvbitrevi_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitrevi_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitrevi_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvbitrevi_d(ucontext_t *uc, unsigned int instr);

// Sign mask coverage
void do_lasx_emu_xvsigncov_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsigncov_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsigncov_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsigncov_d(ucontext_t *uc, unsigned int instr);

// Find first set bit
void do_lasx_emu_xvfrstp_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvfrstp_h(ucontext_t *uc, unsigned int instr);

// Immediate bit select
void do_lasx_emu_xvbitseli_b(ucontext_t *uc, unsigned int instr);

// Logical immediate
void do_lasx_emu_xvandi_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvori_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvxori_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvnori_b(ucontext_t *uc, unsigned int instr);

// Vector mask

// Saturate
void do_lasx_emu_xvsat_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsat_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsat_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsat_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvsat_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsat_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsat_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsat_du(ucontext_t *uc, unsigned int instr);

#endif // LASX_BITMANIP_H
