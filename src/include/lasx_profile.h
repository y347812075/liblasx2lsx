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

#ifndef LASX_PROFILE_H
#define LASX_PROFILE_H

#include <stdint.h>

/* Profile entry types */
enum {
    PROFILE_NONE   =  0,  /* unset / invalid (skipped in dump) */
    PROFILE_BLOCK  =  1,  /* per-instruction block JIT */
    PROFILE_FRAG   =  2,  /* fragment translation */
    PROFILE_LOOP   =  3,  /* loop (xvmap) optimization */
    PROFILE_USEDEF =  4,  /* usedef batch translation */
    PROFILE_SINGLE =  5,  /* single-instruction fallback */
};

/* Profile mode bitmask flags (for LIBLASX2LSX_PROFILE) */
#define LASX_PROFILE_THREAD   (1 << 0)   /* periodic thread dump */
#define LASX_PROFILE_SIGNAL   (1 << 1)   /* SIGUSR1-triggered dump */
#define LASX_PROFILE_JIT      (1 << 2)   /* JIT batch instruction log */

extern int lasx_profile_mode;

#ifdef LASX_PROFILE

#include <pthread.h>
#include <time.h>

#define LASX_PROFILE_MAX 65536

/* Phase 1: allocate a profile slot, record pc/instr/type,
   and optionally emit atomic counter instrumentation into the JIT buffer.
   Returns the count of instructions emitted (0 if nothing emitted). */
int profile_emit(void *as, void *entry, unsigned long pc, int type);

/* Return the most recently allocated profile index. */
int profile_current_pidx(void);

/* Phase 2: update type and count_ratio for the given profile slot. */
void profile_set(int pidx, int type, int ratio);

/* Start/stop periodic profile dump thread. */
void profile_start_periodic(void);
void profile_stop_periodic(void);
void lasx_profile_dump_periodic(void);

uint64_t *profile_get_count_array(void);

/* JIT batch instruction log (LASX_PROFILE_JIT) */
static inline int jitlog_enabled(void) {
    static int checked = 0;
    static int val = 0;
    if (!checked) {
        val = (lasx_profile_mode & LASX_PROFILE_JIT) ? 1 : 0;
        checked = 1;
    }
    return val;
}

#include "thread_data.h"
#include <unistd.h>
#include <sys/syscall.h>

#define JITLOG(...) do { \
    if (!jitlog_enabled()) break; \
    thread_data_t *td = thread_data_get(); \
    if (td->jit_log_file == NULL) { \
        char name[128]; \
        snprintf(name, sizeof(name), "/tmp/lasx-jit-%d-%ld.log", \
                 getpid(), (long)syscall(SYS_gettid)); \
        td->jit_log_file = fopen(name, "w"); \
    } \
    if (td->jit_log_file) { \
        fprintf(td->jit_log_file, __VA_ARGS__); \
        fflush(td->jit_log_file); \
    } \
} while(0)

#else /* !LASX_PROFILE */

#define LASX_PROFILE_MAX 0

static inline int profile_emit(void *as, void *entry, unsigned long pc, int type)
{ (void)as; (void)entry; (void)pc; (void)type; return 0; }

static inline int profile_current_pidx(void) { return 0; }

static inline void profile_set(int pidx, int type, int ratio)
{ (void)pidx; (void)type; (void)ratio; }

static inline uint64_t *profile_get_count_array(void) { return NULL; }

static inline void profile_start_periodic(void) {}
static inline void profile_stop_periodic(void) {}
static inline void lasx_profile_dump_periodic(void) {}

#define JITLOG(...)  /* no-op without LASX_PROFILE */

#endif /* LASX_PROFILE */

#endif /* LASX_PROFILE_H */
