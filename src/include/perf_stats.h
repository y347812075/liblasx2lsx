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

// perf_stats.h - 高性能指令统计模块
#ifndef PERF_STATS_H
#define PERF_STATS_H

#include <stdint.h>
#include "perf_idx.h"

#define PERF_STATS_ENV "LASX_PERF_STATS"

// 全局计数数组（直接索引）
extern uint64_t g_perf_counts[P_MAX];
extern int g_perf_enabled;

// 检查是否启用
int perf_stats_enabled(void);

// 初始化
void perf_stats_init(void);

// 快速计数（内联，在 case 中调用）
static inline void perf_inc(int idx) {
    if (__builtin_expect(g_perf_enabled, 0) && idx >= 0 && idx < P_MAX) {
        g_perf_counts[idx]++;
    }
}

// 每秒打印（由 do_lasx_emu 调用）
void perf_stats_print(void);

// 清理
void perf_stats_cleanup(void);

#endif
