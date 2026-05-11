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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* name;
    uint64_t data[8];
} TestData;

int main(int argc, char* argv[])
{
    const char* filename = NULL;
    const char* type = "all";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            type = argv[++i];
        }
    }

    if (!filename) {
        fprintf(stderr, "Usage: %s -f  [-t all|zero|one|minus1|max|allff|pattern]\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    TestData datasets[] = {
        {"zero", {0, 0, 0, 0, 0, 0, 0, 0}},
        {"one", {1, 1, 1, 1, 1, 1, 1, 1}},
        {"minus1",
         {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL}},
        {"max",
         {0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL}},
        {"allff",
         {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL}},
        {"pattern_aa",
         {0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL, 0xAAAAAAAAAAAAAAAAULL}},
        {"pattern_55",
         {0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL}},
        {"bit_lsb", {1, 0, 0, 0, 0, 0, 0, 0}},
        {"bit_msb", {0x8000000000000000ULL, 0, 0, 0, 0, 0, 0, 0}},
        {"walk_ones", {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80}},
        {"walk_zeros",
         {0xFFFFFFFFFFFFFFFEULL, 0xFFFFFFFFFFFFFFFDULL, 0xFFFFFFFFFFFFFFFBULL, 0xFFFFFFFFFFFFFFF7ULL, 0xFFFFFFFFFFFFFFEFULL, 0xFFFFFFFFFFFFFFDFULL, 0xFFFFFFFFFFFFFFBFULL, 0xFFFFFFFFFFFFFF7FULL}},
    };

    int count = 0;
    if (strcmp(type, "all") == 0) {
        for (size_t i = 0; i < sizeof(datasets) / sizeof(datasets[0]); i++) {
            fwrite(datasets[i].data, sizeof(uint64_t), 8, fp);
            printf("Generated: %s\n", datasets[i].name);
            count++;
        }
    } else {
        for (size_t i = 0; i < sizeof(datasets) / sizeof(datasets[0]); i++) {
            if (strcmp(datasets[i].name, type) == 0) {
                fwrite(datasets[i].data, sizeof(uint64_t), 8, fp);
                printf("Generated: %s\n", datasets[i].name);
                count = 1;
                break;
            }
        }
        if (count == 0) {
            fprintf(stderr, "Unknown type: %s\n", type);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf("Written %d test data set(s) to %s\n", count, filename);
    return 0;
}