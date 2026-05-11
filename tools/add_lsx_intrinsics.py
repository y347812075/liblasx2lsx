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

LSX_INTRINSICS = {
    "xvfadd_s": ("__lsx_vfadd_s", "__m128", False),
    "xvfadd_d": ("__lsx_vfadd_d", "__m128d", False),
    "xvfsub_s": ("__lsx_vfsub_s", "__m128", False),
    "xvfsub_d": ("__lsx_vfsub_d", "__m128d", False),
    "xvfmul_s": ("__lsx_vfmul_s", "__m128", False),
    "xvfmul_d": ("__lsx_vfmul_d", "__m128d", False),
    "xvfdiv_s": ("__lsx_vfdiv_s", "__m128", False),
    "xvfdiv_d": ("__lsx_vfdiv_d", "__m128d", False),
    "xvfmax_s": ("__lsx_vfmax_s", "__m128", False),
    "xvfmax_d": ("__lsx_vfmax_d", "__m128d", False),
    "xvfmin_s": ("__lsx_vfmin_s", "__m128", False),
    "xvfmin_d": ("__lsx_vfmin_d", "__m128d", False),
    "xvfmaxa_s": ("__lsx_vfmaxa_s", "__m128", False),
    "xvfmaxa_d": ("__lsx_vfmaxa_d", "__m128d", False),
    "xvfmina_s": ("__lsx_vfmina_s", "__m128", False),
    "xvfmina_d": ("__lsx_vfmina_d", "__m128d", False),
    "xvfsqrt_s": ("__lsx_vfsqrt_s", "__m128", True),
    "xvfsqrt_d": ("__lsx_vfsqrt_d", "__m128d", True),
    "xvfrecip_s": ("__lsx_vfrecip_s", "__m128", True),
    "xvfrecip_d": ("__lsx_vfrecip_d", "__m128d", True),
    "xvfrsqrt_s": ("__lsx_vfrsqrt_s", "__m128", True),
    "xvfrsqrt_d": ("__lsx_vfrsqrt_d", "__m128d", True),
    "xvflogb_s": ("__lsx_vflogb_s", "__m128", True),
    "xvflogb_d": ("__lsx_vflogb_d", "__m128d", True),
    "xvfclass_s": ("__lsx_vfclass_s", "__m128", True),
    "xvfclass_d": ("__lsx_vfclass_d", "__m128d", True),
    "xvfrint_s": ("__lsx_vfrint_s", "__m128", True),
    "xvfrint_d": ("__lsx_vfrint_d", "__m128d", True),
    "xvfrintrm_s": ("__lsx_vfrintrm_s", "__m128", True),
    "xvfrintrm_d": ("__lsx_vfrintrm_d", "__m128d", True),
    "xvfrintrp_s": ("__lsx_vfrintrp_s", "__m128", True),
    "xvfrintrp_d": ("__lsx_vfrintrp_d", "__m128d", True),
    "xvfrintrz_s": ("__lsx_vfrintrz_s", "__m128", True),
    "xvfrintrz_d": ("__lsx_vfrintrz_d", "__m128d", True),
    "xvfrintrne_s": ("__lsx_vfrintrne_s", "__m128", True),
    "xvfrintrne_d": ("__lsx_vfrintrne_d", "__m128d", True),
    "xvfcvt_h_s": ("__lsx_vfcvt_h_s", "__m128", False),
    "xvfcvt_s_d": ("__lsx_vfcvt_s_d", "__m128d", False),
    "xvfcvtl_s_h": ("__lsx_vfcvtl_s_h", "__m128i", True),
    "xvfcvth_s_h": ("__lsx_vfcvth_s_h", "__m128i", True),
    "xvfcvtl_d_s": ("__lsx_vfcvtl_d_s", "__m128", True),
    "xvfcvth_d_s": ("__lsx_vfcvth_d_s", "__m128", True),
    "xvftint_lu_d": ("__lsx_vftint_lu_d", "__m128d", True),
    "xvftint_w_d": ("__lsx_vftint_w_d", "__m128d", False),
    "xvftintrm_w_d": ("__lsx_vftintrm_w_d", "__m128d", False),
    "xvftintrp_w_d": ("__lsx_vftintrp_w_d", "__m128d", False),
    "xvftintrz_w_d": ("__lsx_vftintrz_w_d", "__m128d", False),
    "xvftintrne_w_d": ("__lsx_vftintrne_w_d", "__m128d", False),
    "xvftint_wu_s": ("__lsx_vftint_wu_s", "__m128", True),
    "xvftint_w_s": ("__lsx_vftint_w_s", "__m128", True),
    "xvftintrm_w_s": ("__lsx_vftintrm_w_s", "__m128", True),
    "xvftintrp_w_s": ("__lsx_vftintrp_w_s", "__m128", True),
    "xvftintrz_w_s": ("__lsx_vftintrz_w_s", "__m128", True),
    "xvftintrne_w_s": ("__lsx_vftintrne_w_s", "__m128", True),
    "xvftint_l_d": ("__lsx_vftint_l_d", "__m128d", True),
    "xvftintrm_l_d": ("__lsx_vftintrm_l_d", "__m128d", True),
    "xvftintrp_l_d": ("__lsx_vftintrp_l_d", "__m128d", True),
    "xvftintrz_l_d": ("__lsx_vftintrz_l_d", "__m128d", True),
    "xvftintrne_l_d": ("__lsx_vftintrne_l_d", "__m128d", True),
    "xvffint_s_l": ("__lsx_vffint_s_l", "__m128i", False),
    "xvffint_s_w": ("__lsx_vffint_s_w", "__m128i", True),
    "xvffint_s_wu": ("__lsx_vffint_s_wu", "__m128i", True),
    "xvffint_d_l": ("__lsx_vffint_d_l", "__m128i", True),
    "xvffint_d_lu": ("__lsx_vffint_d_lu", "__m128i", True),
    "xvffintl_d_w": ("__lsx_vffintl_d_w", "__m128i", True),
    "xvffinth_d_w": ("__lsx_vffinth_d_w", "__m128i", True),
    "xvfrecipe_s": ("__lsx_vfrecipe_s", "__m128", True),
    "xvfrecipe_d": ("__lsx_vfrecipe_d", "__m128d", True),
    "xvfrsqrte_s": ("__lsx_vfrsqrte_s", "__m128", True),
    "xvfrsqrte_d": ("__lsx_vfrsqrte_d", "__m128d", True),
}


def gen_code(name, intrinsic, vtype, unary):
    if "_d" in name:
        if unary:
            return f"""    if (td->use_lsx_intrinsics) {{
        for (int i = 0; i < 2; i++) {{
            __m128d xj_lo = (__m128d){{vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)}};
            {vtype} result = {intrinsic}(xj_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }}
        return;
    }}
"""
        else:
            return f"""    if (td->use_lsx_intrinsics) {{
        for (int i = 0; i < 2; i++) {{
            __m128d xj_lo = (__m128d){{vreg_read_f64(uc, td, xj, i * 2), vreg_read_f64(uc, td, xj, i * 2 + 1)}};
            __m128d xk_lo = (__m128d){{vreg_read_f64(uc, td, xk, i * 2), vreg_read_f64(uc, td, xk, i * 2 + 1)}};
            {vtype} result = {intrinsic}(xj_lo, xk_lo);
            double* r = (double*)&result;
            vreg_write_f64(uc, td, xd, i * 2, r[0]);
            vreg_write_f64(uc, td, xd, i * 2 + 1, r[1]);
        }}
        return;
    }}
"""
    else:
        if unary:
            return f"""    if (td->use_lsx_intrinsics) {{
        for (int i = 0; i < 2; i++) {{
            __m128 xj_lo = (__m128){{vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}};
            {vtype} result = {intrinsic}(xj_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }}
        return;
    }}
"""
        else:
            return f"""    if (td->use_lsx_intrinsics) {{
        for (int i = 0; i < 2; i++) {{
            __m128 xj_lo = (__m128){{vreg_read_f32(uc, td, xj, i * 4), vreg_read_f32(uc, td, xj, i * 4 + 1), vreg_read_f32(uc, td, xj, i * 4 + 2), vreg_read_f32(uc, td, xj, i * 4 + 3)}};
            __m128 xk_lo = (__m128){{vreg_read_f32(uc, td, xk, i * 4), vreg_read_f32(uc, td, xk, i * 4 + 1), vreg_read_f32(uc, td, xk, i * 4 + 2), vreg_read_f32(uc, td, xk, i * 4 + 3)}};
            {vtype} result = {intrinsic}(xj_lo, xk_lo);
            float* r = (float*)&result;
            vreg_write_f32(uc, td, xd, i * 4, r[0]);
            vreg_write_f32(uc, td, xd, i * 4 + 1, r[1]);
            vreg_write_f32(uc, td, xd, i * 4 + 2, r[2]);
            vreg_write_f32(uc, td, xd, i * 4 + 3, r[3]);
        }}
        return;
    }}
"""


def process_file(filepath):
    with open(filepath, "r") as f:
        content = f.read()

    lines = content.split("\n")
    new_lines = []
    i = 0
    modified = 0

    while i < len(lines):
        line = lines[i]
        new_lines.append(line)

        m = re.match(r"void do_lasx_emu_(xv\w+)\(ucontext_t", line)
        if m:
            func_name = m.group(1)
            if func_name in LSX_INTRINSICS:
                search_lines = lines[i : i + 20]
                has_intrinsics = any(
                    "if (td->use_lsx_intrinsics)" in l for l in search_lines
                )

                if not has_intrinsics:
                    code = gen_code(
                        func_name,
                        LSX_INTRINSICS[func_name][0],
                        LSX_INTRINSICS[func_name][1],
                        LSX_INTRINSICS[func_name][2],
                    )

                    insert_idx = None
                    seen_open_brace = False
                    for j in range(i + 1, min(i + 25, len(lines))):
                        stripped = lines[j].strip()
                        if stripped == "{":
                            seen_open_brace = True
                            continue
                        if not seen_open_brace:
                            continue
                        if (
                            stripped.startswith("int xd")
                            or stripped.startswith("int xj")
                            or stripped.startswith("int xk")
                        ):
                            continue
                        if (
                            stripped == ""
                            or stripped.startswith("//")
                            or stripped.startswith("thread_data_t")
                        ):
                            continue
                        if stripped.startswith("tdlog"):
                            continue
                        insert_idx = j
                        break

                    if insert_idx is not None:
                        code_lines = code.split("\n")
                        for code_line in code_lines:
                            new_lines.append(code_line)
                        modified += 1
                        print(f"Added intrinsics for {func_name}")

        i += 1

    with open(filepath, "w") as f:
        f.write("\n".join(new_lines))

    print(f"\nTotal modifications: {modified}")


if __name__ == "__main__":
    process_file("src/emu/lasx_fbasic.c")
