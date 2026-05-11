[English](README.en.md)

# liblasx2lsx

LASX（LoongArch SIMD Extension，256 位向量指令）软件模拟库，通过 SIGILL 信号捕获 + LD_PRELOAD 实现。

## 应用场景

某些 LoongArch 平台（如 **2K3000**、**3B6000M** 等）只支持 LSX（128 位向量指令），不支持 LASX（256 位向量指令）。在这些平台上运行编译时开启了 LASX 优化的程序会因 SIGILL 崩溃。

本库通过 LD_PRELOAD 机制在运行时捕获 SIGILL，对 LASX 指令执行软件模拟，使程序无需修改即可正常运行。

- 如果目标程序已实现动态适配（运行时检测 LASX 支持并选择合适代码路径），则无需本库
- 本库解决的是：**编译时假设 LASX 可用，但实际硬件不支持**的场景

## 基本原理

```
应用程序 (编译时生成 LASX 指令)
    │
    ▼
┌─ CPU 执行 ──────────────────────────────────┐
│ LASX 指令 → 硬件不支持 → SIGILL 信号         │
└──────────────────────────────────────────────┘
    │
    ▼
┌─ liblasx2lsx (LD_PRELOAD) ─────────────────┐
│ sigill_handler()                            │
│   ├─ 通过 ucontext_t 读取指令码 + 寄存器状态  │
│   ├─ JIT 翻译 → LSX 指令序列 (首选)          │
│   │   (将 256 位拆为 2 × 128 位执行)         │
│   └─ 纯 C 模拟 (兜底)                       │
│   │                                         │
│ 后续执行: 用 jiscr1 替换原指令, 直接跳到     │
│           JIT 代码, 不再触发 SIGILL          │
└──────────────────────────────────────────────┘
```

JIT 翻译支持三层优化策略：

| 策略 | 说明 |
|------|------|
| **emu** | 纯 C 模拟，每条指令独立处理 |
| **block** | 批量翻译连续 LASX 指令 |
| **usedef** | use/def 分析优化寄存器分配（默认） |
| **loop** | 循环检测 + 整体 JIT 翻译（默认） |

## 编译要求

```bash
gcc -mlsx                           # 编译本库需要 -mlsx 支持 (LSX 128位指令)
```

如果只需使用（不编译），直接下载预编译的 `.so` 即可，无需安装任何工具。

## 编译

```bash
make          # 编译全部：liblasx2lsx.so + 调试版
make clean    # 清理编译产物
make debug    # 仅调试版 (liblasx2lsx_dbg.so)
make profile # 性能分析版 (liblasx2lsx_profile.so)
```

LoongArch 主机使用 `gcc` 本地编译。

## 使用

### 在真实 LoongArch 设备上

```bash
# 1. 编译库
make

# 2. 运行你的程序
LD_PRELOAD=./liblasx2lsx.so ./your_loongarch_binary

# 3. 如果是多线程程序，正常工作（线程安全）
LD_PRELOAD=./liblasx2lsx.so ./your_multithreaded_program
```

### 优化模式控制

通过环境变量 `LIBLASX2LSX_INTERPRET_MODE` 控制：

```bash
# 默认优化组合（推荐）
LD_PRELOAD=./liblasx2lsx.so ./your_binary

# 关闭 JIT，仅纯模拟（调试用）
LIBLASX2LSX_INTERPRET_MODE=emu LD_PRELOAD=./liblasx2lsx.so ./your_binary

# block 模式（无 use/def 优化）
LIBLASX2LSX_INTERPRET_MODE=block LD_PRELOAD=./liblasx2lsx.so ./your_binary
```

模式说明：

| 模式 | 含义 |
|------|------|
| `usedef` | use/def 批量翻译优化 |
| `loop` | 循环检测与 JIT |
| `frag` | 片段 xvmap 优化 |
| `block` | 传统块翻译（与 usedef 互斥） |
| `emu` / `one` | 纯模拟，关闭所有优化 |

### 禁用整个模拟

```bash
LIBLASX2LSX_INTERPRET=off LD_PRELOAD=./liblasx2lsx.so ./your_binary
```

### LD_PRELOAD 机制说明

`LD_PRELOAD` 是 Linux 动态链接器的环境变量。设置后，指定的共享库会在所有其他库之前加载。本库利用此机制：

1. 库的构造函数 `register_sigill_handler()` 在进程启动时自动执行
2. 注册 SIGILL 信号处理器
3. 程序执行中遇到 LASX 指令 → 硬件 SIGILL → 被本库捕获 → 软件模拟
4. 首次模拟后，用 `jiscr1` 指令原地替换原 LASX 指令，后续执行直接跳转 JIT 代码，不再触发 SIGILL

整个过程对目标程序完全透明，无需修改代码或重新链接。

## 测试

```bash
make test                        # 默认优化模式（703 条随机测试）
make test-one                    # 纯模拟模式
make test-single TEST=xvadd.b    # 单条指令测试
```

## 项目结构

```
src/
├── include/          # 头文件
├── emu/              # SIGILL 捕获 + 指令模拟 + 线程数据
├── interpret/        # JIT 解释层（批量翻译 + 优化）
└── lagoon.c          # LoongArch 汇编器（指令编码发射）

tools/                # 辅助脚本
tests/                # 单元测试
random_test/          # 随机测试框架（703 条指令）
doc/
├── development.md    # 开发者文档：架构、调试、改进方向
└── ...               # 其他技术文档
```

## 开发者参考

详细的架构说明、调试方法和已知 Bug 模式见：

- [doc/development.md](doc/development.md) — 开发者文档

## 许可

MIT
