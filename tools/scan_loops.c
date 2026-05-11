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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <ucontext.h>

#include "lagoon.h"
#include "lasx_patterns_xvmap.h"
#include "lasx_interpret.h"

extern void *interpreter_entry;
extern void *interpreter_entry_ptr;

int lasx_interpret_mode;
int lasx_interpret_frag_opt;

static int test_xvmap(unsigned int instr)
{
    lagoon_insn_t insn;
    la_disasm_one(instr, &insn);
    int is_xv = 0;
    for (int j = 0; j < insn.operand_count; j++) {
        if (insn.operands[j].kind == LA_OP_XVPR) { is_xv = 1; break; }
    }
    if (!is_xv)
        return 1;

    uint8_t buf[256];
    lagoon_assembler_t as;
    la_init_assembler(&as, buf, sizeof(buf));
    interpreter_entry = buf;
    interpreter_entry_ptr = buf;
    return lasx_interpret_inst_gen_xvmap(&as, instr);
}

static int find_text_section(const uint8_t *elf, size_t size,
                              uint32_t **text_out, size_t *text_sz,
                              uint64_t *text_vaddr)
{
    if (size < sizeof(Elf64_Ehdr))
        return -1;

    const Elf64_Ehdr *ehdr = (const Elf64_Ehdr *)elf;
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3)
        return -1;

    if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0)
        return -1;

    const Elf64_Shdr *shdr = (const Elf64_Shdr *)(elf + ehdr->e_shoff);
    const Elf64_Shdr *shstrtab_hdr = &shdr[ehdr->e_shstrndx];
    const char *shstrtab = (const char *)(elf + shstrtab_hdr->sh_offset);

    for (int i = 0; i < ehdr->e_shnum; i++) {
        const char *name = shstrtab + shdr[i].sh_name;
        if (strcmp(name, ".text") == 0 &&
            shdr[i].sh_size > 0 &&
            (shdr[i].sh_flags & SHF_EXECINSTR)) {
            *text_out = (uint32_t *)(elf + shdr[i].sh_offset);
            *text_sz = shdr[i].sh_size;
            *text_vaddr = shdr[i].sh_addr;
            return 0;
        }
    }

    for (int i = 0; i < ehdr->e_shnum; i++) {
        const char *name = shstrtab + shdr[i].sh_name;
        if (strcmp(name, ".text") == 0 && shdr[i].sh_size > 0) {
            *text_out = (uint32_t *)(elf + shdr[i].sh_offset);
            *text_sz = shdr[i].sh_size;
            *text_vaddr = shdr[i].sh_addr;
            return 0;
        }
    }

    return -1;
}

static void dump_jit(const uint8_t *buf, size_t len)
{
    for (size_t i = 0; i < len; i += 4) {
        uint32_t word = *(uint32_t *)(buf + i);
        lagoon_insn_t insn;
        char dis[128];
        la_disasm_one(word, &insn);
        la_insn_to_str(&insn, dis, sizeof(dis));
        printf("  [%04zx] %08x  %s\n", i, word, dis);
    }
}

int main(int argc, char *argv[])
{
    int failed_only = 0;
    const char *filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0)
            failed_only = 1;
        else
            filename = argv[i];
    }

    if (!filename) {
        fprintf(stderr, "Usage: %s [-f] <library.so>\n", argv[0]);
        fprintf(stderr, "  -f  only show loops with generate failure\n");
        return 1;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct stat st;
    fstat(fd, &st);
    uint8_t *elf = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (elf == MAP_FAILED) { perror("mmap"); return 1; }

    uint32_t *text;
    size_t text_sz;
    uint64_t text_vaddr;
    if (find_text_section(elf, st.st_size, &text, &text_sz, &text_vaddr) != 0) {
        fprintf(stderr, "No .text section found\n");
        munmap(elf, st.st_size);
        return 1;
    }

    size_t jit_sz = 256 * 1024;
    uint8_t *jit_buf = calloc(1, jit_sz);
    if (!jit_buf) { perror("calloc"); munmap(elf, st.st_size); return 1; }

    interpreter_entry = jit_buf;
    interpreter_entry_ptr = jit_buf;

    reset_loop_detection_state();

    int n = 0;
    int max_n = text_sz / 4;
    int found = 0;

    while (n + 4 <= max_n) {
        lagoon_assembler_t as;
        la_init_assembler(&as, jit_buf, jit_sz);
        interpreter_entry = jit_buf;
        interpreter_entry_ptr = jit_buf;

        int r = check_lasx_loop_pattern(&as, text + n, 0, -1);

        if (r != 0) {
            int loop_len = r < 0 ? -r : r;
            int failed = r > 0;

            if (!failed_only || failed) {
                printf("\n=== LOOP at instruction %d [%lx] len=%d%s ===\n",
                       n, text_vaddr + n * 4, loop_len,
                       failed ? " (generate failed)" : "");

                printf("Original:\n");
                for (int i = 0; i < loop_len && n + i < max_n; i++) {
                    uint32_t w = text[n + i];
                    lagoon_insn_t insn;
                    char dis[128];
                    la_disasm_one(w, &insn);
                    la_insn_to_str(&insn, dis, sizeof(dis));
                    printf("  [%lx] %08x  %s%s\n", text_vaddr + (n + i) * 4, w, dis,
                           failed && !test_xvmap(text[n + i]) ? "  <-- FAIL" : "");
                }

                if (r < 0) {
                    size_t jit_n = as.cursor - as.buffer;
                    printf("JIT (%zu bytes, %zu insns):\n", jit_n, jit_n / 4);
                    dump_jit(as.buffer, jit_n);
                }
            }

            n += loop_len;
            found++;
        } else {
            n++;
        }
    }

    printf("\nDone. %d loops found in %d instructions.\n", found, max_n);
    free(jit_buf);
    munmap(elf, st.st_size);
    return 0;
}
