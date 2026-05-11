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

// thread_data.h
#ifndef THREAD_DATA_H
#define THREAD_DATA_H

#include <pthread.h>
#include <stddef.h>

typedef struct thread_data {
    long long gpr[32];
    union {
        long long data[32][4];
        unsigned int data_w[32][8];
        unsigned short data_h[32][16];
        char data_b[32][32];
    };
    void *log_file;
#ifdef LASX_PROFILE
    void *jit_log_file;
#endif
} thread_data_t;

/*
 * 获取当前线程的私有数据指针。
 * 若当前线程尚未拥有数据，则自动分配已初始化的大小（清零），并关联到当前线程。
 * 若已存在，直接返回原有指针。
 * 返回值：指向线程私有数据的指针，失败（如未调用 thread_data_init）返回 NULL。
 */
void* thread_data_get(void);

/*
 * 释放当前线程的私有数据（如果有），并从全局表中移除。
 * 通常在线程退出前调用，也可配合 pthread_cleanup_push 使用。
 */
void thread_data_free(void);

/*
 * 用于 pthread_cleanup_push 的清理函数包装。
 * 参数 unused 被忽略，内部调用 thread_data_free()。
 */
void thread_data_cleanup(void* unused);

#endif // THREAD_DATA_H
