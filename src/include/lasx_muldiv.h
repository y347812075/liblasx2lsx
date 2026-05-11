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

#ifndef LASX_MULDIV_H
#define LASX_MULDIV_H

#include <ucontext.h>

// Multiply/Divide instructions
// Basic multiply
void do_lasx_emu_xvmul_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmul_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmul_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmul_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmuh_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmuh_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmuh_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmuh_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmuh_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmuh_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmuh_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmuh_du(ucontext_t *uc, unsigned int instr);

// Multiply extend
void do_lasx_emu_xvmulwev_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_q_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmulwod_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_q_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmulwev_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_q_du(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmulwod_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_q_du(ucontext_t *uc, unsigned int instr);



// Multiply extend unsigned double extension - even indexed
void do_lasx_emu_xvmulwev_h_bu_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_w_hu_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_d_wu_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwev_q_du_d(ucontext_t *uc, unsigned int instr);

// Multiply extend unsigned double extension - odd indexed
void do_lasx_emu_xvmulwod_h_bu_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_w_hu_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_d_wu_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmulwod_q_du_d(ucontext_t *uc, unsigned int instr);

// Multiply-add/sub
void do_lasx_emu_xvmadd_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmadd_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmadd_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmadd_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmsub_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmsub_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmsub_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmsub_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmaddwev_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_q_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmaddwod_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_q_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmaddwev_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_q_du(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmaddwod_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_q_du(ucontext_t *uc, unsigned int instr);



// Multiply-add extend unsigned double extension - even indexed
void do_lasx_emu_xvmaddwev_h_bu_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_w_hu_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_d_wu_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwev_q_du_d(ucontext_t *uc, unsigned int instr);

// Multiply-add extend unsigned double extension - odd indexed
void do_lasx_emu_xvmaddwod_h_bu_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_w_hu_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_d_wu_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaddwod_q_du_d(ucontext_t *uc, unsigned int instr);

// Division/Modulo
void do_lasx_emu_xvdiv_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvdiv_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvdiv_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvdiv_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmod_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmod_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmod_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmod_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvdiv_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvdiv_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvdiv_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvdiv_du(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmod_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmod_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmod_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmod_du(ucontext_t *uc, unsigned int instr);

#endif // LASX_MULDIV_H
