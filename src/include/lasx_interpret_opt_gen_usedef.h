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

#ifndef LASX_INTERPRET_OPT_GEN_USEDEF_H
#define LASX_INTERPRET_OPT_GEN_USEDEF_H

#include "lagoon.h"

/* Dispatch: translate one LASX instruction to LSX using temp_vr remapping.
   temp_vr[xr] = VR number for this XR's high half (or -1 if not remapped).
   Returns 1 if handled, 0 if instruction is not XV. */
int lasx_interpret_inst_gen_usedef(void *as, unsigned int instr, int *temp_vr);

/* Block-level use/def analysis result. */
typedef struct {
    int first_use[32];
    int last_use[32];
    int first_write[32];
    int last_write[32];
    int temp_vr[32];
    int insn_count;
} block_usedef_t;

int build_block_usedef(unsigned int *p, int insn_count, block_usedef_t *ud,
                        int temp_vr_map[][32], uint32_t *temp_vr_all_bm);
void emit_block_usedef_prologue(void *as, block_usedef_t *ud);
void emit_block_usedef_epilogue(void *as, block_usedef_t *ud,
                                 int last_temp_vr[32], uint32_t temp_vr_all_bm);
void emit_mini_prologue_epilogue(void *as, int insn_idx,
                                  int prev_map[32], int cur_map[32]);
void emit_xr_load_hi(void *as, int xr, int *temp_vr);
void emit_xr_store_hi(void *as, int xr, int *temp_vr);

/* ===== USEDEF GEN_ macros ===== */

#define GEN_XVABSD_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_bu_opt_usedef(as, instr, tv)
#define GEN_XVADD_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadd_b_opt_usedef(as, instr, tv)
#define GEN_XVADD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadd_d_opt_usedef(as, instr, tv)
#define GEN_XVADD_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadd_q_opt_usedef(as, instr, tv)
#define GEN_XVADDA_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadda_h_opt_usedef(as, instr, tv)
#define GEN_XVADDA_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadda_w_opt_usedef(as, instr, tv)
#define GEN_XVADDI_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddi_bu_opt_usedef(as, instr, tv)
#define GEN_XVADDI_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddi_du_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_w_h_opt_usedef(as, instr, tv)
#define GEN_XVANDN_V_USEDEF(as, instr, tv) __gen_lasx_interpret_xvandn_v_opt_usedef(as, instr, tv)
#define GEN_XVAVGR_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavgr_b_opt_usedef(as, instr, tv)
#define GEN_XVAVGR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavgr_d_opt_usedef(as, instr, tv)
#define GEN_XVAVGR_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavgr_du_opt_usedef(as, instr, tv)
#define GEN_XVAVGR_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavgr_h_opt_usedef(as, instr, tv)
#define GEN_XVAVGR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavgr_w_opt_usedef(as, instr, tv)
#define GEN_XVCLZ_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclz_d_opt_usedef(as, instr, tv)
#define GEN_XVCLZ_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclz_w_opt_usedef(as, instr, tv)
#define GEN_XVFDIV_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfdiv_d_opt_usedef(as, instr, tv)
#define GEN_XVFDIV_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfdiv_s_opt_usedef(as, instr, tv)
#define GEN_XVFMAX_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmax_d_opt_usedef(as, instr, tv)
#define GEN_XVFMIN_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmin_d_opt_usedef(as, instr, tv)
#define GEN_XVFSUB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfsub_d_opt_usedef(as, instr, tv)
#define GEN_XVHADDW_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhaddw_d_w_opt_usedef(as, instr, tv)
#define GEN_XVHADDW_DU_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhaddw_du_wu_opt_usedef(as, instr, tv)
#define GEN_XVHADDW_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhaddw_q_d_opt_usedef(as, instr, tv)
#define GEN_XVHADDW_QU_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhaddw_qu_du_opt_usedef(as, instr, tv)
#define GEN_XVHADDW_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhaddw_w_h_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_HU_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_hu_bu_opt_usedef(as, instr, tv)
#define GEN_XVMADD_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmadd_b_opt_usedef(as, instr, tv)
#define GEN_XVMADD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmadd_d_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_H_BU_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_h_bu_b_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_w_h_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_H_BU_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_h_bu_b_opt_usedef(as, instr, tv)
#define GEN_XVMAX_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_h_opt_usedef(as, instr, tv)
#define GEN_XVMIN_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_h_opt_usedef(as, instr, tv)
#define GEN_XVMSKLTZ_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmskltz_w_opt_usedef(as, instr, tv)
#define GEN_XVMUL_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmul_b_opt_usedef(as, instr, tv)
#define GEN_XVMUL_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmul_d_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_H_BU_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_h_bu_b_opt_usedef(as, instr, tv)
#define GEN_XVNEG_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvneg_h_opt_usedef(as, instr, tv)
#define GEN_XVORN_V_USEDEF(as, instr, tv) __gen_lasx_interpret_xvorn_v_opt_usedef(as, instr, tv)
#define GEN_XVPACKEV_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpackev_d_opt_usedef(as, instr, tv)
#define GEN_XVPACKEV_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpackev_w_opt_usedef(as, instr, tv)
#define GEN_XVPACKOD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpackod_d_opt_usedef(as, instr, tv)
#define GEN_XVPACKOD_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpackod_w_opt_usedef(as, instr, tv)
#define GEN_XVPCNT_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpcnt_b_opt_usedef(as, instr, tv)
#define GEN_XVPCNT_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpcnt_d_opt_usedef(as, instr, tv)
#define GEN_XVPCNT_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpcnt_h_opt_usedef(as, instr, tv)
#define GEN_XVPICKEV_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickev_w_opt_usedef(as, instr, tv)
#define GEN_XVSEQI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseqi_w_opt_usedef(as, instr, tv)
#define GEN_XVSIGNCOV_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsigncov_h_opt_usedef(as, instr, tv)
#define GEN_XVSLE_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_bu_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_h_opt_usedef(as, instr, tv)
#define GEN_XVSLL_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsll_h_opt_usedef(as, instr, tv)
#define GEN_XVSLL_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsll_w_opt_usedef(as, instr, tv)
#define GEN_XVSLLWIL_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsllwil_w_h_opt_usedef(as, instr, tv)
#define GEN_XVSLT_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_bu_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_b_opt_usedef(as, instr, tv)
#define GEN_XVSRANI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrani_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRAR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrar_w_opt_usedef(as, instr, tv)
#define GEN_XVSRLI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrli_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSUB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsub_d_opt_usedef(as, instr, tv)
#define GEN_XVSUB_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsub_q_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_H_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_h_bu_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_w_h_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_H_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_h_bu_opt_usedef(as, instr, tv)
#define GEN_XVLDREPL_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvldrepl_b_opt_usedef(as, instr, tv)
#define GEN_XVLDREPL_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvldrepl_h_opt_usedef(as, instr, tv)
#define GEN_XVLDREPL_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvldrepl_d_opt_usedef(as, instr, tv)
#define GEN_XVSTELM_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvstelm_h_opt_usedef(as, instr, tv)
#define GEN_XVREPLGR2VR_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplgr2vr_b_opt_usedef(as, instr, tv)
#define GEN_XVFCMP_COND_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcmp_cond_d_opt_usedef(as, instr, tv)
#define GEN_XVPICKVE2GR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickve2gr_d_opt_usedef(as, instr, tv)
#define GEN_XVPICKVE2GR_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickve2gr_du_opt_usedef(as, instr, tv)
#define GEN_XVLDI_USEDEF(as, instr, tv) __gen_lasx_interpret_xvldi_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_d_w_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_DU_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_du_wu_opt_usedef(as, instr, tv)
#define GEN_XVFMSUB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmsub_d_opt_usedef(as, instr, tv)
#define GEN_XVFNMADD_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfnmadd_s_opt_usedef(as, instr, tv)
#define GEN_XVFNMADD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfnmadd_d_opt_usedef(as, instr, tv)
#define GEN_XVFNMSUB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfnmsub_d_opt_usedef(as, instr, tv)
#define GEN_XVNORI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvnori_b_opt_usedef(as, instr, tv)
#define GEN_XVXORI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvxori_b_opt_usedef(as, instr, tv)
#define GEN_XVSHUF4I_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvshuf4i_w_opt_usedef(as, instr, tv)
#define GEN_XVSHUF4I_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvshuf4i_b_opt_usedef(as, instr, tv)
#define GEN_XVPERMI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpermi_w_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_D_H_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_d_h_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_W_B_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_w_b_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_D_B_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_d_b_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_DU_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_du_bu_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_DU_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_du_hu_opt_usedef(as, instr, tv)
#define GEN_XVREPL128VEI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrepl128vei_b_opt_usedef(as, instr, tv)
#define GEN_XVREPL128VEI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrepl128vei_h_opt_usedef(as, instr, tv)
#define GEN_XVREPL128VEI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrepl128vei_w_opt_usedef(as, instr, tv)
#define GEN_XVINSVE0_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvinsve0_w_opt_usedef(as, instr, tv)
#define GEN_XVINSVE0_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvinsve0_d_opt_usedef(as, instr, tv)
#define GEN_XVPICKVE_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickve_w_opt_usedef(as, instr, tv)
#define GEN_XVABSD_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_b_opt_usedef(as, instr, tv)
#define GEN_XVABSD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_d_opt_usedef(as, instr, tv)
#define GEN_XVABSD_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_du_opt_usedef(as, instr, tv)
#define GEN_XVABSD_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_h_opt_usedef(as, instr, tv)
#define GEN_XVABSD_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_hu_opt_usedef(as, instr, tv)
#define GEN_XVABSD_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_w_opt_usedef(as, instr, tv)
#define GEN_XVABSD_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvabsd_wu_opt_usedef(as, instr, tv)
#define GEN_XVADDA_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadda_b_opt_usedef(as, instr, tv)
#define GEN_XVADDA_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvadda_d_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_h_b_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_W_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_w_hu_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_h_b_opt_usedef(as, instr, tv)
#define GEN_XVCLZ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclz_b_opt_usedef(as, instr, tv)
#define GEN_XVCLZ_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclz_h_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_h_b_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_h_b_opt_usedef(as, instr, tv)
#define GEN_XVMAX_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_b_opt_usedef(as, instr, tv)
#define GEN_XVMAX_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_bu_opt_usedef(as, instr, tv)
#define GEN_XVMAX_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_d_opt_usedef(as, instr, tv)
#define GEN_XVMAX_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_du_opt_usedef(as, instr, tv)
#define GEN_XVMAX_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_hu_opt_usedef(as, instr, tv)
#define GEN_XVMAX_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_w_opt_usedef(as, instr, tv)
#define GEN_XVMAX_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmax_wu_opt_usedef(as, instr, tv)
#define GEN_XVMAXI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaxi_b_opt_usedef(as, instr, tv)
#define GEN_XVMAXI_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaxi_bu_opt_usedef(as, instr, tv)
#define GEN_XVMAXI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaxi_d_opt_usedef(as, instr, tv)
#define GEN_XVMAXI_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaxi_du_opt_usedef(as, instr, tv)
#define GEN_XVMAXI_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaxi_hu_opt_usedef(as, instr, tv)
#define GEN_XVMAXI_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaxi_wu_opt_usedef(as, instr, tv)
#define GEN_XVMIN_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_b_opt_usedef(as, instr, tv)
#define GEN_XVMIN_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_bu_opt_usedef(as, instr, tv)
#define GEN_XVMIN_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_d_opt_usedef(as, instr, tv)
#define GEN_XVMIN_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_du_opt_usedef(as, instr, tv)
#define GEN_XVMIN_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_hu_opt_usedef(as, instr, tv)
#define GEN_XVMIN_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmin_w_opt_usedef(as, instr, tv)
#define GEN_XVMSKLTZ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmskltz_b_opt_usedef(as, instr, tv)
#define GEN_XVMSKLTZ_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmskltz_d_opt_usedef(as, instr, tv)
#define GEN_XVMSKLTZ_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmskltz_h_opt_usedef(as, instr, tv)
#define GEN_XVMSUB_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmsub_b_opt_usedef(as, instr, tv)
#define GEN_XVMSUB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmsub_d_opt_usedef(as, instr, tv)
#define GEN_XVMSUB_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmsub_w_opt_usedef(as, instr, tv)
#define GEN_XVMUH_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_b_opt_usedef(as, instr, tv)
#define GEN_XVMUH_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_bu_opt_usedef(as, instr, tv)
#define GEN_XVMUH_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_d_opt_usedef(as, instr, tv)
#define GEN_XVMUH_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_du_opt_usedef(as, instr, tv)
#define GEN_XVMUH_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_h_opt_usedef(as, instr, tv)
#define GEN_XVMUH_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_w_opt_usedef(as, instr, tv)
#define GEN_XVMUH_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmuh_wu_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_h_b_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_h_b_opt_usedef(as, instr, tv)
#define GEN_XVNEG_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvneg_d_opt_usedef(as, instr, tv)
#define GEN_XVNEG_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvneg_w_opt_usedef(as, instr, tv)
#define GEN_XVPICKEV_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickev_d_opt_usedef(as, instr, tv)
#define GEN_XVSAT_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_b_opt_usedef(as, instr, tv)
#define GEN_XVSAT_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_bu_opt_usedef(as, instr, tv)
#define GEN_XVSAT_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_d_opt_usedef(as, instr, tv)
#define GEN_XVSAT_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_du_opt_usedef(as, instr, tv)
#define GEN_XVSAT_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_h_opt_usedef(as, instr, tv)
#define GEN_XVSAT_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_w_opt_usedef(as, instr, tv)
#define GEN_XVSEQI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseqi_b_opt_usedef(as, instr, tv)
#define GEN_XVSEQI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseqi_d_opt_usedef(as, instr, tv)
#define GEN_XVSEQI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseqi_h_opt_usedef(as, instr, tv)
#define GEN_XVSHUF4I_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvshuf4i_d_opt_usedef(as, instr, tv)
#define GEN_XVSHUF4I_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvshuf4i_h_opt_usedef(as, instr, tv)
#define GEN_XVSHUF_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvshuf_d_opt_usedef(as, instr, tv)
#define GEN_XVSHUF_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvshuf_w_opt_usedef(as, instr, tv)
#define GEN_XVSIGNCOV_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsigncov_b_opt_usedef(as, instr, tv)
#define GEN_XVSIGNCOV_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsigncov_d_opt_usedef(as, instr, tv)
#define GEN_XVSIGNCOV_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsigncov_w_opt_usedef(as, instr, tv)
#define GEN_XVSLE_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_b_opt_usedef(as, instr, tv)
#define GEN_XVSLE_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_d_opt_usedef(as, instr, tv)
#define GEN_XVSLE_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_du_opt_usedef(as, instr, tv)
#define GEN_XVSLE_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_h_opt_usedef(as, instr, tv)
#define GEN_XVSLE_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_hu_opt_usedef(as, instr, tv)
#define GEN_XVSLE_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_w_opt_usedef(as, instr, tv)
#define GEN_XVSLE_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsle_wu_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_b_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_bu_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_d_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_du_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_hu_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_w_opt_usedef(as, instr, tv)
#define GEN_XVSLEI_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslei_wu_opt_usedef(as, instr, tv)
#define GEN_XVSLL_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsll_d_opt_usedef(as, instr, tv)
#define GEN_XVSLLI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslli_d_opt_usedef(as, instr, tv)
#define GEN_XVSLT_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_b_opt_usedef(as, instr, tv)
#define GEN_XVSLT_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_d_opt_usedef(as, instr, tv)
#define GEN_XVSLT_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_du_opt_usedef(as, instr, tv)
#define GEN_XVSLT_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_h_opt_usedef(as, instr, tv)
#define GEN_XVSLT_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_hu_opt_usedef(as, instr, tv)
#define GEN_XVSLT_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslt_wu_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_bu_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_d_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_du_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_h_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_hu_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_w_opt_usedef(as, instr, tv)
#define GEN_XVSLTI_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslti_wu_opt_usedef(as, instr, tv)
#define GEN_XVSRAI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrai_b_opt_usedef(as, instr, tv)
#define GEN_XVSRAI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrai_d_opt_usedef(as, instr, tv)
#define GEN_XVSRAR_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrar_b_opt_usedef(as, instr, tv)
#define GEN_XVSRAR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrar_d_opt_usedef(as, instr, tv)
#define GEN_XVSRAR_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrar_h_opt_usedef(as, instr, tv)
#define GEN_XVSRARI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrari_b_opt_usedef(as, instr, tv)
#define GEN_XVSRARI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrari_d_opt_usedef(as, instr, tv)
#define GEN_XVSRL_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrl_d_opt_usedef(as, instr, tv)
#define GEN_XVSRL_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrl_h_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_h_b_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_W_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_w_hu_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_h_b_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_d_w_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_D_WU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_d_wu_w_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_H_BU_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_h_bu_b_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_q_d_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_q_du_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_Q_DU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_q_du_d_opt_usedef(as, instr, tv)
#define GEN_XVADDWEV_W_HU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwev_w_hu_h_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_d_w_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_D_WU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_d_wu_w_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_H_BU_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_h_bu_b_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_q_d_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_q_du_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_Q_DU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_q_du_d_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_w_h_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_W_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_w_hu_opt_usedef(as, instr, tv)
#define GEN_XVADDWOD_W_HU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvaddwod_w_hu_h_opt_usedef(as, instr, tv)
#define GEN_XVAVG_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_b_opt_usedef(as, instr, tv)
#define GEN_XVAVG_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_bu_opt_usedef(as, instr, tv)
#define GEN_XVAVG_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_d_opt_usedef(as, instr, tv)
#define GEN_XVAVG_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_du_opt_usedef(as, instr, tv)
#define GEN_XVAVG_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_h_opt_usedef(as, instr, tv)
#define GEN_XVAVG_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_hu_opt_usedef(as, instr, tv)
#define GEN_XVAVG_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_w_opt_usedef(as, instr, tv)
#define GEN_XVAVG_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvavg_wu_opt_usedef(as, instr, tv)
#define GEN_XVBITCLRI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclri_b_opt_usedef(as, instr, tv)
#define GEN_XVBITCLRI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclri_d_opt_usedef(as, instr, tv)
#define GEN_XVBITCLRI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclri_h_opt_usedef(as, instr, tv)
#define GEN_XVBITCLRI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclri_w_opt_usedef(as, instr, tv)
#define GEN_XVBITREVI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrevi_b_opt_usedef(as, instr, tv)
#define GEN_XVBITREVI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrevi_d_opt_usedef(as, instr, tv)
#define GEN_XVBITREVI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrevi_h_opt_usedef(as, instr, tv)
#define GEN_XVBITREVI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrevi_w_opt_usedef(as, instr, tv)
#define GEN_XVBITSETI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitseti_b_opt_usedef(as, instr, tv)
#define GEN_XVBITSETI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitseti_d_opt_usedef(as, instr, tv)
#define GEN_XVBITSETI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitseti_h_opt_usedef(as, instr, tv)
#define GEN_XVBITSETI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitseti_w_opt_usedef(as, instr, tv)
#define GEN_XVBSLL_V_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbsll_v_opt_usedef(as, instr, tv)
#define GEN_XVHADDW_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhaddw_h_b_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_d_w_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_DU_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_du_wu_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_h_b_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_q_d_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_QU_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_qu_du_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_w_h_opt_usedef(as, instr, tv)
#define GEN_XVHSUBW_WU_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvhsubw_wu_hu_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_d_w_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_D_WU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_d_wu_w_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_q_d_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_Q_DU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_q_du_d_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_W_HU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_w_hu_h_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_d_w_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_D_WU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_d_wu_w_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_q_d_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_Q_DU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_q_du_d_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_W_HU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_w_hu_h_opt_usedef(as, instr, tv)
#define GEN_XVMINI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_b_opt_usedef(as, instr, tv)
#define GEN_XVMINI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_d_opt_usedef(as, instr, tv)
#define GEN_XVMINI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_h_opt_usedef(as, instr, tv)
#define GEN_XVMINI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_w_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_d_w_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_D_WU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_d_wu_w_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_q_d_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_q_du_opt_usedef(as, instr, tv)
#define GEN_XVMULWEV_Q_DU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwev_q_du_d_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_d_w_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_D_WU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_d_wu_w_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_H_BU_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_h_bu_b_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_q_d_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_q_du_opt_usedef(as, instr, tv)
#define GEN_XVMULWOD_Q_DU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmulwod_q_du_d_opt_usedef(as, instr, tv)
#define GEN_XVPICKVE_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickve_d_opt_usedef(as, instr, tv)
#define GEN_XVSEQ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseq_b_opt_usedef(as, instr, tv)
#define GEN_XVSEQ_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseq_d_opt_usedef(as, instr, tv)
#define GEN_XVSEQ_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseq_h_opt_usedef(as, instr, tv)
#define GEN_XVSEQ_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseq_w_opt_usedef(as, instr, tv)
#define GEN_XVSLLWIL_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsllwil_d_w_opt_usedef(as, instr, tv)
#define GEN_XVSLLWIL_DU_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsllwil_du_wu_opt_usedef(as, instr, tv)
#define GEN_XVSLLWIL_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsllwil_h_b_opt_usedef(as, instr, tv)
#define GEN_XVSLLWIL_HU_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsllwil_hu_bu_opt_usedef(as, instr, tv)
#define GEN_XVSLLWIL_WU_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsllwil_wu_hu_opt_usedef(as, instr, tv)
#define GEN_XVSRA_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsra_b_opt_usedef(as, instr, tv)
#define GEN_XVSRA_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsra_d_opt_usedef(as, instr, tv)
#define GEN_XVSRA_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsra_h_opt_usedef(as, instr, tv)
#define GEN_XVSRA_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsra_w_opt_usedef(as, instr, tv)
#define GEN_XVSRANI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrani_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRANI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrani_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSRANI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrani_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLR_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlr_b_opt_usedef(as, instr, tv)
#define GEN_XVSRLR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlr_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLR_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlr_h_opt_usedef(as, instr, tv)
#define GEN_XVSRLR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlr_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_d_w_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_q_d_opt_usedef(as, instr, tv)
#define GEN_XVSUBWEV_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwev_q_du_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_d_w_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_q_d_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_q_du_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_w_h_opt_usedef(as, instr, tv)
#define GEN_XVSUBWOD_W_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubwod_w_hu_opt_usedef(as, instr, tv)
#define GEN_XVSTELM_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvstelm_b_opt_usedef(as, instr, tv)
#define GEN_XVLD_USEDEF(as, instr, tv) __gen_lasx_interpret_xvld_opt_usedef(as, instr, tv)
#define GEN_XVST_USEDEF(as, instr, tv) __gen_lasx_interpret_xvst_opt_usedef(as, instr, tv)
#define GEN_XVSTELM_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvstelm_w_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_HU_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_hu_bu_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_WU_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_wu_hu_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_h_b_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_w_h_opt_usedef(as, instr, tv)
#define GEN_VEXT2XV_WU_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_vext2xv_wu_bu_opt_usedef(as, instr, tv)
#define GEN_XVREPLGR2VR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplgr2vr_w_opt_usedef(as, instr, tv)
#define GEN_XVREPLGR2VR_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplgr2vr_h_opt_usedef(as, instr, tv)
#define GEN_XVREPLGR2VR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplgr2vr_d_opt_usedef(as, instr, tv)
#define GEN_XVFCVTH_D_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcvth_d_s_opt_usedef(as, instr, tv)
#define GEN_XVFCVTL_D_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcvtl_d_s_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRZ_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrz_s_opt_usedef(as, instr, tv)
#define GEN_XVNEG_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvneg_b_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRNE_W_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrne_w_s_opt_usedef(as, instr, tv)
#define GEN_XVPCNT_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpcnt_w_opt_usedef(as, instr, tv)
#define GEN_XVFRECIP_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrecip_s_opt_usedef(as, instr, tv)
#define GEN_XVSRLI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrli_h_opt_usedef(as, instr, tv)
#define GEN_XVSLLI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslli_h_opt_usedef(as, instr, tv)
#define GEN_XVSRAI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrai_h_opt_usedef(as, instr, tv)
#define GEN_XVSAT_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsat_hu_opt_usedef(as, instr, tv)
#define GEN_XVSRARI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrari_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVPICKVE2GR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickve2gr_w_opt_usedef(as, instr, tv)
#define GEN_XVSLLI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvslli_b_opt_usedef(as, instr, tv)
#define GEN_XVSRLI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrli_b_opt_usedef(as, instr, tv)
#define GEN_XVINSGR2VR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvinsgr2vr_w_opt_usedef(as, instr, tv)
#define GEN_XVPICKVE2GR_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickve2gr_wu_opt_usedef(as, instr, tv)
#define GEN_XVAVGR_HU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvavgr_hu)(as, instr, tv)
#define GEN_XVAVGR_WU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvavgr_wu)(as, instr, tv)
#define GEN_XVMUL_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmul_h)(as, instr, tv)
#define GEN_XVMUL_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmul_w)(as, instr, tv)
#define GEN_XVMADD_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmadd_h)(as, instr, tv)
#define GEN_XVMADD_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmadd_w)(as, instr, tv)
#define GEN_XVADD_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvadd_h)(as, instr, tv)
#define GEN_XVADD_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvadd_w)(as, instr, tv)
#define GEN_XVSRLI_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsrli_w)(as, instr, tv)
#define GEN_XVLDX_USEDEF(as, instr, tv) __gen_lasx_interpret_xvldx_opt_usedef(as, instr, tv)
#define GEN_XVFMUL_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmul_s)(as, instr, tv)
#define GEN_XVSTX_USEDEF(as, instr, tv) __gen_lasx_interpret_xvstx_opt_usedef(as, instr, tv)
#define GEN_XVILVL_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvl_b)(as, instr, tv)
#define GEN_XVILVL_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvl_h)(as, instr, tv)
#define GEN_XVILVL_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvl_w)(as, instr, tv)
#define GEN_XVILVL_D_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvl_d)(as, instr, tv)
#define GEN_XVILVH_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvh_b)(as, instr, tv)
#define GEN_XVILVH_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvh_h)(as, instr, tv)
#define GEN_XVILVH_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvh_w)(as, instr, tv)
#define GEN_XVILVH_D_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvilvh_d)(as, instr, tv)
#define GEN_XVOR_V_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvor_v)(as, instr, tv)
#define GEN_XVAND_V_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvand_v)(as, instr, tv)
#define GEN_XVSUB_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsub_b)(as, instr, tv)
#define GEN_XVSUB_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsub_h)(as, instr, tv)
#define GEN_XVSUB_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsub_w)(as, instr, tv)
#define GEN_XVFADD_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfadd_s)(as, instr, tv)
#define GEN_XVFSUB_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfsub_s)(as, instr, tv)
#define GEN_XVMSUB_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmsub_h)(as, instr, tv)
#define GEN_XVSLT_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvslt_w)(as, instr, tv)
#define GEN_XVMADDWEV_H_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmaddwev_h_bu)(as, instr, tv)
#define GEN_XVMADDWOD_H_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmaddwod_h_bu)(as, instr, tv)
#define GEN_XVMULWEV_W_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwev_w_h)(as, instr, tv)
#define GEN_XVMADDWOD_W_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmaddwod_w_h)(as, instr, tv)
#define GEN_XVPICKEV_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpickev_b)(as, instr, tv)
#define GEN_XVPICKEV_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpickev_h)(as, instr, tv)
#define GEN_XVPICKOD_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpickod_b)(as, instr, tv)
#define GEN_XVPICKOD_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpickod_h)(as, instr, tv)
#define GEN_XVPACKEV_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpackev_b)(as, instr, tv)
#define GEN_XVPACKEV_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpackev_h)(as, instr, tv)
#define GEN_XVPACKOD_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpackod_b)(as, instr, tv)
#define GEN_XVPACKOD_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvpackod_h)(as, instr, tv)
#define GEN_XVMAXI_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmaxi_h)(as, instr, tv)
#define GEN_XVMAXI_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmaxi_w)(as, instr, tv)
#define GEN_XVSLLI_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvslli_w)(as, instr, tv)
#define GEN_XVSRAI_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsrai_w)(as, instr, tv)
#define GEN_XVSRARI_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsrari_w)(as, instr, tv)
#define GEN_XVSAT_WU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsat_wu)(as, instr, tv)
#define GEN_XVADDWEV_H_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvaddwev_h_bu)(as, instr, tv)
#define GEN_XVADDWOD_H_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvaddwod_h_bu)(as, instr, tv)
#define GEN_XVMULWOD_W_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwod_w_h)(as, instr, tv)
#define GEN_XVMULWOD_W_HU_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwod_w_hu_h)(as, instr, tv)
#define GEN_XVMULWEV_W_HU_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwev_w_hu_h)(as, instr, tv)
#define GEN_XVXOR_V_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvxor_v)(as, instr, tv)
#define GEN_XVHADDW_WU_HU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvhaddw_wu_hu)(as, instr, tv)
#define GEN_XVHADDW_HU_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvhaddw_hu_bu)(as, instr, tv)
#define GEN_XVFMAX_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmax_s)(as, instr, tv)
#define GEN_XVMULWEV_H_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwev_h_bu)(as, instr, tv)
#define GEN_XVFMIN_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmin_s)(as, instr, tv)
#define GEN_XVMUH_HU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmuh_hu)(as, instr, tv)
#define GEN_XVSHUF_H_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvshuf_h)(as, instr, tv)
#define GEN_XVADDI_WU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvaddi_wu)(as, instr, tv)
#define GEN_XVADDI_HU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvaddi_hu)(as, instr, tv)
#define GEN_XVNOR_V_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvnor_v)(as, instr, tv)
#define GEN_XVSRL_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsrl_w)(as, instr, tv)
#define GEN_XVMULWEV_W_HU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwev_w_hu)(as, instr, tv)
#define GEN_XVMULWOD_W_HU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwod_w_hu)(as, instr, tv)
#define GEN_XVMULWOD_H_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmulwod_h_bu)(as, instr, tv)
#define GEN_XVFADD_D_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfadd_d)(as, instr, tv)
#define GEN_XVFMUL_D_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmul_d)(as, instr, tv)
#define GEN_XVMIN_WU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvmin_wu)(as, instr, tv)
#define GEN_XVAVGR_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvavgr_bu)(as, instr, tv)
#define GEN_XVSLL_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsll_b)(as, instr, tv)
#define GEN_XVSRL_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsrl_b)(as, instr, tv)
#define GEN_XVSADD_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvsadd_bu)(as, instr, tv)
#define GEN_XVSSUB_BU_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvssub_bu)(as, instr, tv)
#define GEN_XVFCVT_S_D_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfcvt_s_d)(as, instr, tv)
#define GEN_XVPERM_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvperm_w_opt_usedef(as, instr, tv)
#define GEN_XVBSRL_V_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvbsrl_v)(as, instr, tv)
#define GEN_XVSSRANI_H_W_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvssrani_h_w)(as, instr, tv)
#define GEN_XVFMADD_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmadd_s)(as, instr, tv)
#define GEN_XVFNMSUB_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfnmsub_s)(as, instr, tv)
#define GEN_XVFMSUB_S_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmsub_s)(as, instr, tv)
#define GEN_XVLDREPL_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvldrepl_w_opt_usedef(as, instr, tv)
#define GEN_XVBITSEL_V_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvbitsel_v)(as, instr, tv)
#define GEN_XVSHUF_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvshuf_b)(as, instr, tv)
#define GEN_XVSTELM_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvstelm_d_opt_usedef(as, instr, tv)
#define GEN_XVFMADD_D_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvfmadd_d)(as, instr, tv)
#define GEN_XVFCMP_COND_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcmp_cond_s_opt_usedef(as, instr, tv)
#define GEN_XVPERMI_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpermi_q_opt_usedef(as, instr, tv)
#define GEN_XVORI_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvori_b)(as, instr, tv)
#define GEN_XVANDI_B_USEDEF(as, instr, tv) GEN_USE_DISPATCH_17(xvandi_b)(as, instr, tv)
#define GEN_XVPERMI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpermi_d_opt_usedef(as, instr, tv)
#define GEN_USE_DISPATCH_17(name) __gen_lasx_interpret_##name##_opt_usedef

/* ===== USEDEF function declarations ===== */

void __gen_lasx_interpret_vext2xv_d_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_d_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_du_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_du_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_du_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_hu_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_w_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_wu_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_vext2xv_wu_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvabsd_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadd_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadd_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadd_q_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadda_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadda_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadda_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvadda_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddi_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddi_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_d_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_d_wu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_h_bu_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_q_du_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_q_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_w_hu_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwev_w_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_d_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_d_wu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_h_bu_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_q_du_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_q_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_w_hu_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvaddwod_w_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvandn_v_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavg_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavgr_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavgr_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavgr_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavgr_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvavgr_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitclri_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitclri_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitclri_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitclri_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitrevi_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitrevi_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitrevi_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitrevi_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitseti_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitseti_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitseti_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbitseti_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvbsll_v_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvclz_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvclz_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvclz_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvclz_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfcmp_cond_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfcmp_cond_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfcvth_d_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfcvtl_d_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfdiv_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfdiv_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfmax_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfmin_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfmsub_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfnmadd_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfnmadd_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfnmsub_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfrecip_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfrintrz_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvfsub_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvftintrne_w_s_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhaddw_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhaddw_du_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhaddw_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhaddw_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhaddw_qu_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhaddw_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_du_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_hu_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_qu_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvhsubw_wu_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvinsgr2vr_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvinsve0_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvinsve0_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvld_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvldi_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvldrepl_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvldrepl_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvldrepl_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvldrepl_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvldx_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmadd_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmadd_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_d_wu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_h_bu_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_q_du_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwev_w_hu_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_d_wu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_h_bu_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_q_du_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaddwod_w_hu_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmax_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaxi_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaxi_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaxi_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaxi_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaxi_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmaxi_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmin_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmini_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmini_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmini_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmini_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmskltz_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmskltz_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmskltz_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmskltz_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmsub_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmsub_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmsub_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmuh_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmul_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmul_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_d_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_d_wu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_h_bu_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_q_du_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwev_q_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_d_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_d_wu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_h_bu_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_q_du_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvmulwod_q_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvneg_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvneg_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvneg_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvneg_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvnori_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvorn_v_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpackev_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpackev_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpackod_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpackod_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpcnt_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpcnt_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpcnt_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpcnt_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvperm_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpermi_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpermi_q_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpermi_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickev_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickev_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickve2gr_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickve2gr_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickve2gr_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickve2gr_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickve_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvpickve_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvrepl128vei_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvrepl128vei_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvrepl128vei_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvreplgr2vr_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvreplgr2vr_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvreplgr2vr_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvreplgr2vr_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsat_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseq_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseq_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseq_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseq_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseqi_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseqi_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseqi_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvseqi_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvshuf4i_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvshuf4i_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvshuf4i_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvshuf4i_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvshuf_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvshuf_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsigncov_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsigncov_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsigncov_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsigncov_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsle_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslei_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsll_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsll_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsll_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslli_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslli_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslli_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsllwil_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsllwil_du_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsllwil_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsllwil_hu_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsllwil_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsllwil_wu_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslt_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvslti_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsra_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsra_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsra_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsra_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrai_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrai_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrai_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrani_b_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrani_d_q_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrani_h_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrani_w_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrar_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrar_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrar_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrar_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrari_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrari_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrari_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrl_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrl_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrli_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrli_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrli_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrlr_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrlr_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrlr_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsrlr_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrani_b_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrarni_bu_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrarni_h_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrarni_hu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrarni_w_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrlni_bu_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrlni_h_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrlni_hu_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvssrlni_w_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvst_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvstelm_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvstelm_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvstelm_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvstelm_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvstx_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsub_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsub_q_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_d_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_h_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_q_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwev_w_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_d_w_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_d_wu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_h_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_h_bu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_q_d_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_q_du_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_w_h_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvsubwod_w_hu_opt_usedef(void *pas, unsigned int instr, int *temp_vr);
void __gen_lasx_interpret_xvxori_b_opt_usedef(void *pas, unsigned int instr, int *temp_vr);


#define GEN_XVBITCLR_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclr_b_opt_usedef(as, instr, tv)
#define GEN_XVBITCLR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclr_d_opt_usedef(as, instr, tv)
#define GEN_XVBITCLR_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclr_h_opt_usedef(as, instr, tv)
#define GEN_XVBITCLR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitclr_w_opt_usedef(as, instr, tv)
#define GEN_XVBITREV_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrev_b_opt_usedef(as, instr, tv)
#define GEN_XVBITREV_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrev_d_opt_usedef(as, instr, tv)
#define GEN_XVBITREV_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrev_h_opt_usedef(as, instr, tv)
#define GEN_XVBITREV_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitrev_w_opt_usedef(as, instr, tv)
#define GEN_XVBITSET_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitset_b_opt_usedef(as, instr, tv)
#define GEN_XVBITSET_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitset_d_opt_usedef(as, instr, tv)
#define GEN_XVBITSET_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitset_h_opt_usedef(as, instr, tv)
#define GEN_XVBITSET_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitset_w_opt_usedef(as, instr, tv)
#define GEN_XVDIV_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_b_opt_usedef(as, instr, tv)
#define GEN_XVDIV_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_bu_opt_usedef(as, instr, tv)
#define GEN_XVDIV_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_d_opt_usedef(as, instr, tv)
#define GEN_XVDIV_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_du_opt_usedef(as, instr, tv)
#define GEN_XVDIV_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_h_opt_usedef(as, instr, tv)
#define GEN_XVDIV_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_hu_opt_usedef(as, instr, tv)
#define GEN_XVDIV_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_w_opt_usedef(as, instr, tv)
#define GEN_XVDIV_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvdiv_wu_opt_usedef(as, instr, tv)
#define GEN_XVFCVT_H_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcvt_h_s_opt_usedef(as, instr, tv)
#define GEN_XVFFINT_S_L_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffint_s_l_opt_usedef(as, instr, tv)
#define GEN_XVFMAXA_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmaxa_d_opt_usedef(as, instr, tv)
#define GEN_XVFMAXA_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmaxa_s_opt_usedef(as, instr, tv)
#define GEN_XVFMINA_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmina_d_opt_usedef(as, instr, tv)
#define GEN_XVFMINA_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfmina_s_opt_usedef(as, instr, tv)
#define GEN_XVFRSTP_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrstp_b_opt_usedef(as, instr, tv)
#define GEN_XVFRSTP_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrstp_h_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRM_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrm_w_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRNE_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrne_w_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRP_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrp_w_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZ_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrz_w_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINT_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftint_w_d_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_q_du_opt_usedef(as, instr, tv)
#define GEN_XVMADDWEV_W_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwev_w_hu_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_D_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_d_wu_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_Q_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_q_du_opt_usedef(as, instr, tv)
#define GEN_XVMADDWOD_W_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmaddwod_w_hu_opt_usedef(as, instr, tv)
#define GEN_XVMOD_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_b_opt_usedef(as, instr, tv)
#define GEN_XVMOD_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_bu_opt_usedef(as, instr, tv)
#define GEN_XVMOD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_d_opt_usedef(as, instr, tv)
#define GEN_XVMOD_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_du_opt_usedef(as, instr, tv)
#define GEN_XVMOD_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_h_opt_usedef(as, instr, tv)
#define GEN_XVMOD_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_hu_opt_usedef(as, instr, tv)
#define GEN_XVMOD_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_w_opt_usedef(as, instr, tv)
#define GEN_XVMOD_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmod_wu_opt_usedef(as, instr, tv)
#define GEN_XVPICKOD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickod_d_opt_usedef(as, instr, tv)
#define GEN_XVPICKOD_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvpickod_w_opt_usedef(as, instr, tv)
#define GEN_XVROTR_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotr_b_opt_usedef(as, instr, tv)
#define GEN_XVROTR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotr_d_opt_usedef(as, instr, tv)
#define GEN_XVROTR_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotr_h_opt_usedef(as, instr, tv)
#define GEN_XVROTR_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotr_w_opt_usedef(as, instr, tv)
#define GEN_XVSADD_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_b_opt_usedef(as, instr, tv)
#define GEN_XVSADD_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_d_opt_usedef(as, instr, tv)
#define GEN_XVSADD_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_du_opt_usedef(as, instr, tv)
#define GEN_XVSADD_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_h_opt_usedef(as, instr, tv)
#define GEN_XVSADD_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_hu_opt_usedef(as, instr, tv)
#define GEN_XVSADD_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_w_opt_usedef(as, instr, tv)
#define GEN_XVSADD_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsadd_wu_opt_usedef(as, instr, tv)
#define GEN_XVSRAN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsran_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRAN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsran_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRAN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsran_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSRARN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarn_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRARN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarn_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRARN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarn_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrln_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRLN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrln_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRLN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrln_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLRN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrn_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRLRN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrn_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRLRN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrn_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRAN_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssran_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRAN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssran_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRAN_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssran_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRAN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssran_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRAN_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssran_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRAN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssran_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRARN_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarn_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRARN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarn_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRARN_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarn_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRARN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarn_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRARN_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarn_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRARN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarn_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLN_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrln_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrln_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLN_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrln_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrln_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLN_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrln_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrln_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRN_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrn_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRN_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrn_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRN_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrn_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRN_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrn_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRN_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrn_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRN_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrn_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_b_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_d_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_du_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_h_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_hu_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_w_opt_usedef(as, instr, tv)
#define GEN_XVSSUB_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssub_wu_opt_usedef(as, instr, tv)


#define GEN_XVCLO_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclo_b_opt_usedef(as, instr, tv)
#define GEN_XVCLO_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclo_d_opt_usedef(as, instr, tv)
#define GEN_XVCLO_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclo_h_opt_usedef(as, instr, tv)
#define GEN_XVCLO_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvclo_w_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_DU_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_du_wu_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_d_w_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_HU_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_hu_bu_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_H_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_h_b_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_QU_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_qu_du_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_q_d_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_WU_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_wu_hu_opt_usedef(as, instr, tv)
#define GEN_XVEXTH_W_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvexth_w_h_opt_usedef(as, instr, tv)
#define GEN_XVEXTL_QU_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvextl_qu_du_opt_usedef(as, instr, tv)
#define GEN_XVEXTL_Q_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvextl_q_d_opt_usedef(as, instr, tv)
#define GEN_XVFCLASS_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfclass_d_opt_usedef(as, instr, tv)
#define GEN_XVFCLASS_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfclass_s_opt_usedef(as, instr, tv)
#define GEN_XVFCVTH_S_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcvth_s_h_opt_usedef(as, instr, tv)
#define GEN_XVFCVTL_S_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfcvtl_s_h_opt_usedef(as, instr, tv)
#define GEN_XVFFINTH_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffinth_d_w_opt_usedef(as, instr, tv)
#define GEN_XVFFINTL_D_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffintl_d_w_opt_usedef(as, instr, tv)
#define GEN_XVFFINT_D_L_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffint_d_l_opt_usedef(as, instr, tv)
#define GEN_XVFFINT_D_LU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffint_d_lu_opt_usedef(as, instr, tv)
#define GEN_XVFFINT_S_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffint_s_w_opt_usedef(as, instr, tv)
#define GEN_XVFFINT_S_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvffint_s_wu_opt_usedef(as, instr, tv)
#define GEN_XVFLOGB_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvflogb_d_opt_usedef(as, instr, tv)
#define GEN_XVFLOGB_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvflogb_s_opt_usedef(as, instr, tv)
#define GEN_XVFRECIPE_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrecipe_d_opt_usedef(as, instr, tv)
#define GEN_XVFRECIPE_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrecipe_s_opt_usedef(as, instr, tv)
#define GEN_XVFRECIP_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrecip_d_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRM_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrm_d_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRM_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrm_s_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRNE_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrne_d_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRNE_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrne_s_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRP_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrp_d_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRP_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrp_s_opt_usedef(as, instr, tv)
#define GEN_XVFRINTRZ_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrintrz_d_opt_usedef(as, instr, tv)
#define GEN_XVFRINT_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrint_d_opt_usedef(as, instr, tv)
#define GEN_XVFRINT_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrint_s_opt_usedef(as, instr, tv)
#define GEN_XVFRSQRTE_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrsqrte_d_opt_usedef(as, instr, tv)
#define GEN_XVFRSQRTE_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrsqrte_s_opt_usedef(as, instr, tv)
#define GEN_XVFRSQRT_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrsqrt_d_opt_usedef(as, instr, tv)
#define GEN_XVFRSQRT_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrsqrt_s_opt_usedef(as, instr, tv)
#define GEN_XVFSQRT_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfsqrt_d_opt_usedef(as, instr, tv)
#define GEN_XVFSQRT_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfsqrt_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTH_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftinth_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTL_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintl_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRMH_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrmh_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRML_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrml_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRM_L_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrm_l_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRM_W_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrm_w_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRNEH_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrneh_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRNEL_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrnel_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRNE_L_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrne_l_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRPH_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrph_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRPL_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrpl_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRP_L_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrp_l_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRP_W_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrp_w_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZH_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrzh_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZL_L_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrzl_l_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZ_LU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrz_lu_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZ_L_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrz_l_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZ_WU_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrz_wu_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINTRZ_W_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftintrz_w_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINT_LU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftint_lu_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINT_L_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftint_l_d_opt_usedef(as, instr, tv)
#define GEN_XVFTINT_WU_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftint_wu_s_opt_usedef(as, instr, tv)
#define GEN_XVFTINT_W_S_USEDEF(as, instr, tv) __gen_lasx_interpret_xvftint_w_s_opt_usedef(as, instr, tv)
#define GEN_XVMSKGEZ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmskgez_b_opt_usedef(as, instr, tv)
#define GEN_XVMSKNZ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmsknz_b_opt_usedef(as, instr, tv)


#define GEN_XVBITSELI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvbitseli_b_opt_usedef(as, instr, tv)
#define GEN_XVEXTRINS_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvextrins_b_opt_usedef(as, instr, tv)
#define GEN_XVEXTRINS_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvextrins_d_opt_usedef(as, instr, tv)
#define GEN_XVEXTRINS_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvextrins_h_opt_usedef(as, instr, tv)
#define GEN_XVEXTRINS_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvextrins_w_opt_usedef(as, instr, tv)
#define GEN_XVSRARNI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarni_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSRLNI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlni_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSRLRNI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrni_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_DU_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_du_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_DU_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_du_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_DU_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_du_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_d_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_DU_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_du_q_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_D_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_d_q_opt_usedef(as, instr, tv)
#define GEN_XVROTRI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotri_d_opt_usedef(as, instr, tv)
#define GEN_XVSRARNI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarni_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLNI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlni_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLRI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlri_d_opt_usedef(as, instr, tv)
#define GEN_XVSRLRNI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrni_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_w_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_WU_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_wu_d_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_W_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_w_d_opt_usedef(as, instr, tv)
#define GEN_XVFRSTPI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrstpi_b_opt_usedef(as, instr, tv)
#define GEN_XVFRSTPI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvfrstpi_h_opt_usedef(as, instr, tv)
#define GEN_XVMINI_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_bu_opt_usedef(as, instr, tv)
#define GEN_XVMINI_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_du_opt_usedef(as, instr, tv)
#define GEN_XVMINI_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_hu_opt_usedef(as, instr, tv)
#define GEN_XVMINI_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvmini_wu_opt_usedef(as, instr, tv)
#define GEN_XVROTRI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotri_w_opt_usedef(as, instr, tv)
#define GEN_XVSRARNI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarni_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRLNI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlni_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSRLRI_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlri_w_opt_usedef(as, instr, tv)
#define GEN_XVSRLRNI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrni_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_HU_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_hu_w_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_H_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_h_w_opt_usedef(as, instr, tv)
#define GEN_XVSUBI_BU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubi_bu_opt_usedef(as, instr, tv)
#define GEN_XVSUBI_DU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubi_du_opt_usedef(as, instr, tv)
#define GEN_XVSUBI_HU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubi_hu_opt_usedef(as, instr, tv)
#define GEN_XVSUBI_WU_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsubi_wu_opt_usedef(as, instr, tv)
#define GEN_XVROTRI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotri_h_opt_usedef(as, instr, tv)
#define GEN_XVSRARNI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrarni_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRLNI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlni_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSRLRI_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlri_h_opt_usedef(as, instr, tv)
#define GEN_XVSRLRNI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlrni_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRANI_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrani_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRARNI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrarni_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLNI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlni_b_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_BU_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_bu_h_opt_usedef(as, instr, tv)
#define GEN_XVSSRLRNI_B_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvssrlrni_b_h_opt_usedef(as, instr, tv)
#define GEN_XVROTRI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrotri_b_opt_usedef(as, instr, tv)
#define GEN_XVSRLRI_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsrlri_b_opt_usedef(as, instr, tv)

#define GEN_XVREPLVE_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve_b_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve_d_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve_h_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve_w_opt_usedef(as, instr, tv)
#define GEN_XVINSGR2VR_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvinsgr2vr_d_opt_usedef(as, instr, tv)
#define GEN_XVREPL128VEI_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvrepl128vei_d_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE0_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve0_b_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE0_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve0_d_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE0_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve0_h_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE0_Q_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve0_q_opt_usedef(as, instr, tv)
#define GEN_XVREPLVE0_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvreplve0_w_opt_usedef(as, instr, tv)
#define GEN_XVSETALLNEZ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetallnez_b_opt_usedef(as, instr, tv)
#define GEN_XVSETALLNEZ_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetallnez_d_opt_usedef(as, instr, tv)
#define GEN_XVSETALLNEZ_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetallnez_h_opt_usedef(as, instr, tv)
#define GEN_XVSETALLNEZ_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetallnez_w_opt_usedef(as, instr, tv)
#define GEN_XVSETANYEQZ_B_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetanyeqz_b_opt_usedef(as, instr, tv)
#define GEN_XVSETANYEQZ_D_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetanyeqz_d_opt_usedef(as, instr, tv)
#define GEN_XVSETANYEQZ_H_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetanyeqz_h_opt_usedef(as, instr, tv)
#define GEN_XVSETANYEQZ_W_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetanyeqz_w_opt_usedef(as, instr, tv)
#define GEN_XVSETEQZ_V_USEDEF(as, instr, tv) __gen_lasx_interpret_xvseteqz_v_opt_usedef(as, instr, tv)
#define GEN_XVSETNEZ_V_USEDEF(as, instr, tv) __gen_lasx_interpret_xvsetnez_v_opt_usedef(as, instr, tv)

#endif
