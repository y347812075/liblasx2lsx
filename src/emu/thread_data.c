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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ucontext.h>
#include "thread_data.h"

// 使用 __thread 实现高效的线程本地存储
// 避免每次调用都加锁和链表查找
static __thread thread_data_t g_thread_data = {0};

void* thread_data_get(void)
{
    return &g_thread_data;
}

void thread_data_free(void)
{
    // 关闭日志文件
    if (g_thread_data.log_file) {
        fclose(g_thread_data.log_file);
        g_thread_data.log_file = NULL;
    }
}

void thread_data_cleanup(void* unused)
{
    (void)unused;
    thread_data_free();
}


