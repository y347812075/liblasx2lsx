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

#include <inttypes.h>
#include <lasxintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_data(uint8_t* a, int size)
{
    for (int i = 0; i < size; i++)
        a[i] = i;
}

int check_h(uint16_t* result, int count)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        int8_t src_val = (int8_t)(i * 2);
        int16_t expected = (int16_t)src_val;
        if (result[i] != (uint16_t)expected) {
            printf("H[%d]: got %u, expected %u\n", i, result[i], (uint16_t)expected);
            errors++;
        }
    }
    return errors;
}

int check_w(uint32_t* result, int count)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        int16_t src_val = (int16_t)(i * 2);
        int32_t expected = (int32_t)src_val;
        if (result[i] != (uint32_t)expected) {
            printf("W[%d]: got %u, expected %u\n", i, result[i], (uint32_t)expected);
            errors++;
        }
    }
    return errors;
}

int check_d(uint64_t* result, int count)
{
    int errors = 0;
    for (int i = 0; i < count; i++) {
        int32_t src_val = (int32_t)(i * 2);
        int64_t expected = (int64_t)src_val;
        if (result[i] != (uint64_t)expected) {
            printf("D[%d]: got %" PRIu64 ", expected %" PRIu64 "\n", i, result[i], (uint64_t)expected);
            errors++;
        }
    }
    return errors;
}

int main()
{
    int errors = 0;
    unsigned char mem[64] __attribute__((aligned(32)));

    memset(mem, 0, sizeof(mem));
    init_data((uint8_t*)mem, 32);
    __m256i v = __lasx_xvld(mem, 0);
    __m256i r = __lasx_xvexth_h_b(v);
    __lasx_xvst(r, mem, 32);
    errors += check_h((uint16_t*)(mem + 32), 16);
    printf("xvexth.b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data((uint8_t*)mem, 32);
    v = __lasx_xvld(mem, 0);
    r = __lasx_xvexth_w_h(v);
    __lasx_xvst(r, mem, 32);
    errors += check_w((uint32_t*)(mem + 32), 8);
    printf("xvexth.h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data((uint8_t*)mem, 32);
    v = __lasx_xvld(mem, 0);
    r = __lasx_xvexth_d_w(v);
    __lasx_xvst(r, mem, 32);
    errors += check_d((uint64_t*)(mem + 32), 4);
    printf("xvexth.w: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data((uint8_t*)mem, 32);
    v = __lasx_xvld(mem, 0);
    r = __lasx_xvexth_q_d(v);
    __lasx_xvst(r, mem, 32);
    printf("xvexth.d: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data((uint8_t*)mem, 32);
    v = __lasx_xvld(mem, 0);
    r = __lasx_xvextl_q_d(v);
    __lasx_xvst(r, mem, 32);
    errors += check_d((uint64_t*)(mem + 32), 4);
    printf("xvextl.w: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    init_data((uint8_t*)mem, 32);
    v = __lasx_xvld(mem, 0);
    r = __lasx_xvextl_qu_du(v);
    __lasx_xvst(r, mem, 32);
    printf("xvextl.d: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    uint64_t test_gpr = 0x0102030405060708ULL;
    v = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_vext2xv_h_b(v);
    __lasx_xvst(r, mem, 32);
    printf("vext2xv.h_b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_vext2xv_w_b(v);
    __lasx_xvst(r, mem, 32);
    printf("vext2xv.w_b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_vext2xv_d_b(v);
    __lasx_xvst(r, mem, 32);
    printf("vext2xv.d_b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_vext2xv_w_h(v);
    __lasx_xvst(r, mem, 32);
    printf("vext2xv.w_h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_vext2xv_d_h(v);
    __lasx_xvst(r, mem, 32);
    printf("vext2xv.d_h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvrepli_d(0);
    __m256i src = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_xvextrins_b(v, src, 8);
    __lasx_xvst(r, mem, 32);
    printf("xvextrins.b: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvrepli_d(0);
    src = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_xvextrins_h(v, src, 16);
    __lasx_xvst(r, mem, 32);
    printf("xvextrins.h: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvrepli_d(0);
    src = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_xvextrins_w(v, src, 32);
    __lasx_xvst(r, mem, 32);
    printf("xvextrins.w: %s\n", errors ? "FAILED" : "PASSED");

    memset(mem, 0, sizeof(mem));
    v = __lasx_xvrepli_d(0);
    src = __lasx_xvreplgr2vr_d(test_gpr);
    r = __lasx_xvextrins_d(v, src, 0);
    __lasx_xvst(r, mem, 32);
    printf("xvextrins.d: %s\n", errors ? "FAILED" : "PASSED");

    return errors;
}