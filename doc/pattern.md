# LASX Loop Pattern Detection & xvmap JIT Generation

## 文件架构

```
src/
├── interpret/
│   ├── lasx_patterns.c          ← 循环检测 + xvmap 生成（核心）
│   ├── lasx_interpret.c         ← 解释块入口，调用 check_lasx_loop_pattern
│   └── lasx_interpret_opt_gen_xvmap.c ← 单条 xvmap 指令生成器
└── include/
    ├── lasx_patterns.h          ← 循环检测 API
    ├── lasx_interpret.h         ← xvmap 基础设施 API
    └── lasx_interpret_opt.h     ← xvmap 生成宏声明
```

## 两种循环模式

### 模式1: `LOOP_BCC_XX_B` — bcc 在开头 + b 在结尾

```
bcc end       ← loop_start: 条件退出 (beq/bne/blt/...)
xx            ← body_start (bcc+1): 循环体
xx
b loop        ← loop_end-1: 无条件跳回
end:          ← loop_end
```

bcc 向前跳到 `end:`（退出循环），b 向后跳到 `bcc`（继续循环）。

### 模式2: `LOOP_XX_BCC` — bcc 在结尾

```
xx            ← loop_start: 循环体
xx
bcc loop      ← loop_end-1: 条件跳回
```

bcc 向后跳到 `loop`（继续循环或退出）。

**两种模式的统一检测**：从当前指令向前扫描，找到第一条向后跳的分支指令。

`detect_loop_range` 返回：
- `loop_start` — 循环起始索引（可为负，表示在 p[0] 之前）
- `loop_length` — 循环指令数


## 核心数据结构

### `loop_pattern_info_t`（lasx_patterns.h）

```c
typedef enum { LOOP_NONE = 0, LOOP_XX_BCC, LOOP_BCC_XX_B } loop_type_t;

typedef enum {
    LOOP_PATTERN_NONE = 0,
    LOOP_PATTERN_1SRC, LOOP_PATTERN_2SRC, LOOP_PATTERN_DUAL,
} loop_pattern_type_t;

typedef struct {
    loop_type_t ltype;         // LOOP_BCC_XX_B 或 LOOP_XX_BCC
    loop_pattern_type_t type;  // 待用
    int len;                   // 循环指令数
    int rj, rk, xd, xd1, cnt; // 寄存器信息
    int rd_off;
    int branch_offset;
} loop_pattern_info_t;
```

### Branch helper（lasx_patterns.c）

```c
static int is_cond_branch(int insn);    // BEQ/BNE/BLT/BGE/BLTU/BGEU/BEQZ/BNEZ
static int is_uncond_branch(int insn);  // B
static int is_branch(int insn);         // 任意分支
static int extract_branch_offset(unsigned int instr, int opcode);
static int branch_target(unsigned int *p, int pc_idx, int instr_idx);
```


## 完整流程

### 调用入口: `lasx_emu_create_interpret_block()` (lasx_interpret.c)

```c
bool lasx_emu_create_interpret_block(ucontext_t *uc) {
    unsigned int *p = (void*)UC_PC(uc);
    int n = 0;

    while (1) {
        // Step 1: 检测循环
        int pattern_len = check_lasx_loop_pattern(&as, p + n, n);

        if (pattern_len < 0) {
            // 循环已检测并优化
            int len = -pattern_len;
            loop_optimized = 1;
            loop_patch_addr = p + n;          // 原始代码中的 loop_start
            n += len;                         // 跳过循环体
            pattern_exit = 1;
            break;
        }

        // Step 2: 正常逐条翻译
        unsigned int instr = p[n];
        lasx_interpret_inst_gen(&as, instr, 0);
        n++;
    }

    // Step 3: 把 p[loop_start] patch 为 jiscr1 → loop JIT
    if (loop_optimized) {
        uint32_t jiscr1_off = ...;  // loop_jit_entry - interpreter_entry
        *loop_patch_addr = jiscr1_off;
        ibar 0;
    }
}
```

### Step 1: `check_lasx_loop_pattern(as, p, pre_count)` (lasx_patterns.c)

**参数**：
- `as` — lagoon assembler，生成 JIT 代码
- `p` — 当前指令指针
- `pre_count` — 当前 block 中已翻译的指令数（用于 jiscr0 偏移）

**流程**：
1. 调用 `detect_loop_range(p, ...)` 查找循环边界
2. 如果 `pre_count > 0`：在 as 中插入 `jiscr0(pre_count*4)` 分隔 pre-loop 和 loop
3. 调用 `check_xvmap_feasibility(p + loop_start, loop_len, ...)` 扫描 XR 寄存器
4. 调用 `generate_xvmap_loop(as, p, loop_start, loop_len, ...)` 生成 xvmap JIT
5. 返回 `-loop_len`

**jiscr0 插入逻辑**：

```
xvld $xr10, $a2, 0   ← p[0], 正常翻译（block 第一条指令）
                       ← jiscr0(1*4) → 跳到 p[1]
beq $a3, $zero, 32   ← p[1] = loop_start, patch 为 jiscr1 → loop JIT
jiscr1:               ← SCR[0] = p[1], PC = loop_jit_entry
  xvmap prologue
  beq → end_label
  body
  b → loop_label
end_label:
  xvmap epilogue
  jiscr0(loop_len*4)  ← SCR[0] + loop_len*4 = p[1] + N = end:
```

### Step 2: `detect_loop_range(p, loop_start, loop_end, info)` (lasx_patterns.c)

从 `p[0]` 向前扫描寻找第一条向后跳的分支：

```c
for (int i = 0; i < MAX_LOOP_INSN; i++) {
    if (!is_branch(p[i])) continue;

    int target = branch_target(p, 0, i);

    if (target > 0) {
        // i==0 且 p[0] 是 bcc → 可能是模式1，继续扫描
        if (i == 0 && is_cond_branch(p[0])) continue;
        return 0;
    }

    if (target <= 0) {
        // 找到回跳
        if (is_uncond_branch(p[i]) && is_cond_branch(p[target])) {
            // 模式1: b 指令指向 bcc
            *loop_start = target;
            *loop_end = i + 1;
            return i + 1 - target;
        }
        if (is_cond_branch(p[i])) {
            // 模式2: bcc 向后跳
            for (int k = target; k < 0; k++)
                if (is_branch(p[k])) return 0;  // 嵌套循环
            *loop_start = target;
            *loop_end = i + 1;
            return i + 1 - target;
        }
    }
}
return 0;
```

**关键常量**：`MAX_LOOP_INSN = 128` — 最大扫描 128 条指令。

### Step 3: `check_xvmap_feasibility(p, loop_len, xvuse, xv_insn_mask)` (lasx_patterns.c)

用 `la_disasm_one` 反汇编每条指令，统计 VR 和 XR 寄存器使用量：

```c
static int check_xvmap_feasibility(unsigned int *p, int loop_len,
                                    uint32_t *xvuse, uint32_t xv_insn_mask[4])
{
    uint32_t xr_mask = 0, vr_all = 0;
    memset(xv_insn_mask, 0, 16);

    for (int i = 0; i < loop_len; i++) {
        int is_xv = 0;
        la_disasm_one(p[i], &insn);
        for (int j = 0; j < insn.operand_count; j++) {
            if (op->kind == LA_OP_XVPR) {
                xr_mask |= (1u << op->xvpr); vr_all |= (1u << op->xvpr);
                is_xv = 1;
            } else if (op->kind == LA_OP_VPR) {
                vr_all |= (1u << op->vpr);
            } else if (op->kind == LA_OP_FPR) {
                vr_all |= (1u << op->fpr);
            }
        }
        if (is_xv) xv_insn_mask[i / 32] |= (1u << (i & 31));
    }

    *xvuse = xr_mask;
    int xr_cnt = __builtin_popcount(xr_mask);
    if (xr_cnt == 0 || xr_cnt > 16) return -1;

    int total_vr = __builtin_popcount(vr_all);
    int free_vr = 32 - total_vr;
    if (free_vr < xr_cnt) return -1;
    return 0;
}
```

**`xv_insn_mask[4]`**：128-bit 位图，标记每条指令是否是 xv 指令。
- 索引 `i / 32` 确定 word，`i & 31` 确定 bit。
- 在 `generate_xvmap_loop` 中用 `mask_idx = k - loop_start` 访问。

**xvmap 限制**：
- 每个 XR 需要一个空闲 VR 映射高 128-bit
- total_used_VR + xr_count ≤ 32
- xr_count ≤ 16

### Step 4: `generate_xvmap_loop(as, p, loop_start, loop_len, xvuse, xv_insn_mask)`

**参数**：
- `as` — lagoon assembler
- `p` — 原始指令指针（非 shifted）
- `loop_start` — 循环起始索引（可能为负）
- `loop_len` — 循环指令数
- `xvuse` — XR 使用位掩码（传给 lasx_build_xvmap）
- `xv_insn_mask[4]` — XV 指令标记位图

**生成的 JIT 结构**：

```
jit_entry:
  pcaddi r0, 0

  xvmap prologue:
    for each mapped XR:
      vst vmap, r2, TD_OFF_DATA(vmap, 0)   // 保存物理 VR
      vld vmap, r2, TD_OFF_DATA(xr, 2)     // 加载 XR 高 128-bit

  -- pattern 1 --
  loop_label:
    bcc → end_label     // 用 lagoon label API + la_beq/la_bne/...

  loop body:
    for k in [body_start, body_end):
      if xv_insn_mask[k - loop_start]:
        lasx_interpret_inst_gen_xvmap(as, p[k])
      else:
        la_dup(as, p[k])     // 复制原始指令

  -- pattern 1 --
    b → loop_label       // la_b + la_label

  -- pattern 2 --
    bcc → loop_label     // la_bnez/la_beqz/... + la_label

  end_label (la_bind — 回填 bcc)
  xvmap epilogue:
    for each mapped XR:
      vst vmap, r2, TD_OFF_DATA(xr, 2)     // 保存 XR 高 128-bit
      vld vmap, r2, TD_OFF_DATA(vmap, 0)   // 恢复物理 VR

  jiscr0(loop_len * 4)
```

**body 边界**：
- `body_start = loop_start`（模式2）或 `loop_start + 1`（模式1，跳过开头的 bcc）
- `body_end = loop_end - 1` — 排除末尾的分支指令

**bcc opcode 处理**：统一使用 lagoon API，支持所有分支类型：

| opcode | lagoon 函数 |
|--------|-----------|
| BEQ | `la_beq(as, rj, rd, offset)` |
| BNE | `la_bne(as, rj, rd, offset)` |
| BLT | `la_blt(as, rj, rd, offset)` |
| BGE | `la_bge(as, rj, rd, offset)` |
| BLTU | `la_bltu(as, rj, rd, offset)` |
| BGEU | `la_bgeu(as, rj, rd, offset)` |
| BEQZ | `la_beqz(as, rj, offset)` |
| BNEZ | `la_bnez(as, rj, offset)` |

**Lagoon Label 机制**：

```c
lagoon_label_t label = {0};

// Forward branch: 先 emit 指令 + 注册 label，再 bind
la_beq(as, rj, rd, la_label(as, &label));
// ... 生成代码 ...
la_bind(as, &label);  // 回填所有已注册的引用

// Backward branch: 先 bind，再 emit
la_bind(as, &label);
// ... 生成代码 ...
la_b(as, la_label(as, &label));  // 直接返回正确 offset

// 清理
la_label_free(as, &label);
```

**xvmap 指令生成器**（`lasx_interpret_opt_gen_xvmap.c`）：

每条 xv 指令对应一个生成函数，模式：
```c
void __gen_lasx_interpret_xvxxx_opt_xvmap(void *pas, unsigned int instr) {
    lagoon_assembler_t *as = pas;
    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;

    int xd2 = as->xvmap[xd];   // 高 128-bit 映射
    int xj2 = as->xvmap[xj];
    int xk2 = as->xvmap[xk];

    la_vxxx(as, xd, xj, xk);    // 低 128-bit
    la_vxxx(as, xd2, xj2, xk2); // 高 128-bit
}
```

## 分支指令 OPCODE

```c
#define OPCODE_BEQZ  0x10
#define OPCODE_BNEZ  0x11
#define OPCODE_B     0x14
#define OPCODE_BEQ   0x16
#define OPCODE_BNE   0x17
#define OPCODE_BLT   0x18
#define OPCODE_BGE   0x19
#define OPCODE_BLTU  0x1a
#define OPCODE_BGEU  0x1b
#define MAX_LOOP_INSN 128
```

## xvmap 指令分发

在 `lasx_interpret_inst_gen_xvmap()` 中按 opcode shift 分发：

| OP 前缀 | shift | 示例指令 |
|---------|-------|---------|
| OP10 | `>> 22` | xvld, xvst |
| OP12 | `>> 20` | xvfmadd.s |
| OP14 | `>> 18` | xvpermi.q, xvori.b |
| OP17 | `>> 15` | xvadd, xvmul, xvmadd |
| OP18 | `>> 14` | xvsrli.h |
| OP19 | `>> 13` | xvpickve2gr.w |
| OP22 | `>> 10` | vext2xv, xvpcnt |


## jiscr0/jiscr1 机制

```
jiscr1 off ← SCR[0] = PC, PC = SCR[1] + off*4  (unconditional indirect jump + save return)
jiscr0 off ← PC = SCR[0] + off*4                (unconditional indirect jump)
```

**执行流**：
1. 原始代码 p[0] = jiscr1 → JIT 块入口
2. JIT 块执行: pre-loop 指令 + jiscr0(pre_count*4) → p[pre_count]
3. p[pre_count] = loop_start, patched 为 jiscr1 → loop JIT
4. loop JIT 执行: ... → jiscr0(loop_len*4) → p[pre_count + loop_len] = end:

## 环境变量

```bash
LASX_LOOP_PATTERN=1  # 启用循环检测
```

## 调用链总结

```
lasx_emu_create_interpret_block()
  ├── check_lasx_loop_pattern(as, p + n, n)
  │     ├── detect_loop_range(p)
  │     │     └── 向前扫描找向后跳分支，返回 [loop_start, loop_len]
  │     ├── la_jiscr0(pre_count*4)  if pre_count > 0
  │     ├── check_xvmap_feasibility(p + loop_start, loop_len)
  │     │     └── la_disasm_one 统计 XR/FPR/VPR，标记 xv_insn_mask
  │     ├── generate_xvmap_loop(as, p, loop_start, loop_len, xvuse, mask)
  │     │     ├── lasx_build_xvmap → xvmap[] 寄存器映射
  │     │     ├── __gen_xvmap_prologue
  │     │     ├── lagoon label → bcc (模式1，用 la_beq/la_bne/...)
  │     │     ├── lasx_interpret_inst_gen_xvmap / la_dup 逐条翻译
  │     │     ├── lagoon label → b (模式1) 或 bcc (模式2)
  │     │     ├── la_bind → 回填 bcc offset
  │     │     ├── __gen_xvmap_epilogue
  │     │     └── la_jiscr0(loop_len*4)
  │     └── return -loop_len
  └── *loop_patch_addr = jiscr1 → loop_jit_entry
```
