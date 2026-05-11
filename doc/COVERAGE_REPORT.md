# LASX Instruction Test Coverage Report

## Summary

| Metric | Value |
|--------|-------|
| **Total OP Codes** | 680 |
| **Unique Instructions Tested** | 173 |
| **Coverage Percentage** | 25.44% |
| **Uncovered Instructions** | 507 |

## Test Files Analyzed

- `test_debug.c`
- `test_sigill.c`
- `test_xvadd.c`
- `test_xvand.c`
- `test_xvbit.c`
- `test_xvbit_full.c`
- `test_xvbiti.c`
- `test_xvcmp.c`
- `test_xvext.c`
- `test_xvextadd.c`
- `test_xvextract.c`
- `test_xvfbasic.c`
- `test_xvfbasic_full.c`
- `test_xvfcmp.c`
- `test_xvfma.c`
- `test_xvimm.c`
- `test_xvldst.c`
- `test_xvmul.c`
- `test_xvmul_full.c`
- `test_xvperm.c`
- `test_xvperm_full.c`
- `test_xvsh.c`


## Covered Instructions (173)

These instructions have corresponding test cases in the test suite:

### Arithmetic (xvadd/xvsub)

- `xvadd_b`
- `xvadd_d`
- `xvadd_h`
- `xvadd_q`
- `xvadd_w`
- `xvsub_b`
- `xvsub_d`
- `xvsub_h`
- `xvsub_q`
- `xvsub_w`

### Logical (xvand/xvor/xvxor/xvnor)

- `xvandn_v`
- `xvand_v`
- `xvnor_v`
- `xvorn_v`
- `xvor_v`
- `xvxor_v`

### Compare (xvmax/xvmin/xvseq/xvsle/xvslt)

- `xvseq_b`
- `xvseq_d`
- `xvseq_h`
- `xvseq_w`
- `xvsle_b`
- `xvsle_bu`
- `xvsle_d`
- `xvsle_du`
- `xvsle_h`
- `xvsle_hu`
- `xvsle_w`
- `xvsle_wu`
- `xvslt_b`
- `xvslt_bu`
- `xvslt_d`
- `xvslt_du`
- `xvslt_h`
- `xvslt_hu`
- `xvslt_w`
- `xvslt_wu`

### Bit Operations (xvbitclr/xvbitset/xvbitrev)

- `xvbitclri_b`
- `xvbitclri_d`
- `xvbitclri_h`
- `xvbitclri_w`
- `xvbitclr_b`
- `xvbitclr_d`
- `xvbitclr_h`
- `xvbitclr_w`
- `xvbitrev_b`
- `xvbitrev_d`
- `xvbitrev_h`
- `xvbitrev_w`
- `xvbitseti_b`
- `xvbitseti_d`
- `xvbitseti_h`
- `xvbitseti_w`
- `xvbitset_b`
- `xvbitset_d`
- `xvbitset_h`
- `xvbitset_w`

### Floating Point Basic (xvfadd/xvfsub/xvfmul/xvfdiv/xvfsqrt)

- `xvfadd_d`
- `xvfadd_s`
- `xvfdiv_d`
- `xvfdiv_s`
- `xvfmul_d`
- `xvfmul_s`
- `xvfsub_d`
- `xvfsub_s`

### Floating Point Misc (xvfrecip/xvfrsqrt)

- `xvfrecip_d`
- `xvfrecip_s`
- `xvfrsqrt_d`
- `xvfrsqrt_s`

### Permutation (xvshuf/xvpermi/xvpack)

- `xvpackev_b`
- `xvpackev_d`
- `xvpackev_h`
- `xvpackev_w`
- `xvpackod_b`
- `xvpackod_d`
- `xvpackod_h`
- `xvpackod_w`
- `xvpermi_d`
- `xvpermi_q`
- `xvpermi_w`
- `xvshuf_b`
- `xvshuf_d`
- `xvshuf_h`
- `xvshuf_w`

### Extend/Extract (xvexth/xvextl/xvextrins)

- `xvexth_d_w`
- `xvexth_h_b`
- `xvexth_q_d`
- `xvexth_w_h`
- `xvextl_qu_du`
- `xvextl_q_d`
- `xvextrins_b`
- `xvextrins_d`
- `xvextrins_h`
- `xvextrins_w`

### Count (xvclo/xvclz)

- `xvclo_b`
- `xvclo_d`
- `xvclo_h`
- `xvclo_w`
- `xvclz_b`
- `xvclz_d`
- `xvclz_h`
- `xvclz_w`

### Saturation (xvsadd/xvsigncov)

- `xvsadd_b`
- `xvsadd_bu`
- `xvsadd_h`
- `xvsadd_w`
- `xvsigncov_b`
- `xvsigncov_d`
- `xvsigncov_h`
- `xvsigncov_w`

### Load/Store (xvld/xvst)

- `xvld`
- `xvst`

### Replication (xvreplve/xvreplgr2vr)

- `xvreplgr2vr_d`
- `xvreplve_b`
- `xvreplve_d`
- `xvreplve_h`
- `xvreplve_w`

## Uncovered Instructions (507)

The following instructions do **not** have corresponding test cases:

| `xvabsd_b` | `xvabsd_bu` | `xvabsd_d` | `xvabsd_du` |
| `xvabsd_h` | `xvabsd_hu` | `xvabsd_w` | `xvabsd_wu` |
| `xvadda_b` | `xvadda_d` | `xvadda_h` | `xvadda_w` |
| `xvaddi_bu` | `xvaddi_du` | `xvaddi_hu` | `xvaddi_wu` |
| `xvaddwev_d_w` | `xvaddwev_d_wu` | `xvaddwev_d_wu_w` | `xvaddwev_h_b` |
| `xvaddwev_h_bu` | `xvaddwev_h_bu_b` | `xvaddwev_q_d` | `xvaddwev_q_du` |
| `xvaddwev_q_du_d` | `xvaddwev_w_h` | `xvaddwev_w_hu` | `xvaddwev_w_hu_h` |
| `xvaddwod_d_w` | `xvaddwod_d_wu` | `xvaddwod_d_wu_w` | `xvaddwod_h_b` |
| `xvaddwod_h_bu` | `xvaddwod_h_bu_b` | `xvaddwod_q_d` | `xvaddwod_q_du` |
| `xvaddwod_q_du_d` | `xvaddwod_w_h` | `xvaddwod_w_hu` | `xvaddwod_w_hu_h` |
| `xvandi_b` | `xvavgr_b` | `xvavgr_bu` | `xvavgr_d` |
| `xvavgr_du` | `xvavgr_h` | `xvavgr_hu` | `xvavgr_w` |
| `xvavgr_wu` | `xvavg_b` | `xvavg_bu` | `xvavg_d` |
| `xvavg_du` | `xvavg_h` | `xvavg_hu` | `xvavg_w` |
| `xvavg_wu` | `xvbitrevi_b` | `xvbitrevi_d` | `xvbitrevi_h` |
| `xvbitrevi_w` | `xvbitseli_b` | `xvbsll_v` | `xvbsrl_v` |
| `xvdiv_b` | `xvdiv_bu` | `xvdiv_d` | `xvdiv_du` |
| `xvdiv_h` | `xvdiv_hu` | `xvdiv_w` | `xvdiv_wu` |
| `xvexth_du_wu` | `xvexth_hu_bu` | `xvexth_qu_du` | `xvexth_wu_hu` |
| `xvfclass_d` | `xvfclass_s` | `xvfcmp_cond_d` | `xvfcmp_cond_s` |
| `xvfcvth_d_s` | `xvfcvth_s_h` | `xvfcvtl_d_s` | `xvfcvtl_s_h` |
| `xvfcvt_h_s` | `xvfcvt_s_d` | `xvffinth_d_w` | `xvffintl_d_w` |
| `xvffint_d_l` | `xvffint_d_lu` | `xvffint_s_l` | `xvffint_s_w` |
| `xvffint_s_wu` | `xvflogb_d` | `xvflogb_s` | `xvfmaxa_d` |
| `xvfmaxa_s` | `xvfmina_d` | `xvfmina_s` | `xvfrecipe_d` |
| `xvfrecipe_s` | `xvfrintrm_d` | `xvfrintrm_s` | `xvfrintrne_d` |
| `xvfrintrne_s` | `xvfrintrp_d` | `xvfrintrp_s` | `xvfrintrz_d` |
| `xvfrintrz_s` | `xvfrint_d` | `xvfrint_s` | `xvfrsqrte_d` |
| `xvfrsqrte_s` | `xvfrstpi_b` | `xvfrstpi_h` | `xvftinth_l_s` |
| `xvftintl_l_s` | `xvftintrmh_l_s` | `xvftintrml_l_s` | `xvftintrm_l_d` |
| `xvftintrm_w_d` | `xvftintrm_w_s` | `xvftintrneh_l_s` | `xvftintrnel_l_s` |
| `xvftintrne_l_d` | `xvftintrne_w_d` | `xvftintrne_w_s` | `xvftintrph_l_s` |
| `xvftintrpl_l_s` | `xvftintrp_l_d` | `xvftintrp_w_d` | `xvftintrp_w_s` |
| `xvftintrzh_l_s` | `xvftintrzl_l_s` | `xvftintrz_lu_d` | `xvftintrz_l_d` |
| `xvftintrz_wu_s` | `xvftintrz_w_d` | `xvftintrz_w_s` | `xvftint_lu_d` |
| `xvftint_l_d` | `xvftint_wu_s` | `xvftint_w_d` | `xvftint_w_s` |
| `xvhaddw_du_wu` | `xvhaddw_d_w` | `xvhaddw_hu_bu` | `xvhaddw_h_b` |
| `xvhaddw_qu_du` | `xvhaddw_q_d` | `xvhaddw_wu_hu` | `xvhaddw_w_h` |
| `xvhsubw_du_wu` | `xvhsubw_d_w` | `xvhsubw_hu_bu` | `xvhsubw_h_b` |
| `xvhsubw_qu_du` | `xvhsubw_q_d` | `xvhsubw_wu_hu` | `xvhsubw_w_h` |
| `xvilvh_b` | `xvilvh_d` | `xvilvh_h` | `xvilvh_w` |
| `xvilvl_b` | `xvilvl_d` | `xvilvl_h` | `xvilvl_w` |
| `xvinsgr2vr_d` | `xvinsgr2vr_w` | `xvinsve0_d` | `xvinsve0_w` |
| `xvldi` | `xvldrepl_b` | `xvldrepl_d` | `xvldrepl_h` |
| `xvldrepl_w` | `xvldx` | `xvmaddwev_d_w` | `xvmaddwev_d_wu` |
| `xvmaddwev_d_wu_w` | `xvmaddwev_h_b` | `xvmaddwev_h_bu` | `xvmaddwev_h_bu_b` |
| `xvmaddwev_q_d` | `xvmaddwev_q_du` | `xvmaddwev_q_du_d` | `xvmaddwev_w_h` |
| `xvmaddwev_w_hu` | `xvmaddwev_w_hu_h` | `xvmaddwod_d_w` | `xvmaddwod_d_wu` |
| `xvmaddwod_d_wu_w` | `xvmaddwod_h_b` | `xvmaddwod_h_bu` | `xvmaddwod_h_bu_b` |
| `xvmaddwod_q_d` | `xvmaddwod_q_du` | `xvmaddwod_q_du_d` | `xvmaddwod_w_h` |
| `xvmaddwod_w_hu` | `xvmaddwod_w_hu_h` | `xvmadd_b` | `xvmadd_d` |
| `xvmadd_h` | `xvmadd_w` | `xvmaxi_b` | `xvmaxi_bu` |
| `xvmaxi_d` | `xvmaxi_du` | `xvmaxi_h` | `xvmaxi_hu` |
| `xvmaxi_w` | `xvmaxi_wu` | `xvmini_b` | `xvmini_bu` |
| `xvmini_d` | `xvmini_du` | `xvmini_h` | `xvmini_hu` |
| `xvmini_w` | `xvmini_wu` | `xvmod_b` | `xvmod_bu` |
| `xvmod_d` | `xvmod_du` | `xvmod_h` | `xvmod_hu` |
| `xvmod_w` | `xvmod_wu` | `xvmskgez_b` | `xvmskltz_b` |
| `xvmskltz_d` | `xvmskltz_h` | `xvmskltz_w` | `xvmsknz_b` |
| `xvmsub_b` | `xvmsub_d` | `xvmsub_h` | `xvmsub_w` |
| `xvmuh_bu` | `xvmuh_du` | `xvmuh_hu` | `xvmuh_wu` |
| `xvmulwev_d_wu` | `xvmulwev_d_wu_w` | `xvmulwev_h_b` | `xvmulwev_h_bu` |
| `xvmulwev_h_bu_b` | `xvmulwev_q_d` | `xvmulwev_q_du` | `xvmulwev_q_du_d` |
| `xvmulwev_w_hu` | `xvmulwev_w_hu_h` | `xvmulwod_d_wu` | `xvmulwod_d_wu_w` |
| `xvmulwod_h_b` | `xvmulwod_h_bu` | `xvmulwod_h_bu_b` | `xvmulwod_q_d` |
| `xvmulwod_q_du` | `xvmulwod_q_du_d` | `xvmulwod_w_hu` | `xvmulwod_w_hu_h` |
| `xvneg_b` | `xvneg_d` | `xvneg_h` | `xvneg_w` |
| `xvnori_b` | `xvori_b` | `xvpcnt_b` | `xvpcnt_d` |
| `xvpcnt_h` | `xvpcnt_w` | `xvperm_w` | `xvpickev_b` |
| `xvpickev_d` | `xvpickev_h` | `xvpickev_w` | `xvpickod_b` |
| `xvpickod_d` | `xvpickod_h` | `xvpickod_w` | `xvpickve2gr_d` |
| `xvpickve2gr_du` | `xvpickve2gr_w` | `xvpickve2gr_wu` | `xvpickve_d` |
| `xvpickve_w` | `xvrepl128vei_b` | `xvrepl128vei_d` | `xvrepl128vei_h` |
| `xvrepl128vei_w` | `xvreplgr2vr_b` | `xvreplgr2vr_h` | `xvreplgr2vr_w` |
| `xvreplve0_b` | `xvreplve0_d` | `xvreplve0_h` | `xvreplve0_q` |
| `xvreplve0_w` | `xvrotri_b` | `xvrotri_d` | `xvrotri_h` |
| `xvrotri_w` | `xvrotr_b` | `xvrotr_d` | `xvrotr_h` |
| `xvrotr_w` | `xvsadd_d` | `xvsadd_du` | `xvsadd_hu` |
| `xvsadd_wu` | `xvsat_b` | `xvsat_bu` | `xvsat_d` |
| `xvsat_du` | `xvsat_h` | `xvsat_hu` | `xvsat_w` |
| `xvsat_wu` | `xvseqi_b` | `xvseqi_d` | `xvseqi_h` |
| `xvseqi_w` | `xvsetallnez_b` | `xvsetallnez_d` | `xvsetallnez_h` |
| `xvsetallnez_w` | `xvsetanyeqz_b` | `xvsetanyeqz_d` | `xvsetanyeqz_h` |
| `xvsetanyeqz_w` | `xvseteqz_v` | `xvsetnez_v` | `xvshuf4i_b` |
| `xvshuf4i_d` | `xvshuf4i_h` | `xvshuf4i_w` | `xvslei_b` |
| `xvslei_bu` | `xvslei_d` | `xvslei_du` | `xvslei_h` |
| `xvslei_hu` | `xvslei_w` | `xvslei_wu` | `xvslli_b` |
| `xvslli_d` | `xvslli_h` | `xvslli_w` | `xvsllwil_du_wu` |
| `xvsllwil_d_w` | `xvsllwil_hu_bu` | `xvsllwil_h_b` | `xvsllwil_wu_hu` |
| `xvsllwil_w_h` | `xvslti_b` | `xvslti_bu` | `xvslti_d` |
| `xvslti_du` | `xvslti_h` | `xvslti_hu` | `xvslti_w` |
| `xvslti_wu` | `xvsrai_b` | `xvsrai_d` | `xvsrai_h` |
| `xvsrai_w` | `xvsrani_b_h` | `xvsrani_d_q` | `xvsrani_h_w` |
| `xvsrani_w_d` | `xvsran_b_h` | `xvsran_h_w` | `xvsran_w_d` |
| `xvsrari_b` | `xvsrari_d` | `xvsrari_h` | `xvsrari_w` |
| `xvsrarni_b_h` | `xvsrarni_d_q` | `xvsrarni_h_w` | `xvsrarni_w_d` |
| `xvsrarn_b_h` | `xvsrarn_h_w` | `xvsrarn_w_d` | `xvsrar_b` |
| `xvsrar_d` | `xvsrar_h` | `xvsrar_w` | `xvsrli_b` |
| `xvsrli_d` | `xvsrli_h` | `xvsrli_w` | `xvsrlni_b_h` |
| `xvsrlni_d_q` | `xvsrlni_h_w` | `xvsrlni_w_d` | `xvsrln_b_h` |
| `xvsrln_h_w` | `xvsrln_w_d` | `xvsrlri_b` | `xvsrlri_d` |
| `xvsrlri_h` | `xvsrlri_w` | `xvsrlrni_b_h` | `xvsrlrni_d_q` |
| `xvsrlrni_h_w` | `xvsrlrni_w_d` | `xvsrlrn_b_h` | `xvsrlrn_h_w` |
| `xvsrlrn_w_d` | `xvsrlr_b` | `xvsrlr_d` | `xvsrlr_h` |
| `xvsrlr_w` | `xvssrani_bu_h` | `xvssrani_b_h` | `xvssrani_du_q` |
| `xvssrani_d_q` | `xvssrani_hu_w` | `xvssrani_h_w` | `xvssrani_wu_d` |
| `xvssrani_w_d` | `xvssran_bu_h` | `xvssran_b_h` | `xvssran_hu_w` |
| `xvssran_h_w` | `xvssran_wu_d` | `xvssran_w_d` | `xvssrarni_bu_h` |
| `xvssrarni_b_h` | `xvssrarni_du_q` | `xvssrarni_d_q` | `xvssrarni_hu_w` |
| `xvssrarni_h_w` | `xvssrarni_wu_d` | `xvssrarni_w_d` | `xvssrarn_bu_h` |
| `xvssrarn_b_h` | `xvssrarn_hu_w` | `xvssrarn_h_w` | `xvssrarn_wu_d` |
| `xvssrarn_w_d` | `xvssrlni_bu_h` | `xvssrlni_b_h` | `xvssrlni_du_q` |
| `xvssrlni_d_q` | `xvssrlni_hu_w` | `xvssrlni_h_w` | `xvssrlni_wu_d` |
| `xvssrlni_w_d` | `xvssrln_bu_h` | `xvssrln_b_h` | `xvssrln_hu_w` |
| `xvssrln_h_w` | `xvssrln_wu_d` | `xvssrln_w_d` | `xvssrlrni_bu_h` |
| `xvssrlrni_b_h` | `xvssrlrni_du_q` | `xvssrlrni_d_q` | `xvssrlrni_hu_w` |
| `xvssrlrni_h_w` | `xvssrlrni_wu_d` | `xvssrlrni_w_d` | `xvssrlrn_bu_h` |
| `xvssrlrn_b_h` | `xvssrlrn_hu_w` | `xvssrlrn_h_w` | `xvssrlrn_wu_d` |
| `xvssrlrn_w_d` | `xvssub_b` | `xvssub_bu` | `xvssub_d` |
| `xvssub_du` | `xvssub_h` | `xvssub_hu` | `xvssub_w` |
| `xvssub_wu` | `xvstelm_b` | `xvstelm_d` | `xvstelm_h` |
| `xvstelm_w` | `xvstx` | `xvsubi_bu` | `xvsubi_du` |
| `xvsubi_hu` | `xvsubi_wu` | `xvsubwev_d_w` | `xvsubwev_d_wu` |
| `xvsubwev_h_b` | `xvsubwev_h_bu` | `xvsubwev_q_d` | `xvsubwev_q_du` |
| `xvsubwev_w_h` | `xvsubwev_w_hu` | `xvsubwod_d_w` | `xvsubwod_d_wu` |
| `xvsubwod_h_b` | `xvsubwod_h_bu` | `xvsubwod_q_d` | `xvsubwod_q_du` |
| `xvsubwod_w_h` | `xvsubwod_w_hu` | `xvxori_b` |


## Coverage by OP Prefix

| OP Prefix | Total | Covered | Uncovered | Coverage |
|-----------|-------|---------|-----------|----------|
| OP10 | 4 | 2 | 2 | 50.0% |
| OP11 | 2 | 0 | 2 | 0.0% |
| OP12 | 14 | 10 | 4 | 71.4% |
| OP13 | 1 | 0 | 1 | 0.0% |
| OP14 | 17 | 7 | 10 | 41.2% |
| OP15 | 12 | 0 | 12 | 0.0% |
| OP16 | 23 | 2 | 21 | 8.7% |
| OP17 | 445 | 127 | 318 | 28.5% |
| OP18 | 26 | 2 | 24 | 7.7% |
| OP19 | 19 | 2 | 17 | 10.5% |
| OP20 | 6 | 0 | 6 | 0.0% |
| OP21 | 1 | 0 | 1 | 0.0% |
| OP22 | 99 | 21 | 78 | 21.2% |
| OP24 | 10 | 0 | 10 | 0.0% |
| OP9 | 1 | 0 | 1 | 0.0% |


## Recommendations

1. **High Priority**: Add tests for commonly used arithmetic instructions:
   - `xvabsd_*` - Absolute difference
   - `xvmadd/xvmsub` - Multiply-add/subtract
   - `xvilvh/xvilvl` - Interleave

2. **Floating Point**: Add tests for:
   - `xvfclass` - Classify floating point
   - `xvfcvt*` - Floating point conversion
   - `xvffint*` - Float to int conversion
   - `xvfrint*` - Floating point rounding

3. **Bit Manipulation**: Add tests for:
   - `xvrotri_*` - Rotate immediate
   - `xvsati_*` - Saturate
   - `xvpcnt` - Population count

4. **Memory Operations**: Add tests for:
   - `xvldrepl_*` - Load and replicate
   - `xvstelm_*` - Store element

5. **Extended Multiply**: Add tests for:
   - `xvmulwev/xvmulwod` variants with more element sizes

---

*Report generated by analyzing test files in `tests/test_*.c` and OP code definitions in `lasx_emu_private.h`*
