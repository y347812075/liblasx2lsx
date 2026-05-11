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

def parse_lasx_instructions(filename):
    """Parse lasx_instructions.md and extract all instructions with their fixed opcode bits."""
    instructions = []
    in_table = False
    
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    for line in lines:
        # Check if we're entering a table section
        if '| 助记符 |' in line:
            in_table = True
            continue
        # End of table
        if line.strip() == '' and in_table and len(instructions) > 0:
            in_table = False
            continue
        if in_table and line.startswith('| '):
            parts = [p.strip() for p in line.split('|')[1:-1]]
            if len(parts) >= 2:
                mnemonic = parts[0]
                encoding = parts[1]
                # Remove the mnemonic from the encoding
                encoding_clean = encoding.split()[1:] if len(encoding.split()) > 1 else [encoding]
                binary_str = ''.join(encoding_clean)
                # Remove all spaces (they are just for readability per the instructions)
                binary_str = binary_str.replace(' ', '')
                # Now we have a string of 0, 1, and . where . is any bit
                # Extract the fixed bits (0 and 1) and their positions from the left (MSB to LSB)
                # LASX instructions are 32 bits, but most fixed bits are in the high bits
                # For opcode matching, we only care about the fixed bits from the left
                # up to the first wildcard (.), or up to the last fixed bit
                fixed_bits = []
                for i, c in enumerate(binary_str):
                    if c in '01':
                        fixed_bits.append((i, int(c)))
                    # Ignore dots, they are variable bits
                if fixed_bits:
                    # The matching is usually done by shifting the full 32-bit instruction
                    # right until we only have fixed bits. Let's calculate what the opcode mask
                    # and opcode value should be for matching.
                    # First, find the maximum position (leftmost is 0, rightmost is 31)
                    max_pos = max(p[0] for p in fixed_bits) if fixed_bits else -1
                    # The full instruction is 32 bits, so after max_pos, all are variable bits
                    # We need to shift right by (31 - max_pos) to get the final opcode value
                    # Wait, actually looking at how it's done in lasx_emu_private.h, most opcodes
                    # are 17 bits (when they do instr >> 15), which means the first 17 bits from
                    # the left (bits 31-15) are the fixed bits.
                    full_value = 0
                    full_mask = 0
                    for pos, bit in fixed_bits:
                        shift = 31 - pos  # because bit 31 is the highest bit (leftmost)
                        full_value |= (bit << shift)
                        full_mask |= (1 << shift)
                    
                    # Calculate what the expected value would be for different shift approaches
                    # Most 17-bit opcodes: shift >> 15 means we get the first 17 bits (bits 31-15)
                    # So the opcode value in the header are these 17 bits, already shifted right
                    opcode_17 = full_value >> 15
                    
                    # Check how many bits the opcode is
                    # If all fixed bits are within the first 17 bits (bits 31-15), it's an OP17
                    all_in_17 = all(pos <= 16 for pos, _ in fixed_bits)  # positions 0-16 are bits 31-15
                    is_op17 = all_in_17
                    
                    instructions.append({
                        'mnemonic': mnemonic,
                        'full_binary': binary_str,
                        'fixed_bits': fixed_bits,
                        'full_value': full_value,
                        'full_mask': full_mask,
                        'opcode_17': opcode_17,
                        'is_op17': is_op17,
                    })
    
    return instructions

def parse_current_header(filename):
    """Parse lasx_emu_private.h and extract all defined opcode macros."""
    opcodes = {}
    pattern = re.compile(r'#define\s+(OP[0-9]+_([A-Z0-9_]+))\s+(0b[01]+)')
    
    with open(filename, 'r') as f:
        content = f.read()
    
    for match in pattern.finditer(content):
        macro_name = match.group(1)
        mnemonic_key = match.group(2).lower()
        binary_value = match.group(3)
        # Convert binary string to integer
        value = int(binary_value, 2)
        bits_count = len(binary_value) - 2  # remove 0b prefix
        opcodes[mnemonic_key] = {
            'macro_name': macro_name,
            'value': value,
            'bits_count': bits_count,
            'binary': binary_value
        }
    
    return opcodes

def main():
    print("Parsing lasx_instructions.md...")
    instructions = parse_lasx_instructions('lasx_instructions.md')
    
    print(f"Found {len(instructions)} instructions in lasx_instructions.md")
    
    print("\nParsing lasx_emu_private.h...")
    current_opcodes = parse_current_header('lasx_emu_private.h')
    
    print(f"Found {len(current_opcodes)} defined opcodes in header")
    
    # Check which instructions from lasx_instructions are missing in the header
    missing = []
    incorrect = []
    
    # Group instructions by base mnemonic (to match how they're defined in header)
    # Header uses OP17_<MNEMONIC> where MNEMONIC is uppercase with underscores
    # Our parsed mnemonic is xv* from the table
    
    print("\nChecking for missing instructions...")
    
    # Mapping: parsed mnemonic -> instruction
    inst_by_mnem = {inst['mnemonic']: inst for inst in instructions}
    
    # Check for each instruction from the table that should be in the header
    for inst in instructions:
        mnem = inst['mnemonic']
        # Convert mnem to the key in current_opcodes
        # mnem is xvsomething -> something is the key (xvadd_b -> add_b)
        # But in header it's OP17_XVADD_B -> key is xvadd_b
        # Wait: in header, the macro is OP17_XVADD_B, so the key after OP[0-9]+_ is XVADD_B -> lower case is xvadd_b
        # Which matches mnem which is xvadd_b
        key = mnem.lower()
        found = key in current_opcodes
        if not found:
            # Maybe the macro has different naming, check for variations
            # Check if any current key ends with this key
            found_alt = False
            for curr_key in current_opcodes:
                if curr_key.endswith(key):
                    found_alt = True
                    break
            if not found_alt:
                missing.append(inst)
        else:
            # Check if the value matches
            # Get the actual bit count from the defined macro
            actual_bits = current_opcodes[key]['bits_count']
            actual = current_opcodes[key]['value']
            
            # Calculate expected value by shifting right to get the same number of bits
            # We need to shift right by (32 - actual_bits) to get the top actual_bits bits
            expected = inst['full_value'] >> (32 - actual_bits)
            
            if expected != actual:
                expected_bin_len = actual_bits
                incorrect.append({
                    'mnemonic': mnem,
                    'expected': expected,
                    'expected_bin': f'0b{bin(expected)[2:].zfill(expected_bin_len)}',
                    'actual': actual,
                    'actual_bin': current_opcodes[key]['binary'],
                    'diff': expected ^ actual,
                })
    
    print(f"\nResults:")
    print(f"- Missing instructions in header: {len(missing)}")
    print(f"- Incorrect opcode values: {len(incorrect)}")
    
    if missing:
        print("\nMissing instructions:")
        for i, inst in enumerate(missing[:20]):
            print(f"  {i+1}. {inst['mnemonic']} - expected OP17 value: 0b{bin(inst['opcode_17'])[2:].zfill(17)} = 0x{inst['opcode_17']:x}")
        if len(missing) > 20:
            print(f"  ... and {len(missing) - 20} more")
    
    if incorrect:
        print("\nIncorrect opcode values:")
        for i, item in enumerate(incorrect):
            print(f"  {i+1}. {item['mnemonic']}:")
            print(f"    Expected: {item['expected_bin']} (0x{item['expected']:x})")
            print(f"    Actual:   {item['actual_bin']} (0x{item['actual']:x})")
    
    # Write all data to report files
    with open('lasx_instruction_report.txt', 'w') as f:
        f.write(f"Total instructions in lasx_instructions.md: {len(instructions)}\n")
        f.write(f"Total opcodes in header: {len(current_opcodes)}\n")
        f.write(f"Missing: {len(missing)}\n")
        f.write(f"Incorrect: {len(incorrect)}\n\n")
        
        if missing:
            f.write("Missing instructions:\n")
            for inst in missing:
                f.write(f"{inst['mnemonic']}: 0b{bin(inst['opcode_17'])[2:].zfill(17)} 0x{inst['opcode_17']:x}\n")
        
        if incorrect:
            f.write("\nIncorrect opcodes:\n")
            for item in incorrect:
                f.write(f"{item['mnemonic']}: expected {item['expected_bin']} got {item['actual_bin']}\n")
    
    print(f"\nFull report written to lasx_instruction_report.txt")
    
    return missing, incorrect

if __name__ == '__main__':
    main()
