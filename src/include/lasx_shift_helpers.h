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

#ifndef LASX_SHIFT_HELPERS_H
#define LASX_SHIFT_HELPERS_H

#include <stdint.h>

/* sra_n_s/u shift-right-arith narrow signed/unsigned (no saturation) */
int8_t sran_b_h(int16_t val, uint32_t shamt);
int16_t sran_h_w(int32_t val, uint32_t shamt);
int32_t sran_w_d(int64_t val, uint32_t shamt);
int64_t sran_d_q(__int128_t val, uint32_t shamt);

/* sra_r_n_s/u shift-right-arith round narrow (no saturation) */
int8_t srarn_b_h(int16_t val, uint32_t shamt);
int16_t srarn_h_w(int32_t val, uint32_t shamt);
int32_t srarn_w_d(int64_t val, uint32_t shamt);
int64_t srarn_d_q(__int128_t val, uint32_t shamt);

/* srl_n_s/u shift-right-logic narrow signed/unsigned (no saturation) */
uint8_t srln_b_h(uint16_t val, uint32_t shamt);
uint16_t srln_h_w(uint32_t val, uint32_t shamt);
uint32_t srln_w_d(uint64_t val, uint32_t shamt);
uint64_t srln_d_q(__uint128_t val, uint32_t shamt);

/* srl_r_n_s/u shift-right-logic round narrow (no saturation) */
uint8_t srlrn_b_h(uint16_t val, uint32_t shamt);
uint16_t srlrn_h_w(uint32_t val, uint32_t shamt);
uint32_t srlrn_w_d(uint64_t val, uint32_t shamt);
uint64_t srlrn_d_q(__uint128_t val, uint32_t shamt);

/* s_srl_n_s/u saturated shift-right-logic narrow signed/unsigned */
int8_t ssrlns_b_h(int16_t val, uint32_t shamt);
int16_t ssrlns_h_w(int32_t val, uint32_t shamt);
int32_t ssrlns_w_d(int64_t val, uint32_t shamt);
int64_t ssrlns_d_q(__int128_t val, uint32_t shamt);
uint8_t ssrlnu_bu_h(uint16_t val, uint32_t shamt);
uint16_t ssrlnu_hu_w(uint32_t val, uint32_t shamt);
uint32_t ssrlnu_wu_d(uint64_t val, uint32_t shamt);
uint64_t ssrlnu_du_q(__uint128_t val, uint32_t shamt);

/* s_srl_r_n_s/u saturated shift-right-logic round narrow signed/unsigned */
int8_t ssrlrns_b_h(uint16_t val, uint32_t shamt);
int16_t ssrlrns_h_w(uint32_t val, uint32_t shamt);
int32_t ssrlrns_w_d(uint64_t val, uint32_t shamt);
int64_t ssrlrns_d_q(__uint128_t val, uint32_t shamt);
uint8_t ssrlrnu_bu_h(uint16_t val, uint32_t shamt);
uint16_t ssrlrnu_hu_w(uint32_t val, uint32_t shamt);
uint32_t ssrlrnu_wu_d(uint64_t val, uint32_t shamt);
uint64_t ssrlrnu_du_q(__uint128_t val, uint32_t shamt);

/* s_sra_n_s/u saturated shift-right-arith narrow signed/unsigned */
int8_t ssrans_b_h(int16_t val, uint32_t shamt);
int16_t ssrans_h_w(int32_t val, uint32_t shamt);
int32_t ssrans_w_d(int64_t val, uint32_t shamt);
int64_t ssrans_d_q(__int128_t val, uint32_t shamt);
uint8_t ssranu_bu_h(int16_t val, uint32_t shamt);
uint16_t ssranu_hu_w(int32_t val, uint32_t shamt);
uint32_t ssranu_wu_d(int64_t val, uint32_t shamt);
uint64_t ssranu_du_q(__int128_t val, uint32_t shamt);

/* s_sra_r_n_s/u saturated shift-right-arith round narrow signed/unsigned */
int8_t ssrarns_b_h(int16_t val, uint32_t shamt);
int16_t ssrarns_h_w(int32_t val, uint32_t shamt);
int32_t ssrarns_w_d(int64_t val, uint32_t shamt);
int64_t ssrarns_d_q(__int128_t val, uint32_t shamt);
uint8_t ssrarnu_bu_h(int16_t val, uint32_t shamt);
uint16_t ssrarnu_hu_w(int32_t val, uint32_t shamt);
uint32_t ssrarnu_wu_d(int64_t val, uint32_t shamt);
uint64_t ssrarnu_du_q(__int128_t val, uint32_t shamt);

#endif /* LASX_SHIFT_HELPERS_H */
