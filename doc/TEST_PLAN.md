# LASX 指令测试计划

## 当前状态

已实现指令 (723个):
- lasx_arith.c (10): xvadd, xvsub
- lasx_logic.c (6): xvand, xvor, xvxor, xvnor, xvandn, xvorn
- lasx_mem.c (12): xvld, xvst, xvldrepl, xvstelm
- lasx_imm.c (9): 立即数操作
- lasx_muldiv.c (94): 乘除法
- lasx_extract.c (60): 提取操作
- lasx_bitmanip.c (44): 位操作
- lasx_cmp.c (93): 比较操作
- lasx_fbasic.c (96): 基础浮点
- lasx_fma.c (8): 融合乘加
- lasx_perm.c (37): 置换操作
- lasx_extadd.c (124): 扩展加法
- lasx_shift.c (128): 移位操作
- lasx_fcmp.c (2): 浮点比较

现有测试覆盖:
- test_xvadd: xvadd/xvsub (b/h/w/d/q)
- test_xvand: xvand/xvor/xvxor/xvnor/xvandn/xvorn
- test_sigill: xvld/xvst/vst

## 缺失测试

### 高优先级
1. **Memory** - xvld/xvst 变体、xvldrepl、xvstelm
2. **Shift** - xvshl/xvshr (算术/逻辑)
3. **Compare** - xvslt/xvsle/xvsgt/xvsge/xveq/xvne
4. **Bitmanip** - xvbitclr/xvbitset/xvbitrev

### 中优先级
5. **Mul/Div** - xvmul/xvdiv (有符号/无符号)
6. **Extract** - xvextl/xvextsl/xvextsh/xvextwh
7. **Perm** - xvpermi/xvshuf/xvilvh/xvilvl
8. **Imm** - xvaddi/xvslli/xvsrli

### 低优先级
9. **Floating Point** - xvfadd/xvfsub/xvfmul/xvfdiv
10. **ExtAdd** - xvaddw/xvsubw variants
11. **FMA** - xvfmadd/xvfmsub

## 测试文件规划

```
tests/
├── test_sigill.c     # 已存在 - SIGILL捕获测试
├── test_xvadd.c      # 已存在 - 算术测试
├── test_xvand.c      # 已存在 - 逻辑测试
├── test_xvld.c       # 新增 - 内存加载测试
├── test_xvst.c       # 新增 - 内存存储测试
├── test_xvsh.c       # 新增 - 移位测试
├── test_xvcmp.c      # 新增 - 比较测试
├── test_xvbit.c      # 新增 - 位操作测试
├── test_xvmul.c      # 新增 - 乘除测试
├── test_xvext.c      # 新增 - 提取测试
├── test_xvperm.c     # 新增 - 置换测试
├── test_xvimm.c      # 新增 - 立即数测试
├── test_xvfpu.c      # 新增 - 浮点测试 (可选)
└── test_all.sh       # 新增 - 运行所有测试脚本
```

## 测试策略

每个测试文件应:
1. 使用内联汇编触发 LASX 指令
2. 与 QEMU 硬件执行结果对比
3. 使用 `-cpu la464,lasx=off` 触发模拟
4. 使用 `-cpu la464` 验证硬件执行
5. 对比两者结果确保模拟正确

## 执行计划

Phase 1 (本周):
- test_xvld.c - 内存加载测试
- test_xvsh.c - 移位测试  
- test_xvcmp.c - 比较测试

Phase 2 (下周):
- test_xvbit.c - 位操作测试
- test_xvmul.c - 乘除测试
- test_xvext.c - 提取测试

Phase 3 (后续):
- test_xvperm.c - 置换测试
- test_xvimm.c - 立即数测试
- test_xvfpu.c - 浮点测试 (如果需要)