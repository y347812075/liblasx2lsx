# 调试随机测试经验总结

本文档记录调试 LASX 指令模拟器随机测试的常见问题和解决方案。

## 1. 调试流程

### 1.1 基本调试步骤

```bash
# 1. 先用 native 模式验证测试代码正确性（QEMU 原生 LASX）
make test-single-native TEST=xxx

# 2. 再用模拟器模式测试
make test-single TEST=xxx

# 3. 如果 native 通过但 emulator 失败，说明模拟器实现有问题
# 4. 如果都失败，检查测试代码预期值是否正确
```

### 1.2 调试单条指令

```bash
# 编译并运行单个随机测试
loongarch64-unknown-linux-gnu-gcc -o random_test/test/xxx random_test/src/xxx.c
LD_PRELOAD=./liblasx2lsx.so qemu-loongarch64-static -cpu la464,lasx=off ./random_test/test/xxx -f random_test/random_data

# 与预期结果对比
./random_test/test/xxx -f random_test/random_data > output.txt
diff output.txt random_test/result/xxx.exe.result
```

## 2. 常见问题

### 2.1 字段解析错误

**问题**：指令解析时使用了错误的 bit 位置。

**排查方法**：
1. 查看 `insns.decode` 文件确定指令格式
2. 使用 objdump 查看实际生成的机器码
3. 手动解析各字段

**示例**（xvfcmp）：
```
# insns.decode 定义
&vvv_fcond    vd vj vk fcond
@vvv_fcond    .... ........ fcond:5  vk:5 vj:5 vd:5    &vvv_fcond

# 机器码 0c910020 (xvfcmp.clt.s $xr0, $xr1, $xr0)
# 二进制: 0000 1100 1001 0001 0000 0010 0000 0000
# 解析:
#   bits 4:0 (fcond)   = 0b00010 = 2  (CLT)
#   bits 9:5 (xk)      = 0b00000 = 0  (xr0)
#   bits 14:10 (vj)    = 0b00001 = 1  (xr1)
#   bits 19:15 (vd)    = 0b00000 = 0  (xr0)
```

**正确代码**：
```c
int fcond = (instr >> 15) & 0x1F;  // 注意是 >> 15，不是 >> 16
int xd = instr & 0x1f;              // 注意是 instr，不是 instr >> 0
int xj = (instr >> 5) & 0x1f;
int xk = (instr >> 10) & 0x1f;
```

### 2.2 输出格式错误

**问题**：输出值格式与文档描述不符。

**排查方法**：
1. 查阅 `lasx.md` 获取指令的伪代码
2. 使用 objdump 查看 native 执行结果
3. 对比预期值和实际值

**常见错误**：
- xvfcmp：输出应该是每个元素全为1或全为0，不是单个 bit
- xvclo/xvclz：输出应该是 8-bit 计数，不是 32-bit 全宽

**xvfcmp 正确实现**：
```c
// Single precision: 每个 32 位元素全为 1 或全为 0
if (cond) {
    dst |= (j == 0) ? 0xffffffffULL : (0xffffffffULL << 32);
}

// Double precision: 每个 64 位元素全为 1 或全为 0
uint64_t dst = cond ? 0xFFFFFFFFFFFFFFFFULL : 0ULL;
```

### 2.3 寄存器顺序错误

**问题**：指令语义是 `rd = rk & ~rj`，但实现成了 `rd = rj & ~rk`。

**常见错误指令**：
| 指令 | 正确语义 | 常见错误 |
|------|---------|---------|
| `xvandn.v` | `rd = rk & ~rj` | `rd = rj & ~rk` |
| `xvorn.v` | `rd = rk \| ~rj` | `rd = rj \| ~rk` |

### 2.4 大小端问题

**问题**：测试预期的字节序与实际不符。

**现象**：
- 测试在 test-native 和 test 中都失败，错误值完全相同
- 错误值呈现出镜像或字节交换模式

**排查**：检查内存布局和元素在向量中的存储顺序。

## 3. 调试技巧

### 3.1 使用 objdump 分析指令

```bash
# 查看生成的指令
loongarch64-unknown-linux-gnu-objdump -d test_program | grep xvfcmp

# 输出示例:
# 120000980:  0c910020  xvfcmp.clt.s $xr0, $xr1, $xr0
```

### 3.2 使用 DEBUG 版本库

```bash
# 编译 debug 版本
make all  # 自动生成 liblasx2lsx_dbg.so

# 运行并查看日志
LD_PRELOAD=./liblasx2lsx_dbg.so qemu-loongarch64-static -cpu la464,lasx=off ./test_program

# 日志文件在 /tmp/lasx-<pid>-<tid>.log
cat /tmp/lasx-*.log
```

### 3.3 添加临时调试输出

在 `lasx_*.c` 中添加 tdlog：
```c
tdlog("%16lx : emulate xvfcmp_cond_s fcond=%d xd=%d xj=%d xk=%d %08x\n", 
      (unsigned long)UC_PC(uc), fcond, xd, xj, xk, instr);
```

注意：tdlog 只在 DEBUG 模式下有效（编译时加了 -DDEBUG）。

### 3.4 使用内联汇编测试

当 intrinsics 有问题时，用内联汇编验证：

```c
// test_asm.c
uint64_t data[6][4] __attribute__((aligned(32)));

int main() {
    float a = 1.0f, b = 2.0f;
    *((float*)data[0]) = a;
    *((float*)data[0]+1) = b;
    
    __asm__ volatile(
        "xvld $xr0, %0, 0\n"
        "xvld $xr1, %0, 32\n"
        "xvfcmp.clt.s $xr2, $xr1, $xr0\n"
        "xvst $xr2, %0, 64\n"
        ::"r"(data):"memory","$xr0","$xr1","$xr2");
    
    printf("Result: 0x%016lx 0x%016lx\n", data[2][0], data[2][1]);
    return 0;
}
```

```bash
loongarch64-unknown-linux-gnu-gcc -mlasx -o test_asm test_asm.c
LD_PRELOAD=./liblasx2lsx.so qemu-loongarch64-static -cpu la464,lasx=off ./test_asm
```

### 3.5 检查 LD_PRELOAD 是否生效

```bash
LD_DEBUG=libs LD_PRELOAD=./liblasx2lsx.so qemu-loongarch64-static -cpu la464,lasx=off ./test_program 2>&1 | grep liblasx2lsx
```

应该看到：
```
2974530:    calling init: ./liblasx2lsx.so
```

### 3.6 检查 SIGILL 是否触发

```bash
# 不使用 LD_PRELOAD，看是否会 SIGILL
qemu-loongarch64-static -cpu la464,lasx=off ./test_program
# 输出: qemu: uncaught target signal 4 (Illegal instruction) - core dumped

# 使用 LD_PRELOAD，应该正常运行
LD_PRELOAD=./liblasx2lsx.so qemu-loongarch64-static -cpu la464,lasx=off ./test_program
```

## 4. QEMU 注意事项

### 4.1 LASX 模式差异

```bash
# QEMU 原生 LASX（硬件模拟）
qemu-loongarch64-static -cpu la464 ./test_program

# QEMU 禁用 LASX（触发模拟）
qemu-loongarch64-static -cpu la464,lasx=off ./test_program
```

### 4.2 已知 QEMU 问题

某些指令在 QEMU 原生 LASX 模式下会触发 SIGILL：
- 需要使用 intrinsics 绕过
- 或者将测试加入 `TESTS_DEBUG`

## 5. 测试代码问题

### 5.1 测试预期值计算错误

**现象**：
- test-single-native 失败
- test-single 也失败
- 错误值完全相同

**解决**：检查测试代码的预期值计算逻辑。

### 5.2 内存布局问题

**常见错误**：
- 源/目标内存偏移计算错误
- 对齐问题（应使用 `__attribute__((aligned(32)))`）

### 5.3 intrinsics vs 内联汇编

| 特性 | Intrinsics | 内联汇编 |
|-----|------------|---------|
| 汇编器依赖 | 无 | 需要 binutils |
| 兼容性 | 更好 | 受限于版本 |

当 binutils 不支持某些指令时，使用 intrinsics：
```c
#include <lasxintrin.h>
__m256i r = __lasx_xvfcmp_clt_s(a, b);
```

## 6. 修复检查清单

添加/修复新指令后：

- [ ] 运行 `make test-single TEST=xxx` 验证
- [ ] 运行 `make test-single-native TEST=xxx` 验证（如果适用）
- [ ] 使用 clang-format 格式化代码
- [ ] 检查是否有内存泄漏或越界

## 7. 常见错误模式汇总

| 错误类型 | 错误代码 | 正确代码 |
|---------|---------|---------|
| 错误的 bit 位置 | `(instr >> 16) & 0x1F` | `(instr >> 15) & 0x1F` |
| 错误的写粒度 | `vreg_write_64` 写 16-bit | `vreg_write_16` |
| 寄存器顺序反 | `rd = rj & ~rk` | `rd = rk & ~rj` |
| 输出格式错 | `dst \|= (1ULL << j)` | `dst \|= 0xffffffffULL` |

### 7.1 Read-Modify-Write Bug

这是最常见且最严重的 bug。核心原则是：**循环中如果需要读取 xd 的原始值，必须使用 vreg_read_X 函数从寄存器读取，不能使用已修改的临时变量**。

**原则：按元素宽度循环，使用对应的 vreg_write/read**

根据输出元素的宽度选择正确的函数，不要按 4 个 slot 处理：
| 元素宽度 | 读写函数 | 循环次数（元素个数） |
|---------|---------|---------------------|
| 8-bit | `vreg_read_8` / `vreg_write_8` | 32/16/8/4 |
| 16-bit | `vreg_read_16` / `vreg_write_16` | 16/8/4/2 |
| 32-bit | `vreg_read_32` / `vreg_write_32` | 8/4/2/1 |
| 64-bit | `vreg_read_64` / `vreg_write_64` | 4/2/1 |

**情况1：xd 与 xj/xk 相同**
当目标寄存器 xd 与源寄存器 xj 或 xk 相同时，必须确保每次迭代从原始值计算，而不是从已修改的 xd 读取：

```c
// 错误写法（有严重 bug）
for (int i = 0; i < 16; i++) {
    uint64_t src_j = vreg_read_64(uc, td, xj, i / 4);
    uint64_t src_k = vreg_read_64(uc, td, xk, i / 4);
    // ... 计算 ...
    // 如果 xd == xj 或 xd == xk，错误！
    uint64_t dst = vreg_read_64(uc, td, xd, i / 4);
    // ... 修改 dst ...
    vreg_write_64(uc, td, xd, i / 4, dst);
}

// 正确写法：只从 xj/xk 读取计算，不在循环中读取 xd
for (int i = 0; i < 16; i++) {
    uint64_t src_j = vreg_read_64(uc, td, xj, i / 4);  // 从 xj 读
    uint64_t src_k = vreg_read_64(uc, td, xk, i / 4);  // 从 xk 读

    // 从 src_j/src_k 提取元素并计算
    uint16_t val_j = (src_j >> ((i % 4) * 16)) & 0xffff;
    uint16_t val_k = (src_k >> ((i % 4) * 16)) & 0xffff;
    uint16_t result = val_j + val_k;

    // 使用正确的写粒度：16 个 16-bit 元素
    vreg_write_16(uc, td, xd, i, result);
}
```

**情况2：临时变量在循环中被修改，需要读取 xd 原始值**
如果循环中修改了临时变量，同时又需要读取 xd 的原始值参与计算，**必须用 vreg_read_X 读取 xd（从寄存器读原始值），不能用临时变量**：

```c
// 错误：临时变量被修改后，又用临时变量计算
uint64_t xd_copy = vreg_read_64(uc, td, xd, 0);  // 先读出来
for (int i = 0; i < 8; i++) {
    // 错误：使用 xd_copy（已被修改的值），而不是 xd 的原始值
    uint64_t result = xd_copy + ...;  // 错！
    xd_copy = result;  // 修改了临时变量
}

// 正确做法：如果需要 xd 原始值，每次迭代都从寄存器读取
for (int i = 0; i < 8; i++) {
    // 每次都从寄存器读取 xd 的原始值
    uint64_t xd_orig = vreg_read_64(uc, td, xd, 0);

    uint64_t src_j = vreg_read_64(uc, td, xj, 0);
    uint64_t src_k = vreg_read_64(uc, td, xk, 0);

    // 使用 xd_orig（原始值）参与计算
    uint32_t val_j = (src_j >> (i * 32)) & 0xffffffff;
    uint32_t val_k = (src_k >> (i * 32)) & 0xffffffff;
    uint32_t xd_val = (xd_orig >> (i * 32)) & 0xffffffff;
    uint32_t result = val_j + val_k + xd_val;

    // 写入
    vreg_write_32(uc, td, xd, i, result);
}

// 或者：如果只需要修改部分元素，先把 xd 全部读出来
// 然后修改临时变量，最后一次性写回（适用于批量处理的场景）
{
    uint64_t dst0 = vreg_read_64(uc, td, xd, 0);
    uint64_t dst1 = vreg_read_64(uc, td, xd, 1);
    uint64_t dst2 = vreg_read_64(uc, td, xd, 2);
    uint64_t dst3 = vreg_read_64(uc, td, xd, 3);

    // 处理 H[0-3] -> dst0
    for (int i = 0; i < 4; i++) {
        uint64_t src_j = vreg_read_64(uc, td, xj, 0);
        uint64_t src_k = vreg_read_64(uc, td, xk, 0);
        uint16_t val_j = (src_j >> (i * 16)) & 0xffff;
        uint16_t val_k = (src_k >> (i * 16)) & 0xffff;
        uint16_t res = val_j + val_k;
        dst0 &= ~(0xffffULL << (i * 16));
        dst0 |= (uint64_t)res << (i * 16);
    }

    // ... 处理其他 slot ...

    // 最后一次性写回
    vreg_write_64(uc, td, xd, 0, dst0);
    vreg_write_64(uc, td, xd, 1, dst1);
    vreg_write_64(uc, td, xd, 2, dst2);
    vreg_write_64(uc, td, xd, 3, dst3);
}
```

## 8. 调试命令速查

```bash
# 运行随机测试
make test-single TEST=xxx
make test-single-native TEST=xxx

# 编译测试
loongarch64-unknown-linux-gnu-gcc -mlasx -o test tests/test_xxx.c

# 运行测试
LD_PRELOAD=./liblasx2lsx.so qemu-loongarch64-static -cpu la464,lasx=off ./test

# 查看反汇编
loongarch64-unknown-linux-gnu-objdump -d test | grep xvxxx

# 调试版本
LD_PRELOAD=./liblasx2lsx_dbg.so qemu-loongarch64-static -cpu la464,lasx=off ./test
cat /tmp/lasx-*.log
```

## 9. 溢出处理经验

### 9.1 平均指令 (xvavg/xvavgr)

**核心原则**：加法前扩展位宽，防止溢出。

| 指令类型 | 源宽度 | 扩展宽度 | 示例 |
|---------|-------|---------|------|
| `_b` | 8-bit | 16-bit | `int16_t res = (int16_t)a + (int16_t)b` |
| `_h` | 16-bit | 32-bit | `int32_t res = (int32_t)a + (int32_t)b` |
| `_w` | 32-bit | 64-bit | `int64_t res = (int64_t)a + (int64_t)b` |
| `_d` | 64-bit | 128-bit | `__int128_t res = (__int128_t)a + (__int128_t)b` |

**正确实现**：
```c
// xvavg.b: 8位带符号平均
for (int i = 0; i < 32; i++) {
    int8_t src_j = vreg_read_8(uc, td, xj, i);
    int8_t src_k = vreg_read_8(uc, td, xk, i);
    int16_t a = (int16_t)src_j;    // 扩展到 16-bit
    int16_t b = (int16_t)src_k;
    int16_t res = (a + b) >> 1;    // 16-bit 加法不会溢出
    vreg_write_8(uc, td, xd, i, (int8_t)res);
}

// xvavg.w: 32位带符号平均
for (int i = 0; i < 8; i++) {
    int32_t src_j = vreg_read_32(uc, td, xj, i);
    int32_t src_k = vreg_read_32(uc, td, xk, i);
    int64_t a = (int64_t)src_j;    // 扩展到 64-bit
    int64_t b = (int64_t)src_k;
    int64_t res = (a + b) >> 1;
    vreg_write_32(uc, td, xd, i, (int32_t)res);
}

// xvavg.d: 64位带符号平均
for (int i = 0; i < 4; i++) {
    int64_t src_j = vreg_read_64(uc, td, xj, i);
    int64_t src_k = vreg_read_64(uc, td, xk, i);
    __int128_t a = (__int128_t)src_j;  // 扩展到 128-bit
    __int128_t b = (__int128_t)src_k;
    __int128_t res = (a + b) >> 1;
    vreg_write_64(uc, td, xd, i, (int64_t)res);
}
```

**xvavgr（带舍入）**：
```c
// xvavgr.b: 加上 1 再除以 2
int16_t res = (a + b + 1) >> 1;

// xvavgr.bu: 无符号版本
uint16_t res = (a + b + 1) >> 1;
```

### 9.2 常见错误

**错误1：使用除法而非移位**
```c
// 错误：负数除法向零取整，不是算术移位
uint8_t res = (a + b) / 2;

// 正确：算术移位
int16_t res = (a + b) >> 1;
```

**错误2：位宽不足导致溢出**
```c
// 错误：8+8=16 可能溢出到 char
int8_t res = (a + b) >> 1;  // a,b 是 int8_t

// 正确：先扩展到位宽
int16_t res = ((int16_t)a + (int16_t)b) >> 1;
```

**错误3：使用错误的返回类型**
```c
// 错误：vreg_write_64 写入 int64_t 时转成 uint64_t
vreg_write_64(uc, td, xd, i, (uint64_t)(int64_t)res);

// 正确：直接写入 int64_t
vreg_write_64(uc, td, xd, i, (int64_t)res);
```

### 9.3 vreg_read/write 函数选择

| 元素宽度 | 读函数 | 写函数 | 循环次数 |
|---------|-------|-------|---------|
| 8-bit | `vreg_read_8` / `vreg_read_u8` | `vreg_write_8` | 32 |
| 16-bit | `vreg_read_16` / `vreg_read_u16` | `vreg_write_16` | 16 |
| 32-bit | `vreg_read_32` / `vreg_read_u32` | `vreg_write_32` | 8 |
| 64-bit | `vreg_read_64` | `vreg_write_64` | 4 |

**优势**：
1. 自动处理 slot 计算
2. 避免手动位操作错误
3. 代码更清晰易读

## 10. xvshuf.b 调试经验

### 10.1 指令简介

`xvshuf.b` 是 4 寄存器变体向量混洗指令：
```asm
xvshuf.b $xd, $xj, $xk, $xa
```

- `$xd`：目标寄存器
- `$xj`：源寄存器1
- `$xk`：源寄存器2
- `$xa`：选择器寄存器

**语义**：用 `$xa` 中的值作为选择器，从 `$xj` 和 `$xk` 的256位数据中选择字节到 `$xd`。

### 10.2 关键点

1. 选择器来自第4个寄存器（xa），不是 xd
2. 每个64位字使用对应的8字节选择器
3. 前128位用 xa_d[0]/xa_d[1]，后128位用 xa_d[2]/xa_d[3]
4. 选择值 <16 从 xk 取，>=16 从 xj 取

### 10.3 正确实现

```c
void do_lasx_emu_xvshuf_b(ucontext_t* uc, unsigned int instr)
{
    thread_data_t* td = thread_data_get();

    int xd = (instr >> 0) & 0x1f;
    int xj = (instr >> 5) & 0x1f;
    int xk = (instr >> 10) & 0x1f;
    int xa = (instr >> 15) & 0x1f;

    uint64_t xj_d[4] = {
        vreg_read_64(uc, td, xj, 0),
        vreg_read_64(uc, td, xj, 1),
        vreg_read_64(uc, td, xj, 2),
        vreg_read_64(uc, td, xj, 3)
    };
    uint64_t xk_d[4] = {
        vreg_read_64(uc, td, xk, 0),
        vreg_read_64(uc, td, xk, 1),
        vreg_read_64(uc, td, xk, 2),
        vreg_read_64(uc, td, xk, 3)
    };
    uint64_t xa_d[4] = {
        vreg_read_64(uc, td, xa, 0),
        vreg_read_64(uc, td, xa, 1),
        vreg_read_64(uc, td, xa, 2),
        vreg_read_64(uc, td, xa, 3)
    };

    int m = 16;

    for (int i = 0; i < 32; i++) {
        // 确定使用哪个64位选择器：前16字节用 xa_d[0]/xa_d[1]，后16字节用 xa_d[2]/xa_d[3]
        int half = i < 16 ? 0 : 1;
        int sel_word_idx = half * 2 + ((i % 16) / 8);  // 0,0,1,1 对应4个64位字
        int sel_byte_offset = (i % 8) * 8;          // 每个64位字中的8个字节偏移

        // 提取选择器值
        int sel = (xa_d[sel_word_idx] >> sel_byte_offset) & 0xff;
        int k = sel % 32;

        uint8_t src_byte_val;

        if (k < 16) {
            // 从 xk 取（值范围 0-15）
            int src_idx = k + half * 16;
            int src_dword = src_idx / 8;
            int bit_shift = (src_idx % 8) * 8;
            src_byte_val = (xk_d[src_dword] >> bit_shift) & 0xff;
        } else {
            // 从 xj 取（值范围 16-31）
            int src_idx = k - 16 + half * 16;
            int src_dword = src_idx / 8;
            int bit_shift = (src_idx % 8) * 8;
            src_byte_val = (xj_d[src_dword] >> bit_shift) & 0xff;
        }
        vreg_write_8(uc, td, xd, i, src_byte_val);
    }
}
```

### 10.4 常见错误

1. **选择器索引错误**：使用 `(i % 16)` 或 `(i % 8)` 错误
   - 正确：`sel_word_idx = half * 2 + ((i % 16) / 8)`

2. **选择器偏移错误**：应该用每个64位字的8字节选择器
   - 前16字节用 xa_d[0] 和 xa_d[1]，后16字节用 xa_d[2] 和 xa_d[3]

3. **源数据选择错误**：k < 16 从 xk 取，否则从 xj 取

4. **源索引计算错误**：
   - k < 16：`src_idx = k + half * 16`
   - k >= 16：`src_idx = k - 16 + half * 16`

### 10.5 调试技巧

添加临时调试输出：
```c
if (i < 4 || i == 16) {
    fprintf(stderr, "i=%d half=%d sel_word=%d sel_byte_off=%d sel=%d k=%d src_idx=%d src_dword=%d val=0x%02x\n",
            i, half, sel_word_idx, sel_byte_offset, sel, k, src_idx, src_dword, src_byte_val);
}
```

### 10.6 与 xvshuf.h/w/d 的区别

xvshuf.b 是4寄存器变体（有 xa 选择器），而 xvshuf.h/w/d 是3寄存器变体（选择器在 xd 中）：
- xvshuf.b：`$xd, $xj, $xk, $xa`（4个寄存器）
- xvshuf.h：`$xd, $xj, $xk`（3个寄存器，xd 同时作为目标和选择器）

## 11. 饱和舍入移位指令 (xvssrani/xvssrarni) 调试经验

### 11.1 指令简介

xvssrani/xvssrarni 系列指令执行饱和算术右移（有/无舍入）：

| 指令 | 源宽度 | 目标宽度 | 说明 |
|------|-------|---------|------|
| `xvssrani_b_h` | 16-bit | 8-bit | 有符号饱和算术移位 |
| `xvssrarni_b_h` | 16-bit | 8-bit | 有符号饱和舍入算术移位 |
| `xvssrani_bu_h` | 16-bit | 8-bit | 无符号饱和算术移位 |
| `xvssrarni_bu_h` | 16-bit | 8-bit | 无符号饱和舍入算术移位 |
| `xvssrani_d_q` | 128-bit | 64-bit | Q->D 移位（需要128位操作） |
| `xvssrarni_d_q` | 128-bit | 64-bit | Q->D 舍入移位 |

### 11.2 关键点

1. **Q -> D 移位需要 128 位操作**：128 位向量元素需要使用 `vreg_read_w128`/`vreg_write_w128`
2. **饱和处理**：移位后需检查是否超过目标类型范围
3. **舍入处理**：`rounded = shifted + (shifted_val >> (shamt - 1)) & 1`

### 11.3 辅助函数设计

将通用逻辑抽取为辅助函数，提高代码可维护性：

```c
// 有符号饱和算术右移
static int8_t ssrans_b_h(int16_t val, uint32_t shamt)
{
    int64_t shifted_val = val >> shamt;
    if (shifted_val > INT8_MAX) shifted_val = INT8_MAX;
    if (shifted_val < INT8_MIN) shifted_val = INT8_MIN;
    return (int8_t)shifted_val;
}

// 有符号饱和舍入算术右移
static int8_t ssrarns_b_h(int16_t val, uint32_t shamt)
{
    int64_t shifted_val = (val >> shamt) + ((shamt > 0) ? (val >> (shamt - 1)) & 1 : 0);
    if (shifted_val > INT8_MAX) shifted_val = INT8_MAX;
    if (shifted_val < INT8_MIN) shifted_val = INT8_MIN;
    return (int8_t)shifted_val;
}

// 无符号饱和算术右移
static uint8_t ssranu_bu_h(uint16_t val, uint32_t shamt)
{
    uint64_t shifted_val = ((int16_t)val < 0) ? 0 : val >> shamt;
    if (shifted_val > UINT8_MAX) shifted_val = UINT8_MAX;
    return (uint8_t)shifted_val;
}

// 无符号饱和舍入算术右移
static uint8_t ssrarnu_bu_h(uint16_t val, uint32_t shamt)
{
    uint64_t shifted_val = ((int16_t)val < 0) ? 0 :
        (val >> shamt) + ((shamt > 0) ? (val >> (shamt - 1)) & 1 : 0);
    if (shifted_val > UINT8_MAX) shifted_val = UINT8_MAX;
    return (uint8_t)shifted_val;
}
```

### 11.4 Q -> D 移位实现

```c
// 读取 128 位向量元素
__int128_t xj_q = vreg_read_w128(uc, td, xj, i);

// 执行移位
int64_t result = ssrans_d_q(xj_q, shamt);

// 写入结果
vreg_write_64(uc, td, xd, i, result);
```

### 11.5 常见错误

1. **Q->D 移位使用 64 位操作**：D 是 128 位向量元素，需要使用 128 位读写函数
2. **负数移位处理错误**：无符号版本需检查符号位，为负则置零
3. **舍入逻辑错误**：舍入位应该是 `(val >> (shamt - 1)) & 1`，shamt=0 时不执行舍入
4. **移位量位数错误**：D->Q (7bit=0x7f) vs D->D (6bit=0x3f)

### 11.7 xvssrlni_d_q/du_q 与 xvssrani_d_q/du_q 的区别

| 指令前缀 | 操作类型 | 说明 |
|---------|---------|------|
| `xvssrlni_` | 逻辑移位 | ssrlns/ssrlnu，符号位扩展 |
| `xvssrani_` | 算术移位 | ssrans/ssranu，负数清零 |

两者都使用 128 位 Q 操作，输出 4 个 64 位 D 元素。

### 11.6 vreg_read_w128/vreg_write_w128 使用

```c
// 读取 Q[0] 和 Q[1]（每个 Q 是 128 位 = 2 个 64 位 slot）
__int128_t xj_q[2];
for (int i = 0; i < 2; i++) {
    xj_q[i] = vreg_read_w128(uc, td, xj, i);
}

// 写入结果到 D[0-3]
vreg_write_64(uc, td, xd, 0, ssrans_d_q(xj_q[0], shamt));
vreg_write_64(uc, td, xd, 1, ssrans_d_q(xd_q[0], shamt));
vreg_write_64(uc, td, xd, 2, ssrans_d_q(xj_q[1], shamt));
vreg_write_64(uc, td, xd, 3, ssrans_d_q(xd_q[1], shamt));
```

**注意**：vreg_read_w128 的 idx 参数范围是 0-1（对应 Q[0] 和 Q[1]），每个 Q 由两个连续的 64 位 slot 组成。