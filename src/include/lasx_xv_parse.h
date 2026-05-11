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

#ifndef LASX_XV_PARSE_H
#define LASX_XV_PARSE_H

#include <stdbool.h>
#include <stdint.h>

/* Lightweight LASX instruction parser.  Replaces la_disasm_one() in contexts
   where only XR register numbers, store/rmw flags are needed.
   All XVI instructions encode xd at bits[4:0] and xj at bits[9:5].
   xk at bits[14:10] is present for most but NOT for immediate 2OP forms.
   xa at bits[19:15] is present for 4OP forms (FMA, bitsel, shuf.b, insve0). */

static inline bool is_xv_instr(uint32_t instr)
{
    unsigned int top8 = instr >> 24;
    if (top8 >= 0x74 && top8 <= 0x77) return true;     /* bulk ALU/immediate */
    if (top8 == 0x0a) return true;                      /* FMA family */
    if (top8 == 0x0c) return (instr & (1u << 23));      /* xvfcmp (bit23=1) */
    if (top8 == 0x0d) {                                 /* xvbitsel / xvshuf.b */
        unsigned int lo = (instr >> 20) & 0x7;
        return (lo == 2 || lo == 6);
    }
    if (top8 == 0x2c) return (instr & (1u << 23));      /* xvld/xvst (bit23=1) */
    if (top8 == 0x32) return true;                       /* xvldrepl */
    if (top8 == 0x33) return true;                       /* xvstelm */
    if (top8 == 0x38) return (instr & (1u << 19));      /* xvldx/xvstx (bit19=1) */
    return false;
}

/* Parsed XV instruction info.
   xd/xj/xk/xa are XR register numbers if that operand exists, -1 otherwise.
   For GPR operands (base register in loads/stores, etc.) the value is -1. */
typedef struct {
    int  xd;
    int  xj;
    int  xk;
    int  xa;
    int  opcnt;        /* total operand count including immediates/GPRs */
    bool is_store;     /* xd is a data source (xvst/xvstx/xvstelm) */
    bool is_rmw;       /* xd is both written and read (vmadd/xvinsve0/etc) */
} xv_op_info_t;

static inline void parse_xv_op(uint32_t instr, xv_op_info_t *info)
{
    info->xd = instr & 0x1f;
    info->xj = (instr >> 5) & 0x1f;
    info->xk = (instr >> 10) & 0x1f;
    info->xa = (instr >> 15) & 0x1f;
    info->is_store = false;
    info->is_rmw  = false;

    unsigned int top8 = instr >> 24;

    switch (top8) {
    case 0x0a:  /* FMA: 4 XR operands, RMW */
        info->opcnt = 4;
        info->is_rmw = true;
        break;

    case 0x0c:  /* xvfcmp: 3 XR operands (bit23=1 for XV) */
        info->opcnt = 3;
        info->xj = (instr >> 5) & 0x1f;
        info->xk = (instr >> 10) & 0x1f;
        info->xa = -1;
        break;

    case 0x0d:  /* xvbitsel / xvshuf.b: 4 XR */
        info->opcnt = 4;
        info->xj = (instr >> 5) & 0x1f;
        info->xk = (instr >> 10) & 0x1f;
        info->xa = (instr >> 15) & 0x1f;
        break;

    case 0x2c:  /* xvld / xvst: xd=XR, xj=GPR(base), imm=offset */
        info->opcnt = 1;
        info->xj = -1;     /* GPR, not XR */
        info->xk = -1;
        info->xa = -1;
        info->is_store = (instr & (1u << 22));
        break;

    case 0x32:  /* xvldrepl.*: xd=XR, xj=GPR(base) */
        info->opcnt = 1;
        info->xj = -1;
        info->xk = -1;
        info->xa = -1;
        break;

    case 0x33:  /* xvstelm.*: xd=XR(source), xj=GPR(base) */
        info->opcnt = 1;
        info->xj = -1;
        info->xk = -1;
        info->xa = -1;
        info->is_store = true;
        break;

    case 0x38:  /* xvldx / xvstx: xd=XR, xj/xk=GPR */
        info->opcnt = 1;
        info->xj = -1;
        info->xk = -1;
        info->xa = -1;
        info->is_store = (instr & (1u << 18));
        break;

    case 0x74:
    case 0x75:  /* 3-operand bulk: xd/xj/xk = XR */
        info->opcnt = 3;
        info->xj = (instr >> 5) & 0x1f;
        info->xk = (instr >> 10) & 0x1f;
        info->xa = -1;
        /* xvshuf.h/w/d are RMW: opcode region 0x757A-0x757B */
        if (((instr >> 16) & 0xFF) == 0x7A || ((instr >> 16) & 0xFF) == 0x7B ||
            ((instr >> 16) & 0xFF) == 0x2B)
            info->is_rmw = true;
        break;

    case 0x76:
    case 0x77:  /* 2-operand / immediate bulk */
        info->xk = -1;   /* bits[14:10] are immediate or not present */
        info->xa = -1;

        /* Check for GPR-mix instructions first */
        unsigned int hi = instr >> 16;

        /* xvpickve2gr: xd=GPR, xj=XR. 0x76EFxxx */
        if (hi == 0x76EF) {
            info->opcnt = 1;
            info->xd = -1;          /* GPR */
            info->xj = (instr >> 5) & 0x1f;  /* XR source */
            break;
        }

        /* xvinsgr2vr: xd=XR, xj=GPR. 0x76EBxxx */
        if (hi == 0x76EB) {
            info->opcnt = 1;
            info->xj = -1;          /* GPR */
            info->is_rmw = true;
            break;
        }

        /* xvreplgr2vr: xd=XR, xj=GPR. 0x769F0xxx where bits[15:12]=0 */
        if (hi == 0x769F && (instr & 0xF000) == 0) {
            info->opcnt = 1;
            info->xj = -1;
            break;
        }

        /* xvldi: only xd is XR. 0x77E0xxxx (not 0x77E4/0x77E8/0x77EC) */
        if ((instr & 0xFFFC0000) == 0x77E00000) {
            info->opcnt = 1;
            info->xj = -1;
            break;
        }

        /* Default: 2 XR operands (xd, xj) */
        info->opcnt = 2;
        info->xj = (instr >> 5) & 0x1f;
        info->xk = -1;

        /* RMW detection within 0x76-0x77 */
        /* xvshuf4i.d : 0x779C only (.b/.h/.w are not RMW) */
        if ((instr & 0xFFFC0000) == 0x779C0000)
            info->is_rmw = true;

        /* xvbitseli.b : 0x77C4 */
        if ((instr & 0xFFFF0000) == 0x77C40000)
            info->is_rmw = true;

        /* xvextrins.* : 0x7780-0x778C */
        if ((instr & 0xFFF00000) == 0x77800000)
            info->is_rmw = true;

        /* Narrow-shift RMW: 0x77400000-0x776EFFFF.
           Mask 0xFFF00000 keeps bits[31:20]: 0x774 ... 0x776. */
        if ((instr & 0xFFF00000) >= 0x77400000 &&
            (instr & 0xFFF00000) <= 0x77600000)
            info->is_rmw = true;

        /* xvfrstpi.* : 0x769A */
        if ((instr & 0xFFFF0000) == 0x769A0000)
            info->is_rmw = true;

        /* xvpermi.w/q: 0x77E4, 0x77EC (NOT .d which is 0x77E8) */
        if ((instr & 0xFFFC0000) == 0x77E40000 ||
            (instr & 0xFFFC0000) == 0x77EC0000)
            info->is_rmw = true;

        /* xvinsve0.* : 0x76FFC000 / 0x76FFE000 */
        if ((instr & 0xFFFFC000) == 0x76FFC000 ||
            (instr & 0xFFFFE000) == 0x76FFE000)
            info->is_rmw = true;

        break;

    default:
        info->xd = -1;
        info->xj = -1;
        info->xk = -1;
        info->xa = -1;
        info->opcnt = 0;
        break;
    }
}

#endif /* LASX_XV_PARSE_H */
