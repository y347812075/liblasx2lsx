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

"""
Python script to convert inline assembly LASX tests to intrinsics.

This script helps convert test files that use inline assembly (like "xvld $xr0, %0, 0")
to use intrinsics (like "__lasx_xvld(&mem, 0)").

Usage:
    python convert_to_intrinsics.py <input_file.c> <output_file.c>
"""

import re
import sys


# Mapping from inline assembly instructions to intrinsics
# Format: (asm_pattern, intrinsic_template, description)
INSTRUCTION_MAP = {
    # Load/Store
    'xvld': ('__lasx_xvld({addr}, {offset})', 'Vector load'),
    'xvst': ('__lasx_xvst({src}, {addr}, {offset})', 'Vector store'),
    'xvldx': ('__lasx_xvldx({addr}, {index})', 'Vector indexed load'),
    'xvstx': ('__lasx_xvstx({src}, {addr}, {index})', 'Vector indexed store'),
    'xvldrepl': ('__lasx_xvldrepl_{size}({addr}, {offset})', 'Vector load and replicate'),
    'xvst': ('__lasx_xvst{size}({src}, {addr}, {offset})', 'Vector store'),
    
    # Integer arithmetic
    'xvadd': ('__lasx_xvadd_{size}({src1}, {src2})', 'Vector add'),
    'xvsub': ('__lasx_xvsub_{size}({src1}, {src2})', 'Vector subtract'),
    'xvabsd': ('__lasx_xvabsd_{size}({src1}, {src2})', 'Vector absolute difference'),
    'xvmul': ('__lasx_xvmul_{size}({src1}, {src2})', 'Vector multiply'),
    'xvdiv': ('__lasx_xvdiv_{size}({src1}, {src2})', 'Vector divide'),
    
    # Shift
    'xvsll': ('__lasx_xvsll_{size}({src1}, {src2})', 'Vector shift left logical'),
    'xvsrl': ('__lasx_xvsrl_{size}({src1}, {src2})', 'Vector shift right logical'),
    'xvsra': ('__lasx_xvsra_{size}({src1}, {src2})', 'Vector shift right arithmetic'),
    
    # Bitwise
    'xvand': ('__lasx_xvand_v({src1}, {src2})', 'Vector AND'),
    'xvandn': ('__lasx_xvandn_v({src1}, {src2})', 'Vector AND NOT'),
    'xvor': ('__lasx_xvor_v({src1}, {src2})', 'Vector OR'),
    'xvxor': ('__lasx_xvxor_v({src1}, {src2})', 'Vector XOR'),
    'xvnor': ('__lasx_xvnor_v({src1}, {src2})', 'Vector NOR'),
    'xvorn': ('__lasx_xvorn_v({src1}, {src2})', 'Vector OR NOT'),
    
    # Compare
    'xvseq': ('__lasx_xvseq_{size}({src1}, {src2})', 'Vector set equal'),
    'xvslt': ('__lasx_xvslt_{size}({src1}, {src2})', 'Vector set less than'),
    'xvsle': ('__lasx_xvsle_{size}({src1}, {src2})', 'Vector set less or equal'),
    
    # Permutation
    'xvshuf': ('__lasx_xvshuf_{size}({src1}, {src2}, {sel})', 'Vector shuffle'),
    'xvpermi': ('__lasx_xvpermi_{size}({src1}, {imm})', 'Vector permute immediate'),
    'xvbitsel': ('__lasx_xvbitsel_v({src1}, {src2}, {mask})', 'Vector bit select'),
    'xvpackev': ('__lasx_xvpackev_{size}({src1}, {src2})', 'Vector pack even'),
    'xvpackod': ('__lasx_xvpackod_{size}({src1}, {src2})', 'Vector pack odd'),
    'xvreplve': ('__lasx_xvreplve_{size}({src1}, {index})', 'Vector replicate by index'),
    'xvreplgr2vr': ('__lasx_xvreplgr2vr_{size}({value})', 'Vector replicate GPR to vector'),
    'xvrepli': ('__lasx_xvrepli_{size}({imm})', 'Vector replicate immediate'),
    'xvperm': ('__lasx_xvperm_w({src1}, {src2})', 'Vector permute'),
    
    # Float compare (no direct equivalent in some versions)
    'xvfcmp': ('__lasx_xvfcmp_{cond}_{size}({src1}, {src2})', 'Vector float compare'),
    'xvfcmplt': ('__lasx_xvfcmp_clt_{size}({src1}, {src2})', 'Vector float compare less than'),
}


# Common suffixes for different data sizes
SIZE_SUFFIXES = {
    'b': 'b',   # byte (8-bit)
    'h': 'h',   # halfword (16-bit)
    'w': 'w',   # word (32-bit)
    'd': 'd',   # doubleword (64-bit)
    'q': 'q',   # quadword (128-bit)
    'v': 'v',   # vector (full 256-bit)
}


def parse_asm_instruction(asm_line):
    """
    Parse an inline assembly instruction line.
    Returns: (instruction, operands)
    """
    # Remove leading/trailing whitespace
    asm_line = asm_line.strip()
    
    # Match instruction like "xvld $xr0, %0, 0"
    match = re.match(r'(\w+)\s+(.+)', asm_line)
    if match:
        instr = match.group(1)
        ops = match.group(2)
        return instr, ops
    return None, None


def extract_registers(ops):
    """
    Extract register names from operands.
    Returns list of (reg_name, is_xr) tuples
    """
    regs = []
    # Match $xr0, $xr1, etc.
    for reg_match in re.finditer(r'\$(\w+)\s*,?\s*', ops):
        reg = reg_match.group(1)
        if reg.startswith('xr'):
            regs.append((reg, True))
    return regs


def extract_immediates(ops):
    """
    Extract immediate values from operands.
    """
    imms = []
    # Match decimal or hex numbers
    for imm_match in re.finditer(r'(-?0x[0-9a-fA-F]+|-?\d+)', ops):
        imms.append(imm_match.group(1))
    return imms


def generate_intrinsic_call(instr, ops, reg_map=None):
    """
    Generate intrinsic function call from assembly instruction.
    
    reg_map: dict mapping asm registers to C variable names
    """
    if reg_map is None:
        reg_map = {}
    
    # Get base instruction (remove any .b, .h, .w, .d suffix)
    base_instr = re.sub(r'\.[bhdwqv]+$', '', instr)
    
    # Determine size suffix from instruction
    size_match = re.search(r'\.([bhdwqv]+)$', instr)
    size = size_match.group(1) if size_match else 'w'
    
    # Extract components
    regs = extract_registers(ops)
    imms = extract_immediates(ops)
    
    # Handle different instruction types
    if base_instr == 'xvld':
        # xvld $xrN, offset(base)
        base_reg = re.search(r'\$(\w+),?\s*(\d*)\((\w+)\)', ops)
        if base_reg:
            xr = base_reg.group(1)
            offset = base_reg.group(2) or '0'
            base = base_reg.group(3)
            var_name = reg_map.get(xr, 'mem')
            # Simplify: assume mem is the base pointer
            return f"__lasx_xvld({var_name}, {offset})"
    
    elif base_instr == 'xvst':
        # xvst $xrN, offset(base)
        base_reg = re.search(r'\$(\w+),?\s*(\d*)\((\w+)\)', ops)
        if base_reg:
            xr = base_reg.group(1)
            offset = base_reg.group(2) or '0'
            base = base_reg.group(3)
            src = reg_map.get(xr, 'xr0')
            var_name = reg_map.get(base, 'mem')
            return f"__lasx_xvst({src}, {var_name}, {offset})"
    
    elif base_instr in ['xvadd', 'xvsub', 'xvmul', 'xvdiv', 'xvabsd',
                        'xvand', 'xvor', 'xvxor', 'xvnor', 'xvorn', 'xvandn',
                        'xvseq', 'xvslt', 'xvsle',
                        'xvsll', 'xvsrl', 'xvsra',
                        'xvpackev', 'xvpackod']:
        # Two-register operations: op $xrD, $xrJ, $xrK
        match = re.match(r'\$(\w+),?\s*\$(\w+),?\s*\$(\w+)', ops)
        if match:
            dst = reg_map.get(match.group(1), 'xr0')
            src1 = reg_map.get(match.group(2), 'xr0')
            src2 = reg_map.get(match.group(3), 'xr1')
            
            # Map to intrinsic
            intrinsic_name = f"__lasx_{base_instr}"
            if base_instr in ['xvand', 'xvor', 'xvxor', 'xvnor', 'xvorn', 'xvandn']:
                intrinsic_name += "_v"
            else:
                intrinsic_name += f"_{size}"
            
            return f"{intrinsic_name}({src1}, {src2})"
    
    elif base_instr == 'xvshuf':
        # xvshuf.$size $xrD, $xrJ, $xrK  or xvshuf.$size $xrD, $xrJ, $xrK, $xrA
        match = re.match(r'\$(\w+),?\s*\$(\w+),?\s*\$(\w+)(?:,\s*\$(\w+))?', ops)
        if match:
            dst = reg_map.get(match.group(1), 'xr0')
            src1 = reg_map.get(match.group(2), 'xr0')
            src2 = reg_map.get(match.group(3), 'xr1')
            sel = reg_map.get(match.group(4), 'xr2') if match.group(4) else 'sel'
            
            # For xvshuf.f/d, it's 3 operands; for b/h/w/d it's 2 operands
            if size in ['f', 'd']:
                return f"__lasx_xvshuf_{size}({src1}, {src2}, {sel})"
            else:
                return f"__lasx_xvshuf_{size}({src1}, {src2})"
    
    elif base_instr == 'xvpermi':
        # xvpermi.$size $xrD, $xrJ, imm
        match = re.match(r'\$(\w+),?\s*(?:\$(\w+),)?\s*(\w+)', ops)
        if match:
            dst = reg_map.get(match.group(1), 'xr0')
            src1 = reg_map.get(match.group(2), 'xr0') if match.group(2) else dst
            imm = match.group(3)
            return f"__lasx_xvpermi_{size}({src1}, {imm})"
    
    elif base_instr == 'xvreplve':
        # xvreplve.$size $xrD, $xrJ
        match = re.match(r'\$(\w+),?\s*\$(\w+),?\s*\$?(\w+)?', ops)
        if match:
            dst = reg_map.get(match.group(1), 'xr0')
            src = reg_map.get(match.group(2), 'xr0')
            idx = match.group(3) if match.group(3) else '0'
            return f"__lasx_xvreplve_{size}({src}, {idx})"
    
    elif base_instr == 'xvbitsel':
        # xvbitsel.v $xrD, $xrJ, $xrK, $xrA
        match = re.match(r'\$(\w+),?\s*\$(\w+),?\s*\$(\w+),?\s*\$(\w+)', ops)
        if match:
            dst = reg_map.get(match.group(1), 'xr0')
            src1 = reg_map.get(match.group(2), 'xr0')
            src2 = reg_map.get(match.group(3), 'xr1')
            mask = reg_map.get(match.group(4), 'xr2')
            return f"__lasx_xvbitsel_v({src1}, {src2}, {mask})"
    
    elif base_instr == 'xvreplgr2vr':
        # xvreplgr2vr.$size $xrD, $rJ
        match = re.match(r'\$(\w+),?\s*\$(\w+)', ops)
        if match:
            dst = reg_map.get(match.group(1), 'xr0')
            gpr = match.group(2)
            # Need to extract GPR number
            gpr_num = re.search(r'r(\d+)', gpr)
            if gpr_num:
                return f"__lasx_xvreplgr2vr_{size}({gpr})"
    
    # Fallback: return commented out intrinsic
    return f"/* TODO: {instr} {ops} */"


def convert_asm_block(asm_block, func_name):
    """
    Convert an inline assembly block to intrinsics.
    
    asm_block: The assembly code as a string
    func_name: Name of the function (for context)
    """
    lines = asm_block.strip().split('\n')
    
    result_lines = []
    reg_map = {}  # Map from $xr0, $xr1, etc to variable names
    
    # First pass: collect register assignments from "r"(mem) inputs
    # This would need more sophisticated parsing
    
    for line in lines:
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        
        instr, ops = parse_asm_instruction(line)
        if not instr:
            continue
        
        # Try to generate intrinsic call
        intrinsic = generate_intrinsic_call(instr, ops, reg_map)
        
        # Add as comment or code
        result_lines.append(f"    // {line}")
        result_lines.append(f"    {intrinsic};")
    
    return '\n'.join(result_lines)


def extract_asm_functions(c_code):
    """
    Extract functions that contain inline assembly and convert them.
    """
    # Find all functions with __asm__ volatile
    pattern = r'static\s+void\s+(\w+)\s*\([^)]*\)\s*\{[^}]*__asm__\s+volatile'
    
    # This is a simplified version - the full implementation would need
    # more sophisticated parsing of C code
    
    return c_code


def show_usage():
    """Show usage information."""
    print(__doc__)
    print("\nSupported instructions:")
    for instr in sorted(set(k for k in INSTRUCTION_MAP.keys())):
        print(f"  {instr}")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        show_usage()
        sys.exit(1)
    
    # For now, just show what we can do
    print("This script provides instruction mapping for conversion.")
    print("Actual conversion requires manual intervention for complex cases.")
    print("\nSupported instructions:")
    for instr in sorted(set(k for k in INSTRUCTION_MAP.keys())):
        template, desc = INSTRUCTION_MAP[instr]
        print(f"  {instr}: {desc}")
        print(f"    Template: {template}")