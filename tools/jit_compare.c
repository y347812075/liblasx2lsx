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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ucontext.h>
#include "lagoon.h"
#include "lasx_interpret.h"
#include "lasx_interpret_opt_gen_usedef.h"

#define MAX_INSTRS 128
#define BUF_SIZE 65536

static int parse_hex(const char *s, unsigned int *out)
{
    char *end = NULL;
    *out = strtoul(s, &end, 16);
    return (end && *end == '\0');
}

static void disassemble_buf(uint8_t *buf, int count)
{
    uint32_t *code = (uint32_t *)buf;
    for (int i = 0; i < count; i++) {
        lagoon_insn_t insn;
        char str[256];
        la_disasm_one(code[i], &insn);
        la_insn_to_str(&insn, str, sizeof(str));
        printf("    %08x  %s\n", code[i], str);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hex_instr> [hex_instr ...]\n", argv[0]);
        return 1;
    }

    unsigned int insts[MAX_INSTRS];
    int n = 0;
    for (int i = 1; i < argc && n < MAX_INSTRS; i++) {
        if (!parse_hex(argv[i], &insts[n])) {
            fprintf(stderr, "  skip: %s\n", argv[i]);
            continue;
        }
        n++;
    }

    printf("Input instructions (%d):\n", n);
    for (int i = 0; i < n; i++) {
        lagoon_insn_t insn;
        char str[256];
        la_disasm_one(insts[i], &insn);
        la_insn_to_str(&insn, str, sizeof(str));
        printf("  [%2d] %08x  %s\n", i, insts[i], str);
    }
    printf("\n");

    /* ---- block mode ---- */
    uint8_t block_buf[BUF_SIZE];
    lagoon_assembler_t as;
    la_init_assembler(&as, block_buf, BUF_SIZE);
    la_pcaddi(&as, LA_ZERO, 0);
    for (int i = 0; i < n; i++) {
        if (!lasx_interpret_inst_gen(&as, insts[i], 0)) {
            fprintf(stderr, "  [block] unsupported at [%d]: %08x\n", i, insts[i]);
        }
    }
    int block_cnt = la_get_inst_count(&as);
    printf("=== block mode (%d instructions) ===\n", block_cnt);
    disassemble_buf(block_buf, block_cnt);
    printf("\n");

    /* ---- usedef mode ---- */
    uint8_t ud_buf[BUF_SIZE];
    la_init_assembler(&as, ud_buf, BUF_SIZE);
    la_pcaddi(&as, LA_ZERO, 0);

    block_usedef_t ud;
    int temp_vr_map[n][32];
    uint32_t temp_vr_all_bm;
    memset(&ud, 0, sizeof(ud));
    memset(temp_vr_map, -1, sizeof(temp_vr_map));
    int ret = build_block_usedef(insts, n, &ud, temp_vr_map, &temp_vr_all_bm);
    if (ret == 0) {
        emit_block_usedef_prologue(&as, &ud);
        for (int i = 0; i < n; i++) {
            for (int x = 0; x < 32; x++)
                ud.temp_vr[x] = temp_vr_map[i][x];
            if (!lasx_interpret_inst_gen_usedef(&as, insts[i], ud.temp_vr)) {
                fprintf(stderr, "  [usedef] unsupported at [%d]: %08x\n", i, insts[i]);
            }
        }
        emit_block_usedef_epilogue(&as, &ud, temp_vr_map[n-1], temp_vr_all_bm);
    } else {
        fprintf(stderr, "  [usedef] build_block_usedef failed (ret=%d)\n", ret);
    }
    int ud_cnt = la_get_inst_count(&as);
    printf("=== usedef mode (%d instructions) ===\n", ud_cnt);
    disassemble_buf(ud_buf, ud_cnt);
    printf("\n");

    /* summary */
    printf("--- summary ---\n");
    printf("  block:  %d instructions\n", block_cnt);
    printf("  usedef: %d instructions\n", ud_cnt);
    printf("  ratio:  %.2f%%\n", ud_cnt > 0 ? (float)block_cnt / ud_cnt * 100 : 0);

    return 0;
}
