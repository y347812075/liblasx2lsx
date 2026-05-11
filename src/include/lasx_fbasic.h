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

#ifndef LASX_FBASIC_H
#define LASX_FBASIC_H

#include <ucontext.h>

// Basic arithmetic floating point instructions
void do_lasx_emu_xvfadd_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfadd_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfsub_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfsub_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmul_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmul_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfdiv_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfdiv_d(ucontext_t* uc, unsigned int instr);

// Max/min floating point instructions
void do_lasx_emu_xvfmax_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmax_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmin_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmin_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmaxa_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmaxa_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmina_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfmina_d(ucontext_t* uc, unsigned int instr);

// Special floating point instructions
void do_lasx_emu_xvfsqrt_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfsqrt_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrecip_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrecip_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrsqrt_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrsqrt_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvflogb_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvflogb_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfclass_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfclass_d(ucontext_t* uc, unsigned int instr);

// Round floating point instructions
void do_lasx_emu_xvfrint_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrint_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrm_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrm_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrp_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrp_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrz_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrz_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrne_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrintrne_d(ucontext_t* uc, unsigned int instr);

// Convert floating point instructions
void do_lasx_emu_xvfcvt_h_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfcvt_s_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfcvtl_s_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfcvth_s_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfcvtl_d_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfcvth_d_s(ucontext_t* uc, unsigned int instr);

// Fixed <-> float conversion instructions
void do_lasx_emu_xvffint_s_l(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftint_w_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvffint_s_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvffint_d_l(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvffintl_d_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftint_w_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrm_w_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrm_w_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrm_l_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrp_w_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrp_w_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrp_l_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrz_w_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrz_w_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrz_l_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrne_w_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrne_w_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrne_l_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvffint_s_wu(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvffint_d_lu(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftint_wu_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintl_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftinth_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrml_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftint_lu_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrmh_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrph_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrnel_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvffinth_d_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrecipe_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrecipe_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrsqrte_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvfrsqrte_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftint_l_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrz_lu_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrz_wu_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrneh_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrpl_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrzh_l_s(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvftintrzl_l_s(ucontext_t* uc, unsigned int instr);

// Extract instructions
void do_lasx_emu_xvexth_w_h(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvexth_d_w(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvexth_q_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvexth_hu_bu(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvexth_wu_hu(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvexth_du_wu(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvexth_qu_du(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvextl_q_d(ucontext_t* uc, unsigned int instr);
void do_lasx_emu_xvextl_qu_du(ucontext_t* uc, unsigned int instr);

#endif // LASX_FBASIC_H
