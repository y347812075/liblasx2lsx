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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static uint64_t seed = 0;

static uint64_t xorshift64(uint64_t x)
{
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

int main(int argc, char* argv[])
{
    const char* filename = NULL;
    int count = 8;
    int print_flag = 0;
    int use_seed = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            count = atoi(argv[++i]);
            if (count <= 0) {
                fprintf(stderr, "Error: -n must be a positive integer.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-p") == 0) {
            print_flag = 1;
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = strtoull(argv[++i], NULL, 0);
            use_seed = 1;
        }
    }

    if (!filename) {
        fprintf(stderr, "Usage: %s -f <filename> [-n count] [-p] [-s seed]\n", argv[0]);
        return 1;
    }

    FILE* out = fopen(filename, "wb");
    if (!out) {
        perror("fopen output file");
        return 1;
    }

    uint64_t value;
    size_t items_written = 0;
    for (int i = 0; i < count; i++) {
        if (use_seed) {
            seed = xorshift64(seed + i);
            value = seed;
        } else {
            FILE* urand = fopen("/dev/urandom", "rb");
            if (!urand) {
                perror("fopen /dev/urandom");
                fclose(out);
                return 1;
            }
            if (fread(&value, sizeof(value), 1, urand) != 1) {
                fprintf(stderr, "Error reading from /dev/urandom\n");
                fclose(urand);
                fclose(out);
                return 1;
            }
            fclose(urand);
        }
        if (fwrite(&value, sizeof(value), 1, out) != 1) {
            perror("fwrite");
            break;
        }
        if (print_flag) {
            printf("0x%016llx\n", (unsigned long long)value);
        }
        items_written++;
    }

    fclose(out);

    return (items_written == count) ? 0 : 1;
}
