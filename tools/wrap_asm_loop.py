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

"""Wrap each __asm__ volatile block in random_test/src/*.c
with for(int _xv=0;_xv<1;_xv++){} to make them detectable
by xvmap loop JIT. Single iteration preserves semantics."""

import re, os, glob

SRC_DIR = 'random_test/src'

def wrap_file(path):
    with open(path) as f:
        content = f.read()

    changed = False

    # Handle );}  );\n}  ); return res; }  etc.
    new = re.sub(
        r'^([ \t]*)__asm__\s+volatile\((.+?)\);(.*?\})',
        r'\1for (volatile int _xv = 0; _xv < 1; _xv++) {\n'
        r'\1    __asm__ volatile(\2);\n'
        r'\1}\3',
        content, flags=re.DOTALL | re.MULTILINE
    )

    # Replace in-place after successful regex (avoid double-wrapping)
    new = re.sub(
        r'^([ \t]*)for \(int _xv = 0; _xv < 1; _xv\+\+\) \{',
        r'\1for (volatile int _xv = 0; _xv < 1; _xv++) {',
        new, flags=re.MULTILINE
    )

    if new != content:
        changed = True
        with open(path, 'w') as f:
            f.write(new)

    return changed

def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)) + '/..')
    files = sorted(glob.glob(f'{SRC_DIR}/*.c'))
    changed = 0
    for f in files:
        if wrap_file(f):
            changed += 1
    print(f'Wrapped {changed}/{len(files)} files')

if __name__ == '__main__':
    main()
