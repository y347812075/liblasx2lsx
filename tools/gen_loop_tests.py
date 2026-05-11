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

"""Generate xvmap loop test files and run comparison."""

import os, re, sys, subprocess, tempfile

TEMPLATE_3OP_S = '''.text
.globl {func}
.type {func}, @function
# {op} loop test: xvld A → xvld B → {op} C → addi → bnez
{func}:
    xvld $xr0, $a0, 0
    xvld $xr1, $a1, 0
    {op} $xr2, $xr0, $xr1
    addi.d $a0, $a0, {step}
    addi.d $a1, $a1, {step}
    addi.d $a3, $a3, -1
    bnez $a3, {func}
    xvst $xr2, $a2, 0
    jirl $zero, $ra, 0
'''

TEMPLATE_4OP_S = '''.text
.globl {func}
.type {func}, @function
{func}:
    xvld $xr0, $a0, 0
    xvld $xr1, $a1, 0
    {op} $xr2, $xr0, $xr1, $xr2
    addi.d $a0, $a0, {step}
    addi.d $a1, $a1, {step}
    addi.d $a3, $a3, -1
    bnez $a3, {func}
    xvst $xr2, $a2, 0
    jirl $zero, $ra, 0
'''

TEMPLATE_2OP_IMM_S = '''.text
.globl {func}
.type {func}, @function
{func}:
    xvld $xr0, $a0, 0
    {op} $xr1, $xr0, {imm}
    addi.d $a0, $a0, {step}
    addi.d $a3, $a3, -1
    bnez $a3, {func}
    xvst $xr1, $a2, 0
    jirl $zero, $ra, 0
'''

TEMPLATE_2OP_S = '''.text
.globl {func}
.type {func}, @function
{func}:
    xvld $xr0, $a0, 0
    {op} $xr1, $xr0
    addi.d $a0, $a0, {step}
    addi.d $a3, $a3, -1
    bnez $a3, {func}
    xvst $xr1, $a2, 0
    jirl $zero, $ra, 0
'''

TEMPLATE_C_3OP = '#include <stdio.h>\n#include <stdint.h>\n#define N 4\n' \
    '{type} A[N * {count}] __attribute__((aligned(32)));\n' \
    '{type} B[N * {count}] __attribute__((aligned(32)));\n' \
    '{type} C[{count}] __attribute__((aligned(32)));\n' \
    'extern void {func}({type} *a, {type} *b, {type} *c, int n);\n' \
    'int main() {{\n' \
    '    for (int i = 0; i < N * {count}; i++) {{ A[i] = i * 3 + 1; B[i] = i * 5 + 2; }}\n' \
    '    {func}(A, B, C, N);\n' \
    '    for (int i = 0; i < {count}; i++) printf("{fmt}\\n", C[i]);\n' \
    '    return 0;\n' \
    '}}\n'

TEMPLATE_C_2OP = '#include <stdio.h>\n#include <stdint.h>\n#define N 4\n' \
    '{type} A[N * {count}] __attribute__((aligned(32)));\n' \
    '{type} C[{count}] __attribute__((aligned(32)));\n' \
    'extern void {func}({type} *a, {type} *b, {type} *c, int n);\n' \
    'int main() {{\n' \
    '    for (int i = 0; i < N * {count}; i++) A[i] = i * 3 + 1;\n' \
    '    {func}(A, A, C, N);\n' \
    '    for (int i = 0; i < {count}; i++) printf("{fmt}\\n", C[i]);\n' \
    '    return 0;\n' \
    '}}\n'

SUFFIX_INFO = {
    '.b':   (8,  'uint8_t',  '%u', 32),  '.bu':  (8,  'uint8_t',  '%u', 32),
    '.h':   (2,  'uint16_t', '%u', 32),  '.hu':  (2,  'uint16_t', '%u', 32),
    '.w':   (1,  'uint32_t', '%u', 32),  '.wu':  (1,  'uint32_t', '%u', 32),
    '.d':   (2,  'int64_t',  '%ld', 32), '.du':  (2,  'uint64_t', '%lu', 32),
    '.s':   (1,  'float',    '%.6f', 32),
    '_v':   (2,  'uint64_t', '%lu', 32),
}

INSTRUCTIONS = {}

def parse_instructions():
    with open('src/interpret/lasx_interpret_opt_gen_xvmap.c') as f:
        content = f.read()
    for m in re.finditer(r'GEN_XVMAP_3OP\((\w+),\s*(\w+)\)', content):
        name = m.group(1)
        if name.startswith(('xv', 'vext')): INSTRUCTIONS[name] = ('3op', m.group(2))
    for m in re.finditer(r'GEN_XVMAP_4OP\((\w+),\s*(\w+)\)', content):
        name = m.group(1)
        if name.startswith(('xv', 'vext')): INSTRUCTIONS[name] = ('4op', m.group(2))
    for m in re.finditer(r'GEN_XVMAP_2OP_IMM\((\w+),\s*(\w+),\s*(\d+),\s*(\d+)\)', content):
        name = m.group(1)
        if name.startswith(('xv', 'vext')): INSTRUCTIONS[name] = ('2op_imm', m.group(2), int(m.group(3)), int(m.group(4)))
    for m in re.finditer(r'GEN_XVMAP_2OP\((\w+),\s*(\w+)\)', content):
        name = m.group(1)
        if name.startswith(('xv', 'vext')) and name not in INSTRUCTIONS:
            INSTRUCTIONS[name] = ('2op', m.group(2))

def guess_info(name):
    for s, info in SUFFIX_INFO.items():
        if name.endswith(s):
            return info
    # Also try with underscore prefix (e.g., xvadd_w, xvadd_d)
    for s, info in SUFFIX_INFO.items():
        if s.startswith('.') and name.endswith('_' + s[1:]):
            return info
    return (1, 'uint32_t', '%u', 32)

SKIP = {'xvfcmp_cle_s', 'xvfcmp_clt_s', 'xvmulwod_w_h', 'xvmulwev_w_h',
        'vext2xv_h_b', 'vext2xv_w_h', 'vext2xv_wu_bu'}

def gen(name, info):
    pattern = info[0]
    si = guess_info(name)
    if not si: return None
    esize, ctype, fmt, step = si
    func = f"run_{name}"
    op = name.replace('_', '.')

    if pattern in ('3op', '4op'):
        tmpl = TEMPLATE_3OP_S if pattern == '3op' else TEMPLATE_4OP_S
        with open(f'tests/test_xvmap_{name}.s', 'w') as f:
            f.write(tmpl.format(func=func, op=op, step=step))
        with open(f'tests/test_xvmap_{name}.c', 'w') as f:
            f.write(TEMPLATE_C_3OP.format(type=ctype, count=esize*8, func=func, fmt=fmt))
    elif pattern == '2op_imm':
        with open(f'tests/test_xvmap_{name}.s', 'w') as f:
            f.write(TEMPLATE_2OP_IMM_S.format(func=func, op=op, step=step, imm=3))
        with open(f'tests/test_xvmap_{name}.c', 'w') as f:
            f.write(TEMPLATE_C_2OP.format(type=ctype, count=esize*8, func=func, fmt=fmt))
    elif pattern == '2op':
        with open(f'tests/test_xvmap_{name}.s', 'w') as f:
            f.write(TEMPLATE_2OP_S.format(func=func, op=op, step=step))
        with open(f'tests/test_xvmap_{name}.c', 'w') as f:
            f.write(TEMPLATE_C_2OP.format(type=ctype, count=esize*8, func=func, fmt=fmt))
    return name

def run_test(name, qemu, preload, mode):
    """Build and run one test. Returns (stdout, ok)."""
    bin_name = f"test_xvmap_{name}"
    asm_obj = f"tests/test_xvmap_{name}.o"

    # assemble
    r = subprocess.run([AS, '-o', asm_obj, f'tests/test_xvmap_{name}.s'],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return r.stderr, False

    # compile
    r = subprocess.run([CC, '-mlsx', '-mfrecipe', '-O2', '-o', bin_name,
                        f'tests/test_xvmap_{name}.c', asm_obj, '-lm'],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return r.stderr, False

    # run
    env = {'LD_PRELOAD': preload} if preload else {}
    if mode:
        env['LIBLASX2LSX_INTERPRET_MODE'] = mode
    cmd = [qemu, '-cpu', 'la464,lasx=off' if preload else 'la464', f'./{bin_name}']
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, timeout=30)
    return r.stdout, r.returncode == 0

AS = 'loongarch64-unknown-linux-gnu-as'
CC = 'loongarch64-unknown-linux-gnu-gcc'
QEMU = 'qemu-loongarch64-static'
LIB = './liblasx2lsx.so'

def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)) + '/..')
    parse_instructions()

    args = sys.argv[1:]
    gen_only = '--gen-only' in args
    test_list = [a for a in args if not a.startswith('--')]

    if not test_list:
        test_list = sorted(INSTRUCTIONS.keys())

    generated = []
    for name in test_list:
        if name in SKIP: continue
        info = INSTRUCTIONS.get(name)
        if not info: continue
        r = gen(name, info)
        if r: generated.append(r)

    print(f"Generated {len(generated)} tests.")

    if gen_only:
        return

    # Build lib first
    subprocess.run(['make', '-j4'], capture_output=True)

    passed, failed = 0, 0
    for name in generated:
        # Run in block mode (reference) and blockloop mode (test)
        ref_out, ref_ok = run_test(name, qemu=QEMU, preload=LIB, mode='block')
        tst_out, tst_ok = run_test(name, qemu=QEMU, preload=LIB, mode='blockloop')

        if not ref_ok or not tst_ok:
            print(f"FAIL {name}: run error")
            failed += 1
            continue

        if ref_out == tst_out:
            print(f"PASS {name}")
            passed += 1
        else:
            print(f"FAIL {name}: mismatch")
            # Print diff summary
            ref_lines = ref_out.strip().split('\n')
            tst_lines = tst_out.strip().split('\n')
            for i, (r, t) in enumerate(zip(ref_lines, tst_lines)):
                if r != t:
                    print(f"  [{i}] block={r} blockloop={t}")
            failed += 1

    print(f"\nResults: {passed} passed, {failed} failed")

if __name__ == '__main__':
    main()
