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

#ifndef DEBUG_H
#define DEBUG_H

#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread_data.h"

// 环境变量控制 tdlog 开关，默认关闭
// 设置 LASX_DEBUG=1 启用日志
static inline int tdlog_enabled(void) {
    static int enabled = -1;
    if (enabled == -1) {
        const char* env = getenv("LASX_DEBUG");
        enabled = (env != NULL && (strcmp(env, "1") == 0 || strcmp(env, "true") == 0));
    }
    return enabled;
}

#ifdef DEBUG
#define tdlog(...) do { \
    if (!tdlog_enabled()) break; \
    thread_data_t *td = thread_data_get();  \
    if (td->log_file == NULL) { \
        char name[64];  \
        sprintf(name, "/tmp/lasx-%d-%ld.log", getpid(), syscall(SYS_gettid));    \
        td->log_file = fopen(name, "w");    \
    }   \
    if (td->log_file != NULL) { \
        fprintf(td->log_file, __VA_ARGS__);  \
        fflush(td->log_file);   \
    }   \
} while(0)
#else
#define tdlog(...)
#endif

#endif
