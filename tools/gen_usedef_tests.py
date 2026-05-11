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

"""Generate usedef batch test files from JIT log files.

Usage:
  python3 tools/gen_usedef_tests.py --gen           # generate C test files
  python3 tools/gen_usedef_tests.py --build         # compile test files
  python3 tools/gen_usedef_tests.py --gen-expected  # generate block-mode expected results
  python3 tools/gen_usedef_tests.py --test          # run usedef and compare
  python3 tools/gen_usedef_tests.py --all           # do all four steps
  python3 tools/gen_usedef_tests.py --random [N]  # test with N random SEEDs (default 100), stop on first failure
  python3 tools/gen_usedef_tests.py --log-dir DIR   # custom log directory
"""

import re
import os
import sys
import glob
import platform
import argparse
import subprocess
import concurrent.futures
import multiprocessing
import shutil
import tempfile

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

LOG_DIR = "tests/usedef"
GEN_DIR = "tests/usedef/gen"
RESULT_DIR = "tests/usedef/result"
_host = platform.machine()
if _host == 'loongarch64':
    CC = "gcc"
    QEMU = None
else:
    CC = "loongarch64-unknown-linux-gnu-gcc"
    QEMU = "qemu-loongarch64-static"
CFLAGS = "-O0 -mlasx"
LIB = "./liblasx2lsx.so"

DOLLAR = chr(0x24)
RX_GPR = re.compile(r'[' + DOLLAR + r'](\w+)')
RX_XR  = re.compile(r'[' + DOLLAR + r']xr(\d+)')
BUF_SIZE = 32 * 32 * 2  # 32 xr * 32 bytes * 2 (extra margin)

MEM_INSTRS = ('xvld ', 'xvst ', 'xvldx ', 'xvstx ', 'xvldrepl.', 'xvstelm.', 'xvldi ')

# ---------------------------------------------------------------------------
# Log parsing
# ---------------------------------------------------------------------------

class Instr:
    def __init__(self, hexword, asm_text):
        self.hex = hexword
        self.asm = asm_text

class Batch:
    def __init__(self, mode, pc, instrs):
        self.mode = mode
        self.pc = pc
        self.instrs = instrs

    def has_memory_ops(self):
        for ins in self.instrs:
            for mop in MEM_INSTRS:
                if mop in ins.asm:
                    return True
        return False

    def has_gpr_refs(self):
        for ins in self.instrs:
            for m in RX_GPR.finditer(ins.asm):
                name = m.group(1)
                if not re.match(r'xr\d+', name) and not re.match(r'vr\d+', name):
                    return True
        return False

    _GPR_MAP = {
        'zero': 0, 'ra': 1, 'tp': 2, 'sp': 3,
        'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
        'a4': 8, 'a5': 9, 'a6': 10, 'a7': 11,
        't0': 12, 't1': 13, 't2': 14, 't3': 15,
        't4': 16, 't5': 17, 't6': 18, 't7': 19, 't8': 20,
        'x': 21, 'fp': 22,
        's0': 23, 's1': 24, 's2': 25, 's3': 26,
        's4': 27, 's5': 28, 's6': 29, 's7': 30, 's8': 31,
    }
    for _i in range(32):
        _GPR_MAP[f'r{_i}'] = _i

    def gpr_ops(self):
        gprs = set()
        for ins in self.instrs:
            for m in RX_GPR.finditer(ins.asm):
                name = m.group(1)
                if re.match(r'xr\d+', name) or re.match(r'vr\d+', name):
                    continue
                if name in self._GPR_MAP:
                    gprs.add(self._GPR_MAP[name])
        return sorted(gprs)

    def xr_clobbers(self):
        xrs = set()
        for ins in self.instrs:
            for m in RX_XR.finditer(ins.asm):
                xrs.add(int(m.group(1)))
        return [f'xr{i}' for i in sorted(xrs)]


def parse_log_file(filepath):
    with open(filepath) as f:
        content = f.read()
    batches = []
    batch_re = re.compile(
        r'=== batch \d+ instrs mode=(\S+) PC=([0-9a-f]+) ===\n'
        r'(.*?)(?=\n=== |\Z)', re.S)
    for m in batch_re.finditer(content):
        mode = m.group(1)
        pc = m.group(2)
        block = m.group(3).strip()
        instrs = []
        for line in block.split('\n'):
            line = line.strip()
            if not line:
                continue
            im = re.match(r'[0-9a-f]+:\s+([0-9a-f]+)\s+(.*)', line)
            if im:
                instrs.append(Instr(im.group(1), im.group(2)))
        if instrs:
            batches.append(Batch(mode, pc, instrs))
    return batches


def discover_logs(log_dir):
    result = []
    for f in sorted(glob.glob(os.path.join(log_dir, "*.log"))):
        name = os.path.basename(f).replace('.log', '')
        safe_name = "test_usedef_" + re.sub(r'\W', '_', name)
        result.append((f, safe_name))
    return result


# ---------------------------------------------------------------------------
# C file generation
# ---------------------------------------------------------------------------

def generate_c_file(batches, test_name, seed=0):
    """Generate C test file. Skips batches with xvld/xvst (need GPR setup)."""
    lines = []
    lines.append('/* Auto-generated by tools/gen_usedef_tests.py */')
    lines.append('#include <stdio.h>')
    lines.append('#include <stdlib.h>')
    lines.append('#include <stdint.h>')
    lines.append('#include <inttypes.h>')
    lines.append('')
    lines.append(f'static uint64_t buf[{BUF_SIZE // 8}] __attribute__((aligned(256)));')
    lines.append('')
    lines.append('static uint64_t lcg(uint64_t *s) {')
    lines.append('    *s = *s * 6364136223846793005ULL + 1442695040888963407ULL;')
    lines.append('    return *s;')
    lines.append('}')
    lines.append('')

    # Filter: keep only pure-computation batches (no xvld/xvst/etc)
    kept = [(bi, b) for bi, b in enumerate(batches) if not b.has_memory_ops()]

    for bi, batch in kept:
        clobber = batch.xr_clobbers()
        cp = ['"memory"'] + [f'"{DOLLAR}{xr}"' for xr in clobber]
        clobber_str = ', '.join(cp)

        lines.append(f'__attribute__((noinline))')
        lines.append(f'static uint64_t batch{bi}(void) {{')
        # Fill buf with deterministic random data using seed-offset
        lines.append(f'    {{ uint64_t s = {seed} + {bi};')
        lines.append(f'      for (int _z = 0; _z < (int)(sizeof(buf)/sizeof(buf[0])); _z++)')
        lines.append(f'          buf[_z] = lcg(&s); }}')
        # nop before
        lines.append('    __asm__ volatile(".word 0x03400000\\n" ::: );')
        # Load random data into all xr from buf (no xr clobbers — sequential asm is safe)
        lines.append('    __asm__ volatile(')
        for i in range(32):
            lines.append(f'        "xvld $xr{i}, %[ptr], {i * 32}\\n"')
        lines.append(f'        :: [ptr] "r"(buf) : "memory");')
        # nop separator
        lines.append('    __asm__ volatile(".word 0x03400000\\n" ::: );')
        # Merge GPR loading and batch instructions into one asm block
        gprs = batch.gpr_ops()
        all_clobber = ['"memory"'] + [f'"{DOLLAR}{xr}"' for xr in clobber]
        lines.append('    __asm__ volatile(')
        for gn in gprs:
            lines.append(f'        "ld.d $r{gn}, %[ptr], {1024 + gn * 8}\\n"')
        for ins in batch.instrs:
            lines.append(f'        ".word 0x{ins.hex}\\n"')
        for gn in gprs:
            all_clobber.append(f'"{DOLLAR}r{gn}"')
        all_clobber_str = ', '.join(all_clobber)
        lines.append(f'        :: [ptr] "r"(buf) : {all_clobber_str});')
        # nop between batch and store
        lines.append('    __asm__ volatile(".word 0x03400000\\n" ::: );')
        # Store all xr back to buf (capture results, no xr clobbers)
        lines.append('    __asm__ volatile(')
        for i in range(32):
            lines.append(f'        "xvst $xr{i}, %[ptr], {i * 32}\\n"')
        lines.append(f'        :: [ptr] "r"(buf) : "memory");')
        # nop after
        lines.append('    __asm__ volatile(".word 0x03400000\\n" ::: );')
        # Hash buf to produce batch output
        lines.append('    uint64_t h = 0;')
        lines.append('    for (int _z = 0; _z < 32 * 4; _z++)')  # 32 xr × 4 uint64 each
        lines.append('        h = h * 31 + buf[_z];')
        lines.append('    return h;')
        lines.append('}')
        lines.append('')

    lines.append('int main(int argc, char *argv[]) {')
    lines.append('    (void)argc; (void)argv;')
    for bi, batch in kept:
        lines.append(f'    printf("batch{bi}:%016lx\\n", batch{bi}());')
    lines.append('    return 0;')
    lines.append('}')
    return '\n'.join(lines) + '\n'


# ---------------------------------------------------------------------------
# Build / test
# ---------------------------------------------------------------------------

def build_test(test_name, gen_dir):
    src = os.path.join(gen_dir, test_name + '.c')
    out = os.path.join(gen_dir, test_name)
    cmd = [CC] + CFLAGS.split() + ['-o', out, src]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        err = r.stderr.strip()[:300]
        print(f"    FAIL: {err}")
        return False
    return True


def _run(test_bin, mode, timeout=60):
    env = os.environ.copy()
    env['LD_PRELOAD'] = LIB
    env['LIBLASX2LSX_INTERPRET_MODE'] = mode
    try:
        cmd = [QEMU, '-cpu', 'la464,lasx=off', test_bin] if QEMU else [test_bin]
        r = subprocess.run(cmd, env=env, capture_output=True, text=True, timeout=60)
        if r.returncode != 0:
            return None
        return r.stdout.strip()
    except Exception:
        return None


def gen_expected(test_name, gen_dir, result_dir):
    """Generate expected output using native QEMU (no emulator)."""
    test_bin = os.path.join(gen_dir, test_name)
    result_file = os.path.join(result_dir, test_name + '.native.result')
    if not os.path.exists(test_bin):
        print(f"  {test_name}: binary not found, skipping")
        return False
    try:
        cmd = [QEMU, test_bin] if QEMU else [test_bin]
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
        if r.returncode != 0:
            print(f"    RUN FAIL (rc={r.returncode})")
            return False
        out = r.stdout.strip()
    except Exception as e:
        print(f"    ERROR: {e}")
        return False
    with open(result_file, 'w') as f:
        f.write(out + '\n')
    print(f"  {test_name}: {len(out.splitlines())} lines -> {result_file}")
    return True


def run_test(test_name, gen_dir, result_dir):
    """Run test with usedef and block modes, compare against native."""
    test_bin = os.path.join(gen_dir, test_name)
    result_file = os.path.join(result_dir, test_name + '.native.result')
    if not os.path.exists(test_bin) or not os.path.exists(result_file):
        print(f"  {test_name}: binary or result missing, skipping")
        return False
    with open(result_file) as f:
        expected = f.read().strip()

    ok = True
    for mode in ('block', 'usedef'):
        out = _run(test_bin, mode)
        if out is None:
            print(f"  {test_name}: {mode} FAIL (crash)")
            ok = False
        elif out != expected:
            print(f"  {test_name}: {mode} MISMATCH")
            elines = expected.splitlines()
            olines = out.splitlines()
            for i, (e, o) in enumerate(zip(elines, olines)):
                if e != o:
                    print(f"    line {i}: native={e[:50]}... {mode}={o[:50]}...")
                    break
            ok = False
        else:
            print(f"  {test_name}: {mode} PASS")
    return ok


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description='Generate and test usedef batch tests')
    parser.add_argument('--gen', action='store_true')
    parser.add_argument('--build', action='store_true')
    parser.add_argument('--gen-expected', action='store_true')
    parser.add_argument('--test', action='store_true')
    parser.add_argument('--all', action='store_true')
    parser.add_argument('--random', type=int, nargs='?', const=100, default=0,
                        help='random test with N SEEDs (default 100), stop on first failure')
    parser.add_argument('--jobs', type=int, default=0,
                        help='parallel jobs for --random (default: CPU count)')
    parser.add_argument('--log-dir', default=LOG_DIR)
    args = parser.parse_args()

    os.makedirs(GEN_DIR, exist_ok=True)
    os.makedirs(RESULT_DIR, exist_ok=True)

    if args.all:
        args.gen = args.build = args.gen_expected = args.test = True

    log_entries = discover_logs(args.log_dir)
    if not log_entries:
        print(f"No log files found in {args.log_dir}")
        return 1

    names = [e[1] for e in log_entries]
    print(f"Found {len(log_entries)} log files: {names}")

    if args.gen:
        print("\n=== Generate C files ===")
        for log_path, test_name in log_entries:
            src = os.path.join(GEN_DIR, test_name + '.c')
            batches = parse_log_file(log_path)
            content = generate_c_file(batches, test_name, seed=0)
            n_kept = content.count('__attribute__')
            with open(src, 'w') as f:
                f.write(content)
            print(f"  {test_name}: {len(batches)} batches -> {n_kept} kept")

    if args.build:
        print("\n=== Build ===")
        ok = sum(1 for _, n in log_entries if build_test(n, GEN_DIR))
        print(f"  {ok}/{len(log_entries)} compiled")

    if args.gen_expected:
        print("\n=== Generate expected results (block mode) ===")
        ok = sum(1 for _, n in log_entries if gen_expected(n, GEN_DIR, RESULT_DIR))
        print(f"  {ok}/{len(log_entries)} generated")

    if args.test:
        print("\n=== Test (usedef vs block) ===")
        ok = fail = 0
        for _, n in log_entries:
            if run_test(n, GEN_DIR, RESULT_DIR):
                ok += 1
            else:
                fail += 1
        print(f"  {ok} passed, {fail} failed")
        if fail:
            return 1

    if args.random:
        n = args.random
        njobs = args.jobs if args.jobs > 0 else multiprocessing.cpu_count()
        random_dir = os.path.join(LOG_DIR, 'random')
        os.makedirs(random_dir, exist_ok=True)
        print(f"\n=== Random test with {n} SEEDs ({njobs} workers) ===")

        def run_one_seed(_seed):
            """Test one seed. Returns (seed, ok, failures) where failures is a list of strings."""
            sd = os.path.join(random_dir, f'SEED_{_seed:04d}')
            os.makedirs(sd, exist_ok=True)
            fails = []
            for log_path, test_name in log_entries:
                src = os.path.join(sd, test_name + '.c')
                test_bin = os.path.join(sd, test_name)

                batches = parse_log_file(log_path)
                content = generate_c_file(batches, test_name, seed=_seed)
                with open(src, 'w') as f:
                    f.write(content)

                if not build_test(test_name, sd):
                    fails.append(f'{test_name}: BUILD FAIL')
                    break

                # Native (QEMU or direct)
                try:
                    cmd = [QEMU, test_bin] if QEMU else [test_bin]
                    r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
                    if r.returncode != 0:
                        fails.append(f'{test_name}: native RUN FAIL (rc={r.returncode})')
                        break
                    native_out = r.stdout.strip()
                except Exception as e:
                    fails.append(f'{test_name}: native ERROR {e}')
                    break

                # Block mode
                block_out = _run(test_bin, 'block')
                if block_out is None:
                    fails.append(f'{test_name}: block CRASH')
                    break
                if block_out != native_out:
                    for e, o in zip(native_out.splitlines(), block_out.splitlines()):
                        if e != o:
                            fails.append(f'{test_name}: block MISMATCH\n    native={e}\n    block={o}')
                            break
                    break

                # Usedef mode
                usedef_out = _run(test_bin, 'usedef')
                if usedef_out is None:
                    fails.append(f'{test_name}: usedef CRASH')
                    break
                if usedef_out != native_out:
                    for e, u in zip(native_out.splitlines(), usedef_out.splitlines()):
                        if e != u:
                            fails.append(f'{test_name}: usedef MISMATCH\n    native={e}\n    usedef={u}')
                            break
                    break
            return (_seed, len(fails) == 0, fails)

        failed_seeds = []
        with concurrent.futures.ThreadPoolExecutor(max_workers=njobs) as pool:
            fut = {pool.submit(run_one_seed, s): s for s in range(1, n + 1)}
            done = 0
            for f in concurrent.futures.as_completed(fut):
                seed, ok, fails = f.result()
                done += 1
                pct = done * 100 // n
                if ok:
                    print(f"\r  [{pct}%] SEED={seed}: OK", end='', flush=True)
                else:
                    print(f"\n  FAILED SEED={seed}")
                    for msg in fails:
                        print(f"    {msg}")
                    failed_seeds.append(seed)
                    # Cancel remaining
                    for ff in fut:
                        ff.cancel()
                    break

        if failed_seeds:
            print(f"\n  FAILED at SEEDs: {failed_seeds}")
            return 1
        print(f"\n  All {n} SEEDs passed")
        return 0

    return 0


if __name__ == '__main__':
    sys.exit(main())
