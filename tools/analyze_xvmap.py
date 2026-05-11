#!/usr/bin/env python3
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

"""Analyze LoongArch assembly for xvmap loop JIT feasibility.

Usage:
    tools/analyze_xvmap.py < input.asm
    loongarch64-unknown-linux-gnu-objdump -d binary | tools/analyze_xvmap.py
"""

import sys, re

XR_MAX = 16
VR_ALL = 32

# Parse lines like: "  6751c8:       2c800321        xvld            $xr1, $s2, 0"
LINE_RE = re.compile(
    r'^\s*[0-9a-f]+:\s+[0-9a-f]+\s+(\S+)\s*(.*)$', re.IGNORECASE
)


def extract_xrs(operands: str) -> set[int]:
    """Return set of XR indices from operand string."""
    return {int(m.group(1)) for m in re.finditer(r'\$xr(\d+)', operands)}


def extract_vrs(operands: str) -> set[int]:
    """Return set of VR/LSX indices from operand string (vr or fpr)."""
    return {int(m.group(1)) for m in re.finditer(r'\$(?:vr|[fv])(\d+)', operands)}


def main():
    loop_body = sys.stdin.read()
    if not loop_body.strip():
        print("Usage: loongarch64-unknown-linux-gnu-objdump -d <binary> | tools/analyze_xvmap.py")
        sys.exit(1)

    lines = loop_body.strip().split('\n')
    instrs = []
    for line in lines:
        m = LINE_RE.match(line)
        if m:
            instrs.append((m.group(1), m.group(2)))

    if not instrs:
        print("No instructions parsed. Input format: '  addr:  hex  mnemonic operands'")
        sys.exit(1)

    xr_mask = set()
    vr_all = set()
    xv_count = 0

    for i, (mnemonic, operands) in enumerate(instrs):
        is_xv = mnemonic.startswith('xv') or mnemonic.startswith('vext')
        if is_xv:
            xv_count += 1
            xrs = extract_xrs(operands)
            xr_mask |= xrs
            vr_all |= xrs  # each XR also occupies its base VR slot

        vrs = extract_vrs(operands)
        vr_all |= vrs

    xr_cnt = len(xr_mask)
    vr_used = len(vr_all)
    free_vr = VR_ALL - vr_used
    feasible = xr_cnt > 0 and xr_cnt <= XR_MAX and free_vr >= xr_cnt

    print(f"Loop instructions: {len(instrs)}")
    print(f"XV instructions:   {xv_count}")
    print(f"XR registers used: {sorted(xr_mask)}  (cnt={xr_cnt}, max={XR_MAX})")
    print(f"VR registers used: {sorted(vr_all)}  (free={free_vr}, need={xr_cnt})")
    print(f"xvmap feasible:    {'YES' if feasible else 'NO'}")
    if not feasible:
        if xr_cnt == 0:
            print("  reason: no XV/XR instructions in body")
        elif xr_cnt > XR_MAX:
            print(f"  reason: too many XR ({xr_cnt} > {XR_MAX})")
        else:
            print(f"  reason: not enough free VR ({free_vr} < {xr_cnt})")
    else:
        # Also estimate xvmap JIT size (rough)
        print(f"  estimated xvmap entries: {xr_cnt}")
        print(f"  estimated prologue:      {xr_cnt * 2} insns (save + load)")
        # Each XV instruction ≈ 2 LSX instructions + loop control copy
        jit_insns = xr_cnt * 2 + xv_count * 2 + (len(instrs) - xv_count) + 4
        print(f"  estimated JIT size:      ~{jit_insns} insns ({jit_insns * 4} bytes)")


if __name__ == '__main__':
    main()
