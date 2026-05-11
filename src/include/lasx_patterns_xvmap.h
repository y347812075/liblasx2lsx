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

#ifndef LASX_PATTERNS_XVMAP_H
#define LASX_PATTERNS_XVMAP_H

#include "lagoon.h"

//#define LOOP_PATTERN_DEBUG
#ifdef LOOP_PATTERN_DEBUG
#define LOOP_DBG(fmt, ...) fprintf(stderr, "[LOOP] " fmt "\n", ##__VA_ARGS__)
#else
#define LOOP_DBG(fmt, ...) do {} while(0)  // NOLINT
#endif

int check_lasx_loop_pattern(unsigned int *p, int offset);
int detect_loop_range(unsigned int *p, int *loop_start, int *loop_end);
void *generate_xvmap_loop(lagoon_assembler_t *as, unsigned int *p,
                          int loop_start, int loop_len, uint32_t xvuse,
                          uint32_t xv_insn_mask[4]);
int check_xvmap_feasibility(unsigned int *p, int loop_len,
                            uint32_t *xvuse, uint32_t xv_insn_mask[4]);
bool try_optimize_loop(lagoon_assembler_t *as, unsigned int *p, void *entry);
int lasx_build_xvmap(int32_t mask, int *map);
void __gen_xvmap_prologue(void *as, int *map);
void __gen_xvmap_epilogue(void *as, int *map);

extern void *detected_loop_jit_entry;
extern unsigned int *detected_loop_start;

#endif
