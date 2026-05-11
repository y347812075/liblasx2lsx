# xvmap 热/冷 XR 溢出设计方案

## 背景

当前 `check_xvmap_feasibility` 要求循环内使用的 XR 寄存器数 ≤ 16，否则直接返回 -1。
实际热点循环（如 `xcast_remove_session` 的 shuf+mac 循环）使用 24 个 XR，
其中 12 个为只读常量（系数表），在整个循环体内不做修改。

这 12 个常量 XR 不需要 xvmap 的高半部独立管理——它们从线程数据
`TD_OFF_DATA(xr, 2)` 按需加载即可，不会在循环体中被覆写。

## 设计原则

- **热 XR**（≤ 12 个）：计算中反复读写的寄存器（加载、累加、输出），走完整 xvmap
- **冷 XR**（任意数量）：只读常量，从 thread_data 按需加载高半部，不占 xvmap 条目
- **冷 XR 的高半部**：由含冷操作数的指令自行通过 block 级生成器（`gen_reg_save_vr / gen_reg_restore_vr`）管理

## 方案概述

保持 xvmap 框架完全不变，仅在循环体入口的指令分发处做条件分支：

```
对每条循环体指令:
  if 指令的所有 XR 操作数都在 hot_mask 中:
    走 lasx_interpret_inst_gen_xvmap (现有 xvmap 生成器)
  else:
    走 lasx_interpret_inst_gen (block 级生成器, 自动管理 VR)
```

## 文件修改清单

### 1. `check_xvmap_feasibility` 修改（约 20 行）

**当前**：`int check_xvmap_feasibility(p, loop_len, &xvuse, xv_insn_mask)`
- xr_cnt > 16 → 返回 -1

**修改**：新增 `uint32_t *cold_mask` 输出参数
```
int check_xvmap_feasibility(
    unsigned int *p, int loop_len,
    uint32_t *xvuse,          // 热 XR 位掩码（≤ 16）
    uint32_t *cold_mask,      // [新增] 被使用但不计入热区的 XR
    uint32_t xv_insn_mask[4]
);
```

逻辑：
```
1. 遍历循环体，收集所有 XR 到 xr_all_mask
2. 对每个 XR：在循环体内被写入（dest 操作数）→ 标记为 hot
3. 在循环体内被读取但从未写入 → 标记为 cold
4. hot_cnt = popcount(hot_mask)
5. if hot_cnt > 16 → return -1
6. total_vr 只计 hot_mask 中的 VR 占用
7. free_vr = 32 - total_vr; if free_vr < hot_cnt → return -1
8. *xvuse = hot_mask; *cold_mask = cold_mask
9. return 0
```

读写分析实现：
```
遍历所有指令:
  对每个 operands:
    if operand.kind == LA_OP_XVPR:
      xr = operand.xvpr
      all_mask |= (1 << xr)
      如果 operand 是 dest（第一个操作数）:
        written[xr] = 1
hot_mask = { xr | all_mask 有 xr 且 written[xr] }
cold_mask = { xr | all_mask 有 xr 且 !written[xr] }
```

### 2. `lasx_build_xvmap` 无需修改

当前实现已支持 `map[i] = -1` 表示未使用（冷 XR）。只需传入的
`mask` 只包含 hot XR，函数行为不变。

### 3. `generate_xvmap_loop` 修改（约 35 行）

**函数签名**: 新增 `uint32_t cold_mask` 参数
```
static void *generate_xvmap_loop(
    lagoon_assembler_t *as, unsigned int *p,
    int loop_start, int loop_len,
    uint32_t xvuse,            // hot XRs
    uint32_t cold_mask,        // [新增] cold XRs
    uint32_t xv_insn_mask[4],
    int profile_pidx
);
```

**核心修改: 指令分发生成**

原代码：
```c
for (int k = body_start; k < body_end; k++) {
    int mask_idx = k - loop_start;
    if (xv_insn_mask[mask_idx / 32] & (1u << (mask_idx & 31))) {
        if (!lasx_interpret_inst_gen_xvmap(as, p[k]))
            goto fail;
    } else {
        la_dup(as, p[k]);
    }
}
```

修改为：
```c
for (int k = body_start; k < body_end; k++) {
    int mask_idx = k - loop_start;
    if (!(xv_insn_mask[mask_idx / 32] & (1u << (mask_idx & 31)))) {
        la_dup(as, p[k]);  // 非 XV 指令，原样复制
        continue;
    }
    // XV 指令：检查操作数是否全部为 hot
    lagoon_insn_t insn;
    la_disasm_one(p[k], &insn);
    int has_cold = 0;
    for (int j = 0; j < insn.operand_count; j++) {
        if (insn.operands[j].kind == LA_OP_XVPR) {
            if (cold_mask & (1u << insn.operands[j].xvpr)) {
                has_cold = 1;
                break;
            }
        }
    }
    if (has_cold) {
        // 含冷操作数，走 block 级生成器
        // gen_reg_save_vr / gen_reg_restore_vr 自动管理冷 VR
        if (!lasx_interpret_inst_gen(as, p[k], 0))
            goto fail;
    } else {
        // 全热操作数，走 xvmap 生成器
        if (!lasx_interpret_inst_gen_xvmap(as, p[k]))
            goto fail;
    }
}
```

**注意事项**：

- `lasx_interpret_inst_gen` 内部使用 `gen_reg_save_vr` 存入 `TD_OFF_DATA`。
  冷 XR 的高半部保存在 thread_data 中，`gen_reg_save_vr` 会临时占用一个
  scratch VR（通过 `gen_reg_find_free_vrs` 分配，从空闲 VR 池中选取）。
  
- 循环体中 hot XR 仅占 12 个 temp VR，加上 32 - 12 - 12 = 8 个空闲 VR
  （12 个 hot VR 的原始 VR 槽位也在 vr_all 中），足够 block 级生成器使用。

- `TD_OFF_DATA` 使用方式不变：
  * hot XR: `TD_OFF_DATA(xr, 2)` 存储高半部，由 xvmap prologue/epilogue 管理
  * cold XR: `TD_OFF_DATA(xr, 2)` 存储完整高半部（只读，不需要回写）

### 4. `check_lasx_loop_pattern` 修改（约 10 行）

透传 `cold_mask` 并传给 `generate_xvmap_loop`：

```c
uint32_t xvuse = 0;
uint32_t cold_mask = 0;  // 新增
uint32_t xv_insn_mask[4] = {0};

if (check_xvmap_feasibility(p + loop_start, loop_len,
                             &xvuse, &cold_mask, xv_insn_mask) != 0) {
    LOOP_DBG("xvmap not feasible");
    return 0;
}

void *jit_entry = generate_xvmap_loop(as, p, loop_start, loop_len,
                                       xvuse, cold_mask, xv_insn_mask,
                                       profile_pidx);
```

### 5. scan_loops.c 适配

`check_lasx_loop_pattern` 签名变化透传即可（`cold_mask` 为内部参数，不暴露）。

## 指令分类示例 (xcast_remove_session 循环)

| 指令 | 操作数 | 含冷 XR? | 生成方式 |
|------|--------|----------|---------|
| xvld xr1, s2, 0 | xr1(dest), s2 | 全热(xr1 被写) | xvmap |
| xvld xr2, s2, 16 | xr2(dest) | 全热 | xvmap |
| xvld xr3, t6, 0 | xr3(dest) | 全热 | xvmap |
| xvpermi.d xr4, xr1, 0x4e | xr4(dest), xr1 | 全热 | xvmap |
| xvshuf.b xr5, xr4, xr1, xr17 | xr5,xr4,xr1,xr17 | **冷(xr17)** | block |
| xvmulwev.w.h xr8, xr5, xr23 | xr8,xr5,xr23 | **冷(xr23)** | block |
| xvmulwev.h.bu.b xr11, xr5, xr14 | xr11,xr5,xr14 | **冷(xr14)** | block |
| xvsrari.w xr8, xr8, 0xa | xr8(仅 hot) | 全热 | xvmap |
| xvmaxi.w xr8, xr8, 0 | xr8(仅 hot) | 全热 | xvmap |
| xvpickev.h xr7, xr9, xr8 | xr7,xr9,xr8 | 全热 | xvmap |
| vst vr4, t1, 0 | vr4,t1 | LSX 指令 | la_dup |
| vst vr12, s3, 0 | vr12,s3 | LSX 指令 | la_dup |

## JIT 结构示例

```
[prologue]
    save hot temp VRs (12 × vst)
    load hot xvmap entries (12 × vld)
    ; cold XR 不需要 prologue 操作

[body]
loop_label:
    ; profile counter (if profile enabled)
    ; ─── 全热指令，走 xvmap ───
    vld v31, s2, 16     ; xvld xr1 (xvmap: high=31, low=1)
    vld v1, s2, 0
    vld v30, s2, 48     ; xvld xr2
    vld v2, s2, 32
    vld v29, t6, 16     ; xvld xr3
    vld v3, t6, 0
    vshuf4i.d v4, v1, 0x4e   ; xvpermi.d xr4,xr1  (xvmap)
    vori.b v28, v2, 0
    ...

    ; ─── 含冷指令，走 block 级 ───
    ; gen_reg_save_vr: 保存 scratch VR 到 TD_OFF_DATA
    vst scratch, tp, TD_OFF_DATA(scratch, 0)
    ; 加载冷 XR xr17 的高半部
    vld scratch, tp, TD_OFF_DATA(17, 2)
    ; 执行主指令
    vshuf.b v5, v4, v1, v17     ; xvshuf.b xr5,xr4,xr1,xr17 (低半)
    vshuf.b scratch2, v28, v31, v17  ; 高半
    ; 存储结果高半部
    vst scratch2, tp, TD_OFF_DATA(5, 2)
    ; gen_reg_restore_vr: 恢复 scratch VR
    vld scratch, tp, TD_OFF_DATA(scratch, 0)
    ...

    ; 循环控制 (la_dup)
    ldptr.w t0, sp, 12
    addi.w t0, t0, 1
    st.w t0, sp, 12
    ldptr.w t0, sp, 12
    slli.w t0, t0, 0
    bge zero, t0, loop_label

[epilogue]
    save hot xvmap entries back (12 × vst)
    restore hot temp VRs (12 × vld)
    jiscr0
```

## 验证计划

1. 单元测试：`make unittest-loop` — 现有 123 测试全热，不受影响
2. 随机测试：`make test-block` / `make test-loop` — 703 测试全热，不受影响
3. 新增测试：构造一个使用 >16 XR 的循环，验证 cold spill 正确性
4. `xcast_remove_session` 实际热点循环验证

## 实现顺序

1. `check_xvmap_feasibility` 新增 cold_mask + 读写分析
2. `generate_xvmap_loop` 新增 cold 分支 + `lasx_interpret_inst_gen` 调用
3. `check_lasx_loop_pattern` 透传
4. 签名适配 scan_loops.c
5. 测试验证

## 预期效果

- 24 XR → 12 hot + 12 cold，xvmap 可用
- ~40% 的指令走 xvmap（全热路径，每指令 ~2 LSX）
- ~60% 的指令走 block 级（含冷路径，每指令 ~10 LSX）
- 整体 JIT 约增加 30% 代码量，但捕获整个循环为 JIT（无 SIGILL 回退）
