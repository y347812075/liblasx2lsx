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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "lagoon.h"
#include "lasx_profile.h"

#ifdef LASX_PROFILE

static uint64_t profile_pc[LASX_PROFILE_MAX];
static uint32_t profile_instr[LASX_PROFILE_MAX];
static uint64_t profile_count[LASX_PROFILE_MAX];
static int profile_count_ratio[LASX_PROFILE_MAX];
static uint64_t profile_type[LASX_PROFILE_MAX];
static char *profile_type_str[] = {
    [PROFILE_NONE]   = "none",
    [PROFILE_BLOCK]  = "block",
    [PROFILE_FRAG]   = "fragment",
    [PROFILE_LOOP]   = "loop",
    [PROFILE_USEDEF] = "usedef",
    [PROFILE_SINGLE] = "single",
};
static uint64_t profile_count_prev[LASX_PROFILE_MAX];
static int profile_num = 0;
static pthread_t profile_tid;
static volatile int profile_running = 0;

int profile_emit(void *pas, void *entry, unsigned long pc, int type)
{
    int pidx = __atomic_fetch_add(&profile_num, 1, __ATOMIC_RELAXED);
    if (pidx >= LASX_PROFILE_MAX) return 0;

    profile_pc[pidx] = pc;
    profile_instr[pidx] = *((uint32_t*)pc);
    profile_type[pidx] = type;

    lagoon_assembler_t as;
    if (!pas) {
        la_init_assembler(&as, entry, 0x1000);
        la_pcaddi(&as, LA_ZERO, 0);
        pas = &as;
    }

    /* Emit atomic inc instrumentation into JIT buffer */
    la_movgr2scr(pas, LA_SCR3, LA_T1);
    la_lu12i_w(pas, LA_RX, 0x70000);
    la_ld_d(pas, LA_RX, LA_RX, 8);
    la_load_immediate32(pas, LA_T1, pidx);
    la_slli_d(pas, LA_T1, LA_T1, 3);
    la_add_d(pas, LA_RX, LA_RX, LA_T1);
    la_ori(pas, LA_T1, LA_ZERO, 1);
    la_amadd_d(pas, LA_ZERO, LA_T1, LA_RX);
    la_movscr2gr(pas, LA_T1, LA_SCR3);

    return la_get_inst_count(pas);
}

int profile_current_pidx(void)
{
    return profile_num - 1;
}

void profile_set(int pidx, int type, int ratio)
{
    if (pidx >= 0 && pidx < LASX_PROFILE_MAX) {
        profile_type[pidx] = type;
        profile_count_ratio[pidx] = ratio;
    }
}

uint64_t *profile_get_count_array(void)
{
    return profile_count;
}

typedef struct {
    int idx;
    uint64_t delta;
} profile_entry_t;

static int cmp_delta_desc(const void *a, const void *b)
{
    const profile_entry_t *ea = (const profile_entry_t *)a;
    const profile_entry_t *eb = (const profile_entry_t *)b;
    if (eb->delta > ea->delta) return 1;
    if (eb->delta < ea->delta) return -1;
    return 0;
}

void lasx_profile_dump_periodic(void)
{
    int n = __atomic_load_n(&profile_num, __ATOMIC_RELAXED);
    if (n > LASX_PROFILE_MAX) n = LASX_PROFILE_MAX;
    if (n == 0) return;

    profile_entry_t *tmp = malloc(n * sizeof(profile_entry_t));
    if (!tmp) return;
    int cnt = 0;

    for (int i = 0; i < n; i++) {
        uint64_t cur = __atomic_load_n(&profile_count[i], __ATOMIC_RELAXED);
        uint64_t delta = cur - profile_count_prev[i];
        if (profile_type[i]) delta *= profile_count_ratio[i];
        profile_count_prev[i] = cur;
        if (delta > 0) {
            tmp[cnt].idx = i;
            tmp[cnt].delta = delta;
            cnt++;
        }
    }

    if (cnt > 0) {
        qsort(tmp, cnt, sizeof(profile_entry_t), cmp_delta_desc);
        int limit = cnt < 1000 ? cnt : 1000;

        uint64_t total = 0;
        uint64_t top_total = 0;
        for (int i = 0; i < cnt; i++) total += tmp[i].delta;
        for (int i = 0; i < limit; i++) top_total += tmp[i].delta;

        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        fprintf(stderr, "\n=== LASX Profile %02d:%02d:%02d ===\n",
                tm->tm_hour, tm->tm_min, tm->tm_sec);
        fprintf(stderr, "entries: %d/%d, active: %d, total: %lu, top%d: %lu (%.1f%%)\n",
                n, LASX_PROFILE_MAX, cnt, total, limit, top_total,
                total > 0 ? 100.0 * top_total / total : 0.0);
        for (int i = 0; i < limit; i++) {
            int idx = tmp[i].idx;
            lagoon_insn_t insn;
            char buf[128];
            la_disasm_one(profile_instr[idx], &insn);
            la_insn_to_str(&insn, buf, sizeof(buf));
            if (profile_type[idx]) {
                fprintf(stderr, "0x%016lx %8lu 0x%08x %s %s ratio %d\n",
                        profile_pc[idx], tmp[i].delta, profile_instr[idx], buf,
                        profile_type_str[profile_type[idx]],
                        profile_count_ratio[idx]);
            } else {
                fprintf(stderr, "0x%016lx %8lu 0x%08x %s\n",
                        profile_pc[idx], tmp[i].delta, profile_instr[idx], buf);
            }
        }
    }
    free(tmp);
}

static void *profile_thread_func(void *arg)
{
    (void)arg;
    while (__atomic_load_n(&profile_running, __ATOMIC_RELAXED)) {
        sleep(1);
        lasx_profile_dump_periodic();
    }
    return NULL;
}

void profile_start_periodic(void)
{
    __atomic_store_n(&profile_running, 1, __ATOMIC_RELAXED);
    pthread_create(&profile_tid, NULL, profile_thread_func, NULL);
}

void profile_stop_periodic(void)
{
    __atomic_store_n(&profile_running, 0, __ATOMIC_RELAXED);
    pthread_join(profile_tid, NULL);
    lasx_profile_dump_periodic();
}
#endif /* LASX_PROFILE */
