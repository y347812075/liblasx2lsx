/* SPDX-License-Identifier: MIT
 *
 * liblasx2lsx — LASX instruction emulation library for LoongArch
 * Copyright (c) 2026 liblasx2lsx contributors
 *
 * Extended context magic values and structures derived from
 * Linux kernel arch/loongarch/include/uapi/asm/sigcontext.h
 * (GPL-2.0-only, Copyright (c) 2023 Loongson Technology Corporation Limited)
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

#ifndef LOONGARCH_EXTCONTEXT_H
#define LOONGARCH_EXTCONTEXT_H

#if defined(CONFIG_LOONGARCH_NEW_WORLD) && defined(__loongarch__)
#include <asm/sigcontext.h>
#include <stddef.h>
#include <ucontext.h>

// Magic values for extended context (from Linux kernel arch/loongarch/include/uapi/asm/sigcontext.h)
// These may not be defined in all cross-compile environments, so provide fallbacks
#ifndef FPU_CTX_MAGIC
#define FPU_CTX_MAGIC 0x46505501
#endif
#ifndef LSX_CTX_MAGIC
#define LSX_CTX_MAGIC 0x53580001
#endif
#ifndef LASX_CTX_MAGIC
#define LASX_CTX_MAGIC 0x41535801
#endif
#ifndef LBT_CTX_MAGIC
#define LBT_CTX_MAGIC 0x42540001
#endif

struct _ctx_layout {
    struct sctx_info* addr;
    unsigned int size;
};

struct extctx_layout {
    struct _ctx_layout fpu;
    struct _ctx_layout lsx;
    struct _ctx_layout lasx;
    struct _ctx_layout lbt;
    struct _ctx_layout end;
};

__attribute__((unused)) static void* get_ctx_through_ctxinfo(struct sctx_info* info) { return (info) ? (void*)((char*)info + sizeof(struct sctx_info)) : (void*)0; }

#define UC_FPU(_uc) ((struct fpu_context*)(get_ctx_through_ctxinfo((_uc)->fpu.addr)))
#define UC_LBT(_uc) ((struct lbt_context*)(get_ctx_through_ctxinfo((_uc)->lbt.addr)))
#define UC_LSX(_uc) ((struct lsx_context*)(get_ctx_through_ctxinfo((_uc)->lsx.addr)))
#define UC_LASX(_uc) ((struct lasx_context*)(get_ctx_through_ctxinfo((_uc)->lasx.addr)))

#define UC_SET_SCR(_uc, i, val) do {                     \
        struct extctx_layout _extctx;                    \
        memset(&_extctx, 0, sizeof(_extctx));            \
        parse_extcontext(_uc, &_extctx);                 \
        if (_extctx.lbt.addr)                            \
            UC_LBT(&_extctx)->regs[i&3] = (uint64_t)val; \
    } while(0);

#define UC_GET_FCC(_uc, _cd)                                                                                                                                                                           \
    ({                                                                                                                                                                                                 \
        struct extctx_layout _extctx;                                                                                                                                                                  \
        memset(&_extctx, 0, sizeof(_extctx));                                                                                                                                                          \
        parse_extcontext(_uc, &_extctx);                                                                                                                                                               \
        uint64_t _fcc = 0;                                                                                                                                                                             \
        if (_extctx.lasx.addr)                                                                                                                                                                         \
            _fcc = ((struct lasx_context*)get_ctx_through_ctxinfo(_extctx.lasx.addr))->fcc;                                                                                                            \
        else if (_extctx.lsx.addr)                                                                                                                                                                     \
            _fcc = ((struct lsx_context*)get_ctx_through_ctxinfo(_extctx.lsx.addr))->fcc;                                                                                                              \
        else if (_extctx.fpu.addr)                                                                                                                                                                     \
            _fcc = ((struct fpu_context*)get_ctx_through_ctxinfo(_extctx.fpu.addr))->fcc;                                                                                                              \
        (_fcc >> ((_cd) * 8)) & 1;                                                                                                                                                                     \
    })

#define UC_SET_FCC(_uc, _cd, _val)                                                                                                                                                                     \
    do {                                                                                                                                                                                               \
        struct extctx_layout _extctx;                                                                                                                                                                  \
        memset(&_extctx, 0, sizeof(_extctx));                                                                                                                                                          \
        parse_extcontext(_uc, &_extctx);                                                                                                                                                               \
        if (_extctx.lasx.addr) {                                                                                                                                                                       \
            struct lasx_context* _ctx = (struct lasx_context*)get_ctx_through_ctxinfo(_extctx.lasx.addr);                                                                                              \
            if (_val)                                                                                                                                                                                  \
                _ctx->fcc |= (1ULL << ((_cd) * 8));                                                                                                                                                    \
            else                                                                                                                                                                                       \
                _ctx->fcc &= ~(1ULL << ((_cd) * 8));                                                                                                                                                   \
        } else if (_extctx.lsx.addr) {                                                                                                                                                                 \
            struct lsx_context* _ctx = (struct lsx_context*)get_ctx_through_ctxinfo(_extctx.lsx.addr);                                                                                                 \
            if (_val)                                                                                                                                                                                  \
                _ctx->fcc |= (1ULL << ((_cd) * 8));                                                                                                                                                    \
            else                                                                                                                                                                                       \
                _ctx->fcc &= ~(1ULL << ((_cd) * 8));                                                                                                                                                   \
        } else if (_extctx.fpu.addr) {                                                                                                                                                                 \
            struct fpu_context* _ctx = (struct fpu_context*)get_ctx_through_ctxinfo(_extctx.fpu.addr);                                                                                                 \
            if (_val)                                                                                                                                                                                  \
                _ctx->fcc |= (1ULL << ((_cd) * 8));                                                                                                                                                    \
            else                                                                                                                                                                                       \
                _ctx->fcc &= ~(1ULL << (_cd));                                                                                                                                                         \
        }                                                                                                                                                                                              \
    } while (0)

#define UC_GET_LASX(_uc, _fp, _bias, _type) (UC_LASX(_uc) ? *(_type*)(((uint8_t*)UC_LASX(_uc)->regs) + (_fp * 32 + _bias * sizeof(_type))) : 0)

#define UC_GET_LSX(_uc, _fp, _bias, _type) (UC_LSX(_uc) ? *(_type*)(((uint8_t*)UC_LSX(_uc)->regs) + (_fp * 16 + _bias * sizeof(_type))) : UC_GET_LASX(_uc, _fp, _bias, _type))

#define UC_GET_FPR(_uc, _fp, _type) (UC_FPU(_uc) ? *(_type*)(UC_FPU(_uc)->regs + _fp) : UC_GET_LSX(_uc, _fp, 0, _type))

#define UC_SET_LASX(_uc, _fp, _bias, _val, _type)                                                                                                                                                      \
    do {                                                                                                                                                                                               \
        if (UC_LASX(_uc)) {                                                                                                                                                                            \
            *(_type*)(((uint8_t*)UC_LASX(_uc)->regs) + (_fp * 32 + _bias * sizeof(_type))) = *(_type*)(uintptr_t)(_val);                                                                               \
        }                                                                                                                                                                                              \
    } while (0)

#define UC_SET_LSX(_uc, _fp, _bias, _val, _type)                                                                                                                                                       \
    do {                                                                                                                                                                                               \
        if (UC_LSX(_uc)) {                                                                                                                                                                             \
            *(_type*)(((uint8_t*)UC_LSX(_uc)->regs) + (_fp * 16 + _bias * sizeof(_type))) = *(_type*)(uintptr_t)(_val);                                                                                \
        } else {                                                                                                                                                                                       \
            UC_SET_LASX(_uc, _fp, _bias, _val, _type);                                                                                                                                                 \
        }                                                                                                                                                                                              \
    } while (0)

#define UC_SET_FPR(_uc, _fp, _val, _type)                                                                                                                                                              \
    do {                                                                                                                                                                                               \
        if (UC_FPU(_uc)) {                                                                                                                                                                             \
            *(_type*)(UC_FPU(_uc)->regs + _fp) = *(_type*)(uintptr_t)(_val);                                                                                                                           \
        } else {                                                                                                                                                                                       \
            UC_SET_LSX(_uc, _fp, 0, _val, _type);                                                                                                                                                      \
        }                                                                                                                                                                                              \
    } while (0)

static inline void parse_extcontext(ucontext_t* uc, struct extctx_layout* extctx)
{
    unsigned int magic, size;
    struct sctx_info* info = (struct sctx_info*)&uc->uc_mcontext.__extcontext;
    while (1) {
        magic = info->magic;
        size = info->size;

        switch (magic) {
        case 0: /* END */ goto done;

        case FPU_CTX_MAGIC:
            if (size >= (sizeof(struct sctx_info) + sizeof(struct fpu_context))) {
                extctx->fpu.addr = info;
                extctx->fpu.size = size;
            }
            break;

        case LSX_CTX_MAGIC:
            if (size >= (sizeof(struct sctx_info) + sizeof(struct lsx_context))) {
                extctx->lsx.addr = info;
                extctx->lsx.size = size;
            }
            break;

        case LASX_CTX_MAGIC:
            if (size >= (sizeof(struct sctx_info) + sizeof(struct lasx_context))) {
                extctx->lasx.addr = info;
                extctx->lasx.size = size;
            }
            break;

        case LBT_CTX_MAGIC:
            if (size >= (sizeof(struct sctx_info) + sizeof(struct lbt_context))) {
                extctx->lbt.addr = info;
                extctx->lbt.size = size;
            }
            break;

        default: break;
        }

        info = (struct sctx_info*)((char*)info + size);
    }

done:
    return;
}
#endif
#endif
