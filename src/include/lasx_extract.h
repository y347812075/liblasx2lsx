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

#ifndef LASX_EXTRACT_H
#define LASX_EXTRACT_H

#include <ucontext.h>

// Extract half instructions
void do_lasx_emu_xvexth_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_hu_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_wu_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_du_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvexth_qu_du(ucontext_t *uc, unsigned int instr);

// Extract lower instructions
void do_lasx_emu_xvextl_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvextl_qu_du(ucontext_t *uc, unsigned int instr);

// Extract insert instructions
void do_lasx_emu_xvextrins_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvextrins_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvextrins_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvextrins_b(ucontext_t *uc, unsigned int instr);

// Extract from general purpose register to LASX
void do_lasx_emu_vext2xv_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_w_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_d_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_d_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_hu_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_wu_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_du_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_wu_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_du_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_vext2xv_du_wu(ucontext_t *uc, unsigned int instr);

// Move GPR to LASX - replicate to all elements
void do_lasx_emu_xvreplgr2vr_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplgr2vr_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplgr2vr_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplgr2vr_d(ucontext_t *uc, unsigned int instr);

// Replicate even elements
void do_lasx_emu_xvreplve_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve_d(ucontext_t *uc, unsigned int instr);

// Pick from LASX to GPR
void do_lasx_emu_xvpickve2gr_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickve2gr_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickve2gr_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickve2gr_du(ucontext_t *uc, unsigned int instr);

// Pick from LASX vector
void do_lasx_emu_xvpickve_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpickve_d(ucontext_t *uc, unsigned int instr);

// Insert GPR with zero extension
void do_lasx_emu_xvinsve0_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvinsve0_d(ucontext_t *uc, unsigned int instr);

// Insert GPR into vector element
void do_lasx_emu_xvinsgr2vr_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvinsgr2vr_d(ucontext_t *uc, unsigned int instr);

// Replicate 128-bit element by immediate index to entire vector
void do_lasx_emu_xvrepl128vei_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvrepl128vei_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvrepl128vei_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvrepl128vei_d(ucontext_t *uc, unsigned int instr);

// Replicate element 0 to entire vector
void do_lasx_emu_xvreplve0_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve0_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve0_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve0_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvreplve0_q(ucontext_t *uc, unsigned int instr);

// Shift left widening instructions
void do_lasx_emu_xvsllwil_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsllwil_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsllwil_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsllwil_hu_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsllwil_wu_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsllwil_du_wu(ucontext_t *uc, unsigned int instr);

#endif // LASX_EXTRACT_H
