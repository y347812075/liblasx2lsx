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

#ifndef LASX_SHIFT_H
#define LASX_SHIFT_H

#include <stdint.h>
#include <ucontext.h>
#include "lasx_shift_helpers.h"

// Variable bit shift by vector register
void do_lasx_emu_xvbsll_v(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvbsrl_v(ucontext_t* uc, unsigned int instr);

// Shift by vector instructions
void do_lasx_emu_xvsll_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsll_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsll_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsll_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrl_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrl_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrl_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrl_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsra_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsra_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsra_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsra_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvrotr_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvrotr_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvrotr_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvrotr_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrlr_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlr_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlr_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlr_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrar_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrar_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrar_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrar_d(ucontext_t* uc, unsigned int instr);

// Narrowing shift instructions
void do_lasx_emu_xvsrln_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrln_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrln_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsran_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsran_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsran_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrlrn_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlrn_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlrn_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrarn_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrarn_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrarn_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrln_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrln_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrln_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssran_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssran_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssran_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrlrn_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrn_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrn_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrarn_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarn_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarn_w_d(ucontext_t* uc, unsigned int instr);

// Unsigned narrowing shift instructions
void do_lasx_emu_xvssrln_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrln_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrln_wu_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssran_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssran_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssran_wu_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrlrn_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrn_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrn_wu_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrarn_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarn_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarn_wu_d(ucontext_t* uc, unsigned int instr);

// Shift by immediate instructions
void do_lasx_emu_xvrotri_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvrotri_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvrotri_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvrotri_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrlri_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlri_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlri_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlri_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrari_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrari_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrari_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrari_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvslli_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvslli_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvslli_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvslli_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrli_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrli_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrli_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrli_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrai_b(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrai_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrai_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrai_d(ucontext_t* uc, unsigned int instr);

// Narrowing shift by immediate instructions
void do_lasx_emu_xvsrlni_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlni_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlni_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrlrni_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlrni_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlrni_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrlni_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlni_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlni_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrlni_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlni_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlni_wu_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrlrni_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrni_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrni_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrlrni_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrni_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrni_wu_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrani_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrani_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrani_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvsrarni_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrarni_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrarni_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrani_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrani_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrani_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrarni_b_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarni_h_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarni_w_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrani_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrani_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrani_wu_d(ucontext_t* uc, unsigned int instr);

void do_lasx_emu_xvssrarni_bu_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarni_hu_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarni_wu_d(ucontext_t* uc, unsigned int instr);

// Narrowing shift by immediate instructions q.d format
void do_lasx_emu_xvsrlni_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrlrni_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlni_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlni_du_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrni_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrlrni_du_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrani_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvsrarni_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrani_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarni_d_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrani_du_q(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvssrarni_du_q(ucontext_t* uc, unsigned int instr);

#endif // LASX_SHIFT_H
