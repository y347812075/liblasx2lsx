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

#ifndef LASX_INTERPRET_OPT_REG_H
#define LASX_INTERPRET_OPT_REG_H

#include "lagoon.h"
#include "thread_data.h"
#include "lasx_interpret.h"

/* Find N free GRs starting from LA_T1, excluding specified registers
 * regs[0..n-1] will be filled with free GRs
 * exclude: array of registers to exclude (can be NULL)
 * nexcl: number of elements in exclude array */
static inline void gen_reg_find_free_grs(int *regs, int n, const int *exclude, int nexcl)
{
    int gr = LA_T1;
    for (int i = 0; i < n; i++) {
        for (;;) {
            int conflict = 0;
            for (int j = 0; j < nexcl; j++) {
                if (gr == exclude[j]) { conflict = 1; break; }
            }
            if (!conflict) {
                for (int j = 0; j < i; j++) {
                    if (gr == regs[j]) { conflict = 1; break; }
                }
            }
            if (!conflict) break;
            gr++;
        }
        regs[i] = gr++;
    }
}

/* Find N free VRs starting from LA_V0, excluding specified registers
 * regs[0..n-1] will be filled with free VRs
 * exclude: array of registers to exclude (can be NULL)
 * nexcl: number of elements in exclude array */
static inline void gen_reg_find_free_vrs(int *regs, int n, const int *exclude, int nexcl)
{
    int vr = LA_V0;
    for (int i = 0; i < n; i++) {
        for (;;) {
            int conflict = 0;
            for (int j = 0; j < nexcl; j++) {
                if (vr == exclude[j]) { conflict = 1; break; }
            }
            if (!conflict) {
                for (int j = 0; j < i; j++) {
                    if (vr == regs[j]) { conflict = 1; break; }
                }
            }
            if (!conflict) break;
            vr++;
        }
        regs[i] = vr++;
    }
}

/* Save GR to thread_data.gpr[gr] (TP must hold td address) */
static inline void gen_reg_save_gr(lagoon_assembler_t *as, int gr)
{
    la_st_d(as, gr, LA_TP, TD_OFF_GPR(gr));
}

/* Restore GR from thread_data.gpr[gr] (TP must hold td address) */
static inline void gen_reg_restore_gr(lagoon_assembler_t *as, int gr)
{
    la_ld_d(as, gr, LA_TP, offsetof(thread_data_t, gpr[gr]));
}

/* Save VR low-128 to thread_data.data[vr][0:1] (TP must hold td address) */
static inline void gen_reg_save_vr(lagoon_assembler_t *as, int vr)
{
    la_vst(as, vr, LA_TP, TD_OFF_DATA(vr, 0));
}

/* Restore VR low-128 from thread_data.data[vr][0:1] (TP must hold td address) */
static inline void gen_reg_restore_vr(lagoon_assembler_t *as, int vr)
{
    la_vld(as, vr, LA_TP, TD_OFF_DATA(vr, 0));
}

#endif /* LASX_INTERPRET_OPT_REG_H */
