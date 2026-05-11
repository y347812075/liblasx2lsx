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
import sys
from pathlib import Path


def load_opcode_definitions():
    priv_path = Path("lasx_emu_private.h")
    if not priv_path.exists():
        print("ERROR: lasx_emu_private.h not found")
        sys.exit(1)

    content = priv_path.read_text()
    opcode_defs = {}

    for match in re.finditer(r"#define (OP(\d+)_\w+)\s+(\d+)", content):
        op_name = match.group(1)
        op_num = int(match.group(2))
        op_value = int(match.group(3))
        opcode_defs[op_name] = (op_num, op_value)

    return opcode_defs


def find_switch_blocks(content):
    switches = []
    for match in re.finditer(r"switch\s*\(\s*instr\s*>>\s*(\d+)\s*\)", content):
        shift = int(match.group(1))
        start = match.end()
        brace_count = 0
        block_start = match.start()

        for i in range(start, len(content)):
            if content[i] == "{":
                brace_count += 1
            elif content[i] == "}":
                brace_count -= 1
                if brace_count == 0:
                    switches.append(
                        {
                            "shift": shift,
                            "start": block_start,
                            "end": i + 1,
                            "block": content[block_start : i + 1],
                        }
                    )
                    break

    return switches


def verify_switch_coverage():
    emu_path = Path("lasx_emu.c")
    if not emu_path.exists():
        print("ERROR: lasx_emu.c not found")
        sys.exit(1)

    content = emu_path.read_text()
    opcode_defs = load_opcode_definitions()

    print("Verifying LASX emulator switch coverage...")
    print("=" * 60)

    switches = find_switch_blocks(content)
    print(f"Found {len(switches)} switch statements\n")

    total_cases = 0
    total_wrong = 0

    for sw in switches:
        shift = sw["shift"]
        expected_op = 32 - shift
        block = sw["block"]

        cases = re.findall(r"case\s+(OP\d+_\w+):", block)

        if not cases:
            continue

        print(f"Switch: instr >> {shift} (OP{expected_op} prefix)")

        wrong = 0
        for case in cases:
            if case in opcode_defs:
                actual_op, _ = opcode_defs[case]
                if actual_op != expected_op:
                    print(f"  WRONG: {case} (OP{actual_op})")
                    wrong += 1

        if wrong > 0:
            print(f"  Errors: {wrong}\n")
            total_wrong += wrong
        else:
            print(f"  OK ({len(cases)} cases)\n")

        total_cases += len(cases)

    print("=" * 60)
    print(f"Total cases: {total_cases}")

    if total_wrong > 0:
        print(f"Total errors: {total_wrong}")
        return 1
    else:
        print("All switch structures verified correctly!")
        return 0


def check_missing_opcodes():
    opcode_defs = load_opcode_definitions()
    emu_path = Path("lasx_emu.c")
    content = emu_path.read_text()

    print("\nChecking for missing implementations...")
    print("=" * 60)

    implemented = set()
    for match in re.finditer(r"do_lasx_emu_(OP\d+_\w+)", content):
        implemented.add(match.group(1))

    missing = []
    for op in opcode_defs:
        if op not in implemented:
            missing.append(op)

    print(f"Implemented: {len(implemented)} / {len(opcode_defs)} OP codes")
    if missing:
        print(f"Note: {len(missing)} OP codes not implemented (this is normal)")
    return 0


if __name__ == "__main__":
    # Run from src/emu/ (relative to script location in tools/)
    _script_dir = Path(__file__).resolve().parent.parent / "src" / "emu"
    import os as _os
    _os.chdir(str(_script_dir))

    result1 = verify_switch_coverage()
    check_missing_opcodes()

    print("\n" + "=" * 60)
    if result1 > 0:
        print(f"ERROR: Found {result1} switch structure errors")
        sys.exit(1)
    else:
        print("PASSED: All switch structures verified correctly!")
        sys.exit(0)
