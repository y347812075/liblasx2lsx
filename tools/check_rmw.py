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

import re, sys, glob


def get_func_body(content, start):
    depth = 1
    p = start
    while depth and p < len(content):
        if content[p] == "{":
            depth += 1
        elif content[p] == "}":
            depth -= 1
        p += 1
    return content[start:p]


def extract_loops(body):
    """Extract all top-level for loops from function body."""
    loops = []
    for m in re.finditer(r"for\s*\([^)]+\)\s*\{", body):
        loop_start = m.start()
        body_after = body[loop_start:]
        depth = 0
        brace_start = -1
        for i, c in enumerate(body_after):
            if c == "{":
                if depth == 0:
                    brace_start = i
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0 and brace_start != -1:
                    loops.append(body_after[brace_start + 1 : i])
                    break
    return loops


def extract_loop_var_and_range(loop):
    """Extract loop variable and range from for loop."""
    m = re.search(r"for\s*\(\s*int\s+(\w+)\s*=\s*(\d+)\s*;\s*\1\s*<\s*(\d+)", loop)
    if m:
        return m.group(1), int(m.group(2)), int(m.group(3))
    return None, 0, 0


def get_bit_width(func_name):
    """Infer bit width from function name: _b=8, _h=16, _w=32, _d=64, _q=128."""
    if "_q_" in func_name or "_q." in func_name:
        return 128
    if "_d_" in func_name or "_d." in func_name:
        return 64
    if "_w_" in func_name or "_w." in func_name:
        return 32
    if "_h_" in func_name or "_h." in func_name:
        return 16
    return 8


def max_idx_for_width(bit_width):
    """Return max index for given bit width."""
    if bit_width >= 128:
        return 1
    elif bit_width >= 64:
        return 3
    elif bit_width >= 32:
        return 7
    elif bit_width >= 16:
        return 15
    return 31


def check_file(filename):
    f = open(filename).read()
    issues = []
    for m in re.finditer(r"void\s+(do_lasx_emu_\w+)\s*\([^)]*\)\s*\{", f):
        name, body = m.group(1), get_func_body(f, m.end())
        if "int xd" not in body:
            continue

        loops = extract_loops(body)
        if not loops:
            continue

        # TYPE 1: Same loop reads and writes xd (classic RMW)
        for loop in loops:
            has_read_xd = bool(re.search(r"vreg_read\w+\([^)]*,\s*xd[,\s)]", loop))
            has_write_xd = bool(
                re.search(r"vreg_write\w+\(", loop) and bool(re.search(r"xd[,)]", loop))
            )
            if has_read_xd and has_write_xd:
                issues.append(f"{name} (TYPE1: same loop RMW)")
                break
        if any(f"{name} (" in i for i in issues):
            continue

        # TYPE 2: Last loop writes xd, also reads xj or xk (prefetch incomplete)
        # Report for ALL operations (accumulate + permute) to be safe
        if len(loops) >= 2:
            last_loop = loops[-1]
            has_write_xd = bool(
                re.search(r"vreg_write\w+\(", last_loop)
                and bool(re.search(r"xd[,)]", last_loop))
            )
            has_read_xj_or_xk = bool(
                re.search(r"vreg_read\w+\([^)]*,\s*x[jk][,\s)]", last_loop)
            )
            if has_write_xd and has_read_xj_or_xk:
                issues.append(f"{name} (TYPE2: prefetch incomplete)")

        # TYPE 3: In-loop read-after-write on same register (idx may overlap)
        # When xd==xj or xd==xk, later iteration may read what earlier wrote
        for loop in loops:
            # Check if loop reads and writes xd
            reads_xd = re.findall(r"vreg_read\w+\([^)]*,\s*xd,\s*(\w+)\)", loop)
            writes_xd = re.findall(r"vreg_write\w+\([^)]*,\s*xd,\s*(\w+)\)", loop)
            if reads_xd and writes_xd:
                # Extract loop variable range
                m = re.search(
                    r"for\s*\(\s*int\s+(\w+)\s*=\s*(\d+)\s*;\s*\w+\s*<\s*(\d+)", loop
                )
                if m:
                    loop_var = m.group(1)
                    start = int(m.group(2))
                    end = int(m.group(3))
                    # Get bit width to calculate max idx
                    bit_width = get_bit_width(name)
                    max_idx = max_idx_for_width(bit_width)
                    # Check if indices could overlap (i.e., read uses same loop var)
                    for read_idx in reads_xd:
                        if loop_var in read_idx:
                            issues.append(f"{name} (TYPE3: in-loop RMW)")
                            break

    return issues


def main():
    files = sys.argv[1:] if len(sys.argv) > 1 else glob.glob("lasx_*.c")
    total = []
    for f in files:
        issues = check_file(f)
        if issues:
            print(f"\n=== {f} ===")
            for n in issues:
                print(f"  {n}")
                total.append(n)
    print(f"\nTotal: {len(total)} functions with RMW issues")
    return len(total)


if __name__ == "__main__":
    sys.exit(main())
