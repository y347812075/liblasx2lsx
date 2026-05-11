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

void my_custom_function(void *mem) {
    __asm__ volatile(
        "xvld $xr0,%0,0\n\r"
        "xvst $xr0,%0,32\n\r"
        "vst  $vr0,%0,64\n\r"
        :
	: "r" (mem)
	: "memory"
        );
}

long long data[1024] __attribute__((aligned(1024)));

int main(void) {
    for (int i = 0; i < 4; ++i) data[i] = i;

    printf("before\n");
    for (int i = 0; i < 10; ++i) printf("addr %p = %llx\n", &data[i], data[i]);

    my_custom_function(data);

    printf("after\n");
    for (int i = 0; i < 10; ++i) printf("addr %p = %llx\n", &data[i], data[i]);
}
