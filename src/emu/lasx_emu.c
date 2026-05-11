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

#include "lasx_emu.h"
#include "lasx_arith.h"
#include "lasx_bitmanip.h"
#include "lasx_cmp.h"
#include "lasx_emu_private.h"
#include "lasx_extadd.h"
#include "lasx_extract.h"
#include "lasx_fbasic.h"
#include "lasx_fcmp.h"
#include "lasx_fma.h"
#include "lasx_imm.h"
#include "lasx_logic.h"
#include "lasx_mem.h"
#include "lasx_muldiv.h"
#include "lasx_perm.h"
#include "lasx_shift.h"
#include "lasx_interpret.h"
#include "perf_stats.h"
#include <signal.h>
#include <stdio.h>
#include <ucontext.h>

const char *lasx_version(void)
{
    return LIBLASX2LSX_VERSION;
}

int do_lasx_emu(ucontext_t* uc, unsigned int instr)
{
    perf_stats_print();
    switch (instr >> 18) {
    case OP14_XVPERMI_W: perf_inc(P_XVPERMI_W); do_lasx_emu_xvpermi_w(uc, instr); return 1;
    case OP14_XVPERMI_D: perf_inc(P_XVPERMI_D); do_lasx_emu_xvpermi_d(uc, instr); return 1;
    case OP14_XVPERMI_Q: perf_inc(P_XVPERMI_Q); do_lasx_emu_xvpermi_q(uc, instr); return 1;
    case OP14_XVLDI: perf_inc(P_XVLDI); do_lasx_emu_xvldi(uc, instr); return 1;
    case OP14_XVANDI_B: perf_inc(P_XVANDI_B); do_lasx_emu_xvandi_b(uc, instr); return 1;
    case OP14_XVBITSELI_B: perf_inc(P_XVBITSELI_B); do_lasx_emu_xvbitseli_b(uc, instr); return 1;
    case OP14_XVEXTRINS_B: perf_inc(P_XVEXTRINS_B); do_lasx_emu_xvextrins_b(uc, instr); return 1;
    case OP14_XVEXTRINS_D: perf_inc(P_XVEXTRINS_D); do_lasx_emu_xvextrins_d(uc, instr); return 1;
    case OP14_XVEXTRINS_H: perf_inc(P_XVEXTRINS_H); do_lasx_emu_xvextrins_h(uc, instr); return 1;
    case OP14_XVEXTRINS_W: perf_inc(P_XVEXTRINS_W); do_lasx_emu_xvextrins_w(uc, instr); return 1;
    case OP14_XVSHUF4I_B: perf_inc(P_XVSHUF4I_B); do_lasx_emu_xvshuf4i_b(uc, instr); return 1;
    case OP14_XVSHUF4I_D: perf_inc(P_XVSHUF4I_D); do_lasx_emu_xvshuf4i_d(uc, instr); return 1;
    case OP14_XVSHUF4I_H: perf_inc(P_XVSHUF4I_H); do_lasx_emu_xvshuf4i_h(uc, instr); return 1;
    case OP14_XVSHUF4I_W: perf_inc(P_XVSHUF4I_W); do_lasx_emu_xvshuf4i_w(uc, instr); return 1;
    case OP14_XVORI_B: perf_inc(P_XVORI_B); do_lasx_emu_xvori_b(uc, instr); return 1;
    case OP14_XVXORI_B: perf_inc(P_XVXORI_B); do_lasx_emu_xvxori_b(uc, instr); return 1;
    case OP14_XVNORI_B: perf_inc(P_XVNORI_B); do_lasx_emu_xvnori_b(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 20) {
    case OP12_XVFMADD_S: perf_inc(P_XVFMADD_S); do_lasx_emu_xvfmadd_s(uc, instr); return 1;
    case OP12_XVFMADD_D: perf_inc(P_XVFMADD_D); do_lasx_emu_xvfmadd_d(uc, instr); return 1;
    case OP12_XVFNMADD_S: perf_inc(P_XVFNMADD_S); do_lasx_emu_xvfnmadd_s(uc, instr); return 1;
    case OP12_XVFNMADD_D: perf_inc(P_XVFNMADD_D); do_lasx_emu_xvfnmadd_d(uc, instr); return 1;
    case OP12_XVFNMSUB_S: perf_inc(P_XVFNMSUB_S); do_lasx_emu_xvfnmsub_s(uc, instr); return 1;
    case OP12_XVFNMSUB_D: perf_inc(P_XVFNMSUB_D); do_lasx_emu_xvfnmsub_d(uc, instr); return 1;
    case OP12_XVFCMP_COND_S: perf_inc(P_XVFCMP_COND_S); do_lasx_emu_xvfcmp_cond_s(uc, instr); return 1;
    case OP12_XVFCMP_COND_D: perf_inc(P_XVFCMP_COND_D); do_lasx_emu_xvfcmp_cond_d(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 15) {
    case OP17_XVLDX: perf_inc(P_XVLDX); do_lasx_emu_xvldx(uc, instr); return 1;
    case OP17_XVSTX: perf_inc(P_XVSTX); do_lasx_emu_xvstx(uc, instr); return 1;
    case OP17_XVPACKOD_B: perf_inc(P_XVPACKOD_B); do_lasx_emu_xvpackod_b(uc, instr); return 1;
    case OP17_XVPACKOD_H: perf_inc(P_XVPACKOD_H); do_lasx_emu_xvpackod_h(uc, instr); return 1;
    case OP17_XVPACKOD_W: perf_inc(P_XVPACKOD_W); do_lasx_emu_xvpackod_w(uc, instr); return 1;
    case OP17_XVPACKOD_D: perf_inc(P_XVPACKOD_D); do_lasx_emu_xvpackod_d(uc, instr); return 1;
    case OP17_XVILVL_B: perf_inc(P_XVILVL_B); do_lasx_emu_xvilvl_b(uc, instr); return 1;
    case OP17_XVILVL_H: perf_inc(P_XVILVL_H); do_lasx_emu_xvilvl_h(uc, instr); return 1;
    case OP17_XVILVL_W: perf_inc(P_XVILVL_W); do_lasx_emu_xvilvl_w(uc, instr); return 1;
    case OP17_XVILVL_D: perf_inc(P_XVILVL_D); do_lasx_emu_xvilvl_d(uc, instr); return 1;
    case OP17_XVILVH_B: perf_inc(P_XVILVH_B); do_lasx_emu_xvilvh_b(uc, instr); return 1;
    case OP17_XVILVH_H: perf_inc(P_XVILVH_H); do_lasx_emu_xvilvh_h(uc, instr); return 1;
    case OP17_XVILVH_W: perf_inc(P_XVILVH_W); do_lasx_emu_xvilvh_w(uc, instr); return 1;
    case OP17_XVILVH_D: perf_inc(P_XVILVH_D); do_lasx_emu_xvilvh_d(uc, instr); return 1;
    case OP17_XVADD_B: perf_inc(P_XVADD_B); do_lasx_emu_xvadd_b(uc, instr); return 1;
    case OP17_XVADD_H: perf_inc(P_XVADD_H); do_lasx_emu_xvadd_h(uc, instr); return 1;
    case OP17_XVADD_W: perf_inc(P_XVADD_W); do_lasx_emu_xvadd_w(uc, instr); return 1;
    case OP17_XVADD_D: perf_inc(P_XVADD_D); do_lasx_emu_xvadd_d(uc, instr); return 1;
    case OP17_XVADD_Q: perf_inc(P_XVADD_Q); do_lasx_emu_xvadd_q(uc, instr); return 1;
    case OP17_XVSUB_B: perf_inc(P_XVSUB_B); do_lasx_emu_xvsub_b(uc, instr); return 1;
    case OP17_XVSUB_H: perf_inc(P_XVSUB_H); do_lasx_emu_xvsub_h(uc, instr); return 1;
    case OP17_XVSUB_W: perf_inc(P_XVSUB_W); do_lasx_emu_xvsub_w(uc, instr); return 1;
    case OP17_XVSUB_D: perf_inc(P_XVSUB_D); do_lasx_emu_xvsub_d(uc, instr); return 1;
    case OP17_XVSUB_Q: perf_inc(P_XVSUB_Q); do_lasx_emu_xvsub_q(uc, instr); return 1;
    case OP17_XVSSUB_B: perf_inc(P_XVSSUB_B); do_lasx_emu_xvssub_b(uc, instr); return 1;
    case OP17_XVSSUB_H: perf_inc(P_XVSSUB_H); do_lasx_emu_xvssub_h(uc, instr); return 1;
    case OP17_XVSSUB_W: perf_inc(P_XVSSUB_W); do_lasx_emu_xvssub_w(uc, instr); return 1;
    case OP17_XVSSUB_D: perf_inc(P_XVSSUB_D); do_lasx_emu_xvssub_d(uc, instr); return 1;
    case OP17_XVSSUB_BU: perf_inc(P_XVSSUB_BU); do_lasx_emu_xvssub_bu(uc, instr); return 1;
    case OP17_XVSSUB_HU: perf_inc(P_XVSSUB_HU); do_lasx_emu_xvssub_hu(uc, instr); return 1;
    case OP17_XVSSUB_WU: perf_inc(P_XVSSUB_WU); do_lasx_emu_xvssub_wu(uc, instr); return 1;
    case OP17_XVSSUB_DU: perf_inc(P_XVSSUB_DU); do_lasx_emu_xvssub_du(uc, instr); return 1;
    case OP17_XVSADD_B: perf_inc(P_XVSADD_B); do_lasx_emu_xvsadd_b(uc, instr); return 1;
    case OP17_XVSADD_H: perf_inc(P_XVSADD_H); do_lasx_emu_xvsadd_h(uc, instr); return 1;
    case OP17_XVSADD_W: perf_inc(P_XVSADD_W); do_lasx_emu_xvsadd_w(uc, instr); return 1;
    case OP17_XVSADD_D: perf_inc(P_XVSADD_D); do_lasx_emu_xvsadd_d(uc, instr); return 1;
    case OP17_XVSADD_BU: perf_inc(P_XVSADD_BU); do_lasx_emu_xvsadd_bu(uc, instr); return 1;
    case OP17_XVSADD_HU: perf_inc(P_XVSADD_HU); do_lasx_emu_xvsadd_hu(uc, instr); return 1;
    case OP17_XVSADD_WU: perf_inc(P_XVSADD_WU); do_lasx_emu_xvsadd_wu(uc, instr); return 1;
    case OP17_XVADDI_BU: perf_inc(P_XVADDI_BU); do_lasx_emu_xvaddi_bu(uc, instr); return 1;
    case OP17_XVADDI_HU: perf_inc(P_XVADDI_HU); do_lasx_emu_xvaddi_hu(uc, instr); return 1;
    case OP17_XVADDI_WU: perf_inc(P_XVADDI_WU); do_lasx_emu_xvaddi_wu(uc, instr); return 1;
    case OP17_XVADDI_DU: perf_inc(P_XVADDI_DU); do_lasx_emu_xvaddi_du(uc, instr); return 1;
    case OP17_XVSUBI_BU: perf_inc(P_XVSUBI_BU); do_lasx_emu_xvsubi_bu(uc, instr); return 1;
    case OP17_XVSUBI_HU: perf_inc(P_XVSUBI_HU); do_lasx_emu_xvsubi_hu(uc, instr); return 1;
    case OP17_XVSUBI_WU: perf_inc(P_XVSUBI_WU); do_lasx_emu_xvsubi_wu(uc, instr); return 1;
    case OP17_XVSUBI_DU: perf_inc(P_XVSUBI_DU); do_lasx_emu_xvsubi_du(uc, instr); return 1;
    case OP17_XVMUL_B: perf_inc(P_XVMUL_B); do_lasx_emu_xvmul_b(uc, instr); return 1;
    case OP17_XVMUL_H: perf_inc(P_XVMUL_H); do_lasx_emu_xvmul_h(uc, instr); return 1;
    case OP17_XVMUL_W: perf_inc(P_XVMUL_W); do_lasx_emu_xvmul_w(uc, instr); return 1;
    case OP17_XVMUL_D: perf_inc(P_XVMUL_D); do_lasx_emu_xvmul_d(uc, instr); return 1;
    case OP17_XVMUH_B: perf_inc(P_XVMUH_B); do_lasx_emu_xvmuh_b(uc, instr); return 1;
    case OP17_XVMUH_H: perf_inc(P_XVMUH_H); do_lasx_emu_xvmuh_h(uc, instr); return 1;
    case OP17_XVMUH_W: perf_inc(P_XVMUH_W); do_lasx_emu_xvmuh_w(uc, instr); return 1;
    case OP17_XVMUH_DU: perf_inc(P_XVMUH_DU); do_lasx_emu_xvmuh_du(uc, instr); return 1;
    case OP17_XVMADD_B: perf_inc(P_XVMADD_B); do_lasx_emu_xvmadd_b(uc, instr); return 1;
    case OP17_XVMADD_H: perf_inc(P_XVMADD_H); do_lasx_emu_xvmadd_h(uc, instr); return 1;
    case OP17_XVMADD_W: perf_inc(P_XVMADD_W); do_lasx_emu_xvmadd_w(uc, instr); return 1;
    case OP17_XVMADD_D: perf_inc(P_XVMADD_D); do_lasx_emu_xvmadd_d(uc, instr); return 1;
    case OP17_XVMSUB_B: perf_inc(P_XVMSUB_B); do_lasx_emu_xvmsub_b(uc, instr); return 1;
    case OP17_XVMSUB_H: perf_inc(P_XVMSUB_H); do_lasx_emu_xvmsub_h(uc, instr); return 1;
    case OP17_XVMSUB_W: perf_inc(P_XVMSUB_W); do_lasx_emu_xvmsub_w(uc, instr); return 1;
    case OP17_XVMSUB_D: perf_inc(P_XVMSUB_D); do_lasx_emu_xvmsub_d(uc, instr); return 1;
    case OP17_XVDIV_B: perf_inc(P_XVDIV_B); do_lasx_emu_xvdiv_b(uc, instr); return 1;
    case OP17_XVDIV_H: perf_inc(P_XVDIV_H); do_lasx_emu_xvdiv_h(uc, instr); return 1;
    case OP17_XVDIV_W: perf_inc(P_XVDIV_W); do_lasx_emu_xvdiv_w(uc, instr); return 1;
    case OP17_XVDIV_D: perf_inc(P_XVDIV_D); do_lasx_emu_xvdiv_d(uc, instr); return 1;
    case OP17_XVMOD_B: perf_inc(P_XVMOD_B); do_lasx_emu_xvmod_b(uc, instr); return 1;
    case OP17_XVMOD_H: perf_inc(P_XVMOD_H); do_lasx_emu_xvmod_h(uc, instr); return 1;
    case OP17_XVMOD_W: perf_inc(P_XVMOD_W); do_lasx_emu_xvmod_w(uc, instr); return 1;
    case OP17_XVMOD_D: perf_inc(P_XVMOD_D); do_lasx_emu_xvmod_d(uc, instr); return 1;
    case OP17_XVDIV_BU: perf_inc(P_XVDIV_BU); do_lasx_emu_xvdiv_bu(uc, instr); return 1;
    case OP17_XVDIV_HU: perf_inc(P_XVDIV_HU); do_lasx_emu_xvdiv_hu(uc, instr); return 1;
    case OP17_XVDIV_WU: perf_inc(P_XVDIV_WU); do_lasx_emu_xvdiv_wu(uc, instr); return 1;
    case OP17_XVDIV_DU: perf_inc(P_XVDIV_DU); do_lasx_emu_xvdiv_du(uc, instr); return 1;
    case OP17_XVMOD_BU: perf_inc(P_XVMOD_BU); do_lasx_emu_xvmod_bu(uc, instr); return 1;
    case OP17_XVMOD_HU: perf_inc(P_XVMOD_HU); do_lasx_emu_xvmod_hu(uc, instr); return 1;
    case OP17_XVMOD_WU: perf_inc(P_XVMOD_WU); do_lasx_emu_xvmod_wu(uc, instr); return 1;
    case OP17_XVMOD_DU: perf_inc(P_XVMOD_DU); do_lasx_emu_xvmod_du(uc, instr); return 1;
    case OP17_XVAND_V: perf_inc(P_XVAND_V); do_lasx_emu_xvand_v(uc, instr); return 1;
    case OP17_XVOR_V: perf_inc(P_XVOR_V); do_lasx_emu_xvor_v(uc, instr); return 1;
    case OP17_XVXOR_V: perf_inc(P_XVXOR_V); do_lasx_emu_xvxor_v(uc, instr); return 1;
    case OP17_XVNOR_V: perf_inc(P_XVNOR_V); do_lasx_emu_xvnor_v(uc, instr); return 1;
    case OP17_XVANDN_V: perf_inc(P_XVANDN_V); do_lasx_emu_xvandn_v(uc, instr); return 1;
    case OP17_XVORN_V: perf_inc(P_XVORN_V); do_lasx_emu_xvorn_v(uc, instr); return 1;
    case OP17_XVMULWEV_H_B: perf_inc(P_XVMULWEV_H_B); do_lasx_emu_xvmulwev_h_b(uc, instr); return 1;
    case OP17_XVMULWEV_W_H: perf_inc(P_XVMULWEV_W_H); do_lasx_emu_xvmulwev_w_h(uc, instr); return 1;
    case OP17_XVMULWEV_D_W: perf_inc(P_XVMULWEV_D_W); do_lasx_emu_xvmulwev_d_w(uc, instr); return 1;
    case OP17_XVMULWEV_Q_D: perf_inc(P_XVMULWEV_Q_D); do_lasx_emu_xvmulwev_q_d(uc, instr); return 1;
    case OP17_XVMULWOD_H_B: perf_inc(P_XVMULWOD_H_B); do_lasx_emu_xvmulwod_h_b(uc, instr); return 1;
    case OP17_XVMULWOD_W_H: perf_inc(P_XVMULWOD_W_H); do_lasx_emu_xvmulwod_w_h(uc, instr); return 1;
    case OP17_XVMULWOD_D_W: perf_inc(P_XVMULWOD_D_W); do_lasx_emu_xvmulwod_d_w(uc, instr); return 1;
    case OP17_XVMULWOD_Q_D: perf_inc(P_XVMULWOD_Q_D); do_lasx_emu_xvmulwod_q_d(uc, instr); return 1;
    case OP17_XVMULWEV_H_BU: perf_inc(P_XVMULWEV_H_BU); do_lasx_emu_xvmulwev_h_bu(uc, instr); return 1;
    case OP17_XVMULWEV_W_HU: perf_inc(P_XVMULWEV_W_HU); do_lasx_emu_xvmulwev_w_hu(uc, instr); return 1;
    case OP17_XVMULWEV_D_WU: perf_inc(P_XVMULWEV_D_WU); do_lasx_emu_xvmulwev_d_wu(uc, instr); return 1;
    case OP17_XVMULWEV_Q_DU: perf_inc(P_XVMULWEV_Q_DU); do_lasx_emu_xvmulwev_q_du(uc, instr); return 1;
    case OP17_XVMULWOD_H_BU: perf_inc(P_XVMULWOD_H_BU); do_lasx_emu_xvmulwod_h_bu(uc, instr); return 1;
    case OP17_XVMULWOD_W_HU: perf_inc(P_XVMULWOD_W_HU); do_lasx_emu_xvmulwod_w_hu(uc, instr); return 1;
    case OP17_XVMULWOD_D_WU: perf_inc(P_XVMULWOD_D_WU); do_lasx_emu_xvmulwod_d_wu(uc, instr); return 1;
    case OP17_XVMULWOD_Q_DU: perf_inc(P_XVMULWOD_Q_DU); do_lasx_emu_xvmulwod_q_du(uc, instr); return 1;
    case OP17_XVMULWEV_H_BU_B: perf_inc(P_XVMULWEV_H_BU_B); do_lasx_emu_xvmulwev_h_bu_b(uc, instr); return 1;
    case OP17_XVMULWEV_W_HU_H: perf_inc(P_XVMULWEV_W_HU_H); do_lasx_emu_xvmulwev_w_hu_h(uc, instr); return 1;
    case OP17_XVMULWEV_D_WU_W: perf_inc(P_XVMULWEV_D_WU_W); do_lasx_emu_xvmulwev_d_wu_w(uc, instr); return 1;
    case OP17_XVMULWEV_Q_DU_D: perf_inc(P_XVMULWEV_Q_DU_D); do_lasx_emu_xvmulwev_q_du_d(uc, instr); return 1;
    case OP17_XVMULWOD_H_BU_B: perf_inc(P_XVMULWOD_H_BU_B); do_lasx_emu_xvmulwod_h_bu_b(uc, instr); return 1;
    case OP17_XVMULWOD_W_HU_H: perf_inc(P_XVMULWOD_W_HU_H); do_lasx_emu_xvmulwod_w_hu_h(uc, instr); return 1;
    case OP17_XVMULWOD_D_WU_W: perf_inc(P_XVMULWOD_D_WU_W); do_lasx_emu_xvmulwod_d_wu_w(uc, instr); return 1;
    case OP17_XVMULWOD_Q_DU_D: perf_inc(P_XVMULWOD_Q_DU_D); do_lasx_emu_xvmulwod_q_du_d(uc, instr); return 1;
    case OP17_XVMADDWEV_H_B: perf_inc(P_XVMADDWEV_H_B); do_lasx_emu_xvmaddwev_h_b(uc, instr); return 1;
    case OP17_XVMADDWEV_W_H: perf_inc(P_XVMADDWEV_W_H); do_lasx_emu_xvmaddwev_w_h(uc, instr); return 1;
    case OP17_XVMADDWEV_D_W: perf_inc(P_XVMADDWEV_D_W); do_lasx_emu_xvmaddwev_d_w(uc, instr); return 1;
    case OP17_XVMADDWEV_Q_D: perf_inc(P_XVMADDWEV_Q_D); do_lasx_emu_xvmaddwev_q_d(uc, instr); return 1;
    case OP17_XVMADDWOD_H_B: perf_inc(P_XVMADDWOD_H_B); do_lasx_emu_xvmaddwod_h_b(uc, instr); return 1;
    case OP17_XVMADDWOD_W_H: perf_inc(P_XVMADDWOD_W_H); do_lasx_emu_xvmaddwod_w_h(uc, instr); return 1;
    case OP17_XVMADDWOD_D_W: perf_inc(P_XVMADDWOD_D_W); do_lasx_emu_xvmaddwod_d_w(uc, instr); return 1;
    case OP17_XVMADDWOD_Q_D: perf_inc(P_XVMADDWOD_Q_D); do_lasx_emu_xvmaddwod_q_d(uc, instr); return 1;
    case OP17_XVMADDWEV_H_BU: perf_inc(P_XVMADDWEV_H_BU); do_lasx_emu_xvmaddwev_h_bu(uc, instr); return 1;
    case OP17_XVMADDWEV_W_HU: perf_inc(P_XVMADDWEV_W_HU); do_lasx_emu_xvmaddwev_w_hu(uc, instr); return 1;
    case OP17_XVMADDWEV_D_WU: perf_inc(P_XVMADDWEV_D_WU); do_lasx_emu_xvmaddwev_d_wu(uc, instr); return 1;
    case OP17_XVMADDWEV_Q_DU: perf_inc(P_XVMADDWEV_Q_DU); do_lasx_emu_xvmaddwev_q_du(uc, instr); return 1;
    case OP17_XVMADDWOD_H_BU: perf_inc(P_XVMADDWOD_H_BU); do_lasx_emu_xvmaddwod_h_bu(uc, instr); return 1;
    case OP17_XVMADDWOD_W_HU: perf_inc(P_XVMADDWOD_W_HU); do_lasx_emu_xvmaddwod_w_hu(uc, instr); return 1;
    case OP17_XVMADDWOD_D_WU: perf_inc(P_XVMADDWOD_D_WU); do_lasx_emu_xvmaddwod_d_wu(uc, instr); return 1;
    case OP17_XVMADDWOD_Q_DU: perf_inc(P_XVMADDWOD_Q_DU); do_lasx_emu_xvmaddwod_q_du(uc, instr); return 1;
    case OP17_XVSIGNCOV_B: perf_inc(P_XVSIGNCOV_B); do_lasx_emu_xvsigncov_b(uc, instr); return 1;
    case OP17_XVSIGNCOV_H: perf_inc(P_XVSIGNCOV_H); do_lasx_emu_xvsigncov_h(uc, instr); return 1;
    case OP17_XVSIGNCOV_W: perf_inc(P_XVSIGNCOV_W); do_lasx_emu_xvsigncov_w(uc, instr); return 1;
    case OP17_XVSIGNCOV_D: perf_inc(P_XVSIGNCOV_D); do_lasx_emu_xvsigncov_d(uc, instr); return 1;
    case OP17_XVFRSTP_B: perf_inc(P_XVFRSTP_B); do_lasx_emu_xvfrstp_b(uc, instr); return 1;
    case OP17_XVFRSTP_H: perf_inc(P_XVFRSTP_H); do_lasx_emu_xvfrstp_h(uc, instr); return 1;
    case OP17_XVFRSTPI_B: perf_inc(P_XVFRSTPI_B); do_lasx_emu_xvfrstpi_b(uc, instr); return 1;
    case OP17_XVFRSTPI_H: perf_inc(P_XVFRSTPI_H); do_lasx_emu_xvfrstpi_h(uc, instr); return 1;
    case OP17_XVBITCLRI_W: perf_inc(P_XVBITCLRI_W); do_lasx_emu_xvbitclri_w(uc, instr); return 1;
    case OP17_XVBITREVI_W: perf_inc(P_XVBITREVI_W); do_lasx_emu_xvbitrevi_w(uc, instr); return 1;
    case OP17_XVBITSET_B: perf_inc(P_XVBITSET_B); do_lasx_emu_xvbitset_b(uc, instr); return 1;
    case OP17_XVBITSET_H: perf_inc(P_XVBITSET_H); do_lasx_emu_xvbitset_h(uc, instr); return 1;
    case OP17_XVBITSETI_W: perf_inc(P_XVBITSETI_W); do_lasx_emu_xvbitseti_w(uc, instr); return 1;
    case OP17_XVFCVT_H_S: perf_inc(P_XVFCVT_H_S); do_lasx_emu_xvfcvt_h_s(uc, instr); return 1;
    case OP17_XVFCVT_S_D: perf_inc(P_XVFCVT_S_D); do_lasx_emu_xvfcvt_s_d(uc, instr); return 1;
    case OP17_XVFTINTRM_W_D: perf_inc(P_XVFTINTRM_W_D); do_lasx_emu_xvftintrm_w_d(uc, instr); return 1;
    case OP17_XVFTINTRNE_W_D: perf_inc(P_XVFTINTRNE_W_D); do_lasx_emu_xvftintrne_w_d(uc, instr); return 1;
    case OP17_XVFTINTRP_W_D: perf_inc(P_XVFTINTRP_W_D); do_lasx_emu_xvftintrp_w_d(uc, instr); return 1;
    case OP17_XVFTINTRZ_W_D: perf_inc(P_XVFTINTRZ_W_D); do_lasx_emu_xvftintrz_w_d(uc, instr); return 1;
    case OP17_XVMADDWEV_D_WU_W: perf_inc(P_XVMADDWEV_D_WU_W); do_lasx_emu_xvmaddwev_d_wu_w(uc, instr); return 1;
    case OP17_XVMADDWEV_H_BU_B: perf_inc(P_XVMADDWEV_H_BU_B); do_lasx_emu_xvmaddwev_h_bu_b(uc, instr); return 1;
    case OP17_XVMADDWEV_Q_DU_D: perf_inc(P_XVMADDWEV_Q_DU_D); do_lasx_emu_xvmaddwev_q_du_d(uc, instr); return 1;
    case OP17_XVMADDWEV_W_HU_H: perf_inc(P_XVMADDWEV_W_HU_H); do_lasx_emu_xvmaddwev_w_hu_h(uc, instr); return 1;
    case OP17_XVMADDWOD_D_WU_W: perf_inc(P_XVMADDWOD_D_WU_W); do_lasx_emu_xvmaddwod_d_wu_w(uc, instr); return 1;
    case OP17_XVMADDWOD_H_BU_B: perf_inc(P_XVMADDWOD_H_BU_B); do_lasx_emu_xvmaddwod_h_bu_b(uc, instr); return 1;
    case OP17_XVMADDWOD_Q_DU_D: perf_inc(P_XVMADDWOD_Q_DU_D); do_lasx_emu_xvmaddwod_q_du_d(uc, instr); return 1;
    case OP17_XVMADDWOD_W_HU_H: perf_inc(P_XVMADDWOD_W_HU_H); do_lasx_emu_xvmaddwod_w_hu_h(uc, instr); return 1;
    case OP17_XVMUH_BU: perf_inc(P_XVMUH_BU); do_lasx_emu_xvmuh_bu(uc, instr); return 1;
    case OP17_XVMUH_D: perf_inc(P_XVMUH_D); do_lasx_emu_xvmuh_d(uc, instr); return 1;
    case OP17_XVMUH_HU: perf_inc(P_XVMUH_HU); do_lasx_emu_xvmuh_hu(uc, instr); return 1;
    case OP17_XVMUH_WU: perf_inc(P_XVMUH_WU); do_lasx_emu_xvmuh_wu(uc, instr); return 1;
    case OP17_XVPACKEV_B: perf_inc(P_XVPACKEV_B); do_lasx_emu_xvpackev_b(uc, instr); return 1;
    case OP17_XVPACKEV_D: perf_inc(P_XVPACKEV_D); do_lasx_emu_xvpackev_d(uc, instr); return 1;
    case OP17_XVPACKEV_H: perf_inc(P_XVPACKEV_H); do_lasx_emu_xvpackev_h(uc, instr); return 1;
    case OP17_XVPACKEV_W: perf_inc(P_XVPACKEV_W); do_lasx_emu_xvpackev_w(uc, instr); return 1;
    case OP17_XVPERM_W: perf_inc(P_XVPERM_W); do_lasx_emu_xvperm_w(uc, instr); return 1;
    case OP17_XVPICKEV_B: perf_inc(P_XVPICKEV_B); do_lasx_emu_xvpickev_b(uc, instr); return 1;
    case OP17_XVPICKEV_D: perf_inc(P_XVPICKEV_D); do_lasx_emu_xvpickev_d(uc, instr); return 1;
    case OP17_XVPICKEV_H: perf_inc(P_XVPICKEV_H); do_lasx_emu_xvpickev_h(uc, instr); return 1;
    case OP17_XVPICKEV_W: perf_inc(P_XVPICKEV_W); do_lasx_emu_xvpickev_w(uc, instr); return 1;
    case OP17_XVPICKOD_B: perf_inc(P_XVPICKOD_B); do_lasx_emu_xvpickod_b(uc, instr); return 1;
    case OP17_XVPICKOD_D: perf_inc(P_XVPICKOD_D); do_lasx_emu_xvpickod_d(uc, instr); return 1;
    case OP17_XVPICKOD_H: perf_inc(P_XVPICKOD_H); do_lasx_emu_xvpickod_h(uc, instr); return 1;
    case OP17_XVPICKOD_W: perf_inc(P_XVPICKOD_W); do_lasx_emu_xvpickod_w(uc, instr); return 1;
    case OP17_XVREPLVE_B: perf_inc(P_XVREPLVE_B); do_lasx_emu_xvreplve_b(uc, instr); return 1;
    case OP17_XVREPLVE_D: perf_inc(P_XVREPLVE_D); do_lasx_emu_xvreplve_d(uc, instr); return 1;
    case OP17_XVREPLVE_H: perf_inc(P_XVREPLVE_H); do_lasx_emu_xvreplve_h(uc, instr); return 1;
    case OP17_XVREPLVE_W: perf_inc(P_XVREPLVE_W); do_lasx_emu_xvreplve_w(uc, instr); return 1;
    case OP17_XVROTRI_W: perf_inc(P_XVROTRI_W); do_lasx_emu_xvrotri_w(uc, instr); return 1;
    case OP17_XVSHUF_D: perf_inc(P_XVSHUF_D); do_lasx_emu_xvshuf_d(uc, instr); return 1;
    case OP17_XVSHUF_H: perf_inc(P_XVSHUF_H); do_lasx_emu_xvshuf_h(uc, instr); return 1;
    case OP17_XVSHUF_W: perf_inc(P_XVSHUF_W); do_lasx_emu_xvshuf_w(uc, instr); return 1;
    case OP17_XVSLLI_W: perf_inc(P_XVSLLI_W); do_lasx_emu_xvslli_w(uc, instr); return 1;
    case OP17_XVSLLWIL_D_W: perf_inc(P_XVSLLWIL_D_W); do_lasx_emu_xvsllwil_d_w(uc, instr); return 1;
    case OP17_XVSLLWIL_DU_WU: perf_inc(P_XVSLLWIL_DU_WU); do_lasx_emu_xvsllwil_du_wu(uc, instr); return 1;
    case OP17_XVSRAI_W: perf_inc(P_XVSRAI_W); do_lasx_emu_xvsrai_w(uc, instr); return 1;
    case OP17_XVSRAN_B_H: perf_inc(P_XVSRAN_B_H); do_lasx_emu_xvsran_b_h(uc, instr); return 1;
    case OP17_XVSRAN_H_W: perf_inc(P_XVSRAN_H_W); do_lasx_emu_xvsran_h_w(uc, instr); return 1;
    case OP17_XVSRAN_W_D: perf_inc(P_XVSRAN_W_D); do_lasx_emu_xvsran_w_d(uc, instr); return 1;
    case OP17_XVSRANI_H_W: perf_inc(P_XVSRANI_H_W); do_lasx_emu_xvsrani_h_w(uc, instr); return 1;
    case OP17_XVSRARI_W: perf_inc(P_XVSRARI_W); do_lasx_emu_xvsrari_w(uc, instr); return 1;
    case OP17_XVSRARN_B_H: perf_inc(P_XVSRARN_B_H); do_lasx_emu_xvsrarn_b_h(uc, instr); return 1;
    case OP17_XVSRARN_H_W: perf_inc(P_XVSRARN_H_W); do_lasx_emu_xvsrarn_h_w(uc, instr); return 1;
    case OP17_XVSRARN_W_D: perf_inc(P_XVSRARN_W_D); do_lasx_emu_xvsrarn_w_d(uc, instr); return 1;
    case OP17_XVSRARNI_H_W: perf_inc(P_XVSRARNI_H_W); do_lasx_emu_xvsrarni_h_w(uc, instr); return 1;
    case OP17_XVSRLI_W: perf_inc(P_XVSRLI_W); do_lasx_emu_xvsrli_w(uc, instr); return 1;
    case OP17_XVSRLN_B_H: perf_inc(P_XVSRLN_B_H); do_lasx_emu_xvsrln_b_h(uc, instr); return 1;
    case OP17_XVSRLN_H_W: perf_inc(P_XVSRLN_H_W); do_lasx_emu_xvsrln_h_w(uc, instr); return 1;
    case OP17_XVSRLN_W_D: perf_inc(P_XVSRLN_W_D); do_lasx_emu_xvsrln_w_d(uc, instr); return 1;
    case OP17_XVSRLNI_H_W: perf_inc(P_XVSRLNI_H_W); do_lasx_emu_xvsrlni_h_w(uc, instr); return 1;
    case OP17_XVSRLRI_W: perf_inc(P_XVSRLRI_W); do_lasx_emu_xvsrlri_w(uc, instr); return 1;
    case OP17_XVSRLRN_B_H: perf_inc(P_XVSRLRN_B_H); do_lasx_emu_xvsrlrn_b_h(uc, instr); return 1;
    case OP17_XVSRLRN_H_W: perf_inc(P_XVSRLRN_H_W); do_lasx_emu_xvsrlrn_h_w(uc, instr); return 1;
    case OP17_XVSRLRN_W_D: perf_inc(P_XVSRLRN_W_D); do_lasx_emu_xvsrlrn_w_d(uc, instr); return 1;
    case OP17_XVSRLRNI_H_W: perf_inc(P_XVSRLRNI_H_W); do_lasx_emu_xvsrlrni_h_w(uc, instr); return 1;
    case OP17_XVSSRAN_B_H: perf_inc(P_XVSSRAN_B_H); do_lasx_emu_xvssran_b_h(uc, instr); return 1;
    case OP17_XVSSRAN_BU_H: perf_inc(P_XVSSRAN_BU_H); do_lasx_emu_xvssran_bu_h(uc, instr); return 1;
    case OP17_XVSSRAN_H_W: perf_inc(P_XVSSRAN_H_W); do_lasx_emu_xvssran_h_w(uc, instr); return 1;
    case OP17_XVSSRAN_HU_W: perf_inc(P_XVSSRAN_HU_W); do_lasx_emu_xvssran_hu_w(uc, instr); return 1;
    case OP17_XVSSRAN_W_D: perf_inc(P_XVSSRAN_W_D); do_lasx_emu_xvssran_w_d(uc, instr); return 1;
    case OP17_XVSSRAN_WU_D: perf_inc(P_XVSSRAN_WU_D); do_lasx_emu_xvssran_wu_d(uc, instr); return 1;
    case OP17_XVSSRANI_H_W: perf_inc(P_XVSSRANI_H_W); do_lasx_emu_xvssrani_h_w(uc, instr); return 1;
    case OP17_XVSSRANI_HU_W: perf_inc(P_XVSSRANI_HU_W); do_lasx_emu_xvssrani_hu_w(uc, instr); return 1;
    case OP17_XVSSRARN_B_H: perf_inc(P_XVSSRARN_B_H); do_lasx_emu_xvssrarn_b_h(uc, instr); return 1;
    case OP17_XVSSRARN_BU_H: perf_inc(P_XVSSRARN_BU_H); do_lasx_emu_xvssrarn_bu_h(uc, instr); return 1;
    case OP17_XVSSRARN_H_W: perf_inc(P_XVSSRARN_H_W); do_lasx_emu_xvssrarn_h_w(uc, instr); return 1;
    case OP17_XVSSRARN_HU_W: perf_inc(P_XVSSRARN_HU_W); do_lasx_emu_xvssrarn_hu_w(uc, instr); return 1;
    case OP17_XVSSRARN_W_D: perf_inc(P_XVSSRARN_W_D); do_lasx_emu_xvssrarn_w_d(uc, instr); return 1;
    case OP17_XVSSRARN_WU_D: perf_inc(P_XVSSRARN_WU_D); do_lasx_emu_xvssrarn_wu_d(uc, instr); return 1;
    case OP17_XVSSRARNI_H_W: perf_inc(P_XVSSRARNI_H_W); do_lasx_emu_xvssrarni_h_w(uc, instr); return 1;
    case OP17_XVSSRARNI_HU_W: perf_inc(P_XVSSRARNI_HU_W); do_lasx_emu_xvssrarni_hu_w(uc, instr); return 1;
    case OP17_XVSSRLN_B_H: perf_inc(P_XVSSRLN_B_H); do_lasx_emu_xvssrln_b_h(uc, instr); return 1;
    case OP17_XVSSRLN_BU_H: perf_inc(P_XVSSRLN_BU_H); do_lasx_emu_xvssrln_bu_h(uc, instr); return 1;
    case OP17_XVSSRLN_H_W: perf_inc(P_XVSSRLN_H_W); do_lasx_emu_xvssrln_h_w(uc, instr); return 1;
    case OP17_XVSSRLN_HU_W: perf_inc(P_XVSSRLN_HU_W); do_lasx_emu_xvssrln_hu_w(uc, instr); return 1;
    case OP17_XVSSRLN_W_D: perf_inc(P_XVSSRLN_W_D); do_lasx_emu_xvssrln_w_d(uc, instr); return 1;
    case OP17_XVSSRLN_WU_D: perf_inc(P_XVSSRLN_WU_D); do_lasx_emu_xvssrln_wu_d(uc, instr); return 1;
    case OP17_XVSSRLNI_H_W: perf_inc(P_XVSSRLNI_H_W); do_lasx_emu_xvssrlni_h_w(uc, instr); return 1;
    case OP17_XVSSRLNI_HU_W: perf_inc(P_XVSSRLNI_HU_W); do_lasx_emu_xvssrlni_hu_w(uc, instr); return 1;
    case OP17_XVSSRLRN_B_H: perf_inc(P_XVSSRLRN_B_H); do_lasx_emu_xvssrlrn_b_h(uc, instr); return 1;
    case OP17_XVSSRLRN_BU_H: perf_inc(P_XVSSRLRN_BU_H); do_lasx_emu_xvssrlrn_bu_h(uc, instr); return 1;
    case OP17_XVSSRLRN_H_W: perf_inc(P_XVSSRLRN_H_W); do_lasx_emu_xvssrlrn_h_w(uc, instr); return 1;
    case OP17_XVSSRLRN_HU_W: perf_inc(P_XVSSRLRN_HU_W); do_lasx_emu_xvssrlrn_hu_w(uc, instr); return 1;
    case OP17_XVSSRLRN_W_D: perf_inc(P_XVSSRLRN_W_D); do_lasx_emu_xvssrlrn_w_d(uc, instr); return 1;
    case OP17_XVSSRLRN_WU_D: perf_inc(P_XVSSRLRN_WU_D); do_lasx_emu_xvssrlrn_wu_d(uc, instr); return 1;
    case OP17_XVSSRLRNI_H_W: perf_inc(P_XVSSRLRNI_H_W); do_lasx_emu_xvssrlrni_h_w(uc, instr); return 1;
    case OP17_XVFADD_S: perf_inc(P_XVFADD_S); do_lasx_emu_xvfadd_s(uc, instr); return 1;
    case OP17_XVFADD_D: perf_inc(P_XVFADD_D); do_lasx_emu_xvfadd_d(uc, instr); return 1;
    case OP17_XVFSUB_S: perf_inc(P_XVFSUB_S); do_lasx_emu_xvfsub_s(uc, instr); return 1;
    case OP17_XVFSUB_D: perf_inc(P_XVFSUB_D); do_lasx_emu_xvfsub_d(uc, instr); return 1;
    case OP17_XVFMUL_S: perf_inc(P_XVFMUL_S); do_lasx_emu_xvfmul_s(uc, instr); return 1;
    case OP17_XVFMUL_D: perf_inc(P_XVFMUL_D); do_lasx_emu_xvfmul_d(uc, instr); return 1;
    case OP17_XVFDIV_S: perf_inc(P_XVFDIV_S); do_lasx_emu_xvfdiv_s(uc, instr); return 1;
    case OP17_XVFDIV_D: perf_inc(P_XVFDIV_D); do_lasx_emu_xvfdiv_d(uc, instr); return 1;
    case OP17_XVFMAX_S: perf_inc(P_XVFMAX_S); do_lasx_emu_xvfmax_s(uc, instr); return 1;
    case OP17_XVFMAX_D: perf_inc(P_XVFMAX_D); do_lasx_emu_xvfmax_d(uc, instr); return 1;
    case OP17_XVFMIN_S: perf_inc(P_XVFMIN_S); do_lasx_emu_xvfmin_s(uc, instr); return 1;
    case OP17_XVFMIN_D: perf_inc(P_XVFMIN_D); do_lasx_emu_xvfmin_d(uc, instr); return 1;
    case OP17_XVFMAXA_S: perf_inc(P_XVFMAXA_S); do_lasx_emu_xvfmaxa_s(uc, instr); return 1;
    case OP17_XVFMAXA_D: perf_inc(P_XVFMAXA_D); do_lasx_emu_xvfmaxa_d(uc, instr); return 1;
    case OP17_XVFMINA_S: perf_inc(P_XVFMINA_S); do_lasx_emu_xvfmina_s(uc, instr); return 1;
    case OP17_XVFMINA_D: perf_inc(P_XVFMINA_D); do_lasx_emu_xvfmina_d(uc, instr); return 1;
    case OP17_XVFFINT_S_L: perf_inc(P_XVFFINT_S_L); do_lasx_emu_xvffint_s_l(uc, instr); return 1;
    case OP17_XVFTINT_W_D: perf_inc(P_XVFTINT_W_D); do_lasx_emu_xvftint_w_d(uc, instr); return 1;
    case OP17_XVBITCLR_B: perf_inc(P_XVBITCLR_B); do_lasx_emu_xvbitclr_b(uc, instr); return 1;
    case OP17_XVBITCLR_H: perf_inc(P_XVBITCLR_H); do_lasx_emu_xvbitclr_h(uc, instr); return 1;
    case OP17_XVBITCLR_W: perf_inc(P_XVBITCLR_W); do_lasx_emu_xvbitclr_w(uc, instr); return 1;
    case OP17_XVBITCLR_D: perf_inc(P_XVBITCLR_D); do_lasx_emu_xvbitclr_d(uc, instr); return 1;
    case OP17_XVBITSET_W: perf_inc(P_XVBITSET_W); do_lasx_emu_xvbitset_w(uc, instr); return 1;
    case OP17_XVBITSET_D: perf_inc(P_XVBITSET_D); do_lasx_emu_xvbitset_d(uc, instr); return 1;
    case OP17_XVBITREV_B: perf_inc(P_XVBITREV_B); do_lasx_emu_xvbitrev_b(uc, instr); return 1;
    case OP17_XVBITREV_H: perf_inc(P_XVBITREV_H); do_lasx_emu_xvbitrev_h(uc, instr); return 1;
    case OP17_XVBITREV_W: perf_inc(P_XVBITREV_W); do_lasx_emu_xvbitrev_w(uc, instr); return 1;
    case OP17_XVBITREV_D: perf_inc(P_XVBITREV_D); do_lasx_emu_xvbitrev_d(uc, instr); return 1;
    case OP17_XVSAT_W: perf_inc(P_XVSAT_W); do_lasx_emu_xvsat_w(uc, instr); return 1;
    case OP17_XVSAT_WU: perf_inc(P_XVSAT_WU); do_lasx_emu_xvsat_wu(uc, instr); return 1;
    case OP17_XVADDWEV_H_B: perf_inc(P_XVADDWEV_H_B); do_lasx_emu_xvaddwev_h_b(uc, instr); return 1;
    case OP17_XVADDWEV_H_BU: perf_inc(P_XVADDWEV_H_BU); do_lasx_emu_xvaddwev_h_bu(uc, instr); return 1;
    case OP17_XVADDWEV_H_BU_B: perf_inc(P_XVADDWEV_H_BU_B); do_lasx_emu_xvaddwev_h_bu_b(uc, instr); return 1;
    case OP17_XVADDWEV_W_H: perf_inc(P_XVADDWEV_W_H); do_lasx_emu_xvaddwev_w_h(uc, instr); return 1;
    case OP17_XVADDWEV_W_HU: perf_inc(P_XVADDWEV_W_HU); do_lasx_emu_xvaddwev_w_hu(uc, instr); return 1;
    case OP17_XVADDWEV_W_HU_H: perf_inc(P_XVADDWEV_W_HU_H); do_lasx_emu_xvaddwev_w_hu_h(uc, instr); return 1;
    case OP17_XVADDWEV_D_W: perf_inc(P_XVADDWEV_D_W); do_lasx_emu_xvaddwev_d_w(uc, instr); return 1;
    case OP17_XVADDWEV_D_WU: perf_inc(P_XVADDWEV_D_WU); do_lasx_emu_xvaddwev_d_wu(uc, instr); return 1;
    case OP17_XVADDWEV_D_WU_W: perf_inc(P_XVADDWEV_D_WU_W); do_lasx_emu_xvaddwev_d_wu_w(uc, instr); return 1;
    case OP17_XVADDWEV_Q_D: perf_inc(P_XVADDWEV_Q_D); do_lasx_emu_xvaddwev_q_d(uc, instr); return 1;
    case OP17_XVADDWEV_Q_DU: perf_inc(P_XVADDWEV_Q_DU); do_lasx_emu_xvaddwev_q_du(uc, instr); return 1;
    case OP17_XVADDWEV_Q_DU_D: perf_inc(P_XVADDWEV_Q_DU_D); do_lasx_emu_xvaddwev_q_du_d(uc, instr); return 1;
    case OP17_XVSUBWEV_H_B: perf_inc(P_XVSUBWEV_H_B); do_lasx_emu_xvsubwev_h_b(uc, instr); return 1;
    case OP17_XVSUBWEV_H_BU: perf_inc(P_XVSUBWEV_H_BU); do_lasx_emu_xvsubwev_h_bu(uc, instr); return 1;
    case OP17_XVSUBWEV_W_H: perf_inc(P_XVSUBWEV_W_H); do_lasx_emu_xvsubwev_w_h(uc, instr); return 1;
    case OP17_XVSUBWEV_W_HU: perf_inc(P_XVSUBWEV_W_HU); do_lasx_emu_xvsubwev_w_hu(uc, instr); return 1;
    case OP17_XVSUBWEV_D_W: perf_inc(P_XVSUBWEV_D_W); do_lasx_emu_xvsubwev_d_w(uc, instr); return 1;
    case OP17_XVSUBWEV_D_WU: perf_inc(P_XVSUBWEV_D_WU); do_lasx_emu_xvsubwev_d_wu(uc, instr); return 1;
    case OP17_XVSUBWEV_Q_D: perf_inc(P_XVSUBWEV_Q_D); do_lasx_emu_xvsubwev_q_d(uc, instr); return 1;
    case OP17_XVSUBWEV_Q_DU: perf_inc(P_XVSUBWEV_Q_DU); do_lasx_emu_xvsubwev_q_du(uc, instr); return 1;
    case OP17_XVADDWOD_H_B: perf_inc(P_XVADDWOD_H_B); do_lasx_emu_xvaddwod_h_b(uc, instr); return 1;
    case OP17_XVADDWOD_H_BU: perf_inc(P_XVADDWOD_H_BU); do_lasx_emu_xvaddwod_h_bu(uc, instr); return 1;
    case OP17_XVADDWOD_H_BU_B: perf_inc(P_XVADDWOD_H_BU_B); do_lasx_emu_xvaddwod_h_bu_b(uc, instr); return 1;
    case OP17_XVADDWOD_W_H: perf_inc(P_XVADDWOD_W_H); do_lasx_emu_xvaddwod_w_h(uc, instr); return 1;
    case OP17_XVADDWOD_W_HU: perf_inc(P_XVADDWOD_W_HU); do_lasx_emu_xvaddwod_w_hu(uc, instr); return 1;
    case OP17_XVADDWOD_W_HU_H: perf_inc(P_XVADDWOD_W_HU_H); do_lasx_emu_xvaddwod_w_hu_h(uc, instr); return 1;
    case OP17_XVADDWOD_D_W: perf_inc(P_XVADDWOD_D_W); do_lasx_emu_xvaddwod_d_w(uc, instr); return 1;
    case OP17_XVADDWOD_D_WU: perf_inc(P_XVADDWOD_D_WU); do_lasx_emu_xvaddwod_d_wu(uc, instr); return 1;
    case OP17_XVADDWOD_D_WU_W: perf_inc(P_XVADDWOD_D_WU_W); do_lasx_emu_xvaddwod_d_wu_w(uc, instr); return 1;
    case OP17_XVADDWOD_Q_D: perf_inc(P_XVADDWOD_Q_D); do_lasx_emu_xvaddwod_q_d(uc, instr); return 1;
    case OP17_XVADDWOD_Q_DU: perf_inc(P_XVADDWOD_Q_DU); do_lasx_emu_xvaddwod_q_du(uc, instr); return 1;
    case OP17_XVADDWOD_Q_DU_D: perf_inc(P_XVADDWOD_Q_DU_D); do_lasx_emu_xvaddwod_q_du_d(uc, instr); return 1;
    case OP17_XVSUBWOD_H_B: perf_inc(P_XVSUBWOD_H_B); do_lasx_emu_xvsubwod_h_b(uc, instr); return 1;
    case OP17_XVSUBWOD_H_BU: perf_inc(P_XVSUBWOD_H_BU); do_lasx_emu_xvsubwod_h_bu(uc, instr); return 1;
    case OP17_XVSUBWOD_W_H: perf_inc(P_XVSUBWOD_W_H); do_lasx_emu_xvsubwod_w_h(uc, instr); return 1;
    case OP17_XVSUBWOD_W_HU: perf_inc(P_XVSUBWOD_W_HU); do_lasx_emu_xvsubwod_w_hu(uc, instr); return 1;
    case OP17_XVSUBWOD_D_W: perf_inc(P_XVSUBWOD_D_W); do_lasx_emu_xvsubwod_d_w(uc, instr); return 1;
    case OP17_XVSUBWOD_D_WU: perf_inc(P_XVSUBWOD_D_WU); do_lasx_emu_xvsubwod_d_wu(uc, instr); return 1;
    case OP17_XVSUBWOD_Q_D: perf_inc(P_XVSUBWOD_Q_D); do_lasx_emu_xvsubwod_q_d(uc, instr); return 1;
    case OP17_XVHADDW_H_B: perf_inc(P_XVHADDW_H_B); do_lasx_emu_xvhaddw_h_b(uc, instr); return 1;
    case OP17_XVHADDW_W_H: perf_inc(P_XVHADDW_W_H); do_lasx_emu_xvhaddw_w_h(uc, instr); return 1;
    case OP17_XVHADDW_D_W: perf_inc(P_XVHADDW_D_W); do_lasx_emu_xvhaddw_d_w(uc, instr); return 1;
    case OP17_XVHADDW_Q_D: perf_inc(P_XVHADDW_Q_D); do_lasx_emu_xvhaddw_q_d(uc, instr); return 1;
    case OP17_XVHSUBW_H_B: perf_inc(P_XVHSUBW_H_B); do_lasx_emu_xvhsubw_h_b(uc, instr); return 1;
    case OP17_XVHSUBW_W_H: perf_inc(P_XVHSUBW_W_H); do_lasx_emu_xvhsubw_w_h(uc, instr); return 1;
    case OP17_XVHSUBW_D_W: perf_inc(P_XVHSUBW_D_W); do_lasx_emu_xvhsubw_d_w(uc, instr); return 1;
    case OP17_XVHSUBW_Q_D: perf_inc(P_XVHSUBW_Q_D); do_lasx_emu_xvhsubw_q_d(uc, instr); return 1;
    case OP17_XVHADDW_HU_BU: perf_inc(P_XVHADDW_HU_BU); do_lasx_emu_xvhaddw_hu_bu(uc, instr); return 1;
    case OP17_XVHADDW_WU_HU: perf_inc(P_XVHADDW_WU_HU); do_lasx_emu_xvhaddw_wu_hu(uc, instr); return 1;
    case OP17_XVHADDW_DU_WU: perf_inc(P_XVHADDW_DU_WU); do_lasx_emu_xvhaddw_du_wu(uc, instr); return 1;
    case OP17_XVHADDW_QU_DU: perf_inc(P_XVHADDW_QU_DU); do_lasx_emu_xvhaddw_qu_du(uc, instr); return 1;
    case OP17_XVHSUBW_HU_BU: perf_inc(P_XVHSUBW_HU_BU); do_lasx_emu_xvhsubw_hu_bu(uc, instr); return 1;
    case OP17_XVHSUBW_WU_HU: perf_inc(P_XVHSUBW_WU_HU); do_lasx_emu_xvhsubw_wu_hu(uc, instr); return 1;
    case OP17_XVHSUBW_DU_WU: perf_inc(P_XVHSUBW_DU_WU); do_lasx_emu_xvhsubw_du_wu(uc, instr); return 1;
    case OP17_XVHSUBW_QU_DU: perf_inc(P_XVHSUBW_QU_DU); do_lasx_emu_xvhsubw_qu_du(uc, instr); return 1;
    case OP17_XVADDA_B: perf_inc(P_XVADDA_B); do_lasx_emu_xvadda_b(uc, instr); return 1;
    case OP17_XVADDA_H: perf_inc(P_XVADDA_H); do_lasx_emu_xvadda_h(uc, instr); return 1;
    case OP17_XVADDA_W: perf_inc(P_XVADDA_W); do_lasx_emu_xvadda_w(uc, instr); return 1;
    case OP17_XVADDA_D: perf_inc(P_XVADDA_D); do_lasx_emu_xvadda_d(uc, instr); return 1;
    case OP17_XVABSD_B: perf_inc(P_XVABSD_B); do_lasx_emu_xvabsd_b(uc, instr); return 1;
    case OP17_XVABSD_H: perf_inc(P_XVABSD_H); do_lasx_emu_xvabsd_h(uc, instr); return 1;
    case OP17_XVABSD_W: perf_inc(P_XVABSD_W); do_lasx_emu_xvabsd_w(uc, instr); return 1;
    case OP17_XVABSD_D: perf_inc(P_XVABSD_D); do_lasx_emu_xvabsd_d(uc, instr); return 1;
    case OP17_XVABSD_BU: perf_inc(P_XVABSD_BU); do_lasx_emu_xvabsd_bu(uc, instr); return 1;
    case OP17_XVABSD_HU: perf_inc(P_XVABSD_HU); do_lasx_emu_xvabsd_hu(uc, instr); return 1;
    case OP17_XVABSD_WU: perf_inc(P_XVABSD_WU); do_lasx_emu_xvabsd_wu(uc, instr); return 1;
    case OP17_XVABSD_DU: perf_inc(P_XVABSD_DU); do_lasx_emu_xvabsd_du(uc, instr); return 1;
    case OP17_XVAVG_B: perf_inc(P_XVAVG_B); do_lasx_emu_xvavg_b(uc, instr); return 1;
    case OP17_XVAVG_H: perf_inc(P_XVAVG_H); do_lasx_emu_xvavg_h(uc, instr); return 1;
    case OP17_XVAVG_W: perf_inc(P_XVAVG_W); do_lasx_emu_xvavg_w(uc, instr); return 1;
    case OP17_XVAVG_D: perf_inc(P_XVAVG_D); do_lasx_emu_xvavg_d(uc, instr); return 1;
    case OP17_XVAVG_BU: perf_inc(P_XVAVG_BU); do_lasx_emu_xvavg_bu(uc, instr); return 1;
    case OP17_XVAVG_HU: perf_inc(P_XVAVG_HU); do_lasx_emu_xvavg_hu(uc, instr); return 1;
    case OP17_XVAVG_WU: perf_inc(P_XVAVG_WU); do_lasx_emu_xvavg_wu(uc, instr); return 1;
    case OP17_XVAVG_DU: perf_inc(P_XVAVG_DU); do_lasx_emu_xvavg_du(uc, instr); return 1;
    case OP17_XVAVGR_B: perf_inc(P_XVAVGR_B); do_lasx_emu_xvavgr_b(uc, instr); return 1;
    case OP17_XVAVGR_H: perf_inc(P_XVAVGR_H); do_lasx_emu_xvavgr_h(uc, instr); return 1;
    case OP17_XVAVGR_W: perf_inc(P_XVAVGR_W); do_lasx_emu_xvavgr_w(uc, instr); return 1;
    case OP17_XVAVGR_D: perf_inc(P_XVAVGR_D); do_lasx_emu_xvavgr_d(uc, instr); return 1;
    case OP17_XVAVGR_BU: perf_inc(P_XVAVGR_BU); do_lasx_emu_xvavgr_bu(uc, instr); return 1;
    case OP17_XVAVGR_HU: perf_inc(P_XVAVGR_HU); do_lasx_emu_xvavgr_hu(uc, instr); return 1;
    case OP17_XVAVGR_WU: perf_inc(P_XVAVGR_WU); do_lasx_emu_xvavgr_wu(uc, instr); return 1;
    case OP17_XVBSLL_V: perf_inc(P_XVBSLL_V); do_lasx_emu_xvbsll_v(uc, instr); return 1;
    case OP17_XVBSRL_V: perf_inc(P_XVBSRL_V); do_lasx_emu_xvbsrl_v(uc, instr); return 1;
    case OP17_XVSLL_B: perf_inc(P_XVSLL_B); do_lasx_emu_xvsll_b(uc, instr); return 1;
    case OP17_XVSLL_H: perf_inc(P_XVSLL_H); do_lasx_emu_xvsll_h(uc, instr); return 1;
    case OP17_XVSLL_W: perf_inc(P_XVSLL_W); do_lasx_emu_xvsll_w(uc, instr); return 1;
    case OP17_XVSLL_D: perf_inc(P_XVSLL_D); do_lasx_emu_xvsll_d(uc, instr); return 1;
    case OP17_XVSRL_B: perf_inc(P_XVSRL_B); do_lasx_emu_xvsrl_b(uc, instr); return 1;
    case OP17_XVSRL_H: perf_inc(P_XVSRL_H); do_lasx_emu_xvsrl_h(uc, instr); return 1;
    case OP17_XVSRL_W: perf_inc(P_XVSRL_W); do_lasx_emu_xvsrl_w(uc, instr); return 1;
    case OP17_XVSRL_D: perf_inc(P_XVSRL_D); do_lasx_emu_xvsrl_d(uc, instr); return 1;
    case OP17_XVSRA_B: perf_inc(P_XVSRA_B); do_lasx_emu_xvsra_b(uc, instr); return 1;
    case OP17_XVSRA_H: perf_inc(P_XVSRA_H); do_lasx_emu_xvsra_h(uc, instr); return 1;
    case OP17_XVSRA_W: perf_inc(P_XVSRA_W); do_lasx_emu_xvsra_w(uc, instr); return 1;
    case OP17_XVSRA_D: perf_inc(P_XVSRA_D); do_lasx_emu_xvsra_d(uc, instr); return 1;
    case OP17_XVROTR_B: perf_inc(P_XVROTR_B); do_lasx_emu_xvrotr_b(uc, instr); return 1;
    case OP17_XVROTR_H: perf_inc(P_XVROTR_H); do_lasx_emu_xvrotr_h(uc, instr); return 1;
    case OP17_XVROTR_W: perf_inc(P_XVROTR_W); do_lasx_emu_xvrotr_w(uc, instr); return 1;
    case OP17_XVROTR_D: perf_inc(P_XVROTR_D); do_lasx_emu_xvrotr_d(uc, instr); return 1;
    case OP17_XVSRLR_B: perf_inc(P_XVSRLR_B); do_lasx_emu_xvsrlr_b(uc, instr); return 1;
    case OP17_XVSRLR_H: perf_inc(P_XVSRLR_H); do_lasx_emu_xvsrlr_h(uc, instr); return 1;
    case OP17_XVSRLR_W: perf_inc(P_XVSRLR_W); do_lasx_emu_xvsrlr_w(uc, instr); return 1;
    case OP17_XVSRLR_D: perf_inc(P_XVSRLR_D); do_lasx_emu_xvsrlr_d(uc, instr); return 1;
    case OP17_XVSRAR_B: perf_inc(P_XVSRAR_B); do_lasx_emu_xvsrar_b(uc, instr); return 1;
    case OP17_XVSRAR_H: perf_inc(P_XVSRAR_H); do_lasx_emu_xvsrar_h(uc, instr); return 1;
    case OP17_XVSRAR_W: perf_inc(P_XVSRAR_W); do_lasx_emu_xvsrar_w(uc, instr); return 1;
    case OP17_XVMAXI_B: perf_inc(P_XVMAXI_B); do_lasx_emu_xvmaxi_b(uc, instr); return 1;
    case OP17_XVMAXI_H: perf_inc(P_XVMAXI_H); do_lasx_emu_xvmaxi_h(uc, instr); return 1;
    case OP17_XVMAXI_W: perf_inc(P_XVMAXI_W); do_lasx_emu_xvmaxi_w(uc, instr); return 1;
    case OP17_XVMAXI_D: perf_inc(P_XVMAXI_D); do_lasx_emu_xvmaxi_d(uc, instr); return 1;
    case OP17_XVMINI_B: perf_inc(P_XVMINI_B); do_lasx_emu_xvmini_b(uc, instr); return 1;
    case OP17_XVMINI_H: perf_inc(P_XVMINI_H); do_lasx_emu_xvmini_h(uc, instr); return 1;
    case OP17_XVMINI_W: perf_inc(P_XVMINI_W); do_lasx_emu_xvmini_w(uc, instr); return 1;
    case OP17_XVMINI_D: perf_inc(P_XVMINI_D); do_lasx_emu_xvmini_d(uc, instr); return 1;
    case OP17_XVMAXI_BU: perf_inc(P_XVMAXI_BU); do_lasx_emu_xvmaxi_bu(uc, instr); return 1;
    case OP17_XVMAXI_HU: perf_inc(P_XVMAXI_HU); do_lasx_emu_xvmaxi_hu(uc, instr); return 1;
    case OP17_XVMAXI_WU: perf_inc(P_XVMAXI_WU); do_lasx_emu_xvmaxi_wu(uc, instr); return 1;
    case OP17_XVMAXI_DU: perf_inc(P_XVMAXI_DU); do_lasx_emu_xvmaxi_du(uc, instr); return 1;
    case OP17_XVMINI_BU: perf_inc(P_XVMINI_BU); do_lasx_emu_xvmini_bu(uc, instr); return 1;
    case OP17_XVMINI_HU: perf_inc(P_XVMINI_HU); do_lasx_emu_xvmini_hu(uc, instr); return 1;
    case OP17_XVMINI_WU: perf_inc(P_XVMINI_WU); do_lasx_emu_xvmini_wu(uc, instr); return 1;
    case OP17_XVMINI_DU: perf_inc(P_XVMINI_DU); do_lasx_emu_xvmini_du(uc, instr); return 1;
    case OP17_XVSEQI_B: perf_inc(P_XVSEQI_B); do_lasx_emu_xvseqi_b(uc, instr); return 1;
    case OP17_XVSEQI_H: perf_inc(P_XVSEQI_H); do_lasx_emu_xvseqi_h(uc, instr); return 1;
    case OP17_XVSEQI_W: perf_inc(P_XVSEQI_W); do_lasx_emu_xvseqi_w(uc, instr); return 1;
    case OP17_XVSEQI_D: perf_inc(P_XVSEQI_D); do_lasx_emu_xvseqi_d(uc, instr); return 1;
    case OP17_XVSLEI_B: perf_inc(P_XVSLEI_B); do_lasx_emu_xvslei_b(uc, instr); return 1;
    case OP17_XVSLEI_H: perf_inc(P_XVSLEI_H); do_lasx_emu_xvslei_h(uc, instr); return 1;
    case OP17_XVSLEI_W: perf_inc(P_XVSLEI_W); do_lasx_emu_xvslei_w(uc, instr); return 1;
    case OP17_XVSLEI_D: perf_inc(P_XVSLEI_D); do_lasx_emu_xvslei_d(uc, instr); return 1;
    case OP17_XVSLEI_BU: perf_inc(P_XVSLEI_BU); do_lasx_emu_xvslei_bu(uc, instr); return 1;
    case OP17_XVSLEI_HU: perf_inc(P_XVSLEI_HU); do_lasx_emu_xvslei_hu(uc, instr); return 1;
    case OP17_XVSLEI_WU: perf_inc(P_XVSLEI_WU); do_lasx_emu_xvslei_wu(uc, instr); return 1;
    case OP17_XVSLEI_DU: perf_inc(P_XVSLEI_DU); do_lasx_emu_xvslei_du(uc, instr); return 1;
    case OP17_XVSLTI_B: perf_inc(P_XVSLTI_B); do_lasx_emu_xvslti_b(uc, instr); return 1;
    case OP17_XVSLTI_H: perf_inc(P_XVSLTI_H); do_lasx_emu_xvslti_h(uc, instr); return 1;
    case OP17_XVSLTI_W: perf_inc(P_XVSLTI_W); do_lasx_emu_xvslti_w(uc, instr); return 1;
    case OP17_XVSLTI_D: perf_inc(P_XVSLTI_D); do_lasx_emu_xvslti_d(uc, instr); return 1;
    case OP17_XVSLTI_BU: perf_inc(P_XVSLTI_BU); do_lasx_emu_xvslti_bu(uc, instr); return 1;
    case OP17_XVSLTI_HU: perf_inc(P_XVSLTI_HU); do_lasx_emu_xvslti_hu(uc, instr); return 1;
    case OP17_XVSLTI_WU: perf_inc(P_XVSLTI_WU); do_lasx_emu_xvslti_wu(uc, instr); return 1;
    case OP17_XVSLTI_DU: perf_inc(P_XVSLTI_DU); do_lasx_emu_xvslti_du(uc, instr); return 1;
    case OP17_XVMAX_B: perf_inc(P_XVMAX_B); do_lasx_emu_xvmax_b(uc, instr); return 1;
    case OP17_XVMAX_H: perf_inc(P_XVMAX_H); do_lasx_emu_xvmax_h(uc, instr); return 1;
    case OP17_XVMAX_W: perf_inc(P_XVMAX_W); do_lasx_emu_xvmax_w(uc, instr); return 1;
    case OP17_XVMAX_D: perf_inc(P_XVMAX_D); do_lasx_emu_xvmax_d(uc, instr); return 1;
    case OP17_XVMIN_B: perf_inc(P_XVMIN_B); do_lasx_emu_xvmin_b(uc, instr); return 1;
    case OP17_XVMIN_H: perf_inc(P_XVMIN_H); do_lasx_emu_xvmin_h(uc, instr); return 1;
    case OP17_XVMIN_W: perf_inc(P_XVMIN_W); do_lasx_emu_xvmin_w(uc, instr); return 1;
    case OP17_XVMIN_D: perf_inc(P_XVMIN_D); do_lasx_emu_xvmin_d(uc, instr); return 1;
    case OP17_XVMAX_BU: perf_inc(P_XVMAX_BU); do_lasx_emu_xvmax_bu(uc, instr); return 1;
    case OP17_XVMAX_HU: perf_inc(P_XVMAX_HU); do_lasx_emu_xvmax_hu(uc, instr); return 1;
    case OP17_XVMAX_WU: perf_inc(P_XVMAX_WU); do_lasx_emu_xvmax_wu(uc, instr); return 1;
    case OP17_XVMAX_DU: perf_inc(P_XVMAX_DU); do_lasx_emu_xvmax_du(uc, instr); return 1;
    case OP17_XVMIN_BU: perf_inc(P_XVMIN_BU); do_lasx_emu_xvmin_bu(uc, instr); return 1;
    case OP17_XVMIN_HU: perf_inc(P_XVMIN_HU); do_lasx_emu_xvmin_hu(uc, instr); return 1;
    case OP17_XVMIN_WU: perf_inc(P_XVMIN_WU); do_lasx_emu_xvmin_wu(uc, instr); return 1;
    case OP17_XVMIN_DU: perf_inc(P_XVMIN_DU); do_lasx_emu_xvmin_du(uc, instr); return 1;
    case OP17_XVSEQ_B: perf_inc(P_XVSEQ_B); do_lasx_emu_xvseq_b(uc, instr); return 1;
    case OP17_XVSEQ_H: perf_inc(P_XVSEQ_H); do_lasx_emu_xvseq_h(uc, instr); return 1;
    case OP17_XVSEQ_W: perf_inc(P_XVSEQ_W); do_lasx_emu_xvseq_w(uc, instr); return 1;
    case OP17_XVSEQ_D: perf_inc(P_XVSEQ_D); do_lasx_emu_xvseq_d(uc, instr); return 1;
    case OP17_XVSLE_B: perf_inc(P_XVSLE_B); do_lasx_emu_xvsle_b(uc, instr); return 1;
    case OP17_XVSLE_H: perf_inc(P_XVSLE_H); do_lasx_emu_xvsle_h(uc, instr); return 1;
    case OP17_XVSLE_W: perf_inc(P_XVSLE_W); do_lasx_emu_xvsle_w(uc, instr); return 1;
    case OP17_XVSLE_D: perf_inc(P_XVSLE_D); do_lasx_emu_xvsle_d(uc, instr); return 1;
    case OP17_XVSLT_B: perf_inc(P_XVSLT_B); do_lasx_emu_xvslt_b(uc, instr); return 1;
    case OP17_XVSLT_H: perf_inc(P_XVSLT_H); do_lasx_emu_xvslt_h(uc, instr); return 1;
    case OP17_XVSLT_W: perf_inc(P_XVSLT_W); do_lasx_emu_xvslt_w(uc, instr); return 1;
    case OP17_XVSLT_D: perf_inc(P_XVSLT_D); do_lasx_emu_xvslt_d(uc, instr); return 1;
    case OP17_XVSLE_BU: perf_inc(P_XVSLE_BU); do_lasx_emu_xvsle_bu(uc, instr); return 1;
    case OP17_XVSLE_HU: perf_inc(P_XVSLE_HU); do_lasx_emu_xvsle_hu(uc, instr); return 1;
    case OP17_XVSLE_WU: perf_inc(P_XVSLE_WU); do_lasx_emu_xvsle_wu(uc, instr); return 1;
    case OP17_XVSLE_DU: perf_inc(P_XVSLE_DU); do_lasx_emu_xvsle_du(uc, instr); return 1;
    case OP17_XVSLT_BU: perf_inc(P_XVSLT_BU); do_lasx_emu_xvslt_bu(uc, instr); return 1;
    case OP17_XVSLT_HU: perf_inc(P_XVSLT_HU); do_lasx_emu_xvslt_hu(uc, instr); return 1;
    case OP17_XVSLT_WU: perf_inc(P_XVSLT_WU); do_lasx_emu_xvslt_wu(uc, instr); return 1;
    case OP17_XVSLT_DU: perf_inc(P_XVSLT_DU); do_lasx_emu_xvslt_du(uc, instr); return 1;
    case OP17_XVAVGR_DU: perf_inc(P_XVAVGR_DU); do_lasx_emu_xvavgr_du(uc, instr); return 1;
    case OP17_XVSRAR_D: perf_inc(P_XVSRAR_D); do_lasx_emu_xvsrar_d(uc, instr); return 1;
    case OP17_XVSSRLRNI_HU_W: perf_inc(P_XVSSRLRNI_HU_W); do_lasx_emu_xvssrlrni_hu_w(uc, instr); return 1;
    case OP17_XVSUBWOD_Q_DU: perf_inc(P_XVSUBWOD_Q_DU); do_lasx_emu_xvsubwod_q_du(uc, instr); return 1;
    case OP17_XVSADD_DU: perf_inc(P_XVSADD_DU); do_lasx_emu_xvsadd_du(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 8) {
    case OP24_XVSETEQZ_V: perf_inc(P_XVSETEQZ_V); do_lasx_emu_xvseteqz_v(uc, instr); return 1;
    case OP24_XVSETNEZ_V: perf_inc(P_XVSETNEZ_V); do_lasx_emu_xvsetnez_v(uc, instr); return 1;
    case OP24_XVSETANYEQZ_B: perf_inc(P_XVSETANYEQZ_B); do_lasx_emu_xvsetanyeqz_b(uc, instr); return 1;
    case OP24_XVSETANYEQZ_H: perf_inc(P_XVSETANYEQZ_H); do_lasx_emu_xvsetanyeqz_h(uc, instr); return 1;
    case OP24_XVSETANYEQZ_W: perf_inc(P_XVSETANYEQZ_W); do_lasx_emu_xvsetanyeqz_w(uc, instr); return 1;
    case OP24_XVSETANYEQZ_D: perf_inc(P_XVSETANYEQZ_D); do_lasx_emu_xvsetanyeqz_d(uc, instr); return 1;
    case OP24_XVSETALLNEZ_B: perf_inc(P_XVSETALLNEZ_B); do_lasx_emu_xvsetallnez_b(uc, instr); return 1;
    case OP24_XVSETALLNEZ_H: perf_inc(P_XVSETALLNEZ_H); do_lasx_emu_xvsetallnez_h(uc, instr); return 1;
    case OP24_XVSETALLNEZ_W: perf_inc(P_XVSETALLNEZ_W); do_lasx_emu_xvsetallnez_w(uc, instr); return 1;
    case OP24_XVSETALLNEZ_D: perf_inc(P_XVSETALLNEZ_D); do_lasx_emu_xvsetallnez_d(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 10) {
    case OP22_XVFSQRT_S: perf_inc(P_XVFSQRT_S); do_lasx_emu_xvfsqrt_s(uc, instr); return 1;
    case OP22_XVFSQRT_D: perf_inc(P_XVFSQRT_D); do_lasx_emu_xvfsqrt_d(uc, instr); return 1;
    case OP22_XVFRECIP_S: perf_inc(P_XVFRECIP_S); do_lasx_emu_xvfrecip_s(uc, instr); return 1;
    case OP22_XVFRECIP_D: perf_inc(P_XVFRECIP_D); do_lasx_emu_xvfrecip_d(uc, instr); return 1;
    case OP22_XVFRSQRT_S: perf_inc(P_XVFRSQRT_S); do_lasx_emu_xvfrsqrt_s(uc, instr); return 1;
    case OP22_XVFRSQRT_D: perf_inc(P_XVFRSQRT_D); do_lasx_emu_xvfrsqrt_d(uc, instr); return 1;
    case OP22_XVFLOGB_S: perf_inc(P_XVFLOGB_S); do_lasx_emu_xvflogb_s(uc, instr); return 1;
    case OP22_XVFLOGB_D: perf_inc(P_XVFLOGB_D); do_lasx_emu_xvflogb_d(uc, instr); return 1;
    case OP22_XVFCLASS_S: perf_inc(P_XVFCLASS_S); do_lasx_emu_xvfclass_s(uc, instr); return 1;
    case OP22_XVFCLASS_D: perf_inc(P_XVFCLASS_D); do_lasx_emu_xvfclass_d(uc, instr); return 1;
    case OP22_XVFRECIPE_S: perf_inc(P_XVFRECIPE_S); do_lasx_emu_xvfrecipe_s(uc, instr); return 1;
    case OP22_XVFRECIPE_D: perf_inc(P_XVFRECIPE_D); do_lasx_emu_xvfrecipe_d(uc, instr); return 1;
    case OP22_XVFRSQRTE_S: perf_inc(P_XVFRSQRTE_S); do_lasx_emu_xvfrsqrte_s(uc, instr); return 1;
    case OP22_XVFRSQRTE_D: perf_inc(P_XVFRSQRTE_D); do_lasx_emu_xvfrsqrte_d(uc, instr); return 1;
    case OP22_XVFRINT_S: perf_inc(P_XVFRINT_S); do_lasx_emu_xvfrint_s(uc, instr); return 1;
    case OP22_XVFRINT_D: perf_inc(P_XVFRINT_D); do_lasx_emu_xvfrint_d(uc, instr); return 1;
    case OP22_XVFRINTRM_S: perf_inc(P_XVFRINTRM_S); do_lasx_emu_xvfrintrm_s(uc, instr); return 1;
    case OP22_XVFRINTRM_D: perf_inc(P_XVFRINTRM_D); do_lasx_emu_xvfrintrm_d(uc, instr); return 1;
    case OP22_XVFRINTRZ_S: perf_inc(P_XVFRINTRZ_S); do_lasx_emu_xvfrintrz_s(uc, instr); return 1;
    case OP22_XVFRINTRZ_D: perf_inc(P_XVFRINTRZ_D); do_lasx_emu_xvfrintrz_d(uc, instr); return 1;
    case OP22_XVFRINTRNE_S: perf_inc(P_XVFRINTRNE_S); do_lasx_emu_xvfrintrne_s(uc, instr); return 1;
    case OP22_XVFRINTRNE_D: perf_inc(P_XVFRINTRNE_D); do_lasx_emu_xvfrintrne_d(uc, instr); return 1;
    case OP22_XVFCVTL_D_S: perf_inc(P_XVFCVTL_D_S); do_lasx_emu_xvfcvtl_d_s(uc, instr); return 1;
    case OP22_XVFCVTH_S_H: perf_inc(P_XVFCVTH_S_H); do_lasx_emu_xvfcvth_s_h(uc, instr); return 1;
    case OP22_XVFCVTH_D_S: perf_inc(P_XVFCVTH_D_S); do_lasx_emu_xvfcvth_d_s(uc, instr); return 1;
    case OP22_XVFFINTL_D_W: perf_inc(P_XVFFINTL_D_W); do_lasx_emu_xvffintl_d_w(uc, instr); return 1;
    case OP22_XVFFINTH_D_W: perf_inc(P_XVFFINTH_D_W); do_lasx_emu_xvffinth_d_w(uc, instr); return 1;
    case OP22_XVFFINT_S_W: perf_inc(P_XVFFINT_S_W); do_lasx_emu_xvffint_s_w(uc, instr); return 1;
    case OP22_XVFFINT_D_L: perf_inc(P_XVFFINT_D_L); do_lasx_emu_xvffint_d_l(uc, instr); return 1;
    case OP22_XVFFINT_S_WU: perf_inc(P_XVFFINT_S_WU); do_lasx_emu_xvffint_s_wu(uc, instr); return 1;
    case OP22_XVFFINT_D_LU: perf_inc(P_XVFFINT_D_LU); do_lasx_emu_xvffint_d_lu(uc, instr); return 1;
    case OP22_XVFTINT_W_S: perf_inc(P_XVFTINT_W_S); do_lasx_emu_xvftint_w_s(uc, instr); return 1;
    case OP22_XVFTINT_L_D: perf_inc(P_XVFTINT_L_D); do_lasx_emu_xvftint_l_d(uc, instr); return 1;
    case OP22_XVFTINTRM_W_S: perf_inc(P_XVFTINTRM_W_S); do_lasx_emu_xvftintrm_w_s(uc, instr); return 1;
    case OP22_XVFTINTRM_L_D: perf_inc(P_XVFTINTRM_L_D); do_lasx_emu_xvftintrm_l_d(uc, instr); return 1;
    case OP22_XVFTINTRP_W_S: perf_inc(P_XVFTINTRP_W_S); do_lasx_emu_xvftintrp_w_s(uc, instr); return 1;
    case OP22_XVFTINTRP_L_D: perf_inc(P_XVFTINTRP_L_D); do_lasx_emu_xvftintrp_l_d(uc, instr); return 1;
    case OP22_XVFTINTRZ_W_S: perf_inc(P_XVFTINTRZ_W_S); do_lasx_emu_xvftintrz_w_s(uc, instr); return 1;
    case OP22_XVFTINTRZ_L_D: perf_inc(P_XVFTINTRZ_L_D); do_lasx_emu_xvftintrz_l_d(uc, instr); return 1;
    case OP22_XVFTINTRNE_W_S: perf_inc(P_XVFTINTRNE_W_S); do_lasx_emu_xvftintrne_w_s(uc, instr); return 1;
    case OP22_XVFTINTRNE_L_D: perf_inc(P_XVFTINTRNE_L_D); do_lasx_emu_xvftintrne_l_d(uc, instr); return 1;
    case OP22_XVFTINT_WU_S: perf_inc(P_XVFTINT_WU_S); do_lasx_emu_xvftint_wu_s(uc, instr); return 1;
    case OP22_XVFTINT_LU_D: perf_inc(P_XVFTINT_LU_D); do_lasx_emu_xvftint_lu_d(uc, instr); return 1;
    case OP22_XVFTINTL_L_S: perf_inc(P_XVFTINTL_L_S); do_lasx_emu_xvftintl_l_s(uc, instr); return 1;
    case OP22_XVFTINTH_L_S: perf_inc(P_XVFTINTH_L_S); do_lasx_emu_xvftinth_l_s(uc, instr); return 1;
    case OP22_XVFTINTRML_L_S: perf_inc(P_XVFTINTRML_L_S); do_lasx_emu_xvftintrml_l_s(uc, instr); return 1;
    case OP22_XVFTINTRMH_L_S: perf_inc(P_XVFTINTRMH_L_S); do_lasx_emu_xvftintrmh_l_s(uc, instr); return 1;
    case OP22_XVFTINTRPH_L_S: perf_inc(P_XVFTINTRPH_L_S); do_lasx_emu_xvftintrph_l_s(uc, instr); return 1;
    case OP22_XVFTINTRNEL_L_S: perf_inc(P_XVFTINTRNEL_L_S); do_lasx_emu_xvftintrnel_l_s(uc, instr); return 1;
    case OP22_XVEXTH_H_B: perf_inc(P_XVEXTH_H_B); do_lasx_emu_xvexth_h_b(uc, instr); return 1;
    case OP22_XVEXTH_W_H: perf_inc(P_XVEXTH_W_H); do_lasx_emu_xvexth_w_h(uc, instr); return 1;
    case OP22_XVEXTH_D_W: perf_inc(P_XVEXTH_D_W); do_lasx_emu_xvexth_d_w(uc, instr); return 1;
    case OP22_XVEXTH_Q_D: perf_inc(P_XVEXTH_Q_D); do_lasx_emu_xvexth_q_d(uc, instr); return 1;
    case OP22_XVEXTH_HU_BU: perf_inc(P_XVEXTH_HU_BU); do_lasx_emu_xvexth_hu_bu(uc, instr); return 1;
    case OP22_XVEXTH_WU_HU: perf_inc(P_XVEXTH_WU_HU); do_lasx_emu_xvexth_wu_hu(uc, instr); return 1;
    case OP22_XVEXTH_DU_WU: perf_inc(P_XVEXTH_DU_WU); do_lasx_emu_xvexth_du_wu(uc, instr); return 1;
    case OP22_XVEXTH_QU_DU: perf_inc(P_XVEXTH_QU_DU); do_lasx_emu_xvexth_qu_du(uc, instr); return 1;
    case OP22_XVEXTL_Q_D: perf_inc(P_XVEXTL_Q_D); do_lasx_emu_xvextl_q_d(uc, instr); return 1;
    case OP22_XVEXTL_QU_DU: perf_inc(P_XVEXTL_QU_DU); do_lasx_emu_xvextl_qu_du(uc, instr); return 1;
    case OP22_XVREPLVE0_B: perf_inc(P_XVREPLVE0_B); do_lasx_emu_xvreplve0_b(uc, instr); return 1;
    case OP22_XVREPLVE0_H: perf_inc(P_XVREPLVE0_H); do_lasx_emu_xvreplve0_h(uc, instr); return 1;
    case OP22_XVREPLVE0_W: perf_inc(P_XVREPLVE0_W); do_lasx_emu_xvreplve0_w(uc, instr); return 1;
    case OP22_XVREPLVE0_D: perf_inc(P_XVREPLVE0_D); do_lasx_emu_xvreplve0_d(uc, instr); return 1;
    case OP22_XVREPLVE0_Q: perf_inc(P_XVREPLVE0_Q); do_lasx_emu_xvreplve0_q(uc, instr); return 1;
    case OP22_XVFCVTL_S_H: perf_inc(P_XVFCVTL_S_H); do_lasx_emu_xvfcvtl_s_h(uc, instr); return 1;
    case OP22_XVFRINTRP_D: perf_inc(P_XVFRINTRP_D); do_lasx_emu_xvfrintrp_d(uc, instr); return 1;
    case OP22_XVFRINTRP_S: perf_inc(P_XVFRINTRP_S); do_lasx_emu_xvfrintrp_s(uc, instr); return 1;
    case OP22_XVFTINTRNEH_L_S: perf_inc(P_XVFTINTRNEH_L_S); do_lasx_emu_xvftintrneh_l_s(uc, instr); return 1;
    case OP22_XVFTINTRPL_L_S: perf_inc(P_XVFTINTRPL_L_S); do_lasx_emu_xvftintrpl_l_s(uc, instr); return 1;
    case OP22_XVFTINTRZ_LU_D: perf_inc(P_XVFTINTRZ_LU_D); do_lasx_emu_xvftintrz_lu_d(uc, instr); return 1;
    case OP22_XVFTINTRZ_WU_S: perf_inc(P_XVFTINTRZ_WU_S); do_lasx_emu_xvftintrz_wu_s(uc, instr); return 1;
    case OP22_XVFTINTRZH_L_S: perf_inc(P_XVFTINTRZH_L_S); do_lasx_emu_xvftintrzh_l_s(uc, instr); return 1;
    case OP22_XVFTINTRZL_L_S: perf_inc(P_XVFTINTRZL_L_S); do_lasx_emu_xvftintrzl_l_s(uc, instr); return 1;
    case OP22_XVMSKGEZ_B: perf_inc(P_XVMSKGEZ_B); do_lasx_emu_xvmskgez_b(uc, instr); return 1;
    case OP22_XVREPLGR2VR_B: perf_inc(P_XVREPLGR2VR_B); do_lasx_emu_xvreplgr2vr_b(uc, instr); return 1;
    case OP22_XVREPLGR2VR_D: perf_inc(P_XVREPLGR2VR_D); do_lasx_emu_xvreplgr2vr_d(uc, instr); return 1;
    case OP22_XVREPLGR2VR_H: perf_inc(P_XVREPLGR2VR_H); do_lasx_emu_xvreplgr2vr_h(uc, instr); return 1;
    case OP22_XVREPLGR2VR_W: perf_inc(P_XVREPLGR2VR_W); do_lasx_emu_xvreplgr2vr_w(uc, instr); return 1;
    case OP22_VEXT2XV_H_B: perf_inc(P_VEXT2XV_H_B); do_lasx_emu_vext2xv_h_b(uc, instr); return 1;
    case OP22_VEXT2XV_W_B: perf_inc(P_VEXT2XV_W_B); do_lasx_emu_vext2xv_w_b(uc, instr); return 1;
    case OP22_VEXT2XV_D_B: perf_inc(P_VEXT2XV_D_B); do_lasx_emu_vext2xv_d_b(uc, instr); return 1;
    case OP22_VEXT2XV_W_H: perf_inc(P_VEXT2XV_W_H); do_lasx_emu_vext2xv_w_h(uc, instr); return 1;
    case OP22_VEXT2XV_D_H: perf_inc(P_VEXT2XV_D_H); do_lasx_emu_vext2xv_d_h(uc, instr); return 1;
    case OP22_VEXT2XV_D_W: perf_inc(P_VEXT2XV_D_W); do_lasx_emu_vext2xv_d_w(uc, instr); return 1;
    case OP22_VEXT2XV_HU_BU: perf_inc(P_VEXT2XV_HU_BU); do_lasx_emu_vext2xv_hu_bu(uc, instr); return 1;
    case OP22_VEXT2XV_WU_BU: perf_inc(P_VEXT2XV_WU_BU); do_lasx_emu_vext2xv_wu_bu(uc, instr); return 1;
    case OP22_VEXT2XV_DU_BU: perf_inc(P_VEXT2XV_DU_BU); do_lasx_emu_vext2xv_du_bu(uc, instr); return 1;
    case OP22_VEXT2XV_WU_HU: perf_inc(P_VEXT2XV_WU_HU); do_lasx_emu_vext2xv_wu_hu(uc, instr); return 1;
    case OP22_VEXT2XV_DU_HU: perf_inc(P_VEXT2XV_DU_HU); do_lasx_emu_vext2xv_du_hu(uc, instr); return 1;
    case OP22_VEXT2XV_DU_WU: perf_inc(P_VEXT2XV_DU_WU); do_lasx_emu_vext2xv_du_wu(uc, instr); return 1;
    case OP22_XVCLO_B: perf_inc(P_XVCLO_B); do_lasx_emu_xvclo_b(uc, instr); return 1;
    case OP22_XVCLO_H: perf_inc(P_XVCLO_H); do_lasx_emu_xvclo_h(uc, instr); return 1;
    case OP22_XVCLO_W: perf_inc(P_XVCLO_W); do_lasx_emu_xvclo_w(uc, instr); return 1;
    case OP22_XVCLO_D: perf_inc(P_XVCLO_D); do_lasx_emu_xvclo_d(uc, instr); return 1;
    case OP22_XVCLZ_B: perf_inc(P_XVCLZ_B); do_lasx_emu_xvclz_b(uc, instr); return 1;
    case OP22_XVCLZ_H: perf_inc(P_XVCLZ_H); do_lasx_emu_xvclz_h(uc, instr); return 1;
    case OP22_XVCLZ_W: perf_inc(P_XVCLZ_W); do_lasx_emu_xvclz_w(uc, instr); return 1;
    case OP22_XVCLZ_D: perf_inc(P_XVCLZ_D); do_lasx_emu_xvclz_d(uc, instr); return 1;
    case OP22_XVPCNT_B: perf_inc(P_XVPCNT_B); do_lasx_emu_xvpcnt_b(uc, instr); return 1;
    case OP22_XVPCNT_H: perf_inc(P_XVPCNT_H); do_lasx_emu_xvpcnt_h(uc, instr); return 1;
    case OP22_XVPCNT_W: perf_inc(P_XVPCNT_W); do_lasx_emu_xvpcnt_w(uc, instr); return 1;
    case OP22_XVPCNT_D: perf_inc(P_XVPCNT_D); do_lasx_emu_xvpcnt_d(uc, instr); return 1;
    case OP22_XVNEG_B: perf_inc(P_XVNEG_B); do_lasx_emu_xvneg_b(uc, instr); return 1;
    case OP22_XVNEG_H: perf_inc(P_XVNEG_H); do_lasx_emu_xvneg_h(uc, instr); return 1;
    case OP22_XVNEG_W: perf_inc(P_XVNEG_W); do_lasx_emu_xvneg_w(uc, instr); return 1;
    case OP22_XVNEG_D: perf_inc(P_XVNEG_D); do_lasx_emu_xvneg_d(uc, instr); return 1;
    case OP22_XVMSKLTZ_B: perf_inc(P_XVMSKLTZ_B); do_lasx_emu_xvmskltz_b(uc, instr); return 1;
    case OP22_XVMSKLTZ_H: perf_inc(P_XVMSKLTZ_H); do_lasx_emu_xvmskltz_h(uc, instr); return 1;
    case OP22_XVMSKLTZ_W: perf_inc(P_XVMSKLTZ_W); do_lasx_emu_xvmskltz_w(uc, instr); return 1;
    case OP22_XVMSKLTZ_D: perf_inc(P_XVMSKLTZ_D); do_lasx_emu_xvmskltz_d(uc, instr); return 1;
    case OP22_XVMSKNZ_B: perf_inc(P_XVMSKNZ_B); do_lasx_emu_xvmsknz_b(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 11) {
    case OP21_XVREPL128VEI_D: perf_inc(P_XVREPL128VEI_D); do_lasx_emu_xvrepl128vei_d(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 12) {
    case OP20_XVINSGR2VR_D: perf_inc(P_XVINSGR2VR_D); do_lasx_emu_xvinsgr2vr_d(uc, instr); return 1;
    case OP20_XVREPL128VEI_W: perf_inc(P_XVREPL128VEI_W); do_lasx_emu_xvrepl128vei_w(uc, instr); return 1;
    case OP20_XVINSVE0_D: perf_inc(P_XVINSVE0_D); do_lasx_emu_xvinsve0_d(uc, instr); return 1;
    case OP20_XVPICKVE2GR_D: perf_inc(P_XVPICKVE2GR_D); do_lasx_emu_xvpickve2gr_d(uc, instr); return 1;
    case OP20_XVPICKVE2GR_DU: perf_inc(P_XVPICKVE2GR_DU); do_lasx_emu_xvpickve2gr_du(uc, instr); return 1;
    case OP20_XVPICKVE_D: perf_inc(P_XVPICKVE_D); do_lasx_emu_xvpickve_d(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 13) {
    case OP19_XVBITSETI_B: perf_inc(P_XVBITSETI_B); do_lasx_emu_xvbitseti_b(uc, instr); return 1;
    case OP19_XVINSGR2VR_W: perf_inc(P_XVINSGR2VR_W); do_lasx_emu_xvinsgr2vr_w(uc, instr); return 1;
    case OP19_XVREPL128VEI_H: perf_inc(P_XVREPL128VEI_H); do_lasx_emu_xvrepl128vei_h(uc, instr); return 1;
    case OP19_XVBITREVI_B: perf_inc(P_XVBITREVI_B); do_lasx_emu_xvbitrevi_b(uc, instr); return 1;
    case OP19_XVSLLI_B: perf_inc(P_XVSLLI_B); do_lasx_emu_xvslli_b(uc, instr); return 1;
    case OP19_XVSRAI_B: perf_inc(P_XVSRAI_B); do_lasx_emu_xvsrai_b(uc, instr); return 1;
    case OP19_XVSRARI_B: perf_inc(P_XVSRARI_B); do_lasx_emu_xvsrari_b(uc, instr); return 1;
    case OP19_XVSRLRI_B: perf_inc(P_XVSRLRI_B); do_lasx_emu_xvsrlri_b(uc, instr); return 1;
    case OP19_XVSAT_B: perf_inc(P_XVSAT_B); do_lasx_emu_xvsat_b(uc, instr); return 1;
    case OP19_XVSAT_BU: perf_inc(P_XVSAT_BU); do_lasx_emu_xvsat_bu(uc, instr); return 1;
    case OP19_XVBITCLRI_B: perf_inc(P_XVBITCLRI_B); do_lasx_emu_xvbitclri_b(uc, instr); return 1;
    case OP19_XVINSVE0_W: perf_inc(P_XVINSVE0_W); do_lasx_emu_xvinsve0_w(uc, instr); return 1;
    case OP19_XVPICKVE2GR_W: perf_inc(P_XVPICKVE2GR_W); do_lasx_emu_xvpickve2gr_w(uc, instr); return 1;
    case OP19_XVPICKVE2GR_WU: perf_inc(P_XVPICKVE2GR_WU); do_lasx_emu_xvpickve2gr_wu(uc, instr); return 1;
    case OP19_XVPICKVE_W: perf_inc(P_XVPICKVE_W); do_lasx_emu_xvpickve_w(uc, instr); return 1;
    case OP19_XVROTRI_B: perf_inc(P_XVROTRI_B); do_lasx_emu_xvrotri_b(uc, instr); return 1;
    case OP19_XVSLLWIL_H_B: perf_inc(P_XVSLLWIL_H_B); do_lasx_emu_xvsllwil_h_b(uc, instr); return 1;
    case OP19_XVSLLWIL_HU_BU: perf_inc(P_XVSLLWIL_HU_BU); do_lasx_emu_xvsllwil_hu_bu(uc, instr); return 1;
    case OP19_XVSRLI_B: perf_inc(P_XVSRLI_B); do_lasx_emu_xvsrli_b(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 14) {
    case OP18_XVREPL128VEI_B: perf_inc(P_XVREPL128VEI_B); do_lasx_emu_xvrepl128vei_b(uc, instr); return 1;
    case OP18_XVBITREVI_H: perf_inc(P_XVBITREVI_H); do_lasx_emu_xvbitrevi_h(uc, instr); return 1;
    case OP18_XVSLLI_H: perf_inc(P_XVSLLI_H); do_lasx_emu_xvslli_h(uc, instr); return 1;
    case OP18_XVSRAI_H: perf_inc(P_XVSRAI_H); do_lasx_emu_xvsrai_h(uc, instr); return 1;
    case OP18_XVSRANI_B_H: perf_inc(P_XVSRANI_B_H); do_lasx_emu_xvsrani_b_h(uc, instr); return 1;
    case OP18_XVSRARI_H: perf_inc(P_XVSRARI_H); do_lasx_emu_xvsrari_h(uc, instr); return 1;
    case OP18_XVSRARNI_B_H: perf_inc(P_XVSRARNI_B_H); do_lasx_emu_xvsrarni_b_h(uc, instr); return 1;
    case OP18_XVSRLNI_B_H: perf_inc(P_XVSRLNI_B_H); do_lasx_emu_xvsrlni_b_h(uc, instr); return 1;
    case OP18_XVSSRLRNI_B_H: perf_inc(P_XVSSRLRNI_B_H); do_lasx_emu_xvssrlrni_b_h(uc, instr); return 1;
    case OP18_XVSAT_H: perf_inc(P_XVSAT_H); do_lasx_emu_xvsat_h(uc, instr); return 1;
    case OP18_XVSAT_HU: perf_inc(P_XVSAT_HU); do_lasx_emu_xvsat_hu(uc, instr); return 1;
    case OP18_XVBITCLRI_H: perf_inc(P_XVBITCLRI_H); do_lasx_emu_xvbitclri_h(uc, instr); return 1;
    case OP18_XVBITSETI_H: perf_inc(P_XVBITSETI_H); do_lasx_emu_xvbitseti_h(uc, instr); return 1;
    case OP18_XVROTRI_H: perf_inc(P_XVROTRI_H); do_lasx_emu_xvrotri_h(uc, instr); return 1;
    case OP18_XVSLLWIL_W_H: perf_inc(P_XVSLLWIL_W_H); do_lasx_emu_xvsllwil_w_h(uc, instr); return 1;
    case OP18_XVSLLWIL_WU_HU: perf_inc(P_XVSLLWIL_WU_HU); do_lasx_emu_xvsllwil_wu_hu(uc, instr); return 1;
    case OP18_XVSRLI_H: perf_inc(P_XVSRLI_H); do_lasx_emu_xvsrli_h(uc, instr); return 1;
    case OP18_XVSRLRI_H: perf_inc(P_XVSRLRI_H); do_lasx_emu_xvsrlri_h(uc, instr); return 1;
    case OP18_XVSRLRNI_B_H: perf_inc(P_XVSRLRNI_B_H); do_lasx_emu_xvsrlrni_b_h(uc, instr); return 1;
    case OP18_XVSSRANI_B_H: perf_inc(P_XVSSRANI_B_H); do_lasx_emu_xvssrani_b_h(uc, instr); return 1;
    case OP18_XVSSRANI_BU_H: perf_inc(P_XVSSRANI_BU_H); do_lasx_emu_xvssrani_bu_h(uc, instr); return 1;
    case OP18_XVSSRARNI_B_H: perf_inc(P_XVSSRARNI_B_H); do_lasx_emu_xvssrarni_b_h(uc, instr); return 1;
    case OP18_XVSSRARNI_BU_H: perf_inc(P_XVSSRARNI_BU_H); do_lasx_emu_xvssrarni_bu_h(uc, instr); return 1;
    case OP18_XVSSRLNI_B_H: perf_inc(P_XVSSRLNI_B_H); do_lasx_emu_xvssrlni_b_h(uc, instr); return 1;
    case OP18_XVSSRLNI_BU_H: perf_inc(P_XVSSRLNI_BU_H); do_lasx_emu_xvssrlni_bu_h(uc, instr); return 1;
    case OP18_XVSSRLRNI_BU_H: perf_inc(P_XVSSRLRNI_BU_H); do_lasx_emu_xvssrlrni_bu_h(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 16) {
    case OP16_XVBITREVI_D: perf_inc(P_XVBITREVI_D); do_lasx_emu_xvbitrevi_d(uc, instr); return 1;
    case OP16_XVROTRI_D: perf_inc(P_XVROTRI_D); do_lasx_emu_xvrotri_d(uc, instr); return 1;
    case OP16_XVSLLI_D: perf_inc(P_XVSLLI_D); do_lasx_emu_xvslli_d(uc, instr); return 1;
    case OP16_XVSRAI_D: perf_inc(P_XVSRAI_D); do_lasx_emu_xvsrai_d(uc, instr); return 1;
    case OP16_XVSRLI_D: perf_inc(P_XVSRLI_D); do_lasx_emu_xvsrli_d(uc, instr); return 1;
    case OP16_XVSRLRI_D: perf_inc(P_XVSRLRI_D); do_lasx_emu_xvsrlri_d(uc, instr); return 1;
    case OP16_XVSRLNI_W_D: perf_inc(P_XVSRLNI_W_D); do_lasx_emu_xvsrlni_w_d(uc, instr); return 1;
    case OP16_XVSRLRNI_W_D: perf_inc(P_XVSRLRNI_W_D); do_lasx_emu_xvsrlrni_w_d(uc, instr); return 1;
    case OP16_XVSSRANI_W_D: perf_inc(P_XVSSRANI_W_D); do_lasx_emu_xvssrani_w_d(uc, instr); return 1;
    case OP16_XVSSRANI_WU_D: perf_inc(P_XVSSRANI_WU_D); do_lasx_emu_xvssrani_wu_d(uc, instr); return 1;
    case OP16_XVSSRARNI_W_D: perf_inc(P_XVSSRARNI_W_D); do_lasx_emu_xvssrarni_w_d(uc, instr); return 1;
    case OP16_XVSSRARNI_WU_D: perf_inc(P_XVSSRARNI_WU_D); do_lasx_emu_xvssrarni_wu_d(uc, instr); return 1;
    case OP16_XVSSRLNI_W_D: perf_inc(P_XVSSRLNI_W_D); do_lasx_emu_xvssrlni_w_d(uc, instr); return 1;
    case OP16_XVSSRLNI_WU_D: perf_inc(P_XVSSRLNI_WU_D); do_lasx_emu_xvssrlni_wu_d(uc, instr); return 1;
    case OP16_XVSSRLRNI_W_D: perf_inc(P_XVSSRLRNI_W_D); do_lasx_emu_xvssrlrni_w_d(uc, instr); return 1;
    case OP16_XVSSRLRNI_WU_D: perf_inc(P_XVSSRLRNI_WU_D); do_lasx_emu_xvssrlrni_wu_d(uc, instr); return 1;
    case OP16_XVSAT_D: perf_inc(P_XVSAT_D); do_lasx_emu_xvsat_d(uc, instr); return 1;
    case OP16_XVSAT_DU: perf_inc(P_XVSAT_DU); do_lasx_emu_xvsat_du(uc, instr); return 1;
    case OP16_XVBITCLRI_D: perf_inc(P_XVBITCLRI_D); do_lasx_emu_xvbitclri_d(uc, instr); return 1;
    case OP16_XVBITSETI_D: perf_inc(P_XVBITSETI_D); do_lasx_emu_xvbitseti_d(uc, instr); return 1;
    case OP16_XVSRANI_W_D: perf_inc(P_XVSRANI_W_D); do_lasx_emu_xvsrani_w_d(uc, instr); return 1;
    case OP16_XVSRARI_D: perf_inc(P_XVSRARI_D); do_lasx_emu_xvsrari_d(uc, instr); return 1;
    case OP16_XVSRARNI_W_D: perf_inc(P_XVSRARNI_W_D); do_lasx_emu_xvsrarni_w_d(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 17) {
    case OP15_XVSRANI_D_Q: perf_inc(P_XVSRANI_D_Q); do_lasx_emu_xvsrani_d_q(uc, instr); return 1;
    case OP15_XVSRARNI_D_Q: perf_inc(P_XVSRARNI_D_Q); do_lasx_emu_xvsrarni_d_q(uc, instr); return 1;
    case OP15_XVSRLNI_D_Q: perf_inc(P_XVSRLNI_D_Q); do_lasx_emu_xvsrlni_d_q(uc, instr); return 1;
    case OP15_XVSRLRNI_D_Q: perf_inc(P_XVSRLRNI_D_Q); do_lasx_emu_xvsrlrni_d_q(uc, instr); return 1;
    case OP15_XVSSRANI_D_Q: perf_inc(P_XVSSRANI_D_Q); do_lasx_emu_xvssrani_d_q(uc, instr); return 1;
    case OP15_XVSSRANI_DU_Q: perf_inc(P_XVSSRANI_DU_Q); do_lasx_emu_xvssrani_du_q(uc, instr); return 1;
    case OP15_XVSSRARNI_D_Q: perf_inc(P_XVSSRARNI_D_Q); do_lasx_emu_xvssrarni_d_q(uc, instr); return 1;
    case OP15_XVSSRARNI_DU_Q: perf_inc(P_XVSSRARNI_DU_Q); do_lasx_emu_xvssrarni_du_q(uc, instr); return 1;
    case OP15_XVSSRLNI_D_Q: perf_inc(P_XVSSRLNI_D_Q); do_lasx_emu_xvssrlni_d_q(uc, instr); return 1;
    case OP15_XVSSRLNI_DU_Q: perf_inc(P_XVSSRLNI_DU_Q); do_lasx_emu_xvssrlni_du_q(uc, instr); return 1;
    case OP15_XVSSRLRNI_D_Q: perf_inc(P_XVSSRLRNI_D_Q); do_lasx_emu_xvssrlrni_d_q(uc, instr); return 1;
    case OP15_XVSSRLRNI_DU_Q: perf_inc(P_XVSSRLRNI_DU_Q); do_lasx_emu_xvssrlrni_du_q(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 20) {
    case OP12_XVBITSEL_V: perf_inc(P_XVBITSEL_V); do_lasx_emu_xvbitsel_v(uc, instr); return 1;
    case OP12_XVFMSUB_D: perf_inc(P_XVFMSUB_D); do_lasx_emu_xvfmsub_d(uc, instr); return 1;
    case OP12_XVFMSUB_S: perf_inc(P_XVFMSUB_S); do_lasx_emu_xvfmsub_s(uc, instr); return 1;
    case OP12_XVSHUF_B: perf_inc(P_XVSHUF_B); do_lasx_emu_xvshuf_b(uc, instr); return 1;
    default: break;
    }
    switch (instr >> 19) {
    case OP13_XVLDREPL_D: perf_inc(P_XVLDREPL_D); do_lasx_emu_xvldrepl_d(uc, instr); return 1;
    default: break;
    }

    // XVLDREPL.W xd, rj, si10    001100100010 si10 rj xd
    // XVSTELM.D xd, rj, si8, idx 001100110001 idx si8 rj xd
    switch (instr >> 20) {
    case OP12_XVLDREPL_W: perf_inc(P_XVLDREPL_W); do_lasx_emu_xvldrepl_w(uc, instr); return 1;
    case OP12_XVSTELM_D: perf_inc(P_XVSTELM_D); do_lasx_emu_xvstelm_d(uc, instr); return 1;
    default: break;
    }
    // XVLDREPL.H xd, rj, si11    00110010010 si11 rj xd
    // XVSTELM.W xd, rj, si8, idx 00110011001 idx si8 rj xd
    switch (instr >> 21) {
    case OP11_XVLDREPL_H: perf_inc(P_XVLDREPL_H); do_lasx_emu_xvldrepl_h(uc, instr); return 1;
    case OP11_XVSTELM_W: perf_inc(P_XVSTELM_W); do_lasx_emu_xvstelm_w(uc, instr); return 1;
    default: break;
    }
    // XVLDREPL.B xd, rj, si12    0011001010 si12 rj xd
    // XVSTELM.H xd, rj, si8, idx 0011001101 idx si8 rj xd
    switch (instr >> 22) {
    case OP10_XVLDREPL_B: perf_inc(P_XVLDREPL_B); do_lasx_emu_xvldrepl_b(uc, instr); return 1;
    case OP10_XVSTELM_H: perf_inc(P_XVSTELM_H); do_lasx_emu_xvstelm_h(uc, instr); return 1;
    default: break;
    }
    // XVSTELM.B xd, rj, si8, idx 001100111 idx si8 rj xd
    switch (instr >> 23) {
    case OP9_XVSTELM_B: perf_inc(P_XVSTELM_B); do_lasx_emu_xvstelm_b(uc, instr); return 1;
    default: break;
    }

    switch (instr >> 22) {
    case OP10_XVLD: perf_inc(P_XVLD); do_lasx_emu_xvld(uc, instr); return 1;
    case OP10_XVST: perf_inc(P_XVST); do_lasx_emu_xvst(uc, instr); return 1;
    default: return 0;
    }
}
