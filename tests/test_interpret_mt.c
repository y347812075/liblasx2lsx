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

#include <lasxintrin.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define NUM_THREADS 4
#define ITERATIONS 10000
#define VECTOR_SIZE 32

// 共享测试数据
typedef struct {
    int thread_id;
    int iterations;
    uint64_t result[4];  // 存储最终结果
} thread_arg_t;

// 每个线程执行的 LASX 测试函数
void* thread_test(void* arg) {
    thread_arg_t* ta = (thread_arg_t*)arg;
    
    // 分配对齐的内存
    uint8_t mem[96];
    
    // 初始化数据
    for (int i = 0; i < 96; i++) {
        mem[i] = (uint8_t)(i + ta->thread_id);
    }
    
    uint64_t sum = 0;
    
    for (int iter = 0; iter < ta->iterations; iter++) {
        // 使用 intrinsics 执行 LASX 指令
        __m256i a = __lasx_xvld(mem, 0);
        __m256i b = __lasx_xvld(mem, 32);
        
        // 执行多种 LASX 运算
        __m256i c = __lasx_xvadd_b(a, b);
        __m256i d = __lasx_xvadd_h(a, b);
        __m256i e = __lasx_xvadd_w(a, b);
        __m256i f = __lasx_xvadd_d(a, b);
        
        // 逻辑运算
        __m256i g = __lasx_xvand_v(a, b);
        __m256i h = __lasx_xvor_v(a, b);
        __m256i i = __lasx_xvxor_v(a, b);
        
        // 乘法
        __m256i j = __lasx_xvmul_b(a, b);
        __m256i k = __lasx_xvmul_h(a, b);
        
        // 累加结果
        __m256i sum_vec = __lasx_xvadd_d(c, d);
        sum_vec = __lasx_xvadd_d(sum_vec, e);
        sum_vec = __lasx_xvadd_d(sum_vec, f);
        
        // 存储结果
        __lasx_xvst(sum_vec, mem, 64);
        
        // 读取结果并累加
        uint64_t* result = (uint64_t*)(mem + 64);
        sum += result[0] + result[1] + result[2] + result[3];
        
        // 修改输入数据，增加变化
        for (int i = 0; i < 32; i++) {
            mem[i] = (uint8_t)(mem[i] + iter);
            mem[i + 32] = (uint8_t)(mem[i + 32] + iter);
        }
    }
    
    // 存储最终结果
    ta->result[0] = sum;
    ta->result[1] = ta->thread_id;
    ta->result[2] = ta->iterations;
    ta->result[3] = 0xDEADBEEF;
    
    printf("Thread %d: completed %d iterations, sum=0x%016lx\n", 
           ta->thread_id, ta->iterations, sum);
    
    return NULL;
}

int main(int argc, char** argv) {
    int num_threads = NUM_THREADS;
    int iterations = ITERATIONS;
    
    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0 || num_threads > 16) {
            num_threads = NUM_THREADS;
        }
    }
    
    if (argc > 2) {
        iterations = atoi(argv[2]);
        if (iterations <= 0) {
            iterations = ITERATIONS;
        }
    }
    
    printf("=== LASX Interpret Mode Multi-thread Test ===\n");
    printf("Threads: %d, Iterations: %d\n\n", num_threads, iterations);
    
    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];
    
    // 创建线程
    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        args[i].iterations = iterations;
        memset(args[i].result, 0, sizeof(args[i].result));
        
        if (pthread_create(&threads[i], NULL, thread_test, &args[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }
    
    // 等待所有线程完成
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n=== Test Completed ===\n");
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d: result[0]=0x%016lx\n", i, args[i].result[0]);
    }
    
    return 0;
}
