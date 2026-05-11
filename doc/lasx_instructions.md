# LASX (LoongArch SIMD) 指令编码汇总

本文档从 QEMU 的 `insns.decode` 文件中提取所有 LASX 指令的编码信息。
QEMU is Copyright (c) 2023-2026 QEMU contributors, licensed under GPL-2.0-only.

## 前言

### LASX 指令总体编码格式

LASX 指令为 **32 位定长编码**，编码布局如下（从高位到低位，即比特 31 到比特 0）：

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

## 特殊字段定义

```
%i9s3     10:s9       !function=shl_3
%i10s2    10:s10      !function=shl_2
%i11s1    10:s11      !function=shl_1
%i8s3     10:s8       !function=shl_3
%i8s2     10:s8       !function=shl_2
%i8s1     10:s8       !function=shl_1
```

## 参数集定义

```
&vv           vd vj
&cv           cd vj
&vvv          vd vj vk
&vv_i         vd vj imm
&vvvv         vd vj vk va
&vvv_fcond    vd vj vk fcond
&vr_i         vd rj imm
&rv_i         rd vj imm
&vr           vd rj
&vvr          vd vj rk
&vrr          vd rj rk
&vr_ii        vd rj imm imm2
&v_i          vd imm
```

## 指令格式定义

```
@vv               .... ........ ..... ..... vj:5 vd:5    &vv
@cv            .... ........ ..... ..... vj:5 .. cd:3    &cv
@vvv               .... ........ ..... vk:5 vj:5 vd:5    &vvv
@vv_ui1      .... ........ ..... .... imm:1 vj:5 vd:5    &vv_i
@vv_ui2       .... ........ ..... ... imm:2 vj:5 vd:5    &vv_i
@vv_ui3        .... ........ ..... .. imm:3 vj:5 vd:5    &vv_i
@vv_ui4         .... ........ ..... . imm:4 vj:5 vd:5    &vv_i
@vv_ui5           .... ........ ..... imm:5 vj:5 vd:5    &vv_i
@vv_ui6            .... ........ .... imm:6 vj:5 vd:5    &vv_i
@vv_ui7             .... ........ ... imm:7 vj:5 vd:5    &vv_i
@vv_ui8              .... ........ .. imm:8 vj:5 vd:5    &vv_i
@vv_i5           .... ........ ..... imm:s5 vj:5 vd:5    &vv_i
@vvvv               .... ........ va:5 vk:5 vj:5 vd:5    &vvvv
@vvv_fcond      .... ........ fcond:5  vk:5 vj:5 vd:5    &vvv_fcond
@vr_ui4         .... ........ ..... . imm:4 rj:5 vd:5    &vr_i
@vr_ui3        .... ........ ..... .. imm:3 rj:5 vd:5    &vr_i
@vr_ui2       .... ........ ..... ... imm:2 rj:5 vd:5    &vr_i
@vr_ui1      .... ........ ..... .... imm:1 rj:5 vd:5    &vr_i
@rv_ui4         .... ........ ..... . imm:4 vj:5 rd:5    &rv_i
@rv_ui3        .... ........ ..... .. imm:3 vj:5 rd:5    &rv_i
@rv_ui2       .... ........ ..... ... imm:2 vj:5 rd:5    &rv_i
@rv_ui1      .... ........ ..... .... imm:1 vj:5 rd:5    &rv_i
@vr               .... ........ ..... ..... rj:5 vd:5    &vr
@vvr               .... ........ ..... rk:5 vj:5 vd:5    &vvr
@vr_i9            .... ........ . ......... rj:5 vd:5    &vr_i imm=%i9s3
@vr_i10            .... ........ .......... rj:5 vd:5    &vr_i imm=%i10s2
@vr_i11            .... ....... ........... rj:5 vd:5    &vr_i imm=%i11s1
@vr_i12                 .... ...... imm:s12 rj:5 vd:5    &vr_i
@vr_i8i1    .... ........ . imm2:1 ........ rj:5 vd:5    &vr_ii imm=%i8s3
@vr_i8i2      .... ........ imm2:2 ........ rj:5 vd:5    &vr_ii imm=%i8s2
@vr_i8i3       .... ....... imm2:3 ........ rj:5 vd:5    &vr_ii imm=%i8s1
@vr_i8i4          .... ...... imm2:4 imm:s8 rj:5 vd:5    &vr_ii
@vr_i8i2x     .... ........ imm2:2 ........ rj:5 vd:5    &vr_ii imm=%i8s3
@vr_i8i3x      .... ....... imm2:3 ........ rj:5 vd:5    &vr_ii imm=%i8s2
@vr_i8i4x       .... ...... imm2:4 ........ rj:5 vd:5    &vr_ii imm=%i8s1
@vr_i8i5x          .... ..... imm2:5 imm:s8 rj:5 vd:5    &vr_ii
@vrr               .... ........ ..... rk:5 rj:5 vd:5    &vrr
@v_i13                   .... ........ .. imm:13 vd:5    &v_i
```

## 指令列表（按分类）

### 算术运算 (共 166 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvabsd_b | `xvabsd_b 0111 01000110 00000 ..... ..... .....` | @vvv | vvv |
| xvabsd_bu | `xvabsd_bu 0111 01000110 00100 ..... ..... .....` | @vvv | vvv |
| xvabsd_d | `xvabsd_d 0111 01000110 00011 ..... ..... .....` | @vvv | vvv |
| xvabsd_du | `xvabsd_du 0111 01000110 00111 ..... ..... .....` | @vvv | vvv |
| xvabsd_h | `xvabsd_h 0111 01000110 00001 ..... ..... .....` | @vvv | vvv |
| xvabsd_hu | `xvabsd_hu 0111 01000110 00101 ..... ..... .....` | @vvv | vvv |
| xvabsd_w | `xvabsd_w 0111 01000110 00010 ..... ..... .....` | @vvv | vvv |
| xvabsd_wu | `xvabsd_wu 0111 01000110 00110 ..... ..... .....` | @vvv | vvv |
| xvadd_b | `xvadd_b 0111 01000000 10100 ..... ..... .....` | @vvv | vvv |
| xvadd_d | `xvadd_d 0111 01000000 10111 ..... ..... .....` | @vvv | vvv |
| xvadd_h | `xvadd_h 0111 01000000 10101 ..... ..... .....` | @vvv | vvv |
| xvadd_q | `xvadd_q 0111 01010010 11010 ..... ..... .....` | @vvv | vvv |
| xvadd_w | `xvadd_w 0111 01000000 10110 ..... ..... .....` | @vvv | vvv |
| xvadda_b | `xvadda_b 0111 01000101 11000 ..... ..... .....` | @vvv | vvv |
| xvadda_d | `xvadda_d 0111 01000101 11011 ..... ..... .....` | @vvv | vvv |
| xvadda_h | `xvadda_h 0111 01000101 11001 ..... ..... .....` | @vvv | vvv |
| xvadda_w | `xvadda_w 0111 01000101 11010 ..... ..... .....` | @vvv | vvv |
| xvaddi_bu | `xvaddi_bu 0111 01101000 10100 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvaddi_du | `xvaddi_du 0111 01101000 10111 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvaddi_hu | `xvaddi_hu 0111 01101000 10101 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvaddi_wu | `xvaddi_wu 0111 01101000 10110 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvaddwev_d_w | `xvaddwev_d_w 0111 01000001 11110 ..... ..... .....` | @vvv | vvv |
| xvaddwev_d_wu | `xvaddwev_d_wu 0111 01000010 11110 ..... ..... .....` | @vvv | vvv |
| xvaddwev_d_wu_w | `xvaddwev_d_wu_w 0111 01000011 11110 ..... ..... .....` | @vvv | vvv |
| xvaddwev_h_b | `xvaddwev_h_b 0111 01000001 11100 ..... ..... .....` | @vvv | vvv |
| xvaddwev_h_bu | `xvaddwev_h_bu 0111 01000010 11100 ..... ..... .....` | @vvv | vvv |
| xvaddwev_h_bu_b | `xvaddwev_h_bu_b 0111 01000011 11100 ..... ..... .....` | @vvv | vvv |
| xvaddwev_q_d | `xvaddwev_q_d 0111 01000001 11111 ..... ..... .....` | @vvv | vvv |
| xvaddwev_q_du | `xvaddwev_q_du 0111 01000010 11111 ..... ..... .....` | @vvv | vvv |
| xvaddwev_q_du_d | `xvaddwev_q_du_d 0111 01000011 11111 ..... ..... .....` | @vvv | vvv |
| xvaddwev_w_h | `xvaddwev_w_h 0111 01000001 11101 ..... ..... .....` | @vvv | vvv |
| xvaddwev_w_hu | `xvaddwev_w_hu 0111 01000010 11101 ..... ..... .....` | @vvv | vvv |
| xvaddwev_w_hu_h | `xvaddwev_w_hu_h 0111 01000011 11101 ..... ..... .....` | @vvv | vvv |
| xvaddwod_d_w | `xvaddwod_d_w 0111 01000010 00110 ..... ..... .....` | @vvv | vvv |
| xvaddwod_d_wu | `xvaddwod_d_wu 0111 01000011 00110 ..... ..... .....` | @vvv | vvv |
| xvaddwod_d_wu_w | `xvaddwod_d_wu_w 0111 01000100 00010 ..... ..... .....` | @vvv | vvv |
| xvaddwod_h_b | `xvaddwod_h_b 0111 01000010 00100 ..... ..... .....` | @vvv | vvv |
| xvaddwod_h_bu | `xvaddwod_h_bu 0111 01000011 00100 ..... ..... .....` | @vvv | vvv |
| xvaddwod_h_bu_b | `xvaddwod_h_bu_b 0111 01000100 00000 ..... ..... .....` | @vvv | vvv |
| xvaddwod_q_d | `xvaddwod_q_d 0111 01000010 00111 ..... ..... .....` | @vvv | vvv |
| xvaddwod_q_du | `xvaddwod_q_du 0111 01000011 00111 ..... ..... .....` | @vvv | vvv |
| xvaddwod_q_du_d | `xvaddwod_q_du_d 0111 01000100 00011 ..... ..... .....` | @vvv | vvv |
| xvaddwod_w_h | `xvaddwod_w_h 0111 01000010 00101 ..... ..... .....` | @vvv | vvv |
| xvaddwod_w_hu | `xvaddwod_w_hu 0111 01000011 00101 ..... ..... .....` | @vvv | vvv |
| xvaddwod_w_hu_h | `xvaddwod_w_hu_h 0111 01000100 00001 ..... ..... .....` | @vvv | vvv |
| xvavg_b | `xvavg_b 0111 01000110 01000 ..... ..... .....` | @vvv | vvv |
| xvavg_bu | `xvavg_bu 0111 01000110 01100 ..... ..... .....` | @vvv | vvv |
| xvavg_d | `xvavg_d 0111 01000110 01011 ..... ..... .....` | @vvv | vvv |
| xvavg_du | `xvavg_du 0111 01000110 01111 ..... ..... .....` | @vvv | vvv |
| xvavg_h | `xvavg_h 0111 01000110 01001 ..... ..... .....` | @vvv | vvv |
| xvavg_hu | `xvavg_hu 0111 01000110 01101 ..... ..... .....` | @vvv | vvv |
| xvavg_w | `xvavg_w 0111 01000110 01010 ..... ..... .....` | @vvv | vvv |
| xvavg_wu | `xvavg_wu 0111 01000110 01110 ..... ..... .....` | @vvv | vvv |
| xvavgr_b | `xvavgr_b 0111 01000110 10000 ..... ..... .....` | @vvv | vvv |
| xvavgr_bu | `xvavgr_bu 0111 01000110 10100 ..... ..... .....` | @vvv | vvv |
| xvavgr_d | `xvavgr_d 0111 01000110 10011 ..... ..... .....` | @vvv | vvv |
| xvavgr_du | `xvavgr_du 0111 01000110 10111 ..... ..... .....` | @vvv | vvv |
| xvavgr_h | `xvavgr_h 0111 01000110 10001 ..... ..... .....` | @vvv | vvv |
| xvavgr_hu | `xvavgr_hu 0111 01000110 10101 ..... ..... .....` | @vvv | vvv |
| xvavgr_w | `xvavgr_w 0111 01000110 10010 ..... ..... .....` | @vvv | vvv |
| xvavgr_wu | `xvavgr_wu 0111 01000110 10110 ..... ..... .....` | @vvv | vvv |
| xvfadd_d | `xvfadd_d 0111 01010011 00010 ..... ..... .....` | @vvv | vvv |
| xvfadd_s | `xvfadd_s 0111 01010011 00001 ..... ..... .....` | @vvv | vvv |
| xvfmadd_d | `xvfmadd_d 0000 10100010 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfmadd_s | `xvfmadd_s 0000 10100001 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfmsub_d | `xvfmsub_d 0000 10100110 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfmsub_s | `xvfmsub_s 0000 10100101 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfnmadd_d | `xvfnmadd_d 0000 10101010 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfnmadd_s | `xvfnmadd_s 0000 10101001 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfnmsub_d | `xvfnmsub_d 0000 10101110 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfnmsub_s | `xvfnmsub_s 0000 10101101 ..... ..... ..... .....` | @vvvv | vvvv |
| xvfsub_d | `xvfsub_d 0111 01010011 00110 ..... ..... .....` | @vvv | vvv |
| xvfsub_s | `xvfsub_s 0111 01010011 00101 ..... ..... .....` | @vvv | vvv |
| xvhaddw_d_w | `xvhaddw_d_w 0111 01000101 01010 ..... ..... .....` | @vvv | vvv |
| xvhaddw_du_wu | `xvhaddw_du_wu 0111 01000101 10010 ..... ..... .....` | @vvv | vvv |
| xvhaddw_h_b | `xvhaddw_h_b 0111 01000101 01000 ..... ..... .....` | @vvv | vvv |
| xvhaddw_hu_bu | `xvhaddw_hu_bu 0111 01000101 10000 ..... ..... .....` | @vvv | vvv |
| xvhaddw_q_d | `xvhaddw_q_d 0111 01000101 01011 ..... ..... .....` | @vvv | vvv |
| xvhaddw_qu_du | `xvhaddw_qu_du 0111 01000101 10011 ..... ..... .....` | @vvv | vvv |
| xvhaddw_w_h | `xvhaddw_w_h 0111 01000101 01001 ..... ..... .....` | @vvv | vvv |
| xvhaddw_wu_hu | `xvhaddw_wu_hu 0111 01000101 10001 ..... ..... .....` | @vvv | vvv |
| xvhsubw_d_w | `xvhsubw_d_w 0111 01000101 01110 ..... ..... .....` | @vvv | vvv |
| xvhsubw_du_wu | `xvhsubw_du_wu 0111 01000101 10110 ..... ..... .....` | @vvv | vvv |
| xvhsubw_h_b | `xvhsubw_h_b 0111 01000101 01100 ..... ..... .....` | @vvv | vvv |
| xvhsubw_hu_bu | `xvhsubw_hu_bu 0111 01000101 10100 ..... ..... .....` | @vvv | vvv |
| xvhsubw_q_d | `xvhsubw_q_d 0111 01000101 01111 ..... ..... .....` | @vvv | vvv |
| xvhsubw_qu_du | `xvhsubw_qu_du 0111 01000101 10111 ..... ..... .....` | @vvv | vvv |
| xvhsubw_w_h | `xvhsubw_w_h 0111 01000101 01101 ..... ..... .....` | @vvv | vvv |
| xvhsubw_wu_hu | `xvhsubw_wu_hu 0111 01000101 10101 ..... ..... .....` | @vvv | vvv |
| xvmadd_b | `xvmadd_b 0111 01001010 10000 ..... ..... .....` | @vvv | vvv |
| xvmadd_d | `xvmadd_d 0111 01001010 10011 ..... ..... .....` | @vvv | vvv |
| xvmadd_h | `xvmadd_h 0111 01001010 10001 ..... ..... .....` | @vvv | vvv |
| xvmadd_w | `xvmadd_w 0111 01001010 10010 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_d_w | `xvmaddwev_d_w 0111 01001010 11010 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_d_wu | `xvmaddwev_d_wu 0111 01001011 01010 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_d_wu_w | `xvmaddwev_d_wu_w 0111 01001011 11010 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_h_b | `xvmaddwev_h_b 0111 01001010 11000 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_h_bu | `xvmaddwev_h_bu 0111 01001011 01000 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_h_bu_b | `xvmaddwev_h_bu_b 0111 01001011 11000 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_q_d | `xvmaddwev_q_d 0111 01001010 11011 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_q_du | `xvmaddwev_q_du 0111 01001011 01011 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_q_du_d | `xvmaddwev_q_du_d 0111 01001011 11011 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_w_h | `xvmaddwev_w_h 0111 01001010 11001 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_w_hu | `xvmaddwev_w_hu 0111 01001011 01001 ..... ..... .....` | @vvv | vvv |
| xvmaddwev_w_hu_h | `xvmaddwev_w_hu_h 0111 01001011 11001 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_d_w | `xvmaddwod_d_w 0111 01001010 11110 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_d_wu | `xvmaddwod_d_wu 0111 01001011 01110 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_d_wu_w | `xvmaddwod_d_wu_w 0111 01001011 11110 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_h_b | `xvmaddwod_h_b 0111 01001010 11100 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_h_bu | `xvmaddwod_h_bu 0111 01001011 01100 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_h_bu_b | `xvmaddwod_h_bu_b 0111 01001011 11100 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_q_d | `xvmaddwod_q_d 0111 01001010 11111 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_q_du | `xvmaddwod_q_du 0111 01001011 01111 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_q_du_d | `xvmaddwod_q_du_d 0111 01001011 11111 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_w_h | `xvmaddwod_w_h 0111 01001010 11101 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_w_hu | `xvmaddwod_w_hu 0111 01001011 01101 ..... ..... .....` | @vvv | vvv |
| xvmaddwod_w_hu_h | `xvmaddwod_w_hu_h 0111 01001011 11101 ..... ..... .....` | @vvv | vvv |
| xvmsub_b | `xvmsub_b 0111 01001010 10100 ..... ..... .....` | @vvv | vvv |
| xvmsub_d | `xvmsub_d 0111 01001010 10111 ..... ..... .....` | @vvv | vvv |
| xvmsub_h | `xvmsub_h 0111 01001010 10101 ..... ..... .....` | @vvv | vvv |
| xvmsub_w | `xvmsub_w 0111 01001010 10110 ..... ..... .....` | @vvv | vvv |
| xvneg_b | `xvneg_b 0111 01101001 11000 01100 ..... .....` | @vv | vv |
| xvneg_d | `xvneg_d 0111 01101001 11000 01111 ..... .....` | @vv | vv |
| xvneg_h | `xvneg_h 0111 01101001 11000 01101 ..... .....` | @vv | vv |
| xvneg_w | `xvneg_w 0111 01101001 11000 01110 ..... .....` | @vv | vv |
| xvsadd_b | `xvsadd_b 0111 01000100 01100 ..... ..... .....` | @vvv | vvv |
| xvsadd_bu | `xvsadd_bu 0111 01000100 10100 ..... ..... .....` | @vvv | vvv |
| xvsadd_d | `xvsadd_d 0111 01000100 01111 ..... ..... .....` | @vvv | vvv |
| xvsadd_du | `xvsadd_du 0111 01000100 10111 ..... ..... .....` | @vvv | vvv |
| xvsadd_h | `xvsadd_h 0111 01000100 01101 ..... ..... .....` | @vvv | vvv |
| xvsadd_hu | `xvsadd_hu 0111 01000100 10101 ..... ..... .....` | @vvv | vvv |
| xvsadd_w | `xvsadd_w 0111 01000100 01110 ..... ..... .....` | @vvv | vvv |
| xvsadd_wu | `xvsadd_wu 0111 01000100 10110 ..... ..... .....` | @vvv | vvv |
| xvssub_b | `xvssub_b 0111 01000100 10000 ..... ..... .....` | @vvv | vvv |
| xvssub_bu | `xvssub_bu 0111 01000100 11000 ..... ..... .....` | @vvv | vvv |
| xvssub_d | `xvssub_d 0111 01000100 10011 ..... ..... .....` | @vvv | vvv |
| xvssub_du | `xvssub_du 0111 01000100 11011 ..... ..... .....` | @vvv | vvv |
| xvssub_h | `xvssub_h 0111 01000100 10001 ..... ..... .....` | @vvv | vvv |
| xvssub_hu | `xvssub_hu 0111 01000100 11001 ..... ..... .....` | @vvv | vvv |
| xvssub_w | `xvssub_w 0111 01000100 10010 ..... ..... .....` | @vvv | vvv |
| xvssub_wu | `xvssub_wu 0111 01000100 11010 ..... ..... .....` | @vvv | vvv |
| xvsub_b | `xvsub_b 0111 01000000 11000 ..... ..... .....` | @vvv | vvv |
| xvsub_d | `xvsub_d 0111 01000000 11011 ..... ..... .....` | @vvv | vvv |
| xvsub_h | `xvsub_h 0111 01000000 11001 ..... ..... .....` | @vvv | vvv |
| xvsub_q | `xvsub_q 0111 01010010 11011 ..... ..... .....` | @vvv | vvv |
| xvsub_w | `xvsub_w 0111 01000000 11010 ..... ..... .....` | @vvv | vvv |
| xvsubi_bu | `xvsubi_bu 0111 01101000 11000 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsubi_du | `xvsubi_du 0111 01101000 11011 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsubi_hu | `xvsubi_hu 0111 01101000 11001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsubi_wu | `xvsubi_wu 0111 01101000 11010 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsubwev_d_w | `xvsubwev_d_w 0111 01000010 00010 ..... ..... .....` | @vvv | vvv |
| xvsubwev_d_wu | `xvsubwev_d_wu 0111 01000011 00010 ..... ..... .....` | @vvv | vvv |
| xvsubwev_h_b | `xvsubwev_h_b 0111 01000010 00000 ..... ..... .....` | @vvv | vvv |
| xvsubwev_h_bu | `xvsubwev_h_bu 0111 01000011 00000 ..... ..... .....` | @vvv | vvv |
| xvsubwev_q_d | `xvsubwev_q_d 0111 01000010 00011 ..... ..... .....` | @vvv | vvv |
| xvsubwev_q_du | `xvsubwev_q_du 0111 01000011 00011 ..... ..... .....` | @vvv | vvv |
| xvsubwev_w_h | `xvsubwev_w_h 0111 01000010 00001 ..... ..... .....` | @vvv | vvv |
| xvsubwev_w_hu | `xvsubwev_w_hu 0111 01000011 00001 ..... ..... .....` | @vvv | vvv |
| xvsubwod_d_w | `xvsubwod_d_w 0111 01000010 01010 ..... ..... .....` | @vvv | vvv |
| xvsubwod_d_wu | `xvsubwod_d_wu 0111 01000011 01010 ..... ..... .....` | @vvv | vvv |
| xvsubwod_h_b | `xvsubwod_h_b 0111 01000010 01000 ..... ..... .....` | @vvv | vvv |
| xvsubwod_h_bu | `xvsubwod_h_bu 0111 01000011 01000 ..... ..... .....` | @vvv | vvv |
| xvsubwod_q_d | `xvsubwod_q_d 0111 01000010 01011 ..... ..... .....` | @vvv | vvv |
| xvsubwod_q_du | `xvsubwod_q_du 0111 01000011 01011 ..... ..... .....` | @vvv | vvv |
| xvsubwod_w_h | `xvsubwod_w_h 0111 01000010 01001 ..... ..... .....` | @vvv | vvv |
| xvsubwod_w_hu | `xvsubwod_w_hu 0111 01000011 01001 ..... ..... .....` | @vvv | vvv |


### 立即数操作 (共 36 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvmaxi_b | `xvmaxi_b 0111 01101001 00000 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmaxi_bu | `xvmaxi_bu 0111 01101001 01000 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmaxi_d | `xvmaxi_d 0111 01101001 00011 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmaxi_du | `xvmaxi_du 0111 01101001 01011 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmaxi_h | `xvmaxi_h 0111 01101001 00001 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmaxi_hu | `xvmaxi_hu 0111 01101001 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmaxi_w | `xvmaxi_w 0111 01101001 00010 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmaxi_wu | `xvmaxi_wu 0111 01101001 01010 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmini_b | `xvmini_b 0111 01101001 00100 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmini_bu | `xvmini_bu 0111 01101001 01100 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmini_d | `xvmini_d 0111 01101001 00111 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmini_du | `xvmini_du 0111 01101001 01111 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmini_h | `xvmini_h 0111 01101001 00101 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmini_hu | `xvmini_hu 0111 01101001 01101 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvmini_w | `xvmini_w 0111 01101001 00110 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvmini_wu | `xvmini_wu 0111 01101001 01110 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvseqi_b | `xvseqi_b 0111 01101000 00000 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvseqi_d | `xvseqi_d 0111 01101000 00011 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvseqi_h | `xvseqi_h 0111 01101000 00001 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvseqi_w | `xvseqi_w 0111 01101000 00010 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslei_b | `xvslei_b 0111 01101000 00100 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslei_bu | `xvslei_bu 0111 01101000 01000 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslei_d | `xvslei_d 0111 01101000 00111 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslei_du | `xvslei_du 0111 01101000 01011 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslei_h | `xvslei_h 0111 01101000 00101 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslei_hu | `xvslei_hu 0111 01101000 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslei_w | `xvslei_w 0111 01101000 00110 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslei_wu | `xvslei_wu 0111 01101000 01010 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslti_b | `xvslti_b 0111 01101000 01100 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslti_bu | `xvslti_bu 0111 01101000 10000 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslti_d | `xvslti_d 0111 01101000 01111 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslti_du | `xvslti_du 0111 01101000 10011 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslti_h | `xvslti_h 0111 01101000 01101 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslti_hu | `xvslti_hu 0111 01101000 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvslti_w | `xvslti_w 0111 01101000 01110 ..... ..... .....` | @vv_i5 | vv_i5 |
| xvslti_wu | `xvslti_wu 0111 01101000 10010 ..... ..... .....` | @vv_ui5 | vv_ui5 |


### 逻辑运算 (共 10 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvand_v | `xvand_v 0111 01010010 01100 ..... ..... .....` | @vvv | vvv |
| xvandi_b | `xvandi_b 0111 01111101 00 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvandn_v | `xvandn_v 0111 01010010 10000 ..... ..... .....` | @vvv | vvv |
| xvnor_v | `xvnor_v 0111 01010010 01111 ..... ..... .....` | @vvv | vvv |
| xvnori_b | `xvnori_b 0111 01111101 11 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvor_v | `xvor_v 0111 01010010 01101 ..... ..... .....` | @vvv | vvv |
| xvori_b | `xvori_b 0111 01111101 01 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvorn_v | `xvorn_v 0111 01010010 10001 ..... ..... .....` | @vvv | vvv |
| xvxor_v | `xvxor_v 0111 01010010 01110 ..... ..... .....` | @vvv | vvv |
| xvxori_b | `xvxori_b 0111 01111101 10 ........ ..... .....` | @vv_ui8 | vv_ui8 |


### 位操作与移位 (共 188 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvbitclr_b | `xvbitclr_b 0111 01010000 11000 ..... ..... .....` | @vvv | vvv |
| xvbitclr_d | `xvbitclr_d 0111 01010000 11011 ..... ..... .....` | @vvv | vvv |
| xvbitclr_h | `xvbitclr_h 0111 01010000 11001 ..... ..... .....` | @vvv | vvv |
| xvbitclr_w | `xvbitclr_w 0111 01010000 11010 ..... ..... .....` | @vvv | vvv |
| xvbitclri_b | `xvbitclri_b 0111 01110001 00000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvbitclri_d | `xvbitclri_d 0111 01110001 0001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvbitclri_h | `xvbitclri_h 0111 01110001 00000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvbitclri_w | `xvbitclri_w 0111 01110001 00001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvbitrev_b | `xvbitrev_b 0111 01010001 00000 ..... ..... .....` | @vvv | vvv |
| xvbitrev_d | `xvbitrev_d 0111 01010001 00011 ..... ..... .....` | @vvv | vvv |
| xvbitrev_h | `xvbitrev_h 0111 01010001 00001 ..... ..... .....` | @vvv | vvv |
| xvbitrev_w | `xvbitrev_w 0111 01010001 00010 ..... ..... .....` | @vvv | vvv |
| xvbitrevi_b | `xvbitrevi_b 0111 01110001 10000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvbitrevi_d | `xvbitrevi_d 0111 01110001 1001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvbitrevi_h | `xvbitrevi_h 0111 01110001 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvbitrevi_w | `xvbitrevi_w 0111 01110001 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvbitsel_v | `xvbitsel_v 0000 11010010 ..... ..... ..... .....` | @vvvv | vvvv |
| xvbitseli_b | `xvbitseli_b 0111 01111100 01 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvbitset_b | `xvbitset_b 0111 01010000 11100 ..... ..... .....` | @vvv | vvv |
| xvbitset_d | `xvbitset_d 0111 01010000 11111 ..... ..... .....` | @vvv | vvv |
| xvbitset_h | `xvbitset_h 0111 01010000 11101 ..... ..... .....` | @vvv | vvv |
| xvbitset_w | `xvbitset_w 0111 01010000 11110 ..... ..... .....` | @vvv | vvv |
| xvbitseti_b | `xvbitseti_b 0111 01110001 01000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvbitseti_d | `xvbitseti_d 0111 01110001 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvbitseti_h | `xvbitseti_h 0111 01110001 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvbitseti_w | `xvbitseti_w 0111 01110001 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvbsll_v | `xvbsll_v 0111 01101000 11100 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvbsrl_v | `xvbsrl_v 0111 01101000 11101 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvclo_b | `xvclo_b 0111 01101001 11000 00000 ..... .....` | @vv | vv |
| xvclo_d | `xvclo_d 0111 01101001 11000 00011 ..... .....` | @vv | vv |
| xvclo_h | `xvclo_h 0111 01101001 11000 00001 ..... .....` | @vv | vv |
| xvclo_w | `xvclo_w 0111 01101001 11000 00010 ..... .....` | @vv | vv |
| xvclz_b | `xvclz_b 0111 01101001 11000 00100 ..... .....` | @vv | vv |
| xvclz_d | `xvclz_d 0111 01101001 11000 00111 ..... .....` | @vv | vv |
| xvclz_h | `xvclz_h 0111 01101001 11000 00101 ..... .....` | @vv | vv |
| xvclz_w | `xvclz_w 0111 01101001 11000 00110 ..... .....` | @vv | vv |
| xvexth_d_w | `xvexth_d_w 0111 01101001 11101 11010 ..... .....` | @vv | vv |
| xvexth_du_wu | `xvexth_du_wu 0111 01101001 11101 11110 ..... .....` | @vv | vv |
| xvexth_h_b | `xvexth_h_b 0111 01101001 11101 11000 ..... .....` | @vv | vv |
| xvexth_hu_bu | `xvexth_hu_bu 0111 01101001 11101 11100 ..... .....` | @vv | vv |
| xvexth_q_d | `xvexth_q_d 0111 01101001 11101 11011 ..... .....` | @vv | vv |
| xvexth_qu_du | `xvexth_qu_du 0111 01101001 11101 11111 ..... .....` | @vv | vv |
| xvexth_w_h | `xvexth_w_h 0111 01101001 11101 11001 ..... .....` | @vv | vv |
| xvexth_wu_hu | `xvexth_wu_hu 0111 01101001 11101 11101 ..... .....` | @vv | vv |
| xvextl_q_d | `xvextl_q_d 0111 01110000 10010 00000 ..... .....` | @vv | vv |
| xvextl_qu_du | `xvextl_qu_du 0111 01110000 11010 00000 ..... .....` | @vv | vv |
| xvextrins_b | `xvextrins_b 0111 01111000 11 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvextrins_d | `xvextrins_d 0111 01111000 00 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvextrins_h | `xvextrins_h 0111 01111000 10 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvextrins_w | `xvextrins_w 0111 01111000 01 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvrotr_b | `xvrotr_b 0111 01001110 11100 ..... ..... .....` | @vvv | vvv |
| xvrotr_d | `xvrotr_d 0111 01001110 11111 ..... ..... .....` | @vvv | vvv |
| xvrotr_h | `xvrotr_h 0111 01001110 11101 ..... ..... .....` | @vvv | vvv |
| xvrotr_w | `xvrotr_w 0111 01001110 11110 ..... ..... .....` | @vvv | vvv |
| xvrotri_b | `xvrotri_b 0111 01101010 00000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvrotri_d | `xvrotri_d 0111 01101010 0001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvrotri_h | `xvrotri_h 0111 01101010 00000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvrotri_w | `xvrotri_w 0111 01101010 00001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsll_b | `xvsll_b 0111 01001110 10000 ..... ..... .....` | @vvv | vvv |
| xvsll_d | `xvsll_d 0111 01001110 10011 ..... ..... .....` | @vvv | vvv |
| xvsll_h | `xvsll_h 0111 01001110 10001 ..... ..... .....` | @vvv | vvv |
| xvsll_w | `xvsll_w 0111 01001110 10010 ..... ..... .....` | @vvv | vvv |
| xvslli_b | `xvslli_b 0111 01110010 11000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvslli_d | `xvslli_d 0111 01110010 1101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvslli_h | `xvslli_h 0111 01110010 11000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvslli_w | `xvslli_w 0111 01110010 11001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsllwil_d_w | `xvsllwil_d_w 0111 01110000 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsllwil_du_wu | `xvsllwil_du_wu 0111 01110000 11001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsllwil_h_b | `xvsllwil_h_b 0111 01110000 10000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsllwil_hu_bu | `xvsllwil_hu_bu 0111 01110000 11000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsllwil_w_h | `xvsllwil_w_h 0111 01110000 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsllwil_wu_hu | `xvsllwil_wu_hu 0111 01110000 11000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsra_b | `xvsra_b 0111 01001110 11000 ..... ..... .....` | @vvv | vvv |
| xvsra_d | `xvsra_d 0111 01001110 11011 ..... ..... .....` | @vvv | vvv |
| xvsra_h | `xvsra_h 0111 01001110 11001 ..... ..... .....` | @vvv | vvv |
| xvsra_w | `xvsra_w 0111 01001110 11010 ..... ..... .....` | @vvv | vvv |
| xvsrai_b | `xvsrai_b 0111 01110011 01000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsrai_d | `xvsrai_d 0111 01110011 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrai_h | `xvsrai_h 0111 01110011 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrai_w | `xvsrai_w 0111 01110011 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsran_b_h | `xvsran_b_h 0111 01001111 01101 ..... ..... .....` | @vvv | vvv |
| xvsran_h_w | `xvsran_h_w 0111 01001111 01110 ..... ..... .....` | @vvv | vvv |
| xvsran_w_d | `xvsran_w_d 0111 01001111 01111 ..... ..... .....` | @vvv | vvv |
| xvsrani_b_h | `xvsrani_b_h 0111 01110101 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrani_d_q | `xvsrani_d_q 0111 01110101 101 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvsrani_h_w | `xvsrani_h_w 0111 01110101 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrani_w_d | `xvsrani_w_d 0111 01110101 1001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrar_b | `xvsrar_b 0111 01001111 00100 ..... ..... .....` | @vvv | vvv |
| xvsrar_d | `xvsrar_d 0111 01001111 00111 ..... ..... .....` | @vvv | vvv |
| xvsrar_h | `xvsrar_h 0111 01001111 00101 ..... ..... .....` | @vvv | vvv |
| xvsrar_w | `xvsrar_w 0111 01001111 00110 ..... ..... .....` | @vvv | vvv |
| xvsrari_b | `xvsrari_b 0111 01101010 10000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsrari_d | `xvsrari_d 0111 01101010 1001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrari_h | `xvsrari_h 0111 01101010 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrari_w | `xvsrari_w 0111 01101010 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrarn_b_h | `xvsrarn_b_h 0111 01001111 10101 ..... ..... .....` | @vvv | vvv |
| xvsrarn_h_w | `xvsrarn_h_w 0111 01001111 10110 ..... ..... .....` | @vvv | vvv |
| xvsrarn_w_d | `xvsrarn_w_d 0111 01001111 10111 ..... ..... .....` | @vvv | vvv |
| xvsrarni_b_h | `xvsrarni_b_h 0111 01110101 11000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrarni_d_q | `xvsrarni_d_q 0111 01110101 111 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvsrarni_h_w | `xvsrarni_h_w 0111 01110101 11001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrarni_w_d | `xvsrarni_w_d 0111 01110101 1101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrl_b | `xvsrl_b 0111 01001110 10100 ..... ..... .....` | @vvv | vvv |
| xvsrl_d | `xvsrl_d 0111 01001110 10111 ..... ..... .....` | @vvv | vvv |
| xvsrl_h | `xvsrl_h 0111 01001110 10101 ..... ..... .....` | @vvv | vvv |
| xvsrl_w | `xvsrl_w 0111 01001110 10110 ..... ..... .....` | @vvv | vvv |
| xvsrli_b | `xvsrli_b 0111 01110011 00000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsrli_d | `xvsrli_d 0111 01110011 0001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrli_h | `xvsrli_h 0111 01110011 00000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrli_w | `xvsrli_w 0111 01110011 00001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrln_b_h | `xvsrln_b_h 0111 01001111 01001 ..... ..... .....` | @vvv | vvv |
| xvsrln_h_w | `xvsrln_h_w 0111 01001111 01010 ..... ..... .....` | @vvv | vvv |
| xvsrln_w_d | `xvsrln_w_d 0111 01001111 01011 ..... ..... .....` | @vvv | vvv |
| xvsrlni_b_h | `xvsrlni_b_h 0111 01110100 00000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrlni_d_q | `xvsrlni_d_q 0111 01110100 001 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvsrlni_h_w | `xvsrlni_h_w 0111 01110100 00001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrlni_w_d | `xvsrlni_w_d 0111 01110100 0001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrlr_b | `xvsrlr_b 0111 01001111 00000 ..... ..... .....` | @vvv | vvv |
| xvsrlr_d | `xvsrlr_d 0111 01001111 00011 ..... ..... .....` | @vvv | vvv |
| xvsrlr_h | `xvsrlr_h 0111 01001111 00001 ..... ..... .....` | @vvv | vvv |
| xvsrlr_w | `xvsrlr_w 0111 01001111 00010 ..... ..... .....` | @vvv | vvv |
| xvsrlri_b | `xvsrlri_b 0111 01101010 01000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsrlri_d | `xvsrlri_d 0111 01101010 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsrlri_h | `xvsrlri_h 0111 01101010 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrlri_w | `xvsrlri_w 0111 01101010 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrlrn_b_h | `xvsrlrn_b_h 0111 01001111 10001 ..... ..... .....` | @vvv | vvv |
| xvsrlrn_h_w | `xvsrlrn_h_w 0111 01001111 10010 ..... ..... .....` | @vvv | vvv |
| xvsrlrn_w_d | `xvsrlrn_w_d 0111 01001111 10011 ..... ..... .....` | @vvv | vvv |
| xvsrlrni_b_h | `xvsrlrni_b_h 0111 01110100 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsrlrni_d_q | `xvsrlrni_d_q 0111 01110100 011 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvsrlrni_h_w | `xvsrlrni_h_w 0111 01110100 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsrlrni_w_d | `xvsrlrni_w_d 0111 01110100 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssran_b_h | `xvssran_b_h 0111 01001111 11101 ..... ..... .....` | @vvv | vvv |
| xvssran_bu_h | `xvssran_bu_h 0111 01010000 01101 ..... ..... .....` | @vvv | vvv |
| xvssran_h_w | `xvssran_h_w 0111 01001111 11110 ..... ..... .....` | @vvv | vvv |
| xvssran_hu_w | `xvssran_hu_w 0111 01010000 01110 ..... ..... .....` | @vvv | vvv |
| xvssran_w_d | `xvssran_w_d 0111 01001111 11111 ..... ..... .....` | @vvv | vvv |
| xvssran_wu_d | `xvssran_wu_d 0111 01010000 01111 ..... ..... .....` | @vvv | vvv |
| xvssrani_b_h | `xvssrani_b_h 0111 01110110 00000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrani_bu_h | `xvssrani_bu_h 0111 01110110 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrani_d_q | `xvssrani_d_q 0111 01110110 001 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrani_du_q | `xvssrani_du_q 0111 01110110 011 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrani_h_w | `xvssrani_h_w 0111 01110110 00001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrani_hu_w | `xvssrani_hu_w 0111 01110110 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrani_w_d | `xvssrani_w_d 0111 01110110 0001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrani_wu_d | `xvssrani_wu_d 0111 01110110 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrarn_b_h | `xvssrarn_b_h 0111 01010000 00101 ..... ..... .....` | @vvv | vvv |
| xvssrarn_bu_h | `xvssrarn_bu_h 0111 01010000 10101 ..... ..... .....` | @vvv | vvv |
| xvssrarn_h_w | `xvssrarn_h_w 0111 01010000 00110 ..... ..... .....` | @vvv | vvv |
| xvssrarn_hu_w | `xvssrarn_hu_w 0111 01010000 10110 ..... ..... .....` | @vvv | vvv |
| xvssrarn_w_d | `xvssrarn_w_d 0111 01010000 00111 ..... ..... .....` | @vvv | vvv |
| xvssrarn_wu_d | `xvssrarn_wu_d 0111 01010000 10111 ..... ..... .....` | @vvv | vvv |
| xvssrarni_b_h | `xvssrarni_b_h 0111 01110110 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrarni_bu_h | `xvssrarni_bu_h 0111 01110110 11000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrarni_d_q | `xvssrarni_d_q 0111 01110110 101 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrarni_du_q | `xvssrarni_du_q 0111 01110110 111 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrarni_h_w | `xvssrarni_h_w 0111 01110110 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrarni_hu_w | `xvssrarni_hu_w 0111 01110110 11001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrarni_w_d | `xvssrarni_w_d 0111 01110110 1001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrarni_wu_d | `xvssrarni_wu_d 0111 01110110 1101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrln_b_h | `xvssrln_b_h 0111 01001111 11001 ..... ..... .....` | @vvv | vvv |
| xvssrln_bu_h | `xvssrln_bu_h 0111 01010000 01001 ..... ..... .....` | @vvv | vvv |
| xvssrln_h_w | `xvssrln_h_w 0111 01001111 11010 ..... ..... .....` | @vvv | vvv |
| xvssrln_hu_w | `xvssrln_hu_w 0111 01010000 01010 ..... ..... .....` | @vvv | vvv |
| xvssrln_w_d | `xvssrln_w_d 0111 01001111 11011 ..... ..... .....` | @vvv | vvv |
| xvssrln_wu_d | `xvssrln_wu_d 0111 01010000 01011 ..... ..... .....` | @vvv | vvv |
| xvssrlni_b_h | `xvssrlni_b_h 0111 01110100 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrlni_bu_h | `xvssrlni_bu_h 0111 01110100 11000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrlni_d_q | `xvssrlni_d_q 0111 01110100 101 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrlni_du_q | `xvssrlni_du_q 0111 01110100 111 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrlni_h_w | `xvssrlni_h_w 0111 01110100 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrlni_hu_w | `xvssrlni_hu_w 0111 01110100 11001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrlni_w_d | `xvssrlni_w_d 0111 01110100 1001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrlni_wu_d | `xvssrlni_wu_d 0111 01110100 1101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrlrn_b_h | `xvssrlrn_b_h 0111 01010000 00001 ..... ..... .....` | @vvv | vvv |
| xvssrlrn_bu_h | `xvssrlrn_bu_h 0111 01010000 10001 ..... ..... .....` | @vvv | vvv |
| xvssrlrn_h_w | `xvssrlrn_h_w 0111 01010000 00010 ..... ..... .....` | @vvv | vvv |
| xvssrlrn_hu_w | `xvssrlrn_hu_w 0111 01010000 10010 ..... ..... .....` | @vvv | vvv |
| xvssrlrn_w_d | `xvssrlrn_w_d 0111 01010000 00011 ..... ..... .....` | @vvv | vvv |
| xvssrlrn_wu_d | `xvssrlrn_wu_d 0111 01010000 10011 ..... ..... .....` | @vvv | vvv |
| xvssrlrni_b_h | `xvssrlrni_b_h 0111 01110101 00000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrlrni_bu_h | `xvssrlrni_bu_h 0111 01110101 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvssrlrni_d_q | `xvssrlrni_d_q 0111 01110101 001 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrlrni_du_q | `xvssrlrni_du_q 0111 01110101 011 ....... ..... .....` | @vv_ui7 | vv_ui7 |
| xvssrlrni_h_w | `xvssrlrni_h_w 0111 01110101 00001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrlrni_hu_w | `xvssrlrni_hu_w 0111 01110101 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvssrlrni_w_d | `xvssrlrni_w_d 0111 01110101 0001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvssrlrni_wu_d | `xvssrlrni_wu_d 0111 01110101 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |


### 比较与最值 (共 44 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvfmax_d | `xvfmax_d 0111 01010011 11010 ..... ..... .....` | @vvv | vvv |
| xvfmax_s | `xvfmax_s 0111 01010011 11001 ..... ..... .....` | @vvv | vvv |
| xvfmaxa_d | `xvfmaxa_d 0111 01010100 00010 ..... ..... .....` | @vvv | vvv |
| xvfmaxa_s | `xvfmaxa_s 0111 01010100 00001 ..... ..... .....` | @vvv | vvv |
| xvfmin_d | `xvfmin_d 0111 01010011 11110 ..... ..... .....` | @vvv | vvv |
| xvfmin_s | `xvfmin_s 0111 01010011 11101 ..... ..... .....` | @vvv | vvv |
| xvfmina_d | `xvfmina_d 0111 01010100 00110 ..... ..... .....` | @vvv | vvv |
| xvfmina_s | `xvfmina_s 0111 01010100 00101 ..... ..... .....` | @vvv | vvv |
| xvmax_b | `xvmax_b 0111 01000111 00000 ..... ..... .....` | @vvv | vvv |
| xvmax_bu | `xvmax_bu 0111 01000111 01000 ..... ..... .....` | @vvv | vvv |
| xvmax_d | `xvmax_d 0111 01000111 00011 ..... ..... .....` | @vvv | vvv |
| xvmax_du | `xvmax_du 0111 01000111 01011 ..... ..... .....` | @vvv | vvv |
| xvmax_h | `xvmax_h 0111 01000111 00001 ..... ..... .....` | @vvv | vvv |
| xvmax_hu | `xvmax_hu 0111 01000111 01001 ..... ..... .....` | @vvv | vvv |
| xvmax_w | `xvmax_w 0111 01000111 00010 ..... ..... .....` | @vvv | vvv |
| xvmax_wu | `xvmax_wu 0111 01000111 01010 ..... ..... .....` | @vvv | vvv |
| xvmin_b | `xvmin_b 0111 01000111 00100 ..... ..... .....` | @vvv | vvv |
| xvmin_bu | `xvmin_bu 0111 01000111 01100 ..... ..... .....` | @vvv | vvv |
| xvmin_d | `xvmin_d 0111 01000111 00111 ..... ..... .....` | @vvv | vvv |
| xvmin_du | `xvmin_du 0111 01000111 01111 ..... ..... .....` | @vvv | vvv |
| xvmin_h | `xvmin_h 0111 01000111 00101 ..... ..... .....` | @vvv | vvv |
| xvmin_hu | `xvmin_hu 0111 01000111 01101 ..... ..... .....` | @vvv | vvv |
| xvmin_w | `xvmin_w 0111 01000111 00110 ..... ..... .....` | @vvv | vvv |
| xvmin_wu | `xvmin_wu 0111 01000111 01110 ..... ..... .....` | @vvv | vvv |
| xvseq_b | `xvseq_b 0111 01000000 00000 ..... ..... .....` | @vvv | vvv |
| xvseq_d | `xvseq_d 0111 01000000 00011 ..... ..... .....` | @vvv | vvv |
| xvseq_h | `xvseq_h 0111 01000000 00001 ..... ..... .....` | @vvv | vvv |
| xvseq_w | `xvseq_w 0111 01000000 00010 ..... ..... .....` | @vvv | vvv |
| xvsle_b | `xvsle_b 0111 01000000 00100 ..... ..... .....` | @vvv | vvv |
| xvsle_bu | `xvsle_bu 0111 01000000 01000 ..... ..... .....` | @vvv | vvv |
| xvsle_d | `xvsle_d 0111 01000000 00111 ..... ..... .....` | @vvv | vvv |
| xvsle_du | `xvsle_du 0111 01000000 01011 ..... ..... .....` | @vvv | vvv |
| xvsle_h | `xvsle_h 0111 01000000 00101 ..... ..... .....` | @vvv | vvv |
| xvsle_hu | `xvsle_hu 0111 01000000 01001 ..... ..... .....` | @vvv | vvv |
| xvsle_w | `xvsle_w 0111 01000000 00110 ..... ..... .....` | @vvv | vvv |
| xvsle_wu | `xvsle_wu 0111 01000000 01010 ..... ..... .....` | @vvv | vvv |
| xvslt_b | `xvslt_b 0111 01000000 01100 ..... ..... .....` | @vvv | vvv |
| xvslt_bu | `xvslt_bu 0111 01000000 10000 ..... ..... .....` | @vvv | vvv |
| xvslt_d | `xvslt_d 0111 01000000 01111 ..... ..... .....` | @vvv | vvv |
| xvslt_du | `xvslt_du 0111 01000000 10011 ..... ..... .....` | @vvv | vvv |
| xvslt_h | `xvslt_h 0111 01000000 01101 ..... ..... .....` | @vvv | vvv |
| xvslt_hu | `xvslt_hu 0111 01000000 10001 ..... ..... .....` | @vvv | vvv |
| xvslt_w | `xvslt_w 0111 01000000 01110 ..... ..... .....` | @vvv | vvv |
| xvslt_wu | `xvslt_wu 0111 01000000 10010 ..... ..... .....` | @vvv | vvv |


### 乘除累加 (共 48 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvdiv_b | `xvdiv_b 0111 01001110 00000 ..... ..... .....` | @vvv | vvv |
| xvdiv_bu | `xvdiv_bu 0111 01001110 01000 ..... ..... .....` | @vvv | vvv |
| xvdiv_d | `xvdiv_d 0111 01001110 00011 ..... ..... .....` | @vvv | vvv |
| xvdiv_du | `xvdiv_du 0111 01001110 01011 ..... ..... .....` | @vvv | vvv |
| xvdiv_h | `xvdiv_h 0111 01001110 00001 ..... ..... .....` | @vvv | vvv |
| xvdiv_hu | `xvdiv_hu 0111 01001110 01001 ..... ..... .....` | @vvv | vvv |
| xvdiv_w | `xvdiv_w 0111 01001110 00010 ..... ..... .....` | @vvv | vvv |
| xvdiv_wu | `xvdiv_wu 0111 01001110 01010 ..... ..... .....` | @vvv | vvv |
| xvfdiv_d | `xvfdiv_d 0111 01010011 10110 ..... ..... .....` | @vvv | vvv |
| xvfdiv_s | `xvfdiv_s 0111 01010011 10101 ..... ..... .....` | @vvv | vvv |
| xvfmul_d | `xvfmul_d 0111 01010011 10010 ..... ..... .....` | @vvv | vvv |
| xvfmul_s | `xvfmul_s 0111 01010011 10001 ..... ..... .....` | @vvv | vvv |
| xvmod_b | `xvmod_b 0111 01001110 00100 ..... ..... .....` | @vvv | vvv |
| xvmod_bu | `xvmod_bu 0111 01001110 01100 ..... ..... .....` | @vvv | vvv |
| xvmod_d | `xvmod_d 0111 01001110 00111 ..... ..... .....` | @vvv | vvv |
| xvmod_du | `xvmod_du 0111 01001110 01111 ..... ..... .....` | @vvv | vvv |
| xvmod_h | `xvmod_h 0111 01001110 00101 ..... ..... .....` | @vvv | vvv |
| xvmod_hu | `xvmod_hu 0111 01001110 01101 ..... ..... .....` | @vvv | vvv |
| xvmod_w | `xvmod_w 0111 01001110 00110 ..... ..... .....` | @vvv | vvv |
| xvmod_wu | `xvmod_wu 0111 01001110 01110 ..... ..... .....` | @vvv | vvv |
| xvmul_b | `xvmul_b 0111 01001000 01000 ..... ..... .....` | @vvv | vvv |
| xvmul_d | `xvmul_d 0111 01001000 01011 ..... ..... .....` | @vvv | vvv |
| xvmul_h | `xvmul_h 0111 01001000 01001 ..... ..... .....` | @vvv | vvv |
| xvmul_w | `xvmul_w 0111 01001000 01010 ..... ..... .....` | @vvv | vvv |
| xvmulwev_d_w | `xvmulwev_d_w 0111 01001001 00010 ..... ..... .....` | @vvv | vvv |
| xvmulwev_d_wu | `xvmulwev_d_wu 0111 01001001 10010 ..... ..... .....` | @vvv | vvv |
| xvmulwev_d_wu_w | `xvmulwev_d_wu_w 0111 01001010 00010 ..... ..... .....` | @vvv | vvv |
| xvmulwev_h_b | `xvmulwev_h_b 0111 01001001 00000 ..... ..... .....` | @vvv | vvv |
| xvmulwev_h_bu | `xvmulwev_h_bu 0111 01001001 10000 ..... ..... .....` | @vvv | vvv |
| xvmulwev_h_bu_b | `xvmulwev_h_bu_b 0111 01001010 00000 ..... ..... .....` | @vvv | vvv |
| xvmulwev_q_d | `xvmulwev_q_d 0111 01001001 00011 ..... ..... .....` | @vvv | vvv |
| xvmulwev_q_du | `xvmulwev_q_du 0111 01001001 10011 ..... ..... .....` | @vvv | vvv |
| xvmulwev_q_du_d | `xvmulwev_q_du_d 0111 01001010 00011 ..... ..... .....` | @vvv | vvv |
| xvmulwev_w_h | `xvmulwev_w_h 0111 01001001 00001 ..... ..... .....` | @vvv | vvv |
| xvmulwev_w_hu | `xvmulwev_w_hu 0111 01001001 10001 ..... ..... .....` | @vvv | vvv |
| xvmulwev_w_hu_h | `xvmulwev_w_hu_h 0111 01001010 00001 ..... ..... .....` | @vvv | vvv |
| xvmulwod_d_w | `xvmulwod_d_w 0111 01001001 00110 ..... ..... .....` | @vvv | vvv |
| xvmulwod_d_wu | `xvmulwod_d_wu 0111 01001001 10110 ..... ..... .....` | @vvv | vvv |
| xvmulwod_d_wu_w | `xvmulwod_d_wu_w 0111 01001010 00110 ..... ..... .....` | @vvv | vvv |
| xvmulwod_h_b | `xvmulwod_h_b 0111 01001001 00100 ..... ..... .....` | @vvv | vvv |
| xvmulwod_h_bu | `xvmulwod_h_bu 0111 01001001 10100 ..... ..... .....` | @vvv | vvv |
| xvmulwod_h_bu_b | `xvmulwod_h_bu_b 0111 01001010 00100 ..... ..... .....` | @vvv | vvv |
| xvmulwod_q_d | `xvmulwod_q_d 0111 01001001 00111 ..... ..... .....` | @vvv | vvv |
| xvmulwod_q_du | `xvmulwod_q_du 0111 01001001 10111 ..... ..... .....` | @vvv | vvv |
| xvmulwod_q_du_d | `xvmulwod_q_du_d 0111 01001010 00111 ..... ..... .....` | @vvv | vvv |
| xvmulwod_w_h | `xvmulwod_w_h 0111 01001001 00101 ..... ..... .....` | @vvv | vvv |
| xvmulwod_w_hu | `xvmulwod_w_hu 0111 01001001 10101 ..... ..... .....` | @vvv | vvv |
| xvmulwod_w_hu_h | `xvmulwod_w_hu_h 0111 01001010 00101 ..... ..... .....` | @vvv | vvv |


### 提取打包 (共 47 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvinsgr2vr_d | `xvinsgr2vr_d 0111 01101110 10111 110 .. ..... .....` | @vr_ui2 | vr_ui2 |
| xvinsgr2vr_w | `xvinsgr2vr_w 0111 01101110 10111 10 ... ..... .....` | @vr_ui3 | vr_ui3 |
| xvinsve0_d | `xvinsve0_d 0111 01101111 11111 110 .. ..... .....` | @vv_ui2 | vv_ui2 |
| xvinsve0_w | `xvinsve0_w 0111 01101111 11111 10 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvldrepl_b | `xvldrepl_b 0011 001010 ............ ..... .....` | @vr_i12 | vr_i12 |
| xvldrepl_d | `xvldrepl_d 0011 00100001 0 ......... ..... .....` | @vr_i9 | vr_i9 |
| xvldrepl_h | `xvldrepl_h 0011 0010010 ........... ..... .....` | @vr_i11 | vr_i11 |
| xvldrepl_w | `xvldrepl_w 0011 00100010 .......... ..... .....` | @vr_i10 | vr_i10 |
| xvpackev_b | `xvpackev_b 0111 01010001 01100 ..... ..... .....` | @vvv | vvv |
| xvpackev_d | `xvpackev_d 0111 01010001 01111 ..... ..... .....` | @vvv | vvv |
| xvpackev_h | `xvpackev_h 0111 01010001 01101 ..... ..... .....` | @vvv | vvv |
| xvpackev_w | `xvpackev_w 0111 01010001 01110 ..... ..... .....` | @vvv | vvv |
| xvpackod_b | `xvpackod_b 0111 01010001 10000 ..... ..... .....` | @vvv | vvv |
| xvpackod_d | `xvpackod_d 0111 01010001 10011 ..... ..... .....` | @vvv | vvv |
| xvpackod_h | `xvpackod_h 0111 01010001 10001 ..... ..... .....` | @vvv | vvv |
| xvpackod_w | `xvpackod_w 0111 01010001 10010 ..... ..... .....` | @vvv | vvv |
| xvpickev_b | `xvpickev_b 0111 01010001 11100 ..... ..... .....` | @vvv | vvv |
| xvpickev_d | `xvpickev_d 0111 01010001 11111 ..... ..... .....` | @vvv | vvv |
| xvpickev_h | `xvpickev_h 0111 01010001 11101 ..... ..... .....` | @vvv | vvv |
| xvpickev_w | `xvpickev_w 0111 01010001 11110 ..... ..... .....` | @vvv | vvv |
| xvpickod_b | `xvpickod_b 0111 01010010 00000 ..... ..... .....` | @vvv | vvv |
| xvpickod_d | `xvpickod_d 0111 01010010 00011 ..... ..... .....` | @vvv | vvv |
| xvpickod_h | `xvpickod_h 0111 01010010 00001 ..... ..... .....` | @vvv | vvv |
| xvpickod_w | `xvpickod_w 0111 01010010 00010 ..... ..... .....` | @vvv | vvv |
| xvpickve2gr_d | `xvpickve2gr_d 0111 01101110 11111 110 .. ..... .....` | @rv_ui2 | rv_ui2 |
| xvpickve2gr_du | `xvpickve2gr_du 0111 01101111 00111 110 .. ..... .....` | @rv_ui2 | rv_ui2 |
| xvpickve2gr_w | `xvpickve2gr_w 0111 01101110 11111 10 ... ..... .....` | @rv_ui3 | rv_ui3 |
| xvpickve2gr_wu | `xvpickve2gr_wu 0111 01101111 00111 10 ... ..... .....` | @rv_ui3 | rv_ui3 |
| xvpickve_d | `xvpickve_d 0111 01110000 00111 110 .. ..... .....` | @vv_ui2 | vv_ui2 |
| xvpickve_w | `xvpickve_w 0111 01110000 00111 10 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvrepl128vei_b | `xvrepl128vei_b 0111 01101111 01111 0 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvrepl128vei_d | `xvrepl128vei_d 0111 01101111 01111 1110 . ..... .....` | @vv_ui1 | vv_ui1 |
| xvrepl128vei_h | `xvrepl128vei_h 0111 01101111 01111 10 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvrepl128vei_w | `xvrepl128vei_w 0111 01101111 01111 110 .. ..... .....` | @vv_ui2 | vv_ui2 |
| xvreplgr2vr_b | `xvreplgr2vr_b 0111 01101001 11110 00000 ..... .....` | @vr | vr |
| xvreplgr2vr_d | `xvreplgr2vr_d 0111 01101001 11110 00011 ..... .....` | @vr | vr |
| xvreplgr2vr_h | `xvreplgr2vr_h 0111 01101001 11110 00001 ..... .....` | @vr | vr |
| xvreplgr2vr_w | `xvreplgr2vr_w 0111 01101001 11110 00010 ..... .....` | @vr | vr |
| xvreplve0_b | `xvreplve0_b 0111 01110000 01110 00000 ..... .....` | @vv | vv |
| xvreplve0_d | `xvreplve0_d 0111 01110000 01111 11000 ..... .....` | @vv | vv |
| xvreplve0_h | `xvreplve0_h 0111 01110000 01111 00000 ..... .....` | @vv | vv |
| xvreplve0_q | `xvreplve0_q 0111 01110000 01111 11100 ..... .....` | @vv | vv |
| xvreplve0_w | `xvreplve0_w 0111 01110000 01111 10000 ..... .....` | @vv | vv |
| xvreplve_b | `xvreplve_b 0111 01010010 00100 ..... ..... .....` | @vvr | vvr |
| xvreplve_d | `xvreplve_d 0111 01010010 00111 ..... ..... .....` | @vvr | vvr |
| xvreplve_h | `xvreplve_h 0111 01010010 00101 ..... ..... .....` | @vvr | vvr |
| xvreplve_w | `xvreplve_w 0111 01010010 00110 ..... ..... .....` | @vvr | vvr |


### 置换重排 (共 20 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvilvh_b | `xvilvh_b 0111 01010001 11000 ..... ..... .....` | @vvv | vvv |
| xvilvh_d | `xvilvh_d 0111 01010001 11011 ..... ..... .....` | @vvv | vvv |
| xvilvh_h | `xvilvh_h 0111 01010001 11001 ..... ..... .....` | @vvv | vvv |
| xvilvh_w | `xvilvh_w 0111 01010001 11010 ..... ..... .....` | @vvv | vvv |
| xvilvl_b | `xvilvl_b 0111 01010001 10100 ..... ..... .....` | @vvv | vvv |
| xvilvl_d | `xvilvl_d 0111 01010001 10111 ..... ..... .....` | @vvv | vvv |
| xvilvl_h | `xvilvl_h 0111 01010001 10101 ..... ..... .....` | @vvv | vvv |
| xvilvl_w | `xvilvl_w 0111 01010001 10110 ..... ..... .....` | @vvv | vvv |
| xvperm_w | `xvperm_w 0111 01010111 11010 ..... ..... .....` | @vvv | vvv |
| xvpermi_d | `xvpermi_d 0111 01111110 10 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvpermi_q | `xvpermi_q 0111 01111110 11 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvpermi_w | `xvpermi_w 0111 01111110 01 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvshuf4i_b | `xvshuf4i_b 0111 01111001 00 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvshuf4i_d | `xvshuf4i_d 0111 01111001 11 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvshuf4i_h | `xvshuf4i_h 0111 01111001 01 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvshuf4i_w | `xvshuf4i_w 0111 01111001 10 ........ ..... .....` | @vv_ui8 | vv_ui8 |
| xvshuf_b | `xvshuf_b 0000 11010110 ..... ..... ..... .....` | @vvvv | vvvv |
| xvshuf_d | `xvshuf_d 0111 01010111 10111 ..... ..... .....` | @vvv | vvv |
| xvshuf_h | `xvshuf_h 0111 01010111 10101 ..... ..... .....` | @vvv | vvv |
| xvshuf_w | `xvshuf_w 0111 01010111 10110 ..... ..... .....` | @vvv | vvv |


### 内存访问 (共 13 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvfrstp_b | `xvfrstp_b 0111 01010010 10110 ..... ..... .....` | @vvv | vvv |
| xvfrstp_h | `xvfrstp_h 0111 01010010 10111 ..... ..... .....` | @vvv | vvv |
| xvfrstpi_b | `xvfrstpi_b 0111 01101001 10100 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvfrstpi_h | `xvfrstpi_h 0111 01101001 10101 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvld | `xvld 0010 110010 ............ ..... .....` | @vr_i12 | vr_i12 |
| xvldi | `xvldi 0111 01111110 00 ............. .....` | @v_i13 | v_i13 |
| xvldx | `xvldx 0011 10000100 10000 ..... ..... .....` | @vrr | vrr |
| xvst | `xvst 0010 110011 ............ ..... .....` | @vr_i12 | vr_i12 |
| xvstelm_b | `xvstelm_b 0011 00111 ..... ........ ..... .....` | @vr_i8i5x | vr_i8i5x |
| xvstelm_d | `xvstelm_d 0011 00110001 .. ........ ..... .....` | @vr_i8i2x | vr_i8i2x |
| xvstelm_h | `xvstelm_h 0011 001101 .... ........ ..... .....` | @vr_i8i4x | vr_i8i4x |
| xvstelm_w | `xvstelm_w 0011 0011001 ... ........ ..... .....` | @vr_i8i3x | vr_i8i3x |
| xvstx | `xvstx 0011 10000100 11000 ..... ..... .....` | @vrr | vrr |


### 浮点基本运算 (共 14 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvfclass_d | `xvfclass_d 0111 01101001 11001 10110 ..... .....` | @vv | vv |
| xvfclass_s | `xvfclass_s 0111 01101001 11001 10101 ..... .....` | @vv | vv |
| xvflogb_d | `xvflogb_d 0111 01101001 11001 10010 ..... .....` | @vv | vv |
| xvflogb_s | `xvflogb_s 0111 01101001 11001 10001 ..... .....` | @vv | vv |
| xvfrecip_d | `xvfrecip_d 0111 01101001 11001 11110 ..... .....` | @vv | vv |
| xvfrecip_s | `xvfrecip_s 0111 01101001 11001 11101 ..... .....` | @vv | vv |
| xvfrecipe_d | `xvfrecipe_d 0111 01101001 11010 00110 ..... .....` | @vv | vv |
| xvfrecipe_s | `xvfrecipe_s 0111 01101001 11010 00101 ..... .....` | @vv | vv |
| xvfrsqrt_d | `xvfrsqrt_d 0111 01101001 11010 00010 ..... .....` | @vv | vv |
| xvfrsqrt_s | `xvfrsqrt_s 0111 01101001 11010 00001 ..... .....` | @vv | vv |
| xvfrsqrte_d | `xvfrsqrte_d 0111 01101001 11010 01010 ..... .....` | @vv | vv |
| xvfrsqrte_s | `xvfrsqrte_s 0111 01101001 11010 01001 ..... .....` | @vv | vv |
| xvfsqrt_d | `xvfsqrt_d 0111 01101001 11001 11010 ..... .....` | @vv | vv |
| xvfsqrt_s | `xvfsqrt_s 0111 01101001 11001 11001 ..... .....` | @vv | vv |


### 浮点比较 (共 2 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvfcmp_cond_d | `xvfcmp_cond_d 0000 11001010 ..... ..... ..... .....` | @vvv_fcond | vvv_fcond |
| xvfcmp_cond_s | `xvfcmp_cond_s 0000 11001001 ..... ..... ..... .....` | @vvv_fcond | vvv_fcond |


### 浮点转换 (共 52 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvfcvt_h_s | `xvfcvt_h_s 0111 01010100 01100 ..... ..... .....` | @vvv | vvv |
| xvfcvt_s_d | `xvfcvt_s_d 0111 01010100 01101 ..... ..... .....` | @vvv | vvv |
| xvfcvth_d_s | `xvfcvth_d_s 0111 01101001 11011 11101 ..... .....` | @vv | vv |
| xvfcvth_s_h | `xvfcvth_s_h 0111 01101001 11011 11011 ..... .....` | @vv | vv |
| xvfcvtl_d_s | `xvfcvtl_d_s 0111 01101001 11011 11100 ..... .....` | @vv | vv |
| xvfcvtl_s_h | `xvfcvtl_s_h 0111 01101001 11011 11010 ..... .....` | @vv | vv |
| xvffint_d_l | `xvffint_d_l 0111 01101001 11100 00010 ..... .....` | @vv | vv |
| xvffint_d_lu | `xvffint_d_lu 0111 01101001 11100 00011 ..... .....` | @vv | vv |
| xvffint_s_l | `xvffint_s_l 0111 01010100 10000 ..... ..... .....` | @vvv | vvv |
| xvffint_s_w | `xvffint_s_w 0111 01101001 11100 00000 ..... .....` | @vv | vv |
| xvffint_s_wu | `xvffint_s_wu 0111 01101001 11100 00001 ..... .....` | @vv | vv |
| xvffinth_d_w | `xvffinth_d_w 0111 01101001 11100 00101 ..... .....` | @vv | vv |
| xvffintl_d_w | `xvffintl_d_w 0111 01101001 11100 00100 ..... .....` | @vv | vv |
| xvfrint_d | `xvfrint_d 0111 01101001 11010 01110 ..... .....` | @vv | vv |
| xvfrint_s | `xvfrint_s 0111 01101001 11010 01101 ..... .....` | @vv | vv |
| xvfrintrm_d | `xvfrintrm_d 0111 01101001 11010 10010 ..... .....` | @vv | vv |
| xvfrintrm_s | `xvfrintrm_s 0111 01101001 11010 10001 ..... .....` | @vv | vv |
| xvfrintrne_d | `xvfrintrne_d 0111 01101001 11010 11110 ..... .....` | @vv | vv |
| xvfrintrne_s | `xvfrintrne_s 0111 01101001 11010 11101 ..... .....` | @vv | vv |
| xvfrintrp_d | `xvfrintrp_d 0111 01101001 11010 10110 ..... .....` | @vv | vv |
| xvfrintrp_s | `xvfrintrp_s 0111 01101001 11010 10101 ..... .....` | @vv | vv |
| xvfrintrz_d | `xvfrintrz_d 0111 01101001 11010 11010 ..... .....` | @vv | vv |
| xvfrintrz_s | `xvfrintrz_s 0111 01101001 11010 11001 ..... .....` | @vv | vv |
| xvftint_l_d | `xvftint_l_d 0111 01101001 11100 01101 ..... .....` | @vv | vv |
| xvftint_lu_d | `xvftint_lu_d 0111 01101001 11100 10111 ..... .....` | @vv | vv |
| xvftint_w_d | `xvftint_w_d 0111 01010100 10011 ..... ..... .....` | @vvv | vvv |
| xvftint_w_s | `xvftint_w_s 0111 01101001 11100 01100 ..... .....` | @vv | vv |
| xvftint_wu_s | `xvftint_wu_s 0111 01101001 11100 10110 ..... .....` | @vv | vv |
| xvftinth_l_s | `xvftinth_l_s 0111 01101001 11101 00001 ..... .....` | @vv | vv |
| xvftintl_l_s | `xvftintl_l_s 0111 01101001 11101 00000 ..... .....` | @vv | vv |
| xvftintrm_l_d | `xvftintrm_l_d 0111 01101001 11100 01111 ..... .....` | @vv | vv |
| xvftintrm_w_d | `xvftintrm_w_d 0111 01010100 10100 ..... ..... .....` | @vvv | vvv |
| xvftintrm_w_s | `xvftintrm_w_s 0111 01101001 11100 01110 ..... .....` | @vv | vv |
| xvftintrmh_l_s | `xvftintrmh_l_s 0111 01101001 11101 00011 ..... .....` | @vv | vv |
| xvftintrml_l_s | `xvftintrml_l_s 0111 01101001 11101 00010 ..... .....` | @vv | vv |
| xvftintrne_l_d | `xvftintrne_l_d 0111 01101001 11100 10101 ..... .....` | @vv | vv |
| xvftintrne_w_d | `xvftintrne_w_d 0111 01010100 10111 ..... ..... .....` | @vvv | vvv |
| xvftintrne_w_s | `xvftintrne_w_s 0111 01101001 11100 10100 ..... .....` | @vv | vv |
| xvftintrneh_l_s | `xvftintrneh_l_s 0111 01101001 11101 01001 ..... .....` | @vv | vv |
| xvftintrnel_l_s | `xvftintrnel_l_s 0111 01101001 11101 01000 ..... .....` | @vv | vv |
| xvftintrp_l_d | `xvftintrp_l_d 0111 01101001 11100 10001 ..... .....` | @vv | vv |
| xvftintrp_w_d | `xvftintrp_w_d 0111 01010100 10101 ..... ..... .....` | @vvv | vvv |
| xvftintrp_w_s | `xvftintrp_w_s 0111 01101001 11100 10000 ..... .....` | @vv | vv |
| xvftintrph_l_s | `xvftintrph_l_s 0111 01101001 11101 00101 ..... .....` | @vv | vv |
| xvftintrpl_l_s | `xvftintrpl_l_s 0111 01101001 11101 00100 ..... .....` | @vv | vv |
| xvftintrz_l_d | `xvftintrz_l_d 0111 01101001 11100 10011 ..... .....` | @vv | vv |
| xvftintrz_lu_d | `xvftintrz_lu_d 0111 01101001 11100 11101 ..... .....` | @vv | vv |
| xvftintrz_w_d | `xvftintrz_w_d 0111 01010100 10110 ..... ..... .....` | @vvv | vvv |
| xvftintrz_w_s | `xvftintrz_w_s 0111 01101001 11100 10010 ..... .....` | @vv | vv |
| xvftintrz_wu_s | `xvftintrz_wu_s 0111 01101001 11100 11100 ..... .....` | @vv | vv |
| xvftintrzh_l_s | `xvftintrzh_l_s 0111 01101001 11101 00111 ..... .....` | @vv | vv |
| xvftintrzl_l_s | `xvftintrzl_l_s 0111 01101001 11101 00110 ..... .....` | @vv | vv |


### 其他操作 (共 40 条指令)

| 助记符 | 编码掩码 | 格式 | 参数集 |
|--------|----------|------|--------|
| xvmskgez_b | `xvmskgez_b 0111 01101001 11000 10100 ..... .....` | @vv | vv |
| xvmskltz_b | `xvmskltz_b 0111 01101001 11000 10000 ..... .....` | @vv | vv |
| xvmskltz_d | `xvmskltz_d 0111 01101001 11000 10011 ..... .....` | @vv | vv |
| xvmskltz_h | `xvmskltz_h 0111 01101001 11000 10001 ..... .....` | @vv | vv |
| xvmskltz_w | `xvmskltz_w 0111 01101001 11000 10010 ..... .....` | @vv | vv |
| xvmsknz_b | `xvmsknz_b 0111 01101001 11000 11000 ..... .....` | @vv | vv |
| xvmuh_b | `xvmuh_b 0111 01001000 01100 ..... ..... .....` | @vvv | vvv |
| xvmuh_bu | `xvmuh_bu 0111 01001000 10000 ..... ..... .....` | @vvv | vvv |
| xvmuh_d | `xvmuh_d 0111 01001000 01111 ..... ..... .....` | @vvv | vvv |
| xvmuh_du | `xvmuh_du 0111 01001000 10011 ..... ..... .....` | @vvv | vvv |
| xvmuh_h | `xvmuh_h 0111 01001000 01101 ..... ..... .....` | @vvv | vvv |
| xvmuh_hu | `xvmuh_hu 0111 01001000 10001 ..... ..... .....` | @vvv | vvv |
| xvmuh_w | `xvmuh_w 0111 01001000 01110 ..... ..... .....` | @vvv | vvv |
| xvmuh_wu | `xvmuh_wu 0111 01001000 10010 ..... ..... .....` | @vvv | vvv |
| xvpcnt_b | `xvpcnt_b 0111 01101001 11000 01000 ..... .....` | @vv | vv |
| xvpcnt_d | `xvpcnt_d 0111 01101001 11000 01011 ..... .....` | @vv | vv |
| xvpcnt_h | `xvpcnt_h 0111 01101001 11000 01001 ..... .....` | @vv | vv |
| xvpcnt_w | `xvpcnt_w 0111 01101001 11000 01010 ..... .....` | @vv | vv |
| xvsat_b | `xvsat_b 0111 01110010 01000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsat_bu | `xvsat_bu 0111 01110010 10000 01 ... ..... .....` | @vv_ui3 | vv_ui3 |
| xvsat_d | `xvsat_d 0111 01110010 0101 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsat_du | `xvsat_du 0111 01110010 1001 ...... ..... .....` | @vv_ui6 | vv_ui6 |
| xvsat_h | `xvsat_h 0111 01110010 01000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsat_hu | `xvsat_hu 0111 01110010 10000 1 .... ..... .....` | @vv_ui4 | vv_ui4 |
| xvsat_w | `xvsat_w 0111 01110010 01001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsat_wu | `xvsat_wu 0111 01110010 10001 ..... ..... .....` | @vv_ui5 | vv_ui5 |
| xvsetallnez_b | `xvsetallnez_b 0111 01101001 11001 01100 ..... 00 ...` | @cv | cv |
| xvsetallnez_d | `xvsetallnez_d 0111 01101001 11001 01111 ..... 00 ...` | @cv | cv |
| xvsetallnez_h | `xvsetallnez_h 0111 01101001 11001 01101 ..... 00 ...` | @cv | cv |
| xvsetallnez_w | `xvsetallnez_w 0111 01101001 11001 01110 ..... 00 ...` | @cv | cv |
| xvsetanyeqz_b | `xvsetanyeqz_b 0111 01101001 11001 01000 ..... 00 ...` | @cv | cv |
| xvsetanyeqz_d | `xvsetanyeqz_d 0111 01101001 11001 01011 ..... 00 ...` | @cv | cv |
| xvsetanyeqz_h | `xvsetanyeqz_h 0111 01101001 11001 01001 ..... 00 ...` | @cv | cv |
| xvsetanyeqz_w | `xvsetanyeqz_w 0111 01101001 11001 01010 ..... 00 ...` | @cv | cv |
| xvseteqz_v | `xvseteqz_v 0111 01101001 11001 00110 ..... 00 ...` | @cv | cv |
| xvsetnez_v | `xvsetnez_v 0111 01101001 11001 00111 ..... 00 ...` | @cv | cv |
| xvsigncov_b | `xvsigncov_b 0111 01010010 11100 ..... ..... .....` | @vvv | vvv |
| xvsigncov_d | `xvsigncov_d 0111 01010010 11111 ..... ..... .....` | @vvv | vvv |
| xvsigncov_h | `xvsigncov_h 0111 01010010 11101 ..... ..... .....` | @vvv | vvv |
| xvsigncov_w | `xvsigncov_w 0111 01010010 11110 ..... ..... .....` | @vvv | vvv |


## 统计

- QEMU insns.decode 中总共 **680** 条 LASX 指令
- 本文档分类整理了 **680** 条指令
- 未分类剩余 **0** 条指令

