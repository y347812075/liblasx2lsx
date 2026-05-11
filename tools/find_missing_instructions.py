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
    """Extract all instruction mnemonics from lasx_instructions.md"""
    instructions = []
    with open(filename, 'r') as f:
        for line in f:
            # Match lines with instruction entries like: | xvabsd_b | `xvabsd_b 0111 01000110 00000 ..... ..... .....` | @vvv | vvv |
            if line.startswith('| ') and '`' in line:
                parts = line.split('|')
                if len(parts) >= 2:
                    mnemonic = parts[1].strip()
                    if mnemonic and not mnemonic.startswith('助记符'):
                        instructions.append(mnemonic)
    return sorted(instructions)

def extract_opcodes_from_privateh(filename):
    """Extract all instruction names from opcode defines in lasx_emu_private.h"""
    opcodes = []
    pattern = re.compile(r'#define\s+OP\d+_([A-Z0-9_]+)\s+0b[01]+')
    with open(filename, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                name = match.group(1)
                # Convert from OP17_XVADD_B to xvadd_b
                mnemonic = name.lower()
                opcodes.append(mnemonic)
    return sorted(opcodes)

def extract_cases_from_emu_c(filename):
    """Extract all instruction names from switch cases in lasx_emu.c"""
    cases = []
    pattern = re.compile(r'case\s+OP\d+_([A-Z0-9_]+):\s+do_lasx_emu_([a-z0-9_]+)\(')
    with open(filename, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                name = match.group(2)
                cases.append(name)
    return sorted(cases)

def main():
    md_instructions = extract_instructions_from_md('lasx_instructions.md')
    h_opcodes = extract_opcodes_from_privateh('lasx_emu_private.h')
    c_cases = extract_cases_from_emu_c('lasx_emu.c')
    
    print(f"Total instructions in lasx_instructions.md: {len(md_instructions)}")
    print(f"Total opcodes in lasx_emu_private.h: {len(h_opcodes)}")
    print(f"Total cases in lasx_emu.c: {len(c_cases)}")
    print()
    
    # Find instructions missing from opcodes
    missing = []
    for instr in md_instructions:
        if instr not in h_opcodes:
            missing.append(instr)
    
    # Check which are already in cases but just missing in opcodes (shouldn't happen)
    missing_opcode_only = []
    missing_both = []
    for instr in missing:
        if instr in c_cases:
            missing_opcode_only.append(instr)
        else:
            missing_both.append(instr)
    
    print(f"Missing instructions (not in opcodes or cases): {len(missing_both)}")
    print()
    
    if missing_both:
        print("List of missing instructions:")
        print("-" * 60)
        # Group by prefix (category)
        categories = defaultdict(list)
        for instr in missing_both:
            prefix = instr.split('_')[0]
            categories[prefix].append(instr)
        
        for prefix in sorted(categories.keys()):
            print(f"\n{prefix.upper()}:")
            for instr in sorted(categories[prefix]):
                print(f"  - {instr}")
    
        print("-" * 60)
        print(f"\nTotal: {len(missing_both)} instructions missing")
    else:
        print("No missing instructions found!")
    
    # Check for inconsistencies between h and c
    h_set = set(h_opcodes)
    c_set = set(c_cases)
    
    in_h_not_in_c = h_set - c_set
    in_c_not_in_h = c_set - h_set
    
    print()
    if in_h_not_in_c:
        print(f"Instructions in opcodes but not in cases: {len(in_h_not_in_c)}")
        for instr in sorted(in_h_not_in_c):
            print(f"  - {instr}")
        print()
    
    if in_c_not_in_h:
        print(f"Instructions in cases but not in opcodes: {len(in_c_not_in_h)}")
        for instr in sorted(in_c_not_in_h):
            print(f"  - {instr}")

if __name__ == '__main__':
    main()
