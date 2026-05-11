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

#ifndef LASX_CMP_H
#define LASX_CMP_H

#include <ucontext.h>

// Compare/select instructions
// Max/min select
void do_lasx_emu_xvmax_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmax_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmax_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmax_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmax_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmax_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmax_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmax_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmin_du(ucontext_t *uc, unsigned int instr);

// Count bits
void do_lasx_emu_xvclo_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclo_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclo_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclo_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclz_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclz_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclz_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvclz_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpcnt_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpcnt_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpcnt_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvpcnt_d(ucontext_t *uc, unsigned int instr);

// Negate
void do_lasx_emu_xvneg_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvneg_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvneg_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvneg_d(ucontext_t *uc, unsigned int instr);

// Mask based on condition
void do_lasx_emu_xvmskltz_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmskltz_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmskltz_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmskltz_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmskgez_b(ucontext_t *uc, unsigned int instr);

// Misc
void do_lasx_emu_xvmsknz_b(ucontext_t *uc, unsigned int instr);

// Compare any/all zero
void do_lasx_emu_xvseteqz_v(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetnez_v(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetanyeqz_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetanyeqz_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetanyeqz_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetanyeqz_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetallnez_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetallnez_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetallnez_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvsetallnez_d(ucontext_t *uc, unsigned int instr);

// Immediate compares
void do_lasx_emu_xvseqi_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvseqi_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvseqi_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvseqi_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvslei_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslei_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvslti_du(ucontext_t *uc, unsigned int instr);

// Immediate max/min
void do_lasx_emu_xvmaxi_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaxi_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaxi_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaxi_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_d(ucontext_t *uc, unsigned int instr);

void do_lasx_emu_xvmaxi_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaxi_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaxi_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmaxi_du(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_bu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_hu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_wu(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvmini_du(ucontext_t *uc, unsigned int instr);

// Immediate find first bit
void do_lasx_emu_xvfrstpi_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvfrstpi_h(ucontext_t *uc, unsigned int instr);

#endif // LASX_CMP_H
