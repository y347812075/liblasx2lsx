# LSX (LoongArch SIMD) 指令编码汇总

本文档从 QEMU 的 `insns.decode` 文件中提取所有 LSX 指令的编码信息。
QEMU is Copyright (c) 2023-2026 QEMU contributors, licensed under GPL-2.0-only.

## 前言

### LSX 指令总体编码格式

LSX 指令为 **32 位定长编码**，编码布局如下（从高位到低位，即比特 31 到比特 0）：

```
 31 28 27    16 15  11 10   5  4  0
+----+----------+------+------+------+
| op  |  op     | rk   | rj   | vd   |  (vvv 格式)
+----+----------+------+------+------+
| op  |  op          | rj   | vd   |  (vv 格式)
+----+--------------+------+------+
```

字段说明：

- `op`: 操作码（最高位部分）
- `rj`, `rk`: 源向量寄存器编号（5 比特，0-31）
- `vd`: 目的向量寄存器编号（5 比特，0-31）
- `imm`: 立即数字段，长度根据格式不同变化

LSX 是 LoongArch 的基础 SIMD 扩展，支持 128 位向量操作，每个向量可以容纳：
- 16 个 8 位整数
- 8 个 16 位整数
- 4 个 32 位整数
- 2 个 64 位整数
- 4 个 32 位浮点数
- 2 个 64 位浮点数

## 特殊字段定义

```
%i14s2     10:s14       !function=shl_2
%sa2p1     15:2         !function=plus_1
%offs21    0:s5 10:16   !function=shl_2
%offs16    10:s16       !function=shl_2
%offs26    0:s10 10:16  !function=shl_2
%i9s3     10:s9       !function=shl_3
%i10s2    10:s10      !function=shl_2
%i11s1    10:s11      !function=shl_1
%i8s3     10:s8       !function=shl_3
%i8s2     10:s8       !function=shl_2
%i8s1     10:s8       !function=shl_1
```


## 参数集定义

```
&cv           cd vj
&rv_i         rd vj imm
&v_i          vd imm
&vr           vd rj
&vr_i         vd rj imm
&vr_ii        vd rj imm imm2
&vrr          vd rj rk
&vv           vd vj
&vv_i         vd vj imm
&vvr          vd vj rk
&vvv          vd vj vk
&vvv_fcond    vd vj vk fcond
&vvvv         vd vj vk va
```


## 指令格式定义

```
@cv            .... ........ ..... ..... vj:5 .. cd:3    &cv
@rv_ui1      .... ........ ..... .... imm:1 vj:5 rd:5    &rv_i
@rv_ui2       .... ........ ..... ... imm:2 vj:5 rd:5    &rv_i
@rv_ui3        .... ........ ..... .. imm:3 vj:5 rd:5    &rv_i
@rv_ui4         .... ........ ..... . imm:4 vj:5 rd:5    &rv_i
@v_i13                   .... ........ .. imm:13 vd:5    &v_i
@vr               .... ........ ..... ..... rj:5 vd:5    &vr
@vr_i10            .... ........ .......... rj:5 vd:5    &vr_i imm=%i10s2
@vr_i11            .... ....... ........... rj:5 vd:5    &vr_i imm=%i11s1
@vr_i12                 .... ...... imm:s12 rj:5 vd:5    &vr_i
@vr_i8i1    .... ........ . imm2:1 ........ rj:5 vd:5    &vr_ii imm=%i8s3
@vr_i8i2      .... ........ imm2:2 ........ rj:5 vd:5    &vr_ii imm=%i8s2
@vr_i8i3       .... ....... imm2:3 ........ rj:5 vd:5    &vr_ii imm=%i8s1
@vr_i8i4          .... ...... imm2:4 imm:s8 rj:5 vd:5    &vr_ii
@vr_i9            .... ........ . ......... rj:5 vd:5    &vr_i imm=%i9s3
@vr_ui1      .... ........ ..... .... imm:1 rj:5 vd:5    &vr_i
@vr_ui2       .... ........ ..... ... imm:2 rj:5 vd:5    &vr_i
@vr_ui3        .... ........ ..... .. imm:3 rj:5 vd:5    &vr_i
@vr_ui4         .... ........ ..... . imm:4 rj:5 vd:5    &vr_i
@vrr               .... ........ ..... rk:5 rj:5 vd:5    &vrr
@vv               .... ........ ..... ..... vj:5 vd:5    &vv
@vv_i5           .... ........ ..... imm:s5 vj:5 vd:5    &vv_i
@vv_ui1      .... ........ ..... .... imm:1 vj:5 vd:5    &vv_i
@vv_ui2       .... ........ ..... ... imm:2 vj:5 vd:5    &vv_i
@vv_ui3        .... ........ ..... .. imm:3 vj:5 vd:5    &vv_i
@vv_ui4         .... ........ ..... . imm:4 vj:5 vd:5    &vv_i
@vv_ui5           .... ........ ..... imm:5 vj:5 vd:5    &vv_i
@vv_ui6            .... ........ .... imm:6 vj:5 vd:5    &vv_i
@vv_ui7             .... ........ ... imm:7 vj:5 vd:5    &vv_i
@vv_ui8              .... ........ .. imm:8 vj:5 vd:5    &vv_i
@vvr               .... ........ ..... rk:5 vj:5 vd:5    &vvr
@vvv               .... ........ ..... vk:5 vj:5 vd:5    &vvv
@vvv_fcond      .... ........ fcond:5  vk:5 vj:5 vd:5    &vvv_fcond
@vvvv               .... ........ va:5 vk:5 vj:5 vd:5    &vvvv
```


## 指令列表（按分类）

### 算术运算 (共 166 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vabsd_b | `vabsd_b 0111 00000110 00000 ..... ..... ..... ` | @vvv | vvv |
| vabsd_bu | `vabsd_bu 0111 00000110 00100 ..... ..... ..... ` | @vvv | vvv |
| vabsd_d | `vabsd_d 0111 00000110 00011 ..... ..... ..... ` | @vvv | vvv |
| vabsd_du | `vabsd_du 0111 00000110 00111 ..... ..... ..... ` | @vvv | vvv |
| vabsd_h | `vabsd_h 0111 00000110 00001 ..... ..... ..... ` | @vvv | vvv |
| vabsd_hu | `vabsd_hu 0111 00000110 00101 ..... ..... ..... ` | @vvv | vvv |
| vabsd_w | `vabsd_w 0111 00000110 00010 ..... ..... ..... ` | @vvv | vvv |
| vabsd_wu | `vabsd_wu 0111 00000110 00110 ..... ..... ..... ` | @vvv | vvv |
| vadd_b | `vadd_b 0111 00000000 10100 ..... ..... ..... ` | @vvv | vvv |
| vadd_d | `vadd_d 0111 00000000 10111 ..... ..... ..... ` | @vvv | vvv |
| vadd_h | `vadd_h 0111 00000000 10101 ..... ..... ..... ` | @vvv | vvv |
| vadd_q | `vadd_q 0111 00010010 11010 ..... ..... ..... ` | @vvv | vvv |
| vadd_w | `vadd_w 0111 00000000 10110 ..... ..... ..... ` | @vvv | vvv |
| vadda_b | `vadda_b 0111 00000101 11000 ..... ..... ..... ` | @vvv | vvv |
| vadda_d | `vadda_d 0111 00000101 11011 ..... ..... ..... ` | @vvv | vvv |
| vadda_h | `vadda_h 0111 00000101 11001 ..... ..... ..... ` | @vvv | vvv |
| vadda_w | `vadda_w 0111 00000101 11010 ..... ..... ..... ` | @vvv | vvv |
| vaddi_bu | `vaddi_bu 0111 00101000 10100 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddi_du | `vaddi_du 0111 00101000 10111 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddi_hu | `vaddi_hu 0111 00101000 10101 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddi_wu | `vaddi_wu 0111 00101000 10110 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddwev_d_w | `vaddwev_d_w 0111 00000001 11110 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_d_wu | `vaddwev_d_wu 0111 00000010 11110 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_d_wu_w | `vaddwev_d_wu_w 0111 00000011 11110 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_h_b | `vaddwev_h_b 0111 00000001 11100 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_h_bu | `vaddwev_h_bu 0111 00000010 11100 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_h_bu_b | `vaddwev_h_bu_b 0111 00000011 11100 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_q_d | `vaddwev_q_d 0111 00000001 11111 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_q_du | `vaddwev_q_du 0111 00000010 11111 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_q_du_d | `vaddwev_q_du_d 0111 00000011 11111 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_w_h | `vaddwev_w_h 0111 00000001 11101 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_w_hu | `vaddwev_w_hu 0111 00000010 11101 ..... ..... ..... ` | @vvv | vvv |
| vaddwev_w_hu_h | `vaddwev_w_hu_h 0111 00000011 11101 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_d_w | `vaddwod_d_w 0111 00000010 00110 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_d_wu | `vaddwod_d_wu 0111 00000011 00110 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_d_wu_w | `vaddwod_d_wu_w 0111 00000100 00010 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_h_b | `vaddwod_h_b 0111 00000010 00100 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_h_bu | `vaddwod_h_bu 0111 00000011 00100 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_h_bu_b | `vaddwod_h_bu_b 0111 00000100 00000 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_q_d | `vaddwod_q_d 0111 00000010 00111 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_q_du | `vaddwod_q_du 0111 00000011 00111 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_q_du_d | `vaddwod_q_du_d 0111 00000100 00011 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_w_h | `vaddwod_w_h 0111 00000010 00101 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_w_hu | `vaddwod_w_hu 0111 00000011 00101 ..... ..... ..... ` | @vvv | vvv |
| vaddwod_w_hu_h | `vaddwod_w_hu_h 0111 00000100 00001 ..... ..... ..... ` | @vvv | vvv |
| vavg_b | `vavg_b 0111 00000110 01000 ..... ..... ..... ` | @vvv | vvv |
| vavg_bu | `vavg_bu 0111 00000110 01100 ..... ..... ..... ` | @vvv | vvv |
| vavg_d | `vavg_d 0111 00000110 01011 ..... ..... ..... ` | @vvv | vvv |
| vavg_du | `vavg_du 0111 00000110 01111 ..... ..... ..... ` | @vvv | vvv |
| vavg_h | `vavg_h 0111 00000110 01001 ..... ..... ..... ` | @vvv | vvv |
| vavg_hu | `vavg_hu 0111 00000110 01101 ..... ..... ..... ` | @vvv | vvv |
| vavg_w | `vavg_w 0111 00000110 01010 ..... ..... ..... ` | @vvv | vvv |
| vavg_wu | `vavg_wu 0111 00000110 01110 ..... ..... ..... ` | @vvv | vvv |
| vavgr_b | `vavgr_b 0111 00000110 10000 ..... ..... ..... ` | @vvv | vvv |
| vavgr_bu | `vavgr_bu 0111 00000110 10100 ..... ..... ..... ` | @vvv | vvv |
| vavgr_d | `vavgr_d 0111 00000110 10011 ..... ..... ..... ` | @vvv | vvv |
| vavgr_du | `vavgr_du 0111 00000110 10111 ..... ..... ..... ` | @vvv | vvv |
| vavgr_h | `vavgr_h 0111 00000110 10001 ..... ..... ..... ` | @vvv | vvv |
| vavgr_hu | `vavgr_hu 0111 00000110 10101 ..... ..... ..... ` | @vvv | vvv |
| vavgr_w | `vavgr_w 0111 00000110 10010 ..... ..... ..... ` | @vvv | vvv |
| vavgr_wu | `vavgr_wu 0111 00000110 10110 ..... ..... ..... ` | @vvv | vvv |
| vfadd_d | `vfadd_d 0111 00010011 00010 ..... ..... ..... ` | @vvv | vvv |
| vfadd_s | `vfadd_s 0111 00010011 00001 ..... ..... ..... ` | @vvv | vvv |
| vfmadd_d | `vfmadd_d 0000 10010010 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmadd_s | `vfmadd_s 0000 10010001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmsub_d | `vfmsub_d 0000 10010110 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmsub_s | `vfmsub_s 0000 10010101 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmadd_d | `vfnmadd_d 0000 10011010 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmadd_s | `vfnmadd_s 0000 10011001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmsub_d | `vfnmsub_d 0000 10011110 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmsub_s | `vfnmsub_s 0000 10011101 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfsub_d | `vfsub_d 0111 00010011 00110 ..... ..... ..... ` | @vvv | vvv |
| vfsub_s | `vfsub_s 0111 00010011 00101 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_d_w | `vhaddw_d_w 0111 00000101 01010 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_du_wu | `vhaddw_du_wu 0111 00000101 10010 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_h_b | `vhaddw_h_b 0111 00000101 01000 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_hu_bu | `vhaddw_hu_bu 0111 00000101 10000 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_q_d | `vhaddw_q_d 0111 00000101 01011 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_qu_du | `vhaddw_qu_du 0111 00000101 10011 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_w_h | `vhaddw_w_h 0111 00000101 01001 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_wu_hu | `vhaddw_wu_hu 0111 00000101 10001 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_d_w | `vhsubw_d_w 0111 00000101 01110 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_du_wu | `vhsubw_du_wu 0111 00000101 10110 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_h_b | `vhsubw_h_b 0111 00000101 01100 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_hu_bu | `vhsubw_hu_bu 0111 00000101 10100 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_q_d | `vhsubw_q_d 0111 00000101 01111 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_qu_du | `vhsubw_qu_du 0111 00000101 10111 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_w_h | `vhsubw_w_h 0111 00000101 01101 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_wu_hu | `vhsubw_wu_hu 0111 00000101 10101 ..... ..... ..... ` | @vvv | vvv |
| vmadd_b | `vmadd_b 0111 00001010 10000 ..... ..... ..... ` | @vvv | vvv |
| vmadd_d | `vmadd_d 0111 00001010 10011 ..... ..... ..... ` | @vvv | vvv |
| vmadd_h | `vmadd_h 0111 00001010 10001 ..... ..... ..... ` | @vvv | vvv |
| vmadd_w | `vmadd_w 0111 00001010 10010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_d_w | `vmaddwev_d_w 0111 00001010 11010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_d_wu | `vmaddwev_d_wu 0111 00001011 01010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_d_wu_w | `vmaddwev_d_wu_w 0111 00001011 11010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_h_b | `vmaddwev_h_b 0111 00001010 11000 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_h_bu | `vmaddwev_h_bu 0111 00001011 01000 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_h_bu_b | `vmaddwev_h_bu_b 0111 00001011 11000 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_q_d | `vmaddwev_q_d 0111 00001010 11011 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_q_du | `vmaddwev_q_du 0111 00001011 01011 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_q_du_d | `vmaddwev_q_du_d 0111 00001011 11011 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_w_h | `vmaddwev_w_h 0111 00001010 11001 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_w_hu | `vmaddwev_w_hu 0111 00001011 01001 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_w_hu_h | `vmaddwev_w_hu_h 0111 00001011 11001 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_d_w | `vmaddwod_d_w 0111 00001010 11110 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_d_wu | `vmaddwod_d_wu 0111 00001011 01110 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_d_wu_w | `vmaddwod_d_wu_w 0111 00001011 11110 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_h_b | `vmaddwod_h_b 0111 00001010 11100 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_h_bu | `vmaddwod_h_bu 0111 00001011 01100 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_h_bu_b | `vmaddwod_h_bu_b 0111 00001011 11100 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_q_d | `vmaddwod_q_d 0111 00001010 11111 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_q_du | `vmaddwod_q_du 0111 00001011 01111 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_q_du_d | `vmaddwod_q_du_d 0111 00001011 11111 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_w_h | `vmaddwod_w_h 0111 00001010 11101 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_w_hu | `vmaddwod_w_hu 0111 00001011 01101 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_w_hu_h | `vmaddwod_w_hu_h 0111 00001011 11101 ..... ..... ..... ` | @vvv | vvv |
| vmsub_b | `vmsub_b 0111 00001010 10100 ..... ..... ..... ` | @vvv | vvv |
| vmsub_d | `vmsub_d 0111 00001010 10111 ..... ..... ..... ` | @vvv | vvv |
| vmsub_h | `vmsub_h 0111 00001010 10101 ..... ..... ..... ` | @vvv | vvv |
| vmsub_w | `vmsub_w 0111 00001010 10110 ..... ..... ..... ` | @vvv | vvv |
| vneg_b | `vneg_b 0111 00101001 11000 01100 ..... ..... ` | @vv | vv |
| vneg_d | `vneg_d 0111 00101001 11000 01111 ..... ..... ` | @vv | vv |
| vneg_h | `vneg_h 0111 00101001 11000 01101 ..... ..... ` | @vv | vv |
| vneg_w | `vneg_w 0111 00101001 11000 01110 ..... ..... ` | @vv | vv |
| vsadd_b | `vsadd_b 0111 00000100 01100 ..... ..... ..... ` | @vvv | vvv |
| vsadd_bu | `vsadd_bu 0111 00000100 10100 ..... ..... ..... ` | @vvv | vvv |
| vsadd_d | `vsadd_d 0111 00000100 01111 ..... ..... ..... ` | @vvv | vvv |
| vsadd_du | `vsadd_du 0111 00000100 10111 ..... ..... ..... ` | @vvv | vvv |
| vsadd_h | `vsadd_h 0111 00000100 01101 ..... ..... ..... ` | @vvv | vvv |
| vsadd_hu | `vsadd_hu 0111 00000100 10101 ..... ..... ..... ` | @vvv | vvv |
| vsadd_w | `vsadd_w 0111 00000100 01110 ..... ..... ..... ` | @vvv | vvv |
| vsadd_wu | `vsadd_wu 0111 00000100 10110 ..... ..... ..... ` | @vvv | vvv |
| vssub_b | `vssub_b 0111 00000100 10000 ..... ..... ..... ` | @vvv | vvv |
| vssub_bu | `vssub_bu 0111 00000100 11000 ..... ..... ..... ` | @vvv | vvv |
| vssub_d | `vssub_d 0111 00000100 10011 ..... ..... ..... ` | @vvv | vvv |
| vssub_du | `vssub_du 0111 00000100 11011 ..... ..... ..... ` | @vvv | vvv |
| vssub_h | `vssub_h 0111 00000100 10001 ..... ..... ..... ` | @vvv | vvv |
| vssub_hu | `vssub_hu 0111 00000100 11001 ..... ..... ..... ` | @vvv | vvv |
| vssub_w | `vssub_w 0111 00000100 10010 ..... ..... ..... ` | @vvv | vvv |
| vssub_wu | `vssub_wu 0111 00000100 11010 ..... ..... ..... ` | @vvv | vvv |
| vsub_b | `vsub_b 0111 00000000 11000 ..... ..... ..... ` | @vvv | vvv |
| vsub_d | `vsub_d 0111 00000000 11011 ..... ..... ..... ` | @vvv | vvv |
| vsub_h | `vsub_h 0111 00000000 11001 ..... ..... ..... ` | @vvv | vvv |
| vsub_q | `vsub_q 0111 00010010 11011 ..... ..... ..... ` | @vvv | vvv |
| vsub_w | `vsub_w 0111 00000000 11010 ..... ..... ..... ` | @vvv | vvv |
| vsubi_bu | `vsubi_bu 0111 00101000 11000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_du | `vsubi_du 0111 00101000 11011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_hu | `vsubi_hu 0111 00101000 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_wu | `vsubi_wu 0111 00101000 11010 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubwev_d_w | `vsubwev_d_w 0111 00000010 00010 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_d_wu | `vsubwev_d_wu 0111 00000011 00010 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_h_b | `vsubwev_h_b 0111 00000010 00000 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_h_bu | `vsubwev_h_bu 0111 00000011 00000 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_q_d | `vsubwev_q_d 0111 00000010 00011 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_q_du | `vsubwev_q_du 0111 00000011 00011 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_w_h | `vsubwev_w_h 0111 00000010 00001 ..... ..... ..... ` | @vvv | vvv |
| vsubwev_w_hu | `vsubwev_w_hu 0111 00000011 00001 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_d_w | `vsubwod_d_w 0111 00000010 01010 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_d_wu | `vsubwod_d_wu 0111 00000011 01010 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_h_b | `vsubwod_h_b 0111 00000010 01000 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_h_bu | `vsubwod_h_bu 0111 00000011 01000 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_q_d | `vsubwod_q_d 0111 00000010 01011 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_q_du | `vsubwod_q_du 0111 00000011 01011 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_w_h | `vsubwod_w_h 0111 00000010 01001 ..... ..... ..... ` | @vvv | vvv |
| vsubwod_w_hu | `vsubwod_w_hu 0111 00000011 01001 ..... ..... ..... ` | @vvv | vvv |


### 立即数操作 (共 44 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vaddi_bu | `vaddi_bu 0111 00101000 10100 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddi_du | `vaddi_du 0111 00101000 10111 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddi_hu | `vaddi_hu 0111 00101000 10101 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vaddi_wu | `vaddi_wu 0111 00101000 10110 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_b | `vmaxi_b 0111 00101001 00000 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_bu | `vmaxi_bu 0111 00101001 01000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_d | `vmaxi_d 0111 00101001 00011 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_du | `vmaxi_du 0111 00101001 01011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_h | `vmaxi_h 0111 00101001 00001 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_hu | `vmaxi_hu 0111 00101001 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_w | `vmaxi_w 0111 00101001 00010 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_wu | `vmaxi_wu 0111 00101001 01010 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_b | `vmini_b 0111 00101001 00100 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_bu | `vmini_bu 0111 00101001 01100 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_d | `vmini_d 0111 00101001 00111 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_du | `vmini_du 0111 00101001 01111 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_h | `vmini_h 0111 00101001 00101 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_hu | `vmini_hu 0111 00101001 01101 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_w | `vmini_w 0111 00101001 00110 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_wu | `vmini_wu 0111 00101001 01110 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vseqi_b | `vseqi_b 0111 00101000 00000 ..... ..... ..... ` | @vv_i5 | vv_i |
| vseqi_d | `vseqi_d 0111 00101000 00011 ..... ..... ..... ` | @vv_i5 | vv_i |
| vseqi_h | `vseqi_h 0111 00101000 00001 ..... ..... ..... ` | @vv_i5 | vv_i |
| vseqi_w | `vseqi_w 0111 00101000 00010 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_b | `vslei_b 0111 00101000 00100 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_bu | `vslei_bu 0111 00101000 01000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslei_d | `vslei_d 0111 00101000 00111 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_du | `vslei_du 0111 00101000 01011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslei_h | `vslei_h 0111 00101000 00101 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_hu | `vslei_hu 0111 00101000 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslei_w | `vslei_w 0111 00101000 00110 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_wu | `vslei_wu 0111 00101000 01010 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_b | `vslti_b 0111 00101000 01100 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_bu | `vslti_bu 0111 00101000 10000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_d | `vslti_d 0111 00101000 01111 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_du | `vslti_du 0111 00101000 10011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_h | `vslti_h 0111 00101000 01101 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_hu | `vslti_hu 0111 00101000 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_w | `vslti_w 0111 00101000 01110 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_wu | `vslti_wu 0111 00101000 10010 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_bu | `vsubi_bu 0111 00101000 11000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_du | `vsubi_du 0111 00101000 11011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_hu | `vsubi_hu 0111 00101000 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsubi_wu | `vsubi_wu 0111 00101000 11010 ..... ..... ..... ` | @vv_ui5 | vv_i |


### 逻辑运算 (共 10 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vand_v | `vand_v 0111 00010010 01100 ..... ..... ..... ` | @vvv | vvv |
| vandi_b | `vandi_b 0111 00111101 00 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vandn_v | `vandn_v 0111 00010010 10000 ..... ..... ..... ` | @vvv | vvv |
| vnor_v | `vnor_v 0111 00010010 01111 ..... ..... ..... ` | @vvv | vvv |
| vnori_b | `vnori_b 0111 00111101 11 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vor_v | `vor_v 0111 00010010 01101 ..... ..... ..... ` | @vvv | vvv |
| vori_b | `vori_b 0111 00111101 01 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vorn_v | `vorn_v 0111 00010010 10001 ..... ..... ..... ` | @vvv | vvv |
| vxor_v | `vxor_v 0111 00010010 01110 ..... ..... ..... ` | @vvv | vvv |
| vxori_b | `vxori_b 0111 00111101 10 ........ ..... ..... ` | @vv_ui8 | vv_i |


### 位操作与移位 (共 200 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vbitclr_b | `vbitclr_b 0111 00010000 11000 ..... ..... ..... ` | @vvv | vvv |
| vbitclr_d | `vbitclr_d 0111 00010000 11011 ..... ..... ..... ` | @vvv | vvv |
| vbitclr_h | `vbitclr_h 0111 00010000 11001 ..... ..... ..... ` | @vvv | vvv |
| vbitclr_w | `vbitclr_w 0111 00010000 11010 ..... ..... ..... ` | @vvv | vvv |
| vbitclri_b | `vbitclri_b 0111 00110001 00000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vbitclri_d | `vbitclri_d 0111 00110001 0001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vbitclri_h | `vbitclri_h 0111 00110001 00000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vbitclri_w | `vbitclri_w 0111 00110001 00001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vbitrev_b | `vbitrev_b 0111 00010001 00000 ..... ..... ..... ` | @vvv | vvv |
| vbitrev_d | `vbitrev_d 0111 00010001 00011 ..... ..... ..... ` | @vvv | vvv |
| vbitrev_h | `vbitrev_h 0111 00010001 00001 ..... ..... ..... ` | @vvv | vvv |
| vbitrev_w | `vbitrev_w 0111 00010001 00010 ..... ..... ..... ` | @vvv | vvv |
| vbitrevi_b | `vbitrevi_b 0111 00110001 10000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vbitrevi_d | `vbitrevi_d 0111 00110001 1001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vbitrevi_h | `vbitrevi_h 0111 00110001 10000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vbitrevi_w | `vbitrevi_w 0111 00110001 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vbitsel_v | `vbitsel_v 0000 11010001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vbitseli_b | `vbitseli_b 0111 00111100 01 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vbitset_b | `vbitset_b 0111 00010000 11100 ..... ..... ..... ` | @vvv | vvv |
| vbitset_d | `vbitset_d 0111 00010000 11111 ..... ..... ..... ` | @vvv | vvv |
| vbitset_h | `vbitset_h 0111 00010000 11101 ..... ..... ..... ` | @vvv | vvv |
| vbitset_w | `vbitset_w 0111 00010000 11110 ..... ..... ..... ` | @vvv | vvv |
| vbitseti_b | `vbitseti_b 0111 00110001 01000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vbitseti_d | `vbitseti_d 0111 00110001 0101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vbitseti_h | `vbitseti_h 0111 00110001 01000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vbitseti_w | `vbitseti_w 0111 00110001 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vbsll_v | `vbsll_v 0111 00101000 11100 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vbsrl_v | `vbsrl_v 0111 00101000 11101 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vclo_b | `vclo_b 0111 00101001 11000 00000 ..... ..... ` | @vv | vv |
| vclo_d | `vclo_d 0111 00101001 11000 00011 ..... ..... ` | @vv | vv |
| vclo_h | `vclo_h 0111 00101001 11000 00001 ..... ..... ` | @vv | vv |
| vclo_w | `vclo_w 0111 00101001 11000 00010 ..... ..... ` | @vv | vv |
| vclz_b | `vclz_b 0111 00101001 11000 00100 ..... ..... ` | @vv | vv |
| vclz_d | `vclz_d 0111 00101001 11000 00111 ..... ..... ` | @vv | vv |
| vclz_h | `vclz_h 0111 00101001 11000 00101 ..... ..... ` | @vv | vv |
| vclz_w | `vclz_w 0111 00101001 11000 00110 ..... ..... ` | @vv | vv |
| vext2xv_d_b | `vext2xv_d_b 0111 01101001 11110 00110 ..... ..... ` | @vv | vv |
| vext2xv_d_h | `vext2xv_d_h 0111 01101001 11110 01000 ..... ..... ` | @vv | vv |
| vext2xv_d_w | `vext2xv_d_w 0111 01101001 11110 01001 ..... ..... ` | @vv | vv |
| vext2xv_du_bu | `vext2xv_du_bu 0111 01101001 11110 01100 ..... ..... ` | @vv | vv |
| vext2xv_du_hu | `vext2xv_du_hu 0111 01101001 11110 01110 ..... ..... ` | @vv | vv |
| vext2xv_du_wu | `vext2xv_du_wu 0111 01101001 11110 01111 ..... ..... ` | @vv | vv |
| vext2xv_h_b | `vext2xv_h_b 0111 01101001 11110 00100 ..... ..... ` | @vv | vv |
| vext2xv_hu_bu | `vext2xv_hu_bu 0111 01101001 11110 01010 ..... ..... ` | @vv | vv |
| vext2xv_w_b | `vext2xv_w_b 0111 01101001 11110 00101 ..... ..... ` | @vv | vv |
| vext2xv_w_h | `vext2xv_w_h 0111 01101001 11110 00111 ..... ..... ` | @vv | vv |
| vext2xv_wu_bu | `vext2xv_wu_bu 0111 01101001 11110 01011 ..... ..... ` | @vv | vv |
| vext2xv_wu_hu | `vext2xv_wu_hu 0111 01101001 11110 01101 ..... ..... ` | @vv | vv |
| vexth_d_w | `vexth_d_w 0111 00101001 11101 11010 ..... ..... ` | @vv | vv |
| vexth_du_wu | `vexth_du_wu 0111 00101001 11101 11110 ..... ..... ` | @vv | vv |
| vexth_h_b | `vexth_h_b 0111 00101001 11101 11000 ..... ..... ` | @vv | vv |
| vexth_hu_bu | `vexth_hu_bu 0111 00101001 11101 11100 ..... ..... ` | @vv | vv |
| vexth_q_d | `vexth_q_d 0111 00101001 11101 11011 ..... ..... ` | @vv | vv |
| vexth_qu_du | `vexth_qu_du 0111 00101001 11101 11111 ..... ..... ` | @vv | vv |
| vexth_w_h | `vexth_w_h 0111 00101001 11101 11001 ..... ..... ` | @vv | vv |
| vexth_wu_hu | `vexth_wu_hu 0111 00101001 11101 11101 ..... ..... ` | @vv | vv |
| vextl_q_d | `vextl_q_d 0111 00110000 10010 00000 ..... ..... ` | @vv | vv |
| vextl_qu_du | `vextl_qu_du 0111 00110000 11010 00000 ..... ..... ` | @vv | vv |
| vextrins_b | `vextrins_b 0111 00111000 11 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vextrins_d | `vextrins_d 0111 00111000 00 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vextrins_h | `vextrins_h 0111 00111000 10 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vextrins_w | `vextrins_w 0111 00111000 01 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vrotr_b | `vrotr_b 0111 00001110 11100 ..... ..... ..... ` | @vvv | vvv |
| vrotr_d | `vrotr_d 0111 00001110 11111 ..... ..... ..... ` | @vvv | vvv |
| vrotr_h | `vrotr_h 0111 00001110 11101 ..... ..... ..... ` | @vvv | vvv |
| vrotr_w | `vrotr_w 0111 00001110 11110 ..... ..... ..... ` | @vvv | vvv |
| vrotri_b | `vrotri_b 0111 00101010 00000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vrotri_d | `vrotri_d 0111 00101010 0001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vrotri_h | `vrotri_h 0111 00101010 00000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vrotri_w | `vrotri_w 0111 00101010 00001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsll_b | `vsll_b 0111 00001110 10000 ..... ..... ..... ` | @vvv | vvv |
| vsll_d | `vsll_d 0111 00001110 10011 ..... ..... ..... ` | @vvv | vvv |
| vsll_h | `vsll_h 0111 00001110 10001 ..... ..... ..... ` | @vvv | vvv |
| vsll_w | `vsll_w 0111 00001110 10010 ..... ..... ..... ` | @vvv | vvv |
| vslli_b | `vslli_b 0111 00110010 11000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vslli_d | `vslli_d 0111 00110010 1101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vslli_h | `vslli_h 0111 00110010 11000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vslli_w | `vslli_w 0111 00110010 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsllwil_d_w | `vsllwil_d_w 0111 00110000 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsllwil_du_wu | `vsllwil_du_wu 0111 00110000 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsllwil_h_b | `vsllwil_h_b 0111 00110000 10000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vsllwil_hu_bu | `vsllwil_hu_bu 0111 00110000 11000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vsllwil_w_h | `vsllwil_w_h 0111 00110000 10000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsllwil_wu_hu | `vsllwil_wu_hu 0111 00110000 11000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsra_b | `vsra_b 0111 00001110 11000 ..... ..... ..... ` | @vvv | vvv |
| vsra_d | `vsra_d 0111 00001110 11011 ..... ..... ..... ` | @vvv | vvv |
| vsra_h | `vsra_h 0111 00001110 11001 ..... ..... ..... ` | @vvv | vvv |
| vsra_w | `vsra_w 0111 00001110 11010 ..... ..... ..... ` | @vvv | vvv |
| vsrai_b | `vsrai_b 0111 00110011 01000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vsrai_d | `vsrai_d 0111 00110011 0101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrai_h | `vsrai_h 0111 00110011 01000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrai_w | `vsrai_w 0111 00110011 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsran_b_h | `vsran_b_h 0111 00001111 01101 ..... ..... ..... ` | @vvv | vvv |
| vsran_h_w | `vsran_h_w 0111 00001111 01110 ..... ..... ..... ` | @vvv | vvv |
| vsran_w_d | `vsran_w_d 0111 00001111 01111 ..... ..... ..... ` | @vvv | vvv |
| vsrani_b_h | `vsrani_b_h 0111 00110101 10000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrani_d_q | `vsrani_d_q 0111 00110101 101 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vsrani_h_w | `vsrani_h_w 0111 00110101 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrani_w_d | `vsrani_w_d 0111 00110101 1001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrar_b | `vsrar_b 0111 00001111 00100 ..... ..... ..... ` | @vvv | vvv |
| vsrar_d | `vsrar_d 0111 00001111 00111 ..... ..... ..... ` | @vvv | vvv |
| vsrar_h | `vsrar_h 0111 00001111 00101 ..... ..... ..... ` | @vvv | vvv |
| vsrar_w | `vsrar_w 0111 00001111 00110 ..... ..... ..... ` | @vvv | vvv |
| vsrari_b | `vsrari_b 0111 00101010 10000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vsrari_d | `vsrari_d 0111 00101010 1001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrari_h | `vsrari_h 0111 00101010 10000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrari_w | `vsrari_w 0111 00101010 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrarn_b_h | `vsrarn_b_h 0111 00001111 10101 ..... ..... ..... ` | @vvv | vvv |
| vsrarn_h_w | `vsrarn_h_w 0111 00001111 10110 ..... ..... ..... ` | @vvv | vvv |
| vsrarn_w_d | `vsrarn_w_d 0111 00001111 10111 ..... ..... ..... ` | @vvv | vvv |
| vsrarni_b_h | `vsrarni_b_h 0111 00110101 11000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrarni_d_q | `vsrarni_d_q 0111 00110101 111 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vsrarni_h_w | `vsrarni_h_w 0111 00110101 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrarni_w_d | `vsrarni_w_d 0111 00110101 1101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrl_b | `vsrl_b 0111 00001110 10100 ..... ..... ..... ` | @vvv | vvv |
| vsrl_d | `vsrl_d 0111 00001110 10111 ..... ..... ..... ` | @vvv | vvv |
| vsrl_h | `vsrl_h 0111 00001110 10101 ..... ..... ..... ` | @vvv | vvv |
| vsrl_w | `vsrl_w 0111 00001110 10110 ..... ..... ..... ` | @vvv | vvv |
| vsrli_b | `vsrli_b 0111 00110011 00000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vsrli_d | `vsrli_d 0111 00110011 0001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrli_h | `vsrli_h 0111 00110011 00000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrli_w | `vsrli_w 0111 00110011 00001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrln_b_h | `vsrln_b_h 0111 00001111 01001 ..... ..... ..... ` | @vvv | vvv |
| vsrln_h_w | `vsrln_h_w 0111 00001111 01010 ..... ..... ..... ` | @vvv | vvv |
| vsrln_w_d | `vsrln_w_d 0111 00001111 01011 ..... ..... ..... ` | @vvv | vvv |
| vsrlni_b_h | `vsrlni_b_h 0111 00110100 00000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrlni_d_q | `vsrlni_d_q 0111 00110100 001 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vsrlni_h_w | `vsrlni_h_w 0111 00110100 00001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrlni_w_d | `vsrlni_w_d 0111 00110100 0001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrlr_b | `vsrlr_b 0111 00001111 00000 ..... ..... ..... ` | @vvv | vvv |
| vsrlr_d | `vsrlr_d 0111 00001111 00011 ..... ..... ..... ` | @vvv | vvv |
| vsrlr_h | `vsrlr_h 0111 00001111 00001 ..... ..... ..... ` | @vvv | vvv |
| vsrlr_w | `vsrlr_w 0111 00001111 00010 ..... ..... ..... ` | @vvv | vvv |
| vsrlri_b | `vsrlri_b 0111 00101010 01000 01 ... ..... ..... ` | @vv_ui3 | vv_i |
| vsrlri_d | `vsrlri_d 0111 00101010 0101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vsrlri_h | `vsrlri_h 0111 00101010 01000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrlri_w | `vsrlri_w 0111 00101010 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrlrn_b_h | `vsrlrn_b_h 0111 00001111 10001 ..... ..... ..... ` | @vvv | vvv |
| vsrlrn_h_w | `vsrlrn_h_w 0111 00001111 10010 ..... ..... ..... ` | @vvv | vvv |
| vsrlrn_w_d | `vsrlrn_w_d 0111 00001111 10011 ..... ..... ..... ` | @vvv | vvv |
| vsrlrni_b_h | `vsrlrni_b_h 0111 00110100 01000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vsrlrni_d_q | `vsrlrni_d_q 0111 00110100 011 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vsrlrni_h_w | `vsrlrni_h_w 0111 00110100 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vsrlrni_w_d | `vsrlrni_w_d 0111 00110100 0101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssran_b_h | `vssran_b_h 0111 00001111 11101 ..... ..... ..... ` | @vvv | vvv |
| vssran_bu_h | `vssran_bu_h 0111 00010000 01101 ..... ..... ..... ` | @vvv | vvv |
| vssran_h_w | `vssran_h_w 0111 00001111 11110 ..... ..... ..... ` | @vvv | vvv |
| vssran_hu_w | `vssran_hu_w 0111 00010000 01110 ..... ..... ..... ` | @vvv | vvv |
| vssran_w_d | `vssran_w_d 0111 00001111 11111 ..... ..... ..... ` | @vvv | vvv |
| vssran_wu_d | `vssran_wu_d 0111 00010000 01111 ..... ..... ..... ` | @vvv | vvv |
| vssrani_b_h | `vssrani_b_h 0111 00110110 00000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrani_bu_h | `vssrani_bu_h 0111 00110110 01000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrani_d_q | `vssrani_d_q 0111 00110110 001 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrani_du_q | `vssrani_du_q 0111 00110110 011 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrani_h_w | `vssrani_h_w 0111 00110110 00001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrani_hu_w | `vssrani_hu_w 0111 00110110 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrani_w_d | `vssrani_w_d 0111 00110110 0001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrani_wu_d | `vssrani_wu_d 0111 00110110 0101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrarn_b_h | `vssrarn_b_h 0111 00010000 00101 ..... ..... ..... ` | @vvv | vvv |
| vssrarn_bu_h | `vssrarn_bu_h 0111 00010000 10101 ..... ..... ..... ` | @vvv | vvv |
| vssrarn_h_w | `vssrarn_h_w 0111 00010000 00110 ..... ..... ..... ` | @vvv | vvv |
| vssrarn_hu_w | `vssrarn_hu_w 0111 00010000 10110 ..... ..... ..... ` | @vvv | vvv |
| vssrarn_w_d | `vssrarn_w_d 0111 00010000 00111 ..... ..... ..... ` | @vvv | vvv |
| vssrarn_wu_d | `vssrarn_wu_d 0111 00010000 10111 ..... ..... ..... ` | @vvv | vvv |
| vssrarni_b_h | `vssrarni_b_h 0111 00110110 10000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrarni_bu_h | `vssrarni_bu_h 0111 00110110 11000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrarni_d_q | `vssrarni_d_q 0111 00110110 101 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrarni_du_q | `vssrarni_du_q 0111 00110110 111 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrarni_h_w | `vssrarni_h_w 0111 00110110 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrarni_hu_w | `vssrarni_hu_w 0111 00110110 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrarni_w_d | `vssrarni_w_d 0111 00110110 1001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrarni_wu_d | `vssrarni_wu_d 0111 00110110 1101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrln_b_h | `vssrln_b_h 0111 00001111 11001 ..... ..... ..... ` | @vvv | vvv |
| vssrln_bu_h | `vssrln_bu_h 0111 00010000 01001 ..... ..... ..... ` | @vvv | vvv |
| vssrln_h_w | `vssrln_h_w 0111 00001111 11010 ..... ..... ..... ` | @vvv | vvv |
| vssrln_hu_w | `vssrln_hu_w 0111 00010000 01010 ..... ..... ..... ` | @vvv | vvv |
| vssrln_w_d | `vssrln_w_d 0111 00001111 11011 ..... ..... ..... ` | @vvv | vvv |
| vssrln_wu_d | `vssrln_wu_d 0111 00010000 01011 ..... ..... ..... ` | @vvv | vvv |
| vssrlni_b_h | `vssrlni_b_h 0111 00110100 10000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrlni_bu_h | `vssrlni_bu_h 0111 00110100 11000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrlni_d_q | `vssrlni_d_q 0111 00110100 101 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrlni_du_q | `vssrlni_du_q 0111 00110100 111 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrlni_h_w | `vssrlni_h_w 0111 00110100 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrlni_hu_w | `vssrlni_hu_w 0111 00110100 11001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrlni_w_d | `vssrlni_w_d 0111 00110100 1001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrlni_wu_d | `vssrlni_wu_d 0111 00110100 1101 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrlrn_b_h | `vssrlrn_b_h 0111 00010000 00001 ..... ..... ..... ` | @vvv | vvv |
| vssrlrn_bu_h | `vssrlrn_bu_h 0111 00010000 10001 ..... ..... ..... ` | @vvv | vvv |
| vssrlrn_h_w | `vssrlrn_h_w 0111 00010000 00010 ..... ..... ..... ` | @vvv | vvv |
| vssrlrn_hu_w | `vssrlrn_hu_w 0111 00010000 10010 ..... ..... ..... ` | @vvv | vvv |
| vssrlrn_w_d | `vssrlrn_w_d 0111 00010000 00011 ..... ..... ..... ` | @vvv | vvv |
| vssrlrn_wu_d | `vssrlrn_wu_d 0111 00010000 10011 ..... ..... ..... ` | @vvv | vvv |
| vssrlrni_b_h | `vssrlrni_b_h 0111 00110101 00000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrlrni_bu_h | `vssrlrni_bu_h 0111 00110101 01000 1 .... ..... ..... ` | @vv_ui4 | vv_i |
| vssrlrni_d_q | `vssrlrni_d_q 0111 00110101 001 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrlrni_du_q | `vssrlrni_du_q 0111 00110101 011 ....... ..... ..... ` | @vv_ui7 | vv_i |
| vssrlrni_h_w | `vssrlrni_h_w 0111 00110101 00001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrlrni_hu_w | `vssrlrni_hu_w 0111 00110101 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vssrlrni_w_d | `vssrlrni_w_d 0111 00110101 0001 ...... ..... ..... ` | @vv_ui6 | vv_i |
| vssrlrni_wu_d | `vssrlrni_wu_d 0111 00110101 0101 ...... ..... ..... ` | @vv_ui6 | vv_i |


### 比较与最值 (共 82 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vfcmp_cond_d | `vfcmp_cond_d 0000 11000110 ..... ..... ..... ..... ` | @vvv_fcond | vvv_fcond |
| vfcmp_cond_s | `vfcmp_cond_s 0000 11000101 ..... ..... ..... ..... ` | @vvv_fcond | vvv_fcond |
| vfmax_d | `vfmax_d 0111 00010011 11010 ..... ..... ..... ` | @vvv | vvv |
| vfmax_s | `vfmax_s 0111 00010011 11001 ..... ..... ..... ` | @vvv | vvv |
| vfmaxa_d | `vfmaxa_d 0111 00010100 00010 ..... ..... ..... ` | @vvv | vvv |
| vfmaxa_s | `vfmaxa_s 0111 00010100 00001 ..... ..... ..... ` | @vvv | vvv |
| vfmin_d | `vfmin_d 0111 00010011 11110 ..... ..... ..... ` | @vvv | vvv |
| vfmin_s | `vfmin_s 0111 00010011 11101 ..... ..... ..... ` | @vvv | vvv |
| vfmina_d | `vfmina_d 0111 00010100 00110 ..... ..... ..... ` | @vvv | vvv |
| vfmina_s | `vfmina_s 0111 00010100 00101 ..... ..... ..... ` | @vvv | vvv |
| vmax_b | `vmax_b 0111 00000111 00000 ..... ..... ..... ` | @vvv | vvv |
| vmax_bu | `vmax_bu 0111 00000111 01000 ..... ..... ..... ` | @vvv | vvv |
| vmax_d | `vmax_d 0111 00000111 00011 ..... ..... ..... ` | @vvv | vvv |
| vmax_du | `vmax_du 0111 00000111 01011 ..... ..... ..... ` | @vvv | vvv |
| vmax_h | `vmax_h 0111 00000111 00001 ..... ..... ..... ` | @vvv | vvv |
| vmax_hu | `vmax_hu 0111 00000111 01001 ..... ..... ..... ` | @vvv | vvv |
| vmax_w | `vmax_w 0111 00000111 00010 ..... ..... ..... ` | @vvv | vvv |
| vmax_wu | `vmax_wu 0111 00000111 01010 ..... ..... ..... ` | @vvv | vvv |
| vmaxi_b | `vmaxi_b 0111 00101001 00000 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_bu | `vmaxi_bu 0111 00101001 01000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_d | `vmaxi_d 0111 00101001 00011 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_du | `vmaxi_du 0111 00101001 01011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_h | `vmaxi_h 0111 00101001 00001 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_hu | `vmaxi_hu 0111 00101001 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmaxi_w | `vmaxi_w 0111 00101001 00010 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmaxi_wu | `vmaxi_wu 0111 00101001 01010 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmin_b | `vmin_b 0111 00000111 00100 ..... ..... ..... ` | @vvv | vvv |
| vmin_bu | `vmin_bu 0111 00000111 01100 ..... ..... ..... ` | @vvv | vvv |
| vmin_d | `vmin_d 0111 00000111 00111 ..... ..... ..... ` | @vvv | vvv |
| vmin_du | `vmin_du 0111 00000111 01111 ..... ..... ..... ` | @vvv | vvv |
| vmin_h | `vmin_h 0111 00000111 00101 ..... ..... ..... ` | @vvv | vvv |
| vmin_hu | `vmin_hu 0111 00000111 01101 ..... ..... ..... ` | @vvv | vvv |
| vmin_w | `vmin_w 0111 00000111 00110 ..... ..... ..... ` | @vvv | vvv |
| vmin_wu | `vmin_wu 0111 00000111 01110 ..... ..... ..... ` | @vvv | vvv |
| vmini_b | `vmini_b 0111 00101001 00100 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_bu | `vmini_bu 0111 00101001 01100 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_d | `vmini_d 0111 00101001 00111 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_du | `vmini_du 0111 00101001 01111 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_h | `vmini_h 0111 00101001 00101 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_hu | `vmini_hu 0111 00101001 01101 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vmini_w | `vmini_w 0111 00101001 00110 ..... ..... ..... ` | @vv_i5 | vv_i |
| vmini_wu | `vmini_wu 0111 00101001 01110 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vseq_b | `vseq_b 0111 00000000 00000 ..... ..... ..... ` | @vvv | vvv |
| vseq_d | `vseq_d 0111 00000000 00011 ..... ..... ..... ` | @vvv | vvv |
| vseq_h | `vseq_h 0111 00000000 00001 ..... ..... ..... ` | @vvv | vvv |
| vseq_w | `vseq_w 0111 00000000 00010 ..... ..... ..... ` | @vvv | vvv |
| vseqi_b | `vseqi_b 0111 00101000 00000 ..... ..... ..... ` | @vv_i5 | vv_i |
| vseqi_d | `vseqi_d 0111 00101000 00011 ..... ..... ..... ` | @vv_i5 | vv_i |
| vseqi_h | `vseqi_h 0111 00101000 00001 ..... ..... ..... ` | @vv_i5 | vv_i |
| vseqi_w | `vseqi_w 0111 00101000 00010 ..... ..... ..... ` | @vv_i5 | vv_i |
| vsle_b | `vsle_b 0111 00000000 00100 ..... ..... ..... ` | @vvv | vvv |
| vsle_bu | `vsle_bu 0111 00000000 01000 ..... ..... ..... ` | @vvv | vvv |
| vsle_d | `vsle_d 0111 00000000 00111 ..... ..... ..... ` | @vvv | vvv |
| vsle_du | `vsle_du 0111 00000000 01011 ..... ..... ..... ` | @vvv | vvv |
| vsle_h | `vsle_h 0111 00000000 00101 ..... ..... ..... ` | @vvv | vvv |
| vsle_hu | `vsle_hu 0111 00000000 01001 ..... ..... ..... ` | @vvv | vvv |
| vsle_w | `vsle_w 0111 00000000 00110 ..... ..... ..... ` | @vvv | vvv |
| vsle_wu | `vsle_wu 0111 00000000 01010 ..... ..... ..... ` | @vvv | vvv |
| vslei_b | `vslei_b 0111 00101000 00100 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_bu | `vslei_bu 0111 00101000 01000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslei_d | `vslei_d 0111 00101000 00111 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_du | `vslei_du 0111 00101000 01011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslei_h | `vslei_h 0111 00101000 00101 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_hu | `vslei_hu 0111 00101000 01001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslei_w | `vslei_w 0111 00101000 00110 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslei_wu | `vslei_wu 0111 00101000 01010 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslt_b | `vslt_b 0111 00000000 01100 ..... ..... ..... ` | @vvv | vvv |
| vslt_bu | `vslt_bu 0111 00000000 10000 ..... ..... ..... ` | @vvv | vvv |
| vslt_d | `vslt_d 0111 00000000 01111 ..... ..... ..... ` | @vvv | vvv |
| vslt_du | `vslt_du 0111 00000000 10011 ..... ..... ..... ` | @vvv | vvv |
| vslt_h | `vslt_h 0111 00000000 01101 ..... ..... ..... ` | @vvv | vvv |
| vslt_hu | `vslt_hu 0111 00000000 10001 ..... ..... ..... ` | @vvv | vvv |
| vslt_w | `vslt_w 0111 00000000 01110 ..... ..... ..... ` | @vvv | vvv |
| vslt_wu | `vslt_wu 0111 00000000 10010 ..... ..... ..... ` | @vvv | vvv |
| vslti_b | `vslti_b 0111 00101000 01100 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_bu | `vslti_bu 0111 00101000 10000 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_d | `vslti_d 0111 00101000 01111 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_du | `vslti_du 0111 00101000 10011 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_h | `vslti_h 0111 00101000 01101 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_hu | `vslti_hu 0111 00101000 10001 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vslti_w | `vslti_w 0111 00101000 01110 ..... ..... ..... ` | @vv_i5 | vv_i |
| vslti_wu | `vslti_wu 0111 00101000 10010 ..... ..... ..... ` | @vv_ui5 | vv_i |


### 乘除累加 (共 104 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vdiv_b | `vdiv_b 0111 00001110 00000 ..... ..... ..... ` | @vvv | vvv |
| vdiv_bu | `vdiv_bu 0111 00001110 01000 ..... ..... ..... ` | @vvv | vvv |
| vdiv_d | `vdiv_d 0111 00001110 00011 ..... ..... ..... ` | @vvv | vvv |
| vdiv_du | `vdiv_du 0111 00001110 01011 ..... ..... ..... ` | @vvv | vvv |
| vdiv_h | `vdiv_h 0111 00001110 00001 ..... ..... ..... ` | @vvv | vvv |
| vdiv_hu | `vdiv_hu 0111 00001110 01001 ..... ..... ..... ` | @vvv | vvv |
| vdiv_w | `vdiv_w 0111 00001110 00010 ..... ..... ..... ` | @vvv | vvv |
| vdiv_wu | `vdiv_wu 0111 00001110 01010 ..... ..... ..... ` | @vvv | vvv |
| vfdiv_d | `vfdiv_d 0111 00010011 10110 ..... ..... ..... ` | @vvv | vvv |
| vfdiv_s | `vfdiv_s 0111 00010011 10101 ..... ..... ..... ` | @vvv | vvv |
| vfmadd_d | `vfmadd_d 0000 10010010 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmadd_s | `vfmadd_s 0000 10010001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmsub_d | `vfmsub_d 0000 10010110 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmsub_s | `vfmsub_s 0000 10010101 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmul_d | `vfmul_d 0111 00010011 10010 ..... ..... ..... ` | @vvv | vvv |
| vfmul_s | `vfmul_s 0111 00010011 10001 ..... ..... ..... ` | @vvv | vvv |
| vfnmadd_d | `vfnmadd_d 0000 10011010 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmadd_s | `vfnmadd_s 0000 10011001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmsub_d | `vfnmsub_d 0000 10011110 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmsub_s | `vfnmsub_s 0000 10011101 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vhaddw_d_w | `vhaddw_d_w 0111 00000101 01010 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_du_wu | `vhaddw_du_wu 0111 00000101 10010 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_h_b | `vhaddw_h_b 0111 00000101 01000 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_hu_bu | `vhaddw_hu_bu 0111 00000101 10000 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_q_d | `vhaddw_q_d 0111 00000101 01011 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_qu_du | `vhaddw_qu_du 0111 00000101 10011 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_w_h | `vhaddw_w_h 0111 00000101 01001 ..... ..... ..... ` | @vvv | vvv |
| vhaddw_wu_hu | `vhaddw_wu_hu 0111 00000101 10001 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_d_w | `vhsubw_d_w 0111 00000101 01110 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_du_wu | `vhsubw_du_wu 0111 00000101 10110 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_h_b | `vhsubw_h_b 0111 00000101 01100 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_hu_bu | `vhsubw_hu_bu 0111 00000101 10100 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_q_d | `vhsubw_q_d 0111 00000101 01111 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_qu_du | `vhsubw_qu_du 0111 00000101 10111 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_w_h | `vhsubw_w_h 0111 00000101 01101 ..... ..... ..... ` | @vvv | vvv |
| vhsubw_wu_hu | `vhsubw_wu_hu 0111 00000101 10101 ..... ..... ..... ` | @vvv | vvv |
| vmadd_b | `vmadd_b 0111 00001010 10000 ..... ..... ..... ` | @vvv | vvv |
| vmadd_d | `vmadd_d 0111 00001010 10011 ..... ..... ..... ` | @vvv | vvv |
| vmadd_h | `vmadd_h 0111 00001010 10001 ..... ..... ..... ` | @vvv | vvv |
| vmadd_w | `vmadd_w 0111 00001010 10010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_d_w | `vmaddwev_d_w 0111 00001010 11010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_d_wu | `vmaddwev_d_wu 0111 00001011 01010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_d_wu_w | `vmaddwev_d_wu_w 0111 00001011 11010 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_h_b | `vmaddwev_h_b 0111 00001010 11000 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_h_bu | `vmaddwev_h_bu 0111 00001011 01000 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_h_bu_b | `vmaddwev_h_bu_b 0111 00001011 11000 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_q_d | `vmaddwev_q_d 0111 00001010 11011 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_q_du | `vmaddwev_q_du 0111 00001011 01011 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_q_du_d | `vmaddwev_q_du_d 0111 00001011 11011 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_w_h | `vmaddwev_w_h 0111 00001010 11001 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_w_hu | `vmaddwev_w_hu 0111 00001011 01001 ..... ..... ..... ` | @vvv | vvv |
| vmaddwev_w_hu_h | `vmaddwev_w_hu_h 0111 00001011 11001 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_d_w | `vmaddwod_d_w 0111 00001010 11110 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_d_wu | `vmaddwod_d_wu 0111 00001011 01110 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_d_wu_w | `vmaddwod_d_wu_w 0111 00001011 11110 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_h_b | `vmaddwod_h_b 0111 00001010 11100 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_h_bu | `vmaddwod_h_bu 0111 00001011 01100 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_h_bu_b | `vmaddwod_h_bu_b 0111 00001011 11100 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_q_d | `vmaddwod_q_d 0111 00001010 11111 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_q_du | `vmaddwod_q_du 0111 00001011 01111 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_q_du_d | `vmaddwod_q_du_d 0111 00001011 11111 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_w_h | `vmaddwod_w_h 0111 00001010 11101 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_w_hu | `vmaddwod_w_hu 0111 00001011 01101 ..... ..... ..... ` | @vvv | vvv |
| vmaddwod_w_hu_h | `vmaddwod_w_hu_h 0111 00001011 11101 ..... ..... ..... ` | @vvv | vvv |
| vmod_b | `vmod_b 0111 00001110 00100 ..... ..... ..... ` | @vvv | vvv |
| vmod_bu | `vmod_bu 0111 00001110 01100 ..... ..... ..... ` | @vvv | vvv |
| vmod_d | `vmod_d 0111 00001110 00111 ..... ..... ..... ` | @vvv | vvv |
| vmod_du | `vmod_du 0111 00001110 01111 ..... ..... ..... ` | @vvv | vvv |
| vmod_h | `vmod_h 0111 00001110 00101 ..... ..... ..... ` | @vvv | vvv |
| vmod_hu | `vmod_hu 0111 00001110 01101 ..... ..... ..... ` | @vvv | vvv |
| vmod_w | `vmod_w 0111 00001110 00110 ..... ..... ..... ` | @vvv | vvv |
| vmod_wu | `vmod_wu 0111 00001110 01110 ..... ..... ..... ` | @vvv | vvv |
| vmsub_b | `vmsub_b 0111 00001010 10100 ..... ..... ..... ` | @vvv | vvv |
| vmsub_d | `vmsub_d 0111 00001010 10111 ..... ..... ..... ` | @vvv | vvv |
| vmsub_h | `vmsub_h 0111 00001010 10101 ..... ..... ..... ` | @vvv | vvv |
| vmsub_w | `vmsub_w 0111 00001010 10110 ..... ..... ..... ` | @vvv | vvv |
| vmul_b | `vmul_b 0111 00001000 01000 ..... ..... ..... ` | @vvv | vvv |
| vmul_d | `vmul_d 0111 00001000 01011 ..... ..... ..... ` | @vvv | vvv |
| vmul_h | `vmul_h 0111 00001000 01001 ..... ..... ..... ` | @vvv | vvv |
| vmul_w | `vmul_w 0111 00001000 01010 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_d_w | `vmulwev_d_w 0111 00001001 00010 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_d_wu | `vmulwev_d_wu 0111 00001001 10010 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_d_wu_w | `vmulwev_d_wu_w 0111 00001010 00010 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_h_b | `vmulwev_h_b 0111 00001001 00000 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_h_bu | `vmulwev_h_bu 0111 00001001 10000 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_h_bu_b | `vmulwev_h_bu_b 0111 00001010 00000 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_q_d | `vmulwev_q_d 0111 00001001 00011 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_q_du | `vmulwev_q_du 0111 00001001 10011 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_q_du_d | `vmulwev_q_du_d 0111 00001010 00011 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_w_h | `vmulwev_w_h 0111 00001001 00001 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_w_hu | `vmulwev_w_hu 0111 00001001 10001 ..... ..... ..... ` | @vvv | vvv |
| vmulwev_w_hu_h | `vmulwev_w_hu_h 0111 00001010 00001 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_d_w | `vmulwod_d_w 0111 00001001 00110 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_d_wu | `vmulwod_d_wu 0111 00001001 10110 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_d_wu_w | `vmulwod_d_wu_w 0111 00001010 00110 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_h_b | `vmulwod_h_b 0111 00001001 00100 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_h_bu | `vmulwod_h_bu 0111 00001001 10100 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_h_bu_b | `vmulwod_h_bu_b 0111 00001010 00100 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_q_d | `vmulwod_q_d 0111 00001001 00111 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_q_du | `vmulwod_q_du 0111 00001001 10111 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_q_du_d | `vmulwod_q_du_d 0111 00001010 00111 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_w_h | `vmulwod_w_h 0111 00001001 00101 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_w_hu | `vmulwod_w_hu 0111 00001001 10101 ..... ..... ..... ` | @vvv | vvv |
| vmulwod_w_hu_h | `vmulwod_w_hu_h 0111 00001010 00101 ..... ..... ..... ` | @vvv | vvv |


### 提取打包 (共 48 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vextrins_b | `vextrins_b 0111 00111000 11 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vextrins_d | `vextrins_d 0111 00111000 00 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vextrins_h | `vextrins_h 0111 00111000 10 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vextrins_w | `vextrins_w 0111 00111000 01 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vinsgr2vr_b | `vinsgr2vr_b 0111 00101110 10111 0 .... ..... ..... ` | @vr_ui4 | vr_i |
| vinsgr2vr_d | `vinsgr2vr_d 0111 00101110 10111 1110 . ..... ..... ` | @vr_ui1 | vr_i |
| vinsgr2vr_h | `vinsgr2vr_h 0111 00101110 10111 10 ... ..... ..... ` | @vr_ui3 | vr_i |
| vinsgr2vr_w | `vinsgr2vr_w 0111 00101110 10111 110 .. ..... ..... ` | @vr_ui2 | vr_i |
| vldrepl_b | `vldrepl_b 0011 000010 ............ ..... ..... ` | @vr_i12 | vr_i |
| vldrepl_d | `vldrepl_d 0011 00000001 0 ......... ..... ..... ` | @vr_i9 | vr_i |
| vldrepl_h | `vldrepl_h 0011 0000010 ........... ..... ..... ` | @vr_i11 | vr_i |
| vldrepl_w | `vldrepl_w 0011 00000010 .......... ..... ..... ` | @vr_i10 | vr_i |
| vpackev_b | `vpackev_b 0111 00010001 01100 ..... ..... ..... ` | @vvv | vvv |
| vpackev_d | `vpackev_d 0111 00010001 01111 ..... ..... ..... ` | @vvv | vvv |
| vpackev_h | `vpackev_h 0111 00010001 01101 ..... ..... ..... ` | @vvv | vvv |
| vpackev_w | `vpackev_w 0111 00010001 01110 ..... ..... ..... ` | @vvv | vvv |
| vpackod_b | `vpackod_b 0111 00010001 10000 ..... ..... ..... ` | @vvv | vvv |
| vpackod_d | `vpackod_d 0111 00010001 10011 ..... ..... ..... ` | @vvv | vvv |
| vpackod_h | `vpackod_h 0111 00010001 10001 ..... ..... ..... ` | @vvv | vvv |
| vpackod_w | `vpackod_w 0111 00010001 10010 ..... ..... ..... ` | @vvv | vvv |
| vpickev_b | `vpickev_b 0111 00010001 11100 ..... ..... ..... ` | @vvv | vvv |
| vpickev_d | `vpickev_d 0111 00010001 11111 ..... ..... ..... ` | @vvv | vvv |
| vpickev_h | `vpickev_h 0111 00010001 11101 ..... ..... ..... ` | @vvv | vvv |
| vpickev_w | `vpickev_w 0111 00010001 11110 ..... ..... ..... ` | @vvv | vvv |
| vpickod_b | `vpickod_b 0111 00010010 00000 ..... ..... ..... ` | @vvv | vvv |
| vpickod_d | `vpickod_d 0111 00010010 00011 ..... ..... ..... ` | @vvv | vvv |
| vpickod_h | `vpickod_h 0111 00010010 00001 ..... ..... ..... ` | @vvv | vvv |
| vpickod_w | `vpickod_w 0111 00010010 00010 ..... ..... ..... ` | @vvv | vvv |
| vpickve2gr_b | `vpickve2gr_b 0111 00101110 11111 0 .... ..... ..... ` | @rv_ui4 | rv_i |
| vpickve2gr_bu | `vpickve2gr_bu 0111 00101111 00111 0 .... ..... ..... ` | @rv_ui4 | rv_i |
| vpickve2gr_d | `vpickve2gr_d 0111 00101110 11111 1110 . ..... ..... ` | @rv_ui1 | rv_i |
| vpickve2gr_du | `vpickve2gr_du 0111 00101111 00111 1110 . ..... ..... ` | @rv_ui1 | rv_i |
| vpickve2gr_h | `vpickve2gr_h 0111 00101110 11111 10 ... ..... ..... ` | @rv_ui3 | rv_i |
| vpickve2gr_hu | `vpickve2gr_hu 0111 00101111 00111 10 ... ..... ..... ` | @rv_ui3 | rv_i |
| vpickve2gr_w | `vpickve2gr_w 0111 00101110 11111 110 .. ..... ..... ` | @rv_ui2 | rv_i |
| vpickve2gr_wu | `vpickve2gr_wu 0111 00101111 00111 110 .. ..... ..... ` | @rv_ui2 | rv_i |
| vreplgr2vr_b | `vreplgr2vr_b 0111 00101001 11110 00000 ..... ..... ` | @vr | vr |
| vreplgr2vr_d | `vreplgr2vr_d 0111 00101001 11110 00011 ..... ..... ` | @vr | vr |
| vreplgr2vr_h | `vreplgr2vr_h 0111 00101001 11110 00001 ..... ..... ` | @vr | vr |
| vreplgr2vr_w | `vreplgr2vr_w 0111 00101001 11110 00010 ..... ..... ` | @vr | vr |
| vreplve_b | `vreplve_b 0111 00010010 00100 ..... ..... ..... ` | @vvr | vvr |
| vreplve_d | `vreplve_d 0111 00010010 00111 ..... ..... ..... ` | @vvr | vvr |
| vreplve_h | `vreplve_h 0111 00010010 00101 ..... ..... ..... ` | @vvr | vvr |
| vreplve_w | `vreplve_w 0111 00010010 00110 ..... ..... ..... ` | @vvr | vvr |
| vreplvei_b | `vreplvei_b 0111 00101111 01111 0 .... ..... ..... ` | @vv_ui4 | vv_i |
| vreplvei_d | `vreplvei_d 0111 00101111 01111 1110 . ..... ..... ` | @vv_ui1 | vv_i |
| vreplvei_h | `vreplvei_h 0111 00101111 01111 10 ... ..... ..... ` | @vv_ui3 | vv_i |
| vreplvei_w | `vreplvei_w 0111 00101111 01111 110 .. ..... ..... ` | @vv_ui2 | vv_i |


### 置换重排 (共 17 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vilvh_b | `vilvh_b 0111 00010001 11000 ..... ..... ..... ` | @vvv | vvv |
| vilvh_d | `vilvh_d 0111 00010001 11011 ..... ..... ..... ` | @vvv | vvv |
| vilvh_h | `vilvh_h 0111 00010001 11001 ..... ..... ..... ` | @vvv | vvv |
| vilvh_w | `vilvh_w 0111 00010001 11010 ..... ..... ..... ` | @vvv | vvv |
| vilvl_b | `vilvl_b 0111 00010001 10100 ..... ..... ..... ` | @vvv | vvv |
| vilvl_d | `vilvl_d 0111 00010001 10111 ..... ..... ..... ` | @vvv | vvv |
| vilvl_h | `vilvl_h 0111 00010001 10101 ..... ..... ..... ` | @vvv | vvv |
| vilvl_w | `vilvl_w 0111 00010001 10110 ..... ..... ..... ` | @vvv | vvv |
| vpermi_w | `vpermi_w 0111 00111110 01 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vshuf4i_b | `vshuf4i_b 0111 00111001 00 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vshuf4i_d | `vshuf4i_d 0111 00111001 11 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vshuf4i_h | `vshuf4i_h 0111 00111001 01 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vshuf4i_w | `vshuf4i_w 0111 00111001 10 ........ ..... ..... ` | @vv_ui8 | vv_i |
| vshuf_b | `vshuf_b 0000 11010101 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vshuf_d | `vshuf_d 0111 00010111 10111 ..... ..... ..... ` | @vvv | vvv |
| vshuf_h | `vshuf_h 0111 00010111 10101 ..... ..... ..... ` | @vvv | vvv |
| vshuf_w | `vshuf_w 0111 00010111 10110 ..... ..... ..... ` | @vvv | vvv |


### 内存访问 (共 29 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vfrstp_b | `vfrstp_b 0111 00010010 10110 ..... ..... ..... ` | @vvv | vvv |
| vfrstp_h | `vfrstp_h 0111 00010010 10111 ..... ..... ..... ` | @vvv | vvv |
| vfrstpi_b | `vfrstpi_b 0111 00101001 10100 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vfrstpi_h | `vfrstpi_h 0111 00101001 10101 ..... ..... ..... ` | @vv_ui5 | vv_i |
| vld | `vld 0010 110000 ............ ..... ..... ` | @vr_i12 | vr_i |
| vldi | `vldi 0111 00111110 00 ............. ..... ` | @v_i13 | v_i |
| vldrepl_b | `vldrepl_b 0011 000010 ............ ..... ..... ` | @vr_i12 | vr_i |
| vldrepl_d | `vldrepl_d 0011 00000001 0 ......... ..... ..... ` | @vr_i9 | vr_i |
| vldrepl_h | `vldrepl_h 0011 0000010 ........... ..... ..... ` | @vr_i11 | vr_i |
| vldrepl_w | `vldrepl_w 0011 00000010 .......... ..... ..... ` | @vr_i10 | vr_i |
| vldx | `vldx 0011 10000100 00000 ..... ..... ..... ` | @vrr | vrr |
| vreplgr2vr_b | `vreplgr2vr_b 0111 00101001 11110 00000 ..... ..... ` | @vr | vr |
| vreplgr2vr_d | `vreplgr2vr_d 0111 00101001 11110 00011 ..... ..... ` | @vr | vr |
| vreplgr2vr_h | `vreplgr2vr_h 0111 00101001 11110 00001 ..... ..... ` | @vr | vr |
| vreplgr2vr_w | `vreplgr2vr_w 0111 00101001 11110 00010 ..... ..... ` | @vr | vr |
| vreplve_b | `vreplve_b 0111 00010010 00100 ..... ..... ..... ` | @vvr | vvr |
| vreplve_d | `vreplve_d 0111 00010010 00111 ..... ..... ..... ` | @vvr | vvr |
| vreplve_h | `vreplve_h 0111 00010010 00101 ..... ..... ..... ` | @vvr | vvr |
| vreplve_w | `vreplve_w 0111 00010010 00110 ..... ..... ..... ` | @vvr | vvr |
| vreplvei_b | `vreplvei_b 0111 00101111 01111 0 .... ..... ..... ` | @vv_ui4 | vv_i |
| vreplvei_d | `vreplvei_d 0111 00101111 01111 1110 . ..... ..... ` | @vv_ui1 | vv_i |
| vreplvei_h | `vreplvei_h 0111 00101111 01111 10 ... ..... ..... ` | @vv_ui3 | vv_i |
| vreplvei_w | `vreplvei_w 0111 00101111 01111 110 .. ..... ..... ` | @vv_ui2 | vv_i |
| vst | `vst 0010 110001 ............ ..... ..... ` | @vr_i12 | vr_i |
| vstelm_b | `vstelm_b 0011 000110 .... ........ ..... ..... ` | @vr_i8i4 | vr_ii |
| vstelm_d | `vstelm_d 0011 00010001 0 . ........ ..... ..... ` | @vr_i8i1 | vr_ii |
| vstelm_h | `vstelm_h 0011 0001010 ... ........ ..... ..... ` | @vr_i8i3 | vr_ii |
| vstelm_w | `vstelm_w 0011 00010010 .. ........ ..... ..... ` | @vr_i8i2 | vr_ii |
| vstx | `vstx 0011 10000100 01000 ..... ..... ..... ` | @vrr | vrr |


### 浮点基本运算 (共 22 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vfadd_d | `vfadd_d 0111 00010011 00010 ..... ..... ..... ` | @vvv | vvv |
| vfadd_s | `vfadd_s 0111 00010011 00001 ..... ..... ..... ` | @vvv | vvv |
| vfclass_d | `vfclass_d 0111 00101001 11001 10110 ..... ..... ` | @vv | vv |
| vfclass_s | `vfclass_s 0111 00101001 11001 10101 ..... ..... ` | @vv | vv |
| vfdiv_d | `vfdiv_d 0111 00010011 10110 ..... ..... ..... ` | @vvv | vvv |
| vfdiv_s | `vfdiv_s 0111 00010011 10101 ..... ..... ..... ` | @vvv | vvv |
| vflogb_d | `vflogb_d 0111 00101001 11001 10010 ..... ..... ` | @vv | vv |
| vflogb_s | `vflogb_s 0111 00101001 11001 10001 ..... ..... ` | @vv | vv |
| vfmul_d | `vfmul_d 0111 00010011 10010 ..... ..... ..... ` | @vvv | vvv |
| vfmul_s | `vfmul_s 0111 00010011 10001 ..... ..... ..... ` | @vvv | vvv |
| vfrecip_d | `vfrecip_d 0111 00101001 11001 11110 ..... ..... ` | @vv | vv |
| vfrecip_s | `vfrecip_s 0111 00101001 11001 11101 ..... ..... ` | @vv | vv |
| vfrecipe_d | `vfrecipe_d 0111 00101001 11010 00110 ..... ..... ` | @vv | vv |
| vfrecipe_s | `vfrecipe_s 0111 00101001 11010 00101 ..... ..... ` | @vv | vv |
| vfrsqrt_d | `vfrsqrt_d 0111 00101001 11010 00010 ..... ..... ` | @vv | vv |
| vfrsqrt_s | `vfrsqrt_s 0111 00101001 11010 00001 ..... ..... ` | @vv | vv |
| vfrsqrte_d | `vfrsqrte_d 0111 00101001 11010 01010 ..... ..... ` | @vv | vv |
| vfrsqrte_s | `vfrsqrte_s 0111 00101001 11010 01001 ..... ..... ` | @vv | vv |
| vfsqrt_d | `vfsqrt_d 0111 00101001 11001 11010 ..... ..... ` | @vv | vv |
| vfsqrt_s | `vfsqrt_s 0111 00101001 11001 11001 ..... ..... ` | @vv | vv |
| vfsub_d | `vfsub_d 0111 00010011 00110 ..... ..... ..... ` | @vvv | vvv |
| vfsub_s | `vfsub_s 0111 00010011 00101 ..... ..... ..... ` | @vvv | vvv |


### 浮点融合乘加 (共 8 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vfmadd_d | `vfmadd_d 0000 10010010 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmadd_s | `vfmadd_s 0000 10010001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmsub_d | `vfmsub_d 0000 10010110 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfmsub_s | `vfmsub_s 0000 10010101 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmadd_d | `vfnmadd_d 0000 10011010 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmadd_s | `vfnmadd_s 0000 10011001 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmsub_d | `vfnmsub_d 0000 10011110 ..... ..... ..... ..... ` | @vvvv | vvvv |
| vfnmsub_s | `vfnmsub_s 0000 10011101 ..... ..... ..... ..... ` | @vvvv | vvvv |


### 浮点比较 (共 2 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vfcmp_cond_d | `vfcmp_cond_d 0000 11000110 ..... ..... ..... ..... ` | @vvv_fcond | vvv_fcond |
| vfcmp_cond_s | `vfcmp_cond_s 0000 11000101 ..... ..... ..... ..... ` | @vvv_fcond | vvv_fcond |


### 浮点转换 (共 52 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| vfcvt_h_s | `vfcvt_h_s 0111 00010100 01100 ..... ..... ..... ` | @vvv | vvv |
| vfcvt_s_d | `vfcvt_s_d 0111 00010100 01101 ..... ..... ..... ` | @vvv | vvv |
| vfcvth_d_s | `vfcvth_d_s 0111 00101001 11011 11101 ..... ..... ` | @vv | vv |
| vfcvth_s_h | `vfcvth_s_h 0111 00101001 11011 11011 ..... ..... ` | @vv | vv |
| vfcvtl_d_s | `vfcvtl_d_s 0111 00101001 11011 11100 ..... ..... ` | @vv | vv |
| vfcvtl_s_h | `vfcvtl_s_h 0111 00101001 11011 11010 ..... ..... ` | @vv | vv |
| vffint_d_l | `vffint_d_l 0111 00101001 11100 00010 ..... ..... ` | @vv | vv |
| vffint_d_lu | `vffint_d_lu 0111 00101001 11100 00011 ..... ..... ` | @vv | vv |
| vffint_s_l | `vffint_s_l 0111 00010100 10000 ..... ..... ..... ` | @vvv | vvv |
| vffint_s_w | `vffint_s_w 0111 00101001 11100 00000 ..... ..... ` | @vv | vv |
| vffint_s_wu | `vffint_s_wu 0111 00101001 11100 00001 ..... ..... ` | @vv | vv |
| vffinth_d_w | `vffinth_d_w 0111 00101001 11100 00101 ..... ..... ` | @vv | vv |
| vffintl_d_w | `vffintl_d_w 0111 00101001 11100 00100 ..... ..... ` | @vv | vv |
| vfrint_d | `vfrint_d 0111 00101001 11010 01110 ..... ..... ` | @vv | vv |
| vfrint_s | `vfrint_s 0111 00101001 11010 01101 ..... ..... ` | @vv | vv |
| vfrintrm_d | `vfrintrm_d 0111 00101001 11010 10010 ..... ..... ` | @vv | vv |
| vfrintrm_s | `vfrintrm_s 0111 00101001 11010 10001 ..... ..... ` | @vv | vv |
| vfrintrne_d | `vfrintrne_d 0111 00101001 11010 11110 ..... ..... ` | @vv | vv |
| vfrintrne_s | `vfrintrne_s 0111 00101001 11010 11101 ..... ..... ` | @vv | vv |
| vfrintrp_d | `vfrintrp_d 0111 00101001 11010 10110 ..... ..... ` | @vv | vv |
| vfrintrp_s | `vfrintrp_s 0111 00101001 11010 10101 ..... ..... ` | @vv | vv |
| vfrintrz_d | `vfrintrz_d 0111 00101001 11010 11010 ..... ..... ` | @vv | vv |
| vfrintrz_s | `vfrintrz_s 0111 00101001 11010 11001 ..... ..... ` | @vv | vv |
| vftint_l_d | `vftint_l_d 0111 00101001 11100 01101 ..... ..... ` | @vv | vv |
| vftint_lu_d | `vftint_lu_d 0111 00101001 11100 10111 ..... ..... ` | @vv | vv |
| vftint_w_d | `vftint_w_d 0111 00010100 10011 ..... ..... ..... ` | @vvv | vvv |
| vftint_w_s | `vftint_w_s 0111 00101001 11100 01100 ..... ..... ` | @vv | vv |
| vftint_wu_s | `vftint_wu_s 0111 00101001 11100 10110 ..... ..... ` | @vv | vv |
| vftinth_l_s | `vftinth_l_s 0111 00101001 11101 00001 ..... ..... ` | @vv | vv |
| vftintl_l_s | `vftintl_l_s 0111 00101001 11101 00000 ..... ..... ` | @vv | vv |
| vftintrm_l_d | `vftintrm_l_d 0111 00101001 11100 01111 ..... ..... ` | @vv | vv |
| vftintrm_w_d | `vftintrm_w_d 0111 00010100 10100 ..... ..... ..... ` | @vvv | vvv |
| vftintrm_w_s | `vftintrm_w_s 0111 00101001 11100 01110 ..... ..... ` | @vv | vv |
| vftintrmh_l_s | `vftintrmh_l_s 0111 00101001 11101 00011 ..... ..... ` | @vv | vv |
| vftintrml_l_s | `vftintrml_l_s 0111 00101001 11101 00010 ..... ..... ` | @vv | vv |
| vftintrne_l_d | `vftintrne_l_d 0111 00101001 11100 10101 ..... ..... ` | @vv | vv |
| vftintrne_w_d | `vftintrne_w_d 0111 00010100 10111 ..... ..... ..... ` | @vvv | vvv |
| vftintrne_w_s | `vftintrne_w_s 0111 00101001 11100 10100 ..... ..... ` | @vv | vv |
| vftintrneh_l_s | `vftintrneh_l_s 0111 00101001 11101 01001 ..... ..... ` | @vv | vv |
| vftintrnel_l_s | `vftintrnel_l_s 0111 00101001 11101 01000 ..... ..... ` | @vv | vv |
| vftintrp_l_d | `vftintrp_l_d 0111 00101001 11100 10001 ..... ..... ` | @vv | vv |
| vftintrp_w_d | `vftintrp_w_d 0111 00010100 10101 ..... ..... ..... ` | @vvv | vvv |
| vftintrp_w_s | `vftintrp_w_s 0111 00101001 11100 10000 ..... ..... ` | @vv | vv |
| vftintrph_l_s | `vftintrph_l_s 0111 00101001 11101 00101 ..... ..... ` | @vv | vv |
| vftintrpl_l_s | `vftintrpl_l_s 0111 00101001 11101 00100 ..... ..... ` | @vv | vv |
| vftintrz_l_d | `vftintrz_l_d 0111 00101001 11100 10011 ..... ..... ` | @vv | vv |
| vftintrz_lu_d | `vftintrz_lu_d 0111 00101001 11100 11101 ..... ..... ` | @vv | vv |
| vftintrz_w_d | `vftintrz_w_d 0111 00010100 10110 ..... ..... ..... ` | @vvv | vvv |
| vftintrz_w_s | `vftintrz_w_s 0111 00101001 11100 10010 ..... ..... ` | @vv | vv |
| vftintrz_wu_s | `vftintrz_wu_s 0111 00101001 11100 11100 ..... ..... ` | @vv | vv |
| vftintrzh_l_s | `vftintrzh_l_s 0111 00101001 11101 00111 ..... ..... ` | @vv | vv |
| vftintrzl_l_s | `vftintrzl_l_s 0111 00101001 11101 00110 ..... ..... ` | @vv | vv |


## 统计

- 总共提取 **686** 条 LSX 指令

