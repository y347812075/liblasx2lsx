# LASX 测试覆盖分析报告

## 1. 实现概览

### 已实现的指令模块 (14个源文件, 约706条指令)

| 模块 | 分类 | 指令数 | 测试状态 |
|------|------|--------|----------|
| lasx_arith.c | 算术运算 | 10 | ✓ test_xvadd.c |
| lasx_logic.c | 逻辑运算 | 6 | DEBUG test_xvand.c |
| lasx_mem.c | 内存操作 | 12 | ✓ test_xvldst.c |
| lasx_imm.c | 立即数操作 | 9 | ✓ test_xvimm.c |
| lasx_muldiv.c | 乘除法 | 94 | ✓ test_xvmul.c |
| lasx_extract.c | 提取操作 | 48 | ✓ test_xvextract.c |
| lasx_bitmanip.c | 位操作 | 45 | ✓ test_xvbit.c, test_xvbiti.c |
| lasx_cmp.c | 比较操作 | 93 | ✓ test_xvcmp.c |
| lasx_fbasic.c | 浮点基础 | 90 | ✓ test_xvfbasic.c |
| lasx_fma.c | 融合乘加 | 8 | ✓ test_xvfma.c |
| lasx_perm.c | 置换操作 | 37 | ✓ test_xvperm.c |
| lasx_extadd.c | 扩展加法 | 124 | DEBUG test_xvextadd.c |
| lasx_fcmp.c | 浮点比较 | 2 | 缺失 |
| lasx_shift.c | 移位操作 | 128 | ✓ test_xvsh.c |

## 2. 当前测试状态

### make test (14个测试通过)
- test_sigill.c - SIGILL信号处理
- test_xvadd.c - 算术运算 (xvadd, xvsub)
- test_xvfbasic.c - 浮点基础 (xvadd.s, xvsub.s, xvmul.s, xvdiv.s, xvsqrt.s, xvmax, xvmin)
- test_xvfma.c - 融合乘加 (xvfmadd, xvfnmadd, xvfmsub, xvfnmsub)
- test_xvimm.c - 立即数 (xvaddi, xvsubi, xvldi)
- test_xvcmp.c - 比较 (xvmax, xvmin, xvclo, xvclz)
- test_xvldst.c - 内存操作 (xvld, xvst)
- test_xvmul.c - 乘除法 (xvmul, xvdiv)
- test_xvperm.c - 置换操作
- test_xvsh.c - 移位操作
- test_xvext.c - 提取操作 (部分)
- test_xvbit.c - 位操作 (xvbitclr, xvbitset, xvbitrev)
- test_xvbiti.c - 位立即数 (xvbitclri, xvbitseti, xvbitrevi)
- test_debug.c - 调试测试

### make test-debug (2个测试)
- test_xvand.c - xvandn/xvorn语义差异 (QEMU vs 规范)
- test_xvextadd.c - xvsadd.w溢出处理差异

## 3. 缺失测试

### 高优先级 (指令数多)

| 模块 | 指令数 | 现有测试 | 建议测试文件 | 复杂度 |
|------|--------|----------|--------------|----------|
| lasx_shift.c | 128 | test_xvsh.c (部分) | test_xvsh_full.c | 高 |
| lasx_extadd.c | 124 | test_xvextadd.c (部分) | test_xvextadd_full.c | 高 |
| lasx_muldiv.c | 94 | test_xvmul.c (部分) | test_xvmul_full.c | 高 |
| lasx_fcmp.c | 2 | 缺失 | test_xvfcmp.c | 低 |
| lasx_fbasic.c | 90 | test_xvfbasic.c (部分) | test_xvfbasic_full.c | 中 |
| lasx_extract.c | 48 | test_xvext.c (部分) | test_xvext_full.c | 中 |
| lasx_perm.c | 37 | test_xvperm.c (部分) | test_xvperm_full.c | 中 |

### 中优先级

| 模块 | 指令数 | 现有测试 | 建议测试文件 |
|------|--------|----------|--------------|
| lasx_bitmanip.c | 45 | test_xvbit.c, test_xvbiti.c (部分) | test_xvbit_full.c |
| lasx_cmp.c | 93 | test_xvcmp.c (部分) | test_xvcmp_full.c |

## 4. 添加测试详细计划

### Phase 1: 简单测试优先

#### 4.1 test_xvfcmp.c - 浮点比较 (2条指令)
**目标**: lasx_fcmp.c
**指令**:
- xvfcmp.s / xvfcmp.d - 比较浮点数，设置标志
- xvfcmplt.s / xvfcmplt.d - 小于比较

**测试函数**: xvfcmp比较a和b, 结果在CC寄存器

**实现难度**: 低
**预计工作量**: 0.5小时

#### 4.2 扩展现有测试

##### 4.2.1 test_xvsh.c 扩展 - 移位操作
**当前覆盖**: 部分移位
**缺失指令类别**:
- xvssran / xvssrann - 带符号右移累加
- xvssrln - 右移累加无符号
- xvslai / xvslain - 左移累加立即数

**实现难度**: 中
**预计工作量**: 1-2小时

##### 4.2.2 test_xvmul.c 扩展 - 乘除法
**当前覆盖**: 基本乘除
**缺失指令类别**:
- xvmulwev / xvmulwod - 乘法累加
- xvmulwev_s / xvmulwod_s - 有符号乘法累加
- xvdivwev / xvdivwod - 除法相关

**实现难度**: 中
**预计工作量**: 2小时

##### 4.2.3 test_xvfbasic.c 扩展 - 浮点基础
**当前覆盖**: add/sub/mul/div/sqrt/max/min/recip
**缺失指令**:
- xvrecippe.h / xvrecippe.w - 近似倒数
- xvrsqrt.s / xvrsqrt.d - 平方根倒数
- xvexth / xvexth.m - 类型转换

**实现难度**: 中
**预计工作量**: 1小时

### Phase 2: 中等复杂度

#### 4.3 test_xvext_full.c - 提取操作扩展
**当前覆盖**: xvexth.b/h/w/d, vext2xv, xvextrins
**缺失指令**:
- xvneg - 取负
- xvabs - 绝对值
- xvand/xvor/xvxor 变体

**实现难度**: 中
**预计工作量**: 2小时

#### 4.4 test_xvperm_full.c - 置换操作扩展
**当前覆盖**: 部分
**缺失指令**:
- xvshuf.f / xvshuf.d - 元素置换
- xvperm - 向量置换
- xvrepl - 广播

**实现难度**: 中
**预计工作量**: 2小时

### Phase 3: 高复杂度

#### 4.5 test_xvextadd_full.c - 扩展加法
**当前测试**: xvsadd部分
**需要覆盖**: 124条指令
**推荐做法**: 
- 分类测试: 饱和加法/饱和减法/平均值/绝对值差
- 每类取2-3个代表性指令

**实现难度**: 高
**推荐策略**: 按功能块添加

#### 4.6 test_xvbit_full.c - 位操作扩展
**当前覆盖**: clr/set/rev相关
**缺失指令**:
- xvfrstp - 找第一个置位
- xvsigncov - 符号覆盖
- xvmepatmsk - 模式掩码

## 5. 执行顺序建议

### 第一批 (快速完成)
1. test_xvfcmp.c - 浮点比较 (2条指令)
2. 扩展现有测试覆盖率

### 第二批 (中等)
3. test_xvsh_full.c - 移位完整
4. test_xvmul_full.c - 乘除法完整
5. test_xvfbasic_full.c - 浮点基础完整

### 第三批 (高复杂度)
6. test_xvextadd_full.c - 按功能模块添加
7. test_xvperm_full.c - 置换完整
8. test_xvbit_full.c - 位操作完整

## 6. 验证方法

### 添加测试后验证
```bash
# 测试代码正确性
make test-native

# 测试模拟器正确性  
make test

# 测试debug版本
make test-debug
```

### 覆盖率检查
- 每个新指令类型需要2-3个测试用例
- 边界条件: 全0, 全1, 符号位, 进位

## 7. 当前已知问题

| 测试 | 问题 | 原因 |
|------|------|------|
| test_xvand | xvandn/vorn失败 | QEMU语义与规范不同 |
| test_xvextadd | xvsadd.w失败 | 溢出处理不同 |

这些测试保留在DEBUG中，等待QEMU修复

## 8. 总结

- 已实现指令: 706条
- 已测试: ~200条 (估算)
- 测试覆盖率: ~28%
- 目标: 达到60%+覆盖率

下一步行动: 
1. 先完成Phase 1的简单测试
2. 扩展现有测试覆盖率
3. 逐步添加Phase 2和Phase 3