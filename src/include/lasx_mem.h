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

#ifndef LASX_MEM_H
#define LASX_MEM_H

#include <ucontext.h>

// Memory instructions
void do_lasx_emu_xvld(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvst(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvldrepl_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvldrepl_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvldrepl_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvldrepl_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvstelm_b(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvstelm_h(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvstelm_w(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvstelm_d(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvldx(ucontext_t *uc, unsigned int instr);
void do_lasx_emu_xvstx(ucontext_t *uc, unsigned int instr);

#endif // LASX_MEM_H
