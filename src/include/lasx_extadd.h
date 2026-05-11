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

#ifndef LASX_EXTADD_H
#define LASX_EXTADD_H

#include <ucontext.h>

// Extended add/subtract instructions - compare-predicate
void do_lasx_emu_xvseq_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvseq_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvseq_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvseq_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvsle_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsle_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslt_du(ucontext_t *uc, unsigned int instr);

// Add/sub with writeback
void do_lasx_emu_xvaddwev_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_q_d(ucontext_t *uc, unsigned int instr);

// Add/sub with writeback and unsigned
void do_lasx_emu_xvaddwev_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_q_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwev_q_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_q_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_h_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_w_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_d_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsubwod_q_du(ucontext_t *uc, unsigned int instr);

// Add/sub with writeback and bundling
void do_lasx_emu_xvaddwev_h_bu_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_w_hu_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_d_wu_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwev_q_du_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_h_bu_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_w_hu_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_d_wu_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvaddwod_q_du_d(ucontext_t *uc, unsigned int instr);

// Saturating add/sub
void do_lasx_emu_xvsadd_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsadd_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvssub_du(ucontext_t *uc, unsigned int instr);

// Half extension add/sub
void do_lasx_emu_xvhaddw_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_h_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_w_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_d_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_q_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_hu_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_wu_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_du_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhaddw_qu_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_hu_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_wu_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_du_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvhsubw_qu_du(ucontext_t *uc, unsigned int instr);

// Accumulate
void do_lasx_emu_xvadda_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvadda_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvadda_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvadda_d(ucontext_t *uc, unsigned int instr);

// Absolute difference
void do_lasx_emu_xvabsd_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvabsd_du(ucontext_t *uc, unsigned int instr);

// Average
void do_lasx_emu_xvavg_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavg_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvavgr_du(ucontext_t *uc, unsigned int instr);

#endif // LASX_EXTADD_H
