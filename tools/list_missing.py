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

import re
from collections import defaultdict

def extract_instructions_from_md(filename):
    instructions = []
    with open(filename, 'r') as f:
        for line in f:
            if line.startswith('| ') and '`' in line:
                parts = line.split('|')
                if len(parts) >= 2:
                    mnemonic = parts[1].strip()
                    if mnemonic and not mnemonic.startswith('助记符'):
                        instructions.append(mnemonic)
    return sorted(instructions)

def extract_opcodes_from_privateh(filename):
    opcodes = []
    pattern = re.compile(r'#define\s+OP\d+_([A-Z0-9_]+)\s+0b[01]+')
    with open(filename, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                name = match.group(1)
                mnemonic = name.lower()
                opcodes.append(mnemonic)
    return set(opcodes)

def extract_cases_from_emu_c(filename):
    cases = []
    pattern = re.compile(r'case\s+OP\d+_([A-Z0-9_]+):\s+do_lasx_emu_([a-z0-9_]+)\(')
    with open(filename, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                name = match.group(2)
                cases.append(name)
    return set(cases)

def main():
    md_instructions = extract_instructions_from_md('lasx_instructions.md')
    h_opcodes = extract_opcodes_from_privateh('lasx_emu_private.h')
    c_cases = extract_cases_from_emu_c('lasx_emu.c')
    
    missing = []
    for instr in md_instructions:
        if instr not in c_cases:
            missing.append(instr)
    
    # Group by category based on naming/type
    categories = defaultdict(list)
    
    for instr in missing:
        # Categorize based on instruction name prefix
        if instr.startswith('xvabsd'):
            cat = 'arith'
        elif instr.startswith('xvadd') or instr.startswith('xvsub'):
            cat = 'arith'
        elif instr.startswith('xvavg') or instr.startswith('xvavgr'):
            cat = 'arith'
        elif instr.startswith('xvsadd') or instr.startswith('xvssub'):
            cat = 'arith'
        elif instr.startswith('xvbit') or instr.startswith('xvclo') or instr.startswith('xvclz') or \
             instr.startswith('xvpcnt') or instr.startswith('xvneg') or instr.startswith('vmsk'):
            cat = 'bitmanip'
        elif instr.startswith('xvfcmp'):
            cat = 'fcmp'
        elif instr.startswith('xvf'):
            cat = 'fbasic'
        elif instr.startswith('xvhadd') or instr.startswith('xvhsub'):
            cat = 'arith'
        elif instr.startswith('xvins') or instr.startswith('xvpick') or instr.startswith('xvrepl'):
            cat = 'extract'
        elif instr.startswith('xvpack') or instr.startswith('xvpick'):
            cat = 'perm'
        elif instr.startswith('xvrotr') or instr.startswith('xvsll') or instr.startswith('xvsra') or \
             instr.startswith('xvsrar') or instr.startswith('xvsrl') or instr.startswith('xvssr'):
            cat = 'shift'
        elif instr.startswith('xvmul') or instr.startswith('xvdiv') or instr.startswith('xvmod') or \
             instr.startswith('xvmadd') or instr.startswith('xvmsub') or instr.startswith('xvmuh'):
            cat = 'muldiv'
        elif instr.startswith('xvseq') or instr.startswith('xvsle') or instr.startswith('xvslt'):
            cat = 'cmp'
        elif instr.startswith('xvshuf') or instr.startswith('xvperm'):
            cat = 'perm'
        else:
            cat = 'other'
        categories[cat].append(instr)
    
    print("Missing instructions by category:")
    print("-" * 60)
    total = 0
    for cat in sorted(categories.keys()):
        instrs = sorted(categories[cat])
        print(f"{cat}: {len(instrs)} instructions")
        total += len(instrs)
        for instr in instrs:
            print(f"  - {instr}")
        print()
    
    print("-" * 60)
    print(f"Total missing: {total}")

if __name__ == '__main__':
    main()
