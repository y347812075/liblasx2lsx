# LASX 指令伪代码文档

> LASX (LA64 Vector SIMD) 指令伪代码说明

---

## 1. xvadd.b

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i]+XR[xk].B[i]
```

## 2. xvadd.h

```asm
for i in range(16) :
XR[xd].H[i] = XR[xj].H[i]+XR[xk].H[i]
```

## 3. xvadd.w

```asm
for i in range(8) :
XR[xd].W[i] = XR[xj].W[i]+XR[xk].W[i]
```

## 4. xvadd.d

```asm
for i in range(4) :
XR[xd].D[i] = XR[xj].D[i]+XR[xk].D[i]
```

## 5. xvadd.q

```asm
for i in range(2) :
XR[xd].Q[i] = XR[xj].Q[i]+XR[xk].Q[i]
```

## 6. xvsub.b

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i]-XR[xk].B[i]
```

## 7. xvsub.h

```asm
for i in range(16) :
XR[xd].H[i] = XR[xj].H[i]-XR[xk].H[i]
```

## 8. xvsub.w

```asm
for i in range(8) :
XR[xd].W[i] = XR[xj].W[i]-XR[xk].W[i]
```

## 9. xvsub.d

```asm
for i in range(4) :
XR[xd].D[i] = XR[xj].D[i]-XR[xk].D[i]
```

## 10. xvsub.q

```asm
for i in range(2) :
XR[xd].Q[i] = XR[xj].Q[i]-XR[xk].Q[i]
```

## 11. xvaddi.bu

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i]+ZeroExtend(ui5, 8)
```

## 12. xvaddi.hu

```asm
for i in range(16) :
XR[xd].H[i] = XR[xj].H[i]+ZeroExtend(ui5, 16)
```

## 13. xvaddi.wu

```asm
for i in range(8) :
XR[xd].W[i] = XR[xj].W[i]+ZeroExtend(ui5, 32)
```

## 14. xvaddi.du

```asm
for i in range(4) :
XR[xd].D[i] = XR[xj].D[i]+ZeroExtend(ui5, 32)
```

## 15. xvsubi.bu

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i]-ZeroExtend(ui5, 8)
```

## 16. xvsubi.hu

```asm
for i in range(16) :
XR[xd].H[i] = XR[xj].H[i]-ZeroExtend(ui5, 16)
```

## 17. xvsubi.wu

```asm
for i in range(8) :
XR[xd].W[i] = XR[xj].W[i]-ZeroExtend(ui5, 32)
```

## 18. xvsubi.du

```asm
for i in range(4) :
XR[xd].D[i] = XR[xj].D[i]-ZeroExtend(ui5, 64)
```

## 19. xvneg.b

```asm
for i in range(32) :
XR[xd].B[i] = -XR[xj].B[i]
```

## 20. xvneg.h

```asm
for i in range(16) :
XR[xd].H[i] = -XR[xj].H[i]
```

## 21. xvneg.w

```asm
for i in range(8) :
XR[xd].W[i] = -XR[xj].W[i]
```

## 22. xvneg.d

```asm
for i in range(4) :
XR[xd].D[i] = -XR[xj].D[i]
```

## 23. xvsadd.b

```asm
for i in range(32) :
XR[xd].B[i] = SSat_Add_S(XR[xj].B[i], XR[xk].B[i])
```

## 24. xvsadd.h

```asm
for i in range(16) :
XR[xd].H[i] = SSat_Add_S(XR[xj].H[i], XR[xk].H[i])
```

## 25. xvsadd.w

```asm
for i in range(8) :
XR[xd].W[i] = SSat_Add_S(XR[xj].W[i], XR[xk].W[i])
```

## 26. xvsadd.d

```asm
for i in range(4) :
XR[xd].D[i] = SSat_Add_S(XR[xj].D[i], XR[xk].D[i])
```

## 27. xvsadd.bu

```asm
for i in range(32) :
XR[xd].B[i] = USat_Add_U(XR[xj].B[i], XR[xk].B[i])
```

## 28. xvsadd.hu

```asm
for i in range(16) :
XR[xd].H[i] = USat_Add_U(XR[xj].H[i], XR[xk].H[i])
```

## 29. xvsadd.wu

```asm
for i in range(8) :
XR[xd].W[i] = USat_Add_U(XR[xj].W[i], XR[xk].W[i])
```

## 30. xvsadd.du

```asm
for i in range(4) :
XR[xd].D[i] = USat_Add_U(XR[xj].D[i], XR[xk].D[i])
```

## 31. xvssub.b

```asm
for i in range(32) :
XR[xd].B[i] = SSat_Sub_S(XR[xj].B[i], XR[xk].B[i])
```

## 32. xvssub.h

```asm
for i in range(16) :
XR[xd].H[i] = SSat_Sub_S(XR[xj].H[i], XR[xk].H[i])
```

## 33. xvssub.w

```asm
for i in range(8) :
XR[xd].W[i] = SSat_Sub_S(XR[xj].W[i], XR[xk].W[i])
```

## 34. xvssub.d

```asm
for i in range(4) :
XR[xd].D[i] = SSat_Sub_S(XR[xj].D[i], XR[xk].D[i])
```

## 35. xvssub.bu

```asm
for i in range(32) :
XR[xd].B[i] = USat_Sub_U(XR[xj].B[i], XR[xk].B[i])
```

## 36. xvssub.hu

```asm
for i in range(16) :
XR[xd].H[i] = USat_Sub_U(XR[xj].H[i], XR[xk].H[i])
```

## 37. xvssub.wu

```asm
for i in range(8) :
XR[xd].W[i] = USat_Sub_U(XR[xj].W[i], XR[xk].W[i])
```

## 38. xvssub.du

```asm
for i in range(4) :
XR[xd].D[i] = USat_Sub_U(XR[xj].D[i], XR[xk].D[i])
```

## 39. xvhaddw.h.b

```asm
for i in range(16) :
XR[xd].H[i]=SignExtend(signed(XR[xj].B[2i+1])+signed(XR[xk].B[2i]),16)
```

## 40. xvhaddw.w.h

```asm
for i in range(8) :
XR[xd].W[i]=SignExtend(signed(XR[xj].H[2i+1])+signed(XR[xk].H[2i]),32)
```

## 41. xvhaddw.d.w

```asm
for i in range(4) :
XR[xd].D[i]=SignExtend(signed(XR[xj].W[2i+1])+signed(XR[xk].W[2i]),64)
```

## 42. xvhaddw.q.d

```asm
for i in range(2) :
XR[xd].Q[i]=SignExtend(signed(XR[xj].D[2i+1])+signed(XR[xk].D[2i]),128)
```

## 43. xvhaddw.hu.bu

```asm
for i in range(16) :
XR[xd].H[i] = ZeroExtend(unsigned(XR[xj].B[2i+1])+unsigned(XR[xk].B[2i]), 16)
```

## 44. xvhaddw.wu.hu

```asm
for i in range(8) :
XR[xd].W[i] = ZeroExtend(unsigned(XR[xj].H[2i+1])+unsigned(XR[xk].H[2i]), 32)
```

## 45. xvhaddw.du.wu

```asm
for i in range(4) :
XR[xd].D[i] = ZeroExtend(unsigned(XR[xj].W[2i+1])+unsigned(XR[xk].W[2i]), 64)
```

## 46. xvhaddw.qu.du

```asm
for i in range(2) :
XR[xd].Q[i] = ZeroExtend(unsigned(XR[xj].D[2i+1])+unsigned(XR[xk].D[2i]), 128)
```

## 47. xvhsubw.h.b

```asm
for i in range(16) :
XR[xd].H[i] = SignExtend(signed(XR[xj].B[2i+1])-signed(XR[xk].B[2i]), 16)
```

## 48. xvhsubw.w.h

```asm
for i in range(8) :
XR[xd].W[i] = SignExtend(signed(XR[xj].H[2i+1])-signed(XR[xk].H[2i]), 32)
```

## 49. xvhsubw.d.w

```asm
for i in range(4) :
XR[xd].D[i] = SignExtend(signed(XR[xj].W[2i+1])-signed(XR[xk].W[2i]), 64)
```

## 50. xvhsubw.q.d

```asm
for i in range(2) :
XR[xd].Q[i] = SignExtend(signed(XR[xj].D[2i+1])-signed(XR[xk].D[2i]), 128)
```

## 51. xvhsubw.hu.bu

```asm
for i in range(16) :
XR[xd].H[i] = ZeroExtend(unsigned(XR[xj].B[2i+1])-unsigned(XR[xk].B[2i]), 16)
```

## 52. xvhsubw.wu.hu

```asm
for i in range(8) :
XR[xd].W[i] = ZeroExtend(unsigned(XR[xj].H[2i+1])-unsigned(XR[xk].H[2i]), 32)
```

## 53. xvhsubw.du.wu

```asm
for i in range(4) :
XR[xd].D[i] = ZeroExtend(unsigned(XR[xj].W[2i+1])-unsigned(XR[xk].W[2i]), 64)
```

## 54. xvhsubw.qu.du

```asm
for i in range(2) :
XR[xd].Q[i] = ZeroExtend(unsigned(XR[xj].D[2i+1])-unsigned(XR[xk].D[2i]), 128)
```

## 55. xvaddwev.h.b

```asm
for i in range(16) :
XR[vd].H[i] = SignExtend(XR[vj].B[2i], 16)+SignExtend(XR[vk].B[2i], 16)
```

## 56. xvaddwev.w.h

```asm
for i in range(8) :
XR[vd].W[i] = SignExtend(XR[vj].H[2i], 32)+SignExtend(XR[vk].H[2i], 32)
```

## 57. xvaddwev.d.w

```asm
for i in range(4) :
XR[vd].D[i] = SignExtend(XR[vj].W[2i], 64)+SignExtend(XR[vk].W[2i], 64)
```

## 58. xvaddwev.q.d

```asm
for i in range(2) :
XR[vd].Q[i] = SignExtend(XR[vj].D[2i], 128)+SignExtend(XR[vk].D[2i], 128)
```

## 59. xvaddwod.h.b

```asm
for i in range(16) :
XR[vd].H[i] = SignExtend(XR[vj].B[2i+1], 16)+SignExtend(XR[vk].B[2i+1], 16)
```

## 60. xvaddwod.w.h

```asm
for i in range(8) :
XR[vd].W[i] = SignExtend(XR[vj].H[2i+1], 32)+SignExtend(XR[vk].H[2i+1], 32)
```

## 61. xvaddwod.d.w

```asm
for i in range(4) :
XR[vd].D[i] = SignExtend(XR[vj].W[2i+1], 64)+SignExtend(XR[vk].W[2i+1], 64)
```

## 62. xvaddwod.q.d

```asm
for i in range(2) :
XR[vd].Q[i] = SignExtend(XR[vj].D[2i+1], 128)+SignExtend(XR[vk].D[2i+1], 128)
```

## 63. xvsubwev.h.b

```asm
for i in range(16) :
XR[vd].H[i] = SignExtend(XR[vj].B[2i], 16)-SignExtend(XR[vk].B[2i], 16)
```

## 64. xvsubwev.w.h

```asm
for i in range(8) :
XR[vd].W[i] = SignExtend(XR[vj].H[2i], 32)-SignExtend(XR[vk].H[2i], 32)
```

## 65. xvsubwev.d.w

```asm
for i in range(4) :
XR[vd].D[i] = SignExtend(XR[vj].W[2i], 64)-SignExtend(XR[vk].W[2i], 64)
```

## 66. xvsubwev.q.d

```asm
for i in range(2) :
XR[vd].Q[i] = SignExtend(XR[vj].D[2i], 128)-SignExtend(XR[vk].D[2i], 128)
```

## 67. xvsubwod.h.b

```asm
for i in range(16) :
XR[vd].H[i] = SignExtend(XR[vj].B[2i+1], 16)-SignExtend(XR[vk].B[2i+1], 16)
```

## 68. xvsubwod.w.h

```asm
for i in range(8) :
XR[vd].W[i] = SignExtend(XR[vj].H[2i+1], 32)-SignExtend(XR[vk].H[2i+1], 32)
```

## 69. xvsubwod.d.w

```asm
for i in range(4) :
XR[vd].D[i] = SignExtend(XR[vj].W[2i+1], 64)-SignExtend(XR[vk].W[2i+1], 64)
```

## 70. xvsubwod.q.d

```asm
for i in range(2) :
XR[vd].Q[i] = SignExtend(XR[vj].D[2i+1], 128)-SignExtend(XR[vk].D[2i+1], 128)
```

## 71. xvaddwev.h.bu

```asm
for i in range(16) :
XR[vd].H[i] = ZeroExtend(XR[vj].B[2i], 16)+ZeroExtend(XR[vk].B[2i], 16)
```

## 72. xvaddwev.w.hu

```asm
for i in range(8) :
XR[vd].W[i] = ZeroExtend(XR[vj].H[2i], 32)+ZeroExtend(XR[vk].H[2i], 32)
```

## 73. xvaddwev.d.wu

```asm
for i in range(4) :
XR[vd].D[i] = ZeroExtend(XR[vj].W[2i], 64)+ZeroExtend(XR[vk].W[2i], 64)
```

## 74. xvaddwev.q.du

```asm
for i in range(2) :
XR[vd].Q[i] = ZeroExtend(XR[vj].D[2i], 128)+ZeroExtend(XR[vk].D[2i], 128)
```

## 75. xvaddwod.h.bu

```asm
for i in range(16) :
XR[vd].H[i] = ZeroExtend(XR[vj].B[2i+1], 16)+ZeroExtend(XR[vk].B[2i+1], 16)
```

## 76. xvaddwod.w.hu

```asm
for i in range(8) :
XR[vd].W[i] = ZeroExtend(XR[vj].H[2i+1], 32)+ZeroExtend(XR[vk].H[2i+1], 32)
```

## 77. xvaddwod.d.wu

```asm
for i in range(4) :
XR[vd].D[i] = ZeroExtend(XR[vj].W[2i+1], 64)+ZeroExtend(XR[vk].W[2i+1], 64)
```

## 78. xvaddwod.q.du

```asm
for i in range(2) :
XR[vd].Q[i] = ZeroExtend(XR[vj].D[2i+1], 128)+ZeroExtend(XR[vk].D[2i+1], 128)
```

## 79. xvsubwev.h.bu

```asm
for i in range(16) :
XR[vd].H[i] = ZeroExtend(XR[vj].B[2i], 16)-ZeroExtend(XR[vk].B[2i], 16)
```

## 80. xvsubwev.w.hu

```asm
for i in range(8) :
XR[vd].W[i] = ZeroExtend(XR[vj].H[2i], 32)-ZeroExtend(XR[vk].H[2i], 32)
```

## 81. xvsubwev.d.wu

```asm
for i in range(4) :
XR[vd].D[i] = ZeroExtend(XR[vj].W[2i], 64)-ZeroExtend(XR[vk].W[2i], 64)
```

## 82. xvsubwev.q.du

```asm
for i in range(2) :
XR[vd].Q[i] = ZeroExtend(XR[vj].D[2i], 128)-ZeroExtend(XR[vk].D[2i], 128)
```

## 83. xvsubwod.h.bu

```asm
for i in range(16) :
XR[vd].H[i] = ZeroExtend(XR[vj].B[2i+1], 16)-ZeroExtend(XR[vk].B[2i+1], 16)
```

## 84. xvsubwod.w.hu

```asm
for i in range(8) :
XR[vd].W[i] = ZeroExtend(XR[vj].H[2i+1], 32)-ZeroExtend(XR[vk].H[2i+1], 32)
```

## 85. xvsubwod.d.wu

```asm
for i in range(4) :
XR[vd].D[i] = ZeroExtend(XR[vj].W[2i+1], 64)-ZeroExtend(XR[vk].W[2i+1], 64)
```

## 86. xvsubwod.q.du

```asm
for i in range(2) :
XR[vd].Q[i] = ZeroExtend(XR[vj].D[2i+1], 128)-ZeroExtend(XR[vk].D[2i+1], 128)
```

## 87. xvaddwev.h.bu.b

```asm
for i in range(16) :
XR[vd].H[i] = ZeroExtend(XR[vj].B[2i], 16)+SignExtend(XR[vk].B[2i], 16)
```

## 88. xvaddwev.w.hu.h

```asm
for i in range(8) :
XR[vd].W[i] = ZeroExtend(XR[vj].H[2i], 32)+SignExtend(XR[vk].H[2i], 32)
```

## 89. xvaddwev.d.wu.w

```asm
for i in range(4) :
XR[vd].D[i] = ZeroExtend(XR[vj].W[2i], 64)+SignExtend(XR[vk].W[2i], 64)
```

## 90. xvaddwev.q.du.d

```asm
for i in range(2) :
XR[vd].Q[i] = ZeroExtend(XR[vj].D[2i], 128)+SignExtend(XR[vk].D[2i], 128)
```

## 91. xvaddwod.h.bu.b

```asm
for i in range(16) :
XR[vd].H[i] = ZeroExtend(XR[vj].B[2i+1], 16)+SignExtend(XR[vk].B[2i+1], 16)
```

## 92. xvaddwod.w.hu.h

```asm
for i in range(8) :
XR[vd].W[i] = ZeroExtend(XR[vj].H[2i+1], 32)+SignExtend(XR[vk].H[2i+1], 32)
```

## 93. xvaddwod.d.wu.w

```asm
for i in range(4) :
XR[vd].D[i] = ZeroExtend(XR[vj].W[2i+1], 64)+SignExtend(XR[vk].W[2i+1], 64)
```

## 94. xvaddwod.q.du.d

```asm
for i in range(2) :
XR[vd].Q[i] = ZeroExtend(XR[vj].D[2i+1], 128)+SignExtend(XR[vk].D[2i+1], 128)
```

## 95. xvavg.b

```asm
for i in range(32) :
XR[xd].B[i] = (signed(XR[xj].B[i])+signed(XR[xk].B[i])) / 2
```

## 96. xvavg.h

```asm
for i in range(16) :
XR[xd].H[i] = (signed(XR[xj].H[i])+signed(XR[xk].H[i])) / 2
```

## 97. xvavg.w

```asm
for i in range(8) :
XR[xd].W[i] = (signed(XR[xj].W[i])+signed(XR[xk].W[i])) / 2
```

## 98. xvavg.d

```asm
for i in range(4) :
XR[xd].D[i] = (signed(XR[xj].D[i])+signed(XR[xk].D[i])) / 2
```

## 99. xvavg.bu

```asm
for i in range(32) :
XR[xd].B[i] = (unsigned(XR[xj].B[i])+unsigned(XR[xk].B[i])) / 2
```

## 100. xvavg.hu

```asm
for i in range(16) :
XR[xd].H[i] = (unsigned(XR[xj].H[i])+unsigned(XR[xk].H[i])) / 2
```

## 101. xvavg.wu

```asm
for i in range(8) :
XR[xd].W[i] = (unsigned(XR[xj].W[i])+unsigned(XR[xk].W[i])) / 2
```

## 102. xvavg.du

```asm
for i in range(4) :
XR[xd].D[i] = (unsigned(XR[xj].D[i])+unsigned(XR[xk].D[i])) / 2
```

## 103. xvavgr.b

```asm
for i in range(32) :
XR[xd].B[i] = (signed(XR[xj].B[i])+signed(XR[xk].B[i])+1) / 2
```

## 104. xvavgr.h

```asm
for i in range(16) :
XR[xd].H[i] = (signed(XR[xj].H[i])+signed(XR[xk].H[i])+1) / 2
```

## 105. xvavgr.w

```asm
for i in range(8) :
XR[xd].W[i] = (signed(XR[xj].W[i])+signed(XR[xk].W[i])+1) / 2
```

## 106. xvavgr.d

```asm
for i in range(4) :
XR[xd].D[i] = (signed(XR[xj].D[i])+signed(XR[xk].D[i])+1) / 2
```

## 107. xvavgr.bu

```asm
for i in range(32) :
XR[xd].B[i] = (unsigned(XR[xj].B[i])+unsigned(XR[xk].B[i])+1) / 2
```

## 108. xvavgr.hu

```asm
for i in range(16) :
XR[xd].H[i] = (unsigned(XR[xj].H[i])+unsigned(XR[xk].H[i])+1) / 2
```

## 109. xvavgr.wu

```asm
for i in range(8) :
XR[xd].W[i] = (unsigned(XR[xj].W[i])+unsigned(XR[xk].W[i])+1) / 2
```

## 110. xvavgr.du

```asm
for i in range(4) :
XR[xd].D[i] = (unsigned(XR[xj].D[i])+unsigned(XR[xk].D[i])+1) / 2
```

## 111. xvabsd.b

```asm
for i in range(32) :
XR[xd].B[i] = Abs_Diff_S(XR[xj].B[i], XR[xk].B[i])
```

## 112. xvabsd.h

```asm
for i in range(16) :
XR[xd].H[i] = Abs_Diff_S(XR[xj].H[i], XR[xk].H[i])
```

## 113. xvabsd.w

```asm
for i in range(8) :
XR[xd].W[i] = Abs_Diff_S(XR[xj].W[i], XR[xk].W[i])
```

## 114. xvabsd.d

```asm
for i in range(4) :
XR[xd].D[i] = Abs_Diff_S(XR[xj].D[i], XR[xk].D[i])
```

## 115. xvabsd.bu

```asm
for i in range(32) :
XR[xd].B[i] = Abs_Diff_U(XR[xj].B[i], XR[xk].B[i])
```

## 116. xvabsd.hu

```asm
for i in range(16) :
XR[xd].H[i] = Abs_Diff_U(XR[xj].H[i], XR[xk].H[i])
```

## 117. xvabsd.wu

```asm
for i in range(8) :
XR[xd].W[i] = Abs_Diff_U(XR[xj].W[i], XR[xk].W[i])
```

## 118. xvabsd.du

```asm
for i in range(4) :
XR[xd].D[i] = Abs_Diff_U(XR[xj].D[i], XR[xk].D[i])
```

## 119. xvadda.b

```asm
for i in range(32) :
XR[xd].B[i] = Abs(XR[xj].B[i])+Abs(XR[xk].B[i])
```

## 120. xvadda.h

```asm
for i in range(16) :
XR[xd].H[i] = Abs(XR[xj].H[i])+Abs(XR[xk].H[i])
```

## 121. xvadda.w

```asm
for i in range(8) :
XR[xd].W[i] = Abs(XR[xj].W[i])+Abs(XR[xk].W[i])
```

## 122. xvadda.d

```asm
for i in range(4) :
XR[xd].D[i] = Abs(XR[xj].D[i])+Abs(XR[xk].D[i])
```

## 123. xvmax.b

```asm
for i in range(32) :
XR[xd].B[i] = Max_S(XR[xj].B[i], XR[xk].B[i])
```

## 124. xvmax.h

```asm
for i in range(16) :
XR[xd].H[i] = Max_S(XR[xj].H[i], XR[xk].H[i])
```

## 125. xvmax.w

```asm
for i in range(8) :
XR[xd].W[i] = Max_S(XR[xj].W[i], XR[xk].W[i])
```

## 126. xvmax.d

```asm
for i in range(4) :
XR[xd].D[i] = Max_S(XR[xj].D[i], XR[xk].D[i])
```

## 127. xvmaxi.b

```asm
for i in range(32) :
XR[xd].B[i] = Max_S(XR[xj].B[i], SignExtend(si5, 8))
```

## 128. xvmaxi.h

```asm
for i in range(16) :
XR[xd].H[i] = Max_S(XR[xj].H[i], SignExtend(si5, 16))
```

## 129. xvmaxi.w

```asm
for i in range(8) :
XR[xd].W[i] = Max_S(XR[xj].W[i], SignExtend(si5, 32))
```

## 130. xvmaxi.d

```asm
for i in range(4) :
XR[xd].D[i] = Max_S(XR[xj].D[i], SignExtend(si5, 64))
```

## 131. xvmax.bu

```asm
for i in range(32) :
XR[xd].B[i] = Max_U(XR[xj].B[i], XR[xk].B[i])
```

## 132. xvmax.hu

```asm
for i in range(16) :
XR[xd].H[i] = Max_U(XR[xj].H[i], XR[xk].H[i])
```

## 133. xvmax.wu

```asm
for i in range(8) :
XR[xd].W[i] = Max_U(XR[xj].W[i], XR[xk].W[i])
```

## 134. xvmax.du

```asm
for i in range(4) :
XR[xd].D[i] = Max_U(XR[xj].D[i], XR[xk].D[i])
```

## 135. xvmaxi.bu

```asm
for i in range(32) :
XR[xd].B[i] = Max_U(XR[xj].B[i], ZeroExtend(ui5, 8))
```

## 136. xvmaxi.hu

```asm
for i in range(16) :
XR[xd].H[i] = Max_U(XR[xj].H[i], ZeroExtend(ui5, 16))
```

## 137. xvmaxi.wu

```asm
for i in range(8) :
XR[xd].W[i] = Max_U(XR[xj].W[i], ZeroExtend(ui5, 32))
```

## 138. xvmaxi.du

```asm
for i in range(4) :
XR[xd].D[i] = Max_U(XR[xj].D[i], ZeroExtend(ui5, 64))
```

## 139. xvmin.b

```asm
for i in range(32) :
XR[xd].B[i] = Min_S(XR[xj].B[i], XR[xk].B[i])
```

## 140. xvmin.h

```asm
for i in range(16) :
XR[xd].H[i] = Min_S(XR[xj].H[i], XR[xk].H[i])
```

## 141. xvmin.w

```asm
for i in range(8) :
XR[xd].W[i] = Min_S(XR[xj].W[i], XR[xk].W[i])
```

## 142. xvmin.d

```asm
for i in range(4) :
XR[xd].D[i] = Min_S(XR[xj].D[i], XR[xk].D[i])
```

## 143. xvmini.b

```asm
for i in range(32) :
XR[xd].B[i] = Min_S(XR[xj].B[i], SignExtend(si5, 8))
```

## 144. xvmini.h

```asm
for i in range(16) :
XR[xd].H[i] = Min_S(XR[xj].H[i], SignExtend(si5, 16))
```

## 145. xvmini.w

```asm
for i in range(8) :
XR[xd].W[i] = Min_S(XR[xj].W[i], SignExtend(si5, 32))
```

## 146. xvmini.d

```asm
for i in range(4) :
XR[xd].D[i] = Min_S(XR[xj].D[i], SignExtend(si5, 64))
```

## 147. xvmin.bu

```asm
for i in range(32) :
XR[xd].B[i] = Min_U(XR[xj].B[i], XR[xk].B[i])
```

## 148. xvmin.hu

```asm
for i in range(16) :
XR[xd].H[i] = Min_U(XR[xj].H[i], XR[xk].H[i])
```

## 149. xvmin.wu

```asm
for i in range(8) :
XR[xd].W[i] = Min_U(XR[xj].W[i], XR[xk].W[i])
```

## 150. xvmin.du

```asm
for i in range(4) :
XR[xd].D[i] = Min_U(XR[xj].D[i], XR[xk].D[i])
```

## 151. xvmini.bu

```asm
for i in range(32) :
XR[xd].B[i] = Min_U(XR[xj].B[i], ZeroExtend(ui5, 8))
```

## 152. xvmini.hu

```asm
for i in range(16) :
XR[xd].H[i] = Min_U(XR[xj].H[i], ZeroExtend(ui5, 16))
```

## 153. xvmini.wu

```asm
for i in range(8) :
XR[xd].W[i] = Min_U(XR[xj].W[i], ZeroExtend(ui5, 32))
```

## 154. xvmini.du

```asm
for i in range(4) :
XR[xd].D[i] = Min_U(XR[xj].D[i], ZeroExtend(ui5, 64))
```

## 155. xvmul.b

```asm
for i in range(32) :
product = XR[xj].B[i]*XR[xk].B[i]
XR[xd].B[i] = product[7:0]
```

## 156. xvmul.h

```asm
for i in range(16) :
product = XR[xj].H[i]*XR[xk].H[i]
XR[xd].H[i] = product[15:0]
```

## 157. xvmul.w

```asm
for i in range(8) :
product = XR[xj].W[i]*XR[xk].W[i]
XR[xd].W[i] = product[31:0]
```

## 158. xvmul.d

```asm
for i in range(4) :
product = XR[xj].D[i]*XR[xk].D[i]
XR[xd].D[i] = product[63:0]
```

## 159. xvmuh.b

```asm
for i in range(32) :
product = signed(XR[xj].B[i]) * signed(XR[xk].B[i])
XR[xd].B[i] = product[15:8]
```

## 160. xvmuh.h

```asm
for i in range(16) :
product = signed(XR[xj].H[i]) * signed(XR[xk].H[i])
XR[xd].H[i] = product[31:16]
```

## 161. xvmuh.w

```asm
for i in range(8) :
product = signed(XR[xj].W[i]) * signed(XR[xk].W[i])
XR[xd].W[i] = product[63:32]
```

## 162. xvmuh.d

```asm
for i in range(4) :
product = signed(XR[xj].D[i]) * signed(XR[xk].D[i])
XR[xd].D[i] = product[127:64]
```

## 163. xvmuh.bu

```asm
for i in range(32) :
product = unsigned(XR[xj].B[i]) * unsigned(XR[xk].B[i])
XR[xd].B[i] = product[15:8]
```

## 164. xvmuh.hu

```asm
for i in range(16) :
product = unsigned(XR[xj].H[i]) * unsigned(XR[xk].H[i])
XR[xd].H[i] = product[31:16]
```

## 165. xvmuh.wu

```asm
for i in range(8) :
product = unsigned(XR[xj].W[i]) * unsigned(XR[xk].W[i])
XR[xd].W[i] = product[63:32]
```

## 166. xvmuh.du

```asm
for i in range(4) :
product = unsigned(XR[xj].D[i]) * unsigned(XR[xk].D[i])
XR[xd].D[i] = product[127:64]
```

## 167. xvmulwev.h.b

```asm
for i in range(16) :
product = signed(XR[vj].B[2i])*signed(XR[vk].B[2i])
VR[vd].H[i] = product[15:0]
```

## 168. xvmulwev.w.h

```asm
for i in range(8) :
product = signed(XR[vj].H[2i])*signed(XR[vk].H[2i])
VR[vd].W[i] = product[31:0]
```

## 169. xvmulwev.d.w

```asm
for i in range(4) :
product = signed(XR[vj].W[2i])*signed(XR[vk].W[2i])
VR[vd].D[i] = product[63:0]
```

## 170. xvmulwev.q.d

```asm
for i in range(2) :
product = signed(XR[vj].D[2i])*signed(XR[vk].D[2i])
VR[vd].Q[i] = product[127:0]
```

## 171. xvmulwod.h.b

```asm
for i in range(16) :
product = signed(XR[vj].B[2i+1])*signed(XR[vk].B[2i+1])
VR[vd].H[i] = product[15:0]
```

## 172. xvmulwod.w.h

```asm
for i in range(8) :
product = signed(XR[vj].H[2i+1])*signed(XR[vk].H[2i+1])
VR[vd].W[i] = product[31:0]
```

## 173. xvmulwod.d.w

```asm
for i in range(4) :
product = signed(XR[vj].W[2i+1])*signed(XR[vk].W[2i+1])
VR[vd].D[i] = product[63:0]
```

## 174. xvmulwod.q.d

```asm
for i in range(2) :
product = signed(XR[vj].D[2i+1])*signed(XR[vk].D[2i+1])
VR[vd].Q[i] = product[127:0]
```

## 175. xvmulwev.h.bu

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i])*unsigned(XR[vk].B[2i])
VR[vd].H[i] = product[15:0]
```

## 176. xvmulwev.w.hu

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i])*unsigned(XR[vk].H[2i])
VR[vd].W[i] = product[31:0]
```

## 177. xvmulwev.d.wu

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i])*unsigned(XR[vk].W[2i])
VR[vd].D[i] = product[63:0]
```

## 178. xvmulwev.q.du

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i])*unsigned(XR[vk].D[2i])
VR[vd].Q[i] = product[127:0]
```

## 179. xvmulwod.h.bu

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i+1])*unsigned(XR[vk].B[2i+1])
VR[vd].H[i] = product[15:0]
```

## 180. xvmulwod.w.hu

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i+1])*unsigned(XR[vk].H[2i+1])
VR[vd].W[i] = product[31:0]
```

## 181. xvmulwod.d.wu

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i+1])*unsigned(XR[vk].W[2i+1])
VR[vd].D[i] = product[63:0]
```

## 182. xvmulwod.q.du

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i+1])*unsigned(XR[vk].D[2i+1])
VR[vd].Q[i] = product[127:0]
```

## 183. xvmulwev.h.bu.b

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i])*signed(XR[vk].B[2i])
VR[vd].H[i] = product[15:0]
```

## 184. xvmulwev.w.hu.h

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i])*signed(XR[vk].H[2i])
VR[vd].W[i] = product[31:0]
```

## 185. xvmulwev.d.wu.w

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i])*signed(XR[vk].W[2i])
VR[vd].D[i] = product[63:0]
```

## 186. xvmulwev.q.du.d

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i])*signed(XR[vk].D[2i])
VR[vd].Q[i] = product[127:0]
```

## 187. xvmulwod.h.bu.b

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i+1])*signed(XR[vk].B[2i+1])
VR[vd].H[i] = product[15:0]
```

## 188. xvmulwod.w.hu.h

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i+1])*signed(XR[vk].H[2i+1])
VR[vd].W[i] = product[31:0]
```

## 189. xvmulwod.d.wu.w

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i+1])*signed(XR[vk].W[2i+1])
VR[vd].D[i] = product[63:0]
```

## 190. xvmulwod.q.du.d

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i+1])*signed(XR[vk].D[2i+1])
VR[vd].Q[i] = product[127:0]
```

## 191. xvmadd.b

```asm
for i in range(32) :
product = XR[xj].B[i]*XR[xk].B[i]
XR[xd].B[i] += product[7:0]
```

## 192. xvmadd.h

```asm
for i in range(16) :
product = XR[xj].H[i]*XR[xk].H[i]
XR[xd].H[i] += product[15:0]
```

## 193. xvmadd.w

```asm
for i in range(8) :
product = XR[xj].W[i]*XR[xk].W[i]
XR[xd].W[i] += product[31:0]
```

## 194. xvmadd.d

```asm
for i in range(4) :
product = XR[xj].D[i]*XR[xk].D[i]
XR[xd].D[i] += product[63:0]
```

## 195. xvmsub.b

```asm
for i in range(32) :
product = XR[xj].B[i]*XR[xk].B[i]
XR[xd].B[i] -= product[7:0]
```

## 196. xvmsub.h

```asm
for i in range(16) :
product = XR[xj].H[i]*XR[xk].H[i]
XR[xd].H[i] -= product[15:0]
```

## 197. xvmsub.w

```asm
for i in range(8) :
product = XR[xj].W[i]*XR[xk].W[i]
XR[xd].W[i] -= product[31:0]
```

## 198. xvmsub.d

```asm
for i in range(4) :
product = XR[xj].D[i]*XR[xk].D[i]
XR[xd].D[i] -= product[63:0]
```

## 199. xvmaddwev.h.b

```asm
for i in range(16) :
product = signed(XR[vj].B[2i])*signed(XR[vk].B[2i])
VR[vd].H[i] += product[15:0]
```

## 200. xvmaddwev.w.h

```asm
for i in range(8) :
product = signed(XR[vj].H[2i])*signed(XR[vk].H[2i])
VR[vd].W[i] += product[31:0]
```

## 201. xvmaddwev.d.w

```asm
for i in range(4) :
product = signed(XR[vj].W[2i])*signed(XR[vk].W[2i])
VR[vd].D[i] += product[63:0]
```

## 202. xvmaddwev.q.d

```asm
for i in range(2) :
product = signed(XR[vj].D[2i])*signed(XR[vk].D[2i])
VR[vd].Q[i] += product[127:0]
```

## 203. xvmaddwod.h.b

```asm
for i in range(16) :
product = signed(XR[vj].B[2i+1])*signed(XR[vk].B[2i+1])
VR[vd].H[i] += product[15:0]
```

## 204. xvmaddwod.w.h

```asm
for i in range(8) :
product = signed(XR[vj].H[2i+1])*signed(XR[vk].H[2i+1])
VR[vd].W[i] += product[31:0]
```

## 205. xvmaddwod.d.w

```asm
for i in range(4) :
product = signed(XR[vj].W[2i+1])*signed(XR[vk].W[2i+1])
VR[vd].D[i] += product[63:0]
```

## 206. xvmaddwod.q.d

```asm
for i in range(2) :
product = signed(XR[vj].D[2i+1])*signed(XR[vk].D[2i+1])
VR[vd].Q[i] += product[127:0]
```

## 207. xvmaddwev.h.bu

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i])*unsigned(XR[vk].B[2i])
VR[vd].H[i] += product[15:0]
```

## 208. xvmaddwev.w.hu

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i])*unsigned(XR[vk].H[2i])
VR[vd].W[i] += product[31:0]
```

## 209. xvmaddwev.d.wu

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i])*unsigned(XR[vk].W[2i])
VR[vd].D[i] += product[63:0]
```

## 210. xvmaddwev.q.du

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i])*unsigned(XR[vk].D[2i])
VR[vd].Q[i] += product[127:0]
```

## 211. xvmaddwod.h.bu

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i+1])*unsigned(XR[vk].B[2i+1])
VR[vd].H[i] += product[15:0]
```

## 212. xvmaddwod.w.hu

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i+1])*unsigned(XR[vk].H[2i+1])
VR[vd].W[i] += product[31:0]
```

## 213. xvmaddwod.d.wu

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i+1])*unsigned(XR[vk].W[2i+1])
VR[vd].D[i] += product[63:0]
```

## 214. xvmaddwod.q.du

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i+1])*unsigned(XR[vk].D[2i+1])
VR[vd].Q[i] += product[127:0]
```

## 215. xvmaddwev.h.bu.b

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i])*signed(XR[vk].B[2i])
VR[vd].H[i] += product[15:0]
```

## 216. xvmaddwev.w.hu.h

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i])*signed(XR[vk].H[2i])
VR[vd].W[i] += product[31:0]
```

## 217. xvmaddwev.d.wu.w

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i])*signed(XR[vk].W[2i])
VR[vd].D[i] += product[63:0]
```

## 218. xvmaddwev.q.du.d

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i])*signed(XR[vk].D[2i])
VR[vd].Q[i] += product[127:0]
```

## 219. xvmaddwod.h.bu.b

```asm
for i in range(16) :
product = unsigned(XR[vj].B[2i+1])*signed(XR[vk].B[2i+1])
VR[vd].H[i] += product[15:0]
```

## 220. xvmaddwod.w.hu.h

```asm
for i in range(8) :
product = unsigned(XR[vj].H[2i+1])*signed(XR[vk].H[2i+1])
VR[vd].W[i] += product[31:0]
```

## 221. xvmaddwod.d.wu.w

```asm
for i in range(4) :
product = unsigned(XR[vj].W[2i+1])*signed(XR[vk].W[2i+1])
VR[vd].D[i] += product[63:0]
```

## 222. xvmaddwod.q.du.d

```asm
for i in range(2) :
product = unsigned(XR[vj].D[2i+1])*signed(XR[vk].D[2i+1])
VR[vd].Q[i] += product[127:0]
```

## 223. xvdiv.b

```asm
for i in range(32) :
XR[xd].B[i] = signed(XR[xj].B[i]) / signed(XR[xk].B[i])
```

## 224. xvdiv.h

```asm
for i in range(16) :
XR[xd].H[i] = signed(XR[xj].H[i]) / signed(XR[xk].H[i])
```

## 225. xvdiv.w

```asm
for i in range(8) :
XR[xd].W[i] = signed(XR[xj].W[i]) / signed(XR[xk].W[i])
```

## 226. xvdiv.d

```asm
for i in range(4) :
XR[xd].D[i] = signed(XR[xj].D[i]) / signed(XR[xk].D[i])
```

## 227. xvdiv.bu

```asm
for i in range(32) :
XR[xd].B[i] = unsigned(XR[xj].B[i]) / unsigned(XR[xk].B[i])
```

## 228. xvdiv.hu

```asm
for i in range(16) :
XR[xd].H[i] = unsigned(XR[xj].H[i]) / unsigned(XR[xk].H[i])
```

## 229. xvdiv.wu

```asm
for i in range(8) :
XR[xd].W[i] = unsigned(XR[xj].W[i]) / unsigned(XR[xk].W[i])
```

## 230. xvdiv.du

```asm
for i in range(4) :
XR[xd].D[i] = unsigned(XR[xj].D[i]) / unsigned(XR[xk].D[i])
```

## 231. xvmod.b

```asm
for i in range(32) :
XR[xd].B[i] = signed(XR[xj].B[i]) % signed(XR[xk].B[i])
```

## 232. xvmod.h

```asm
for i in range(16) :
XR[xd].H[i] = signed(XR[xj].H[i]) % signed(XR[xk].H[i])
```

## 233. xvmod.w

```asm
for i in range(8) :
XR[xd].W[i] = signed(XR[xj].W[i]) % signed(XR[xk].W[i])
```

## 234. xvmod.d

```asm
for i in range(4) :
XR[xd].D[i] = signed(XR[xj].D[i]) % signed(XR[xk].D[i])
```

## 235. xvmod.bu

```asm
for i in range(32) :
XR[xd].B[i] = unsigned(XR[xj].B[i]) % unsigned(XR[xk].B[i])
```

## 236. xvmod.hu

```asm
for i in range(16) :
XR[xd].H[i] = unsigned(XR[xj].H[i]) % unsigned(XR[xk].H[i])
```

## 237. xvmod.wu

```asm
for i in range(8) :
XR[xd].W[i] = unsigned(XR[xj].W[i]) % unsigned(XR[xk].W[i])
```

## 238. xvmod.du

```asm
for i in range(4) :
XR[xd].D[i] = unsigned(XR[xj].D[i]) % unsigned(XR[xk].D[i])
```

## 239. xvsat.b

```asm
for i in range(16) :
XR[xd].B[i] = Sat_S(XR[xj].B[i], ui3)
```

## 240. xvsat.h

```asm
for i in range(8) :
XR[xd].H[i] = Sat_S(XR[xj].H[i], ui4)
```

## 241. xvsat.w

```asm
for i in range(16) :
XR[xd].W[i] = Sat_S(XR[xj].W[i], ui5)
```

## 242. xvsat.d

```asm
for i in range(16) :
XR[xd].D[i] = Sat_S(XR[xj].D[i], ui6)
```

## 243. xvsat.bu

```asm
for i in range(16) :
XR[xd].B[i] = Sat_U(XR[xj].B[i], ui3)
```

## 244. xvsat.hu

```asm
for i in range(8) :
XR[xd].H[i] = Sat_U(XR[xj].H[i], ui4)
```

## 245. xvsat.wu

```asm
for i in range(16) :
XR[xd].W[i] = Sat_U(XR[xj].W[i], ui5)
```

## 246. xvsat.du

```asm
for i in range(16) :
XR[xd].D[i] = Sat_U(XR[xj].D[i], ui6)
```

## 247. xvexth.h.b

```asm
for i in range(8):
XR[xd].H[i] = SignExtend(XR[xj].B[i+8], 16)
XR[xd].H[i+8] = SignExtend(XR[xj].B[i+24], 16)
```

## 248. xvexth.w.h

```asm
for i in range(4):
XR[xd].W[i] = SignExtend(XR[xj].H[i+4], 32)
XR[xd].W[i+4] = SignExtend(XR[xj].H[i+12], 32)
```

## 249. xvexth.d.w

```asm
for i in range(2):
XR[xd].D[i] = SignExtend(XR[xj].W[i+2], 64)
XR[xd].D[i+2] = SignExtend(XR[xj].W[i+6], 64)
```

## 250. xvexth.q.d

```asm
XR[xd].Q[0] = SignExtend(XR[xj].D[1], 128)
XR[xd].Q[1] = SignExtend(XR[xj].D[3], 128)
```

## 251. xvexth.hu.bu

```asm
for i in range(8):
XR[xd].H[i] = ZeroExtend(XR[xj].B[i+8], 16)
XR[xd].H[i+8] = ZeroExtend(XR[xj].B[i+24], 16)
```

## 252. xvexth.wu.hu

```asm
for i in range(4):
XR[xd].W[i] = ZeroExtend(XR[xj].H[i+4], 32)
XR[xd].W[i+4] = ZeroExtend(XR[xj].H[i+12], 32)
```

## 253. xvexth.du.wu

```asm
for i in range(2):
XR[xd].D[i] = ZeroExtend(XR[xj].W[i+2], 64)
XR[xd].D[i+2] = ZeroExtend(XR[xj].W[i+6], 64)
```

## 254. xvexth.qu.du

```asm
XR[xd].Q[0] = ZeroExtend(XR[xj].D[1], 128)
XR[xd].Q[1] = ZeroExtend(XR[xj].D[3], 128)
```

## 255. vext2xv.h.b

```asm
for i in range(16):
XR[xd].H[i] = SignExtend(XR[xj].B[i], 16)
```

## 256. vext2xv.w.b

```asm
for i in range(8):
XR[xd].W[i] = SignExtend(XR[xj].B[i], 32)
```

## 257. vext2xv.d.b

```asm
for i in range(4):
XR[xd].D[i] = SignExtend(XR[xj].B[i], 64)
```

## 258. vext2xv.w.h

```asm
for i in range(8):
XR[xd].W[i] = SignExtend(XR[xj].H[i], 32)
```

## 259. vext2xv.d.h

```asm
for i in range(4):
XR[xd].D[i] = SignExtend(XR[xj].H[i], 64)
```

## 260. vext2xv.d.w

```asm
for i in range(4):
XR[xd].D[i] = SignExtend(XR[xj].W[i], 64)
```

## 261. vext2xv.hu.bu

```asm
for i in range(16):
XR[xd].H[i] = ZeroExtend(XR[xj].B[i], 16)
```

## 262. vext2xv.wu.bu

```asm
for i in range(8):
XR[xd].W[i] = ZeroExtend(XR[xj].B[i], 32)
```

## 263. vext2xv.du.bu

```asm
for i in range(4):
XR[xd].D[i] = ZeroExtend(XR[xj].B[i], 64)
```

## 264. vext2xv.wu.hu

```asm
for i in range(8):
XR[xd].W[i] = ZeroExtend(XR[xj].H[i], 32)
```

## 265. vext2xv.du.hu

```asm
for i in range(4):
XR[xd].D[i] = ZeroExtend(XR[xj].H[i], 64)
```

## 266. vext2xv.du.wu

```asm
for i in range(4):
XR[xd].D[i] = ZeroExtend(XR[xj].W[i], 64)
```

## 267. xvsigncov.b

```asm
for i in range(32):
XR[xd].B[i] = Sign_Convert(XR[xj].B[i], XR[xk].B[i])
```

## 268. xvsigncov.h

```asm
for i in range(16):
XR[xd].H[i] = Sign_Convert(XR[xj].H[i], XR[xk].H[i])
```

## 269. xvsigncov.w

```asm
for i in range(8):
XR[xd].W[i] = Sign_Convert(XR[xj].W[i], XR[xk].W[i])
```

## 270. xvsigncov.d

```asm
for i in range(4):
XR[xd].D[i] = Sign_Convert(XR[xj].D[i], XR[xk].D[i])
```

## 271. xvmskltz.b

```asm
XR[xd] = 0
for i in range(16):
XR[xd].V[i] = XR[xj].B[i] <0 ? 1'b1 : 1'b0
XR[xd].V[128+i] = XR[xj].B[i+16]<0 ? 1'b1 : 1'b0
```

## 272. xvmskltz.h

```asm
XR[xd] = 0
for i in range(8):
XR[xd].V[i] = XR[xj].H[i] <0 ? 1'b1 : 1'b0
XR[xd].V[i+128] = XR[xj].H[i+8] <0 ? 1'b1 : 1'b0
```

## 273. xvmskltz.w

```asm
XR[xd] = 0
for i in range(4):
XR[xd].V[i] = XR[xj].W[i] <0 ? 1'b1 : 1'b0
XR[xd].V[i+128] = XR[xj].W[i+4] <0 ? 1'b1 : 1'b0
```

## 274. xvmskltz.d

```asm
XR[xd] = 0
for i in range(2):
XR[xd].V[i] = XR[xj].D[i] <0 ? 1'b1 : 1'b0
XR[xd].V[i+128] = XR[xj].D[i+2] <0 ? 1'b1 : 1'b0
```

## 275. xvmskgez.b

```asm
XR[xd] = 0
for i in range(16):
XR[xd].V[i] = XR[xj].B[i] >=0 ? 1'b1 : 1'b0
XR[xd].V[128+i] = XR[xj].B[i+16]>=0 ? 1'b1 : 1'b0
```

## 276. xvmsknz.b

```asm
XR[xd] = 0
for i in range(16):
XR[xd].V[i] = XR[xj].B[i] !=0 ? 1'b1 : 1'b0
XR[xd].V[128+i] = XR[xj].B[i+16]!=0 ? 1'b1 : 1'b0
```

## 277. xvldi

```asm
for i in range(4) :
XR[xd].D[i] = Set_Value(i13)
```

## 278. xvand.v

```asm
XR[xd] = XR[xj] & XR[xk]
```

## 279. xvor.v

```asm
XR[xd] = XR[xj] | XR[xk]
```

## 280. xvxor.v

```asm
XR[xd] = XR[xj] ^ XR[xk]
```

## 281. xvnor.v

```asm
XR[xd] = ~(XR[xj] | XR[xk])
```

## 282. xvandn.v

```asm
XR[xd] = ~XR[xj] & XR[xk]
```

## 283. xvorn.v

```asm
XR[xd] = XR[xj] | ~XR[xk]
```

## 284. xvandi.b

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i] & ui8
```

## 285. xvori.b

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i] | ui8
```

## 286. xvxori.b

```asm
for i in range(32) :
XR[xd].B[i] = XR[xj].B[i] ^ ui8
```

## 287. xvnori.b

```asm
for i in range(32) :
XR[xd].B[i] = ~(XR[xj].B[i] | ui8)
```

## 288. xvsll.b

```asm
for i in range(32) :
XR[xd].B[i] = SLL(XR[xj].B[i], XR[xk].B[i]%8)
```

## 289. xvsll.h

```asm
for i in range(16) :
XR[xd].H[i] = SLL(XR[xj].H[i], XR[xk].H[i]%16)
```

## 290. xvsll.w

```asm
for i in range(8) :
XR[xd].W[i] = SLL(XR[xj].W[i], XR[xk].W[i]%32)
```

## 291. xvsll.d

```asm
for i in range(4) :
XR[xd].D[i] = SLL(XR[xj].D[i], XR[xk].D[i]%64)
```

## 292. xvslli.b

```asm
for i in range(32) :
XR[xd].B[i] = SLL(XR[xj].B[i], ui3)
```

## 293. xvslli.h

```asm
for i in range(16) :
XR[xd].H[i] = SLL(XR[xj].H[i], ui4)
```

## 294. xvslli.w

```asm
for i in range(8) :
XR[xd].W[i] = SLL(XR[xj].W[i], ui5)
```

## 295. xvslli.d

```asm
for i in range(4) :
XR[xd].D[i] = SLL(XR[xj].D[i], ui6)
```

## 296. xvsrl.b

```asm
for i in range(32) :
XR[xd].B[i] = SRL(XR[xj].B[i], XR[xk].B[i]%8)
```

## 297. xvsrl.h

```asm
for i in range(16) :
XR[xd].H[i] = SRL(XR[xj].H[i], XR[xk].H[i]%16)
```

## 298. xvsrl.w

```asm
for i in range(8) :
XR[xd].W[i] = SRL(XR[xj].W[i], XR[xk].W[i]%32)
```

## 299. xvsrl.d

```asm
for i in range(4) :
XR[xd].D[i] = SRL(XR[xj].D[i], XR[xk].D[i]%64)
```

## 300. xvsrli.b

```asm
for i in range(32) :
XR[xd].B[i] = SRL(XR[xj].B[i], ui3)
```

## 301. xvsrli.h

```asm
for i in range(16) :
XR[xd].H[i] = SRL(XR[xj].H[i], ui4)
```

## 302. xvsrli.w

```asm
for i in range(8) :
XR[xd].W[i] = SRL(XR[xj].W[i], ui5)
```

## 303. xvsrli.d

```asm
for i in range(4) :
XR[xd].D[i] = SRL(XR[xj].D[i], ui6)
```

## 304. xvsra.b

```asm
for i in range(32) :
XR[xd].B[i] = SRA(XR[xj].B[i], XR[xk].B[i]%8)
```

## 305. xvsra.h

```asm
for i in range(16) :
XR[xd].H[i] = SRA(XR[xj].H[i], XR[xk].H[i]%16)
```

## 306. xvsra.w

```asm
for i in range(8) :
XR[xd].W[i] = SRA(XR[xj].W[i], XR[xk].W[i]%32)
```

## 307. xvsra.d

```asm
for i in range(4) :
XR[xd].D[i] = SRA(XR[xj].D[i], XR[xk].D[i]%64)
```

## 308. xvsrai.b

```asm
for i in range(32) :
XR[xd].B[i] = SRA(XR[xj].B[i], ui3)
```

## 309. xvsrai.h

```asm
for i in range(16) :
XR[xd].H[i] = SRA(XR[xj].H[i], ui4)
```

## 310. xvsrai.w

```asm
for i in range(8) :
XR[xd].W[i] = SRA(XR[xj].W[i], ui5)
```

## 311. xvsrai.d

```asm
for i in range(4) :
XR[xd].D[i] = SRA(XR[xj].D[i], ui6)
```

## 312. xvrotr.b

```asm
for i in range(32) :
XR[xd].B[i] = ROTR(XR[xj].B[i], XR[xk].B[i]%8)
```

## 313. xvrotr.h

```asm
for i in range(16) :
XR[xd].H[i] = ROTR(XR[xj].H[i], XR[xk].H[i]%16)
```

## 314. xvrotr.w

```asm
for i in range(8) :
XR[xd].W[i] = ROTR(XR[xj].W[i], XR[xk].W[i]%32)
```

## 315. xvrotr.d

```asm
for i in range(4) :
XR[xd].D[i] = ROTR(XR[xj].D[i], XR[xk].D[i]%64)
```

## 316. xvrotri.b

```asm
for i in range(32) :
XR[xd].B[i] = ROTR(XR[xj].B[i], ui3)
```

## 317. xvrotri.h

```asm
for i in range(16) :
XR[xd].H[i] = ROTR(XR[xj].H[i], ui4)
```

## 318. xvrotri.w

```asm
for i in range(8) :
XR[xd].W[i] = ROTR(XR[xj].W[i], ui5)
```

## 319. xvrotri.d

```asm
for i in range(4) :
XR[xd].D[i] = ROTR(XR[xj].D[i], ui6)
```

## 320. xvsllwil.h.b

```asm
for i in range(8) :
XR[xd].H[i] = SLLW_S(XR[xj].B[i], ui3)
XR[xd].H[i+8] = SLLW_S(XR[xj].B[i+16], ui3)
```

## 321. xvsllwil.w.h

```asm
for i in range(4) :
XR[xd].W[i] = SLLW_S(XR[xj].H[i], ui4)
XR[xd].W[i+4] = SLLW_S(XR[xj].H[i+8], ui4)
```

## 322. xvsllwil.d.w

```asm
for i in range(2) :
XR[xd].D[i] = SLLW_S(XR[xj].W[i], ui5)
XR[xd].D[i+2] = SLLW_S(XR[xj].W[i+4], ui5)
```

## 323. xvextl.q.d

```asm
for i in range(1) :
XR[xd].Q[i] = SignExtend(XR[vj].D[i], 128)
XR[xd].Q[i+1] = SignExtend(XR[vj].D[i+2], 128)
```

## 324. xvsllwil.hu.bu

```asm
for i in range(8) :
XR[xd].H[i] = SLLW_U(XR[xj].B[i], ui3)
XR[xd].H[i+8] = SLLW_U(XR[xj].B[i+16], ui3)
```

## 325. xvsllwil.wu.hu

```asm
for i in range(4) :
XR[xd].W[i] = SLLW_U(XR[xj].H[i], ui4)
XR[xd].W[i+4] = SLLW_U(XR[xj].H[i+8], ui4)
```

## 326. xvsllwil.du.wu

```asm
for i in range(2) :
XR[xd].D[i] = SLLW_U(XR[xj].W[i], ui5)
XR[xd].D[i+2] = SLLW_U(XR[xj].W[i+4], ui5)
```

## 327. xvextl.qu.du

```asm
for i in range(1) :
XR[xd].Q[i] = ZeroExtend(XR[vj].D[i], 128)
XR[xd].Q[i+1] = ZeroExtend(XR[vj].D[i+2], 128)
```

## 328. xvsrlr.b

```asm
for i in range(32) :
XR[xd].B[i] = SRLR(XR[xj].B[i], XR[xk].B[i]%8)
```

## 329. xvsrlr.h

```asm
for i in range(16) :
XR[xd].H[i] = SRLR(XR[xj].H[i], XR[xk].H[i]%16)
```

## 330. xvsrlr.w

```asm
for i in range(8) :
XR[xd].W[i] = SRLR(XR[xj].W[i], XR[xk].W[i]%32)
```

## 331. xvsrlr.d

```asm
for i in range(4) :
XR[xd].D[i] = SRLR(XR[xj].D[i], XR[xk].D[i]%64)
```

## 332. xvsrlri.b

```asm
for i in range(32) :
XR[xd].B[i] = SRLR(XR[xj].B[i], ui3)
```

## 333. xvsrlri.h

```asm
for i in range(16) :
XR[xd].H[i] = SRLR(XR[xj].H[i], ui4)
```

## 334. xvsrlri.w

```asm
for i in range(8) :
XR[xd].W[i] = SRLR(XR[xj].W[i], ui5)
```

## 335. xvsrlri.d

```asm
for i in range(4) :
XR[xd].D[i] = SRLR(XR[xj].D[i], ui6)
```

## 336. xvsrar.b

```asm
for i in range(32) :
XR[xd].B[i] = SRAR(XR[xj].B[i], XR[xk].B[i]%8)
```

## 337. xvsrar.h

```asm
for i in range(16) :
XR[xd].H[i] = SRAR(XR[xj].H[i], XR[xk].H[i]%16)
```

## 338. xvsrar.w

```asm
for i in range(8) :
XR[xd].W[i] = SRAR(XR[xj].W[i], XR[xk].W[i]%32)
```

## 339. xvsrar.d

```asm
for i in range(4) :
XR[xd].D[i] = SRAR(XR[xj].D[i], XR[xk].D[i]%64)
```

## 340. xvsrari.b

```asm
for i in range(32) :
XR[xd].B[i] = SRAR(XR[xj].B[i], ui3)
```

## 341. xvsrari.h

```asm
for i in range(16) :
XR[xd].H[i] = SRAR(XR[xj].H[i], ui4)
```

## 342. xvsrari.w

```asm
for i in range(8) :
XR[xd].W[i] = SRAR(XR[xj].W[i], ui5)
```

## 343. xvsrari.d

```asm
for i in range(4) :
XR[xd].D[i] = SRAR(XR[xj].D[i], ui6)
```

## 344. xvsrln.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SRLN(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SRLN(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 345. xvsrln.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SRLN(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SRLN(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 346. xvsrln.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SRLN(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SRLN(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 347. xvsran.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SRAN(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SRAN(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 348. xvsran.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SRAN(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SRAN(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 349. xvsran.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SRAN(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SRAN(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 350. xvsrlni.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SRLN(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SRLN(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SRLN(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SRLN(XR[xd].H[i+8], ui4)
```

## 351. xvsrlni.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SRLN(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SRLN(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SRLN(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SRLN(XR[xd].W[i+4], ui5)
```

## 352. xvsrlni.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SRLN(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SRLN(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SRLN(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SRLN(XR[xd].D[i+2], ui6)
```

## 353. xvsrlni.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SRLN(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SRLN(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SRLN(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SRLN(XR[xd].Q[i+1], ui7)
```

## 354. xvsrani.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SRAN(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SRAN(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SRAN(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SRAN(XR[xd].H[i+8], ui4)
```

## 355. xvsrani.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SRAN(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SRAN(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SRAN(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SRAN(XR[xd].W[i+4], ui5)
```

## 356. xvsrani.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SRAN(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SRAN(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SRAN(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SRAN(XR[xd].D[i+2], ui6)
```

## 357. xvsrani.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SRAN(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SRAN(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SRAN(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SRAN(XR[xd].Q[i+1], ui7)
```

## 358. xvsrlrn.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SRLRN(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SRLRN(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 359. xvsrlrn.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SRLRN(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SRLRN(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 360. xvsrlrn.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SRLRN(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SRLRN(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 361. xvsrarn.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SRARN(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SRARN(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 362. xvsrarn.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SRARN(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SRARN(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 363. xvsrarn.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SRARN(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SRARN(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 364. xvsrlrni.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SRLRN(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SRLRN(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SRLRN(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SRLRN(XR[xd].H[i+8], ui4)
```

## 365. xvsrlrni.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SRLRN(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SRLRN(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SRLRN(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SRLRN(XR[xd].W[i+4], ui5)
```

## 366. xvsrlrni.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SRLRN(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SRLRN(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SRLRN(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SRLRN(XR[xd].D[i+2], ui6)
```

## 367. xvsrlrni.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SRLRN(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SRLRN(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SRLRN(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SRLRN(XR[xd].Q[i+1], ui7)
```

## 368. xvsrarni.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SRARN(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SRARN(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SRARN(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SRARN(XR[xd].H[i+8], ui4)
```

## 369. xvsrarni.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SRARN(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SRARN(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SRARN(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SRARN(XR[xd].W[i+4], ui5)
```

## 370. xvsrarni.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SRARN(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SRARN(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SRARN(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SRARN(XR[xd].D[i+2], ui6)
```

## 371. xvsrarni.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SRARN(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SRARN(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SRARN(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SRARN(XR[xd].Q[i+1], ui7)
```

## 372. xvssrln.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRLNS(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRLNS(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 373. xvssrln.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRLNS(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRLNS(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 374. xvssrln.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRLNS(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRLNS(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 375. xvssran.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRANS(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRANS(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 376. xvssran.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRANS(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRANS(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 377. xvssran.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRANS(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRANS(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 378. xvssrln.bu.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRLNU(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRLNU(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 379. xvssrln.hu.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRLNU(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRLNU(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 380. xvssrln.wu.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRLNU(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRLNU(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 381. xvssran.bu.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRANU(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRANU(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 382. xvssran.hu.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRANU(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRANU(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 383. xvssran.wu.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRANU(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRANU(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 384. xvssrlni.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRLNS(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRLNS(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRLNS(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRLNS(XR[xd].H[i+8], ui4)
```

## 385. xvssrlni.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRLNS(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRLNS(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRLNS(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRLNS(XR[xd].W[i+4], ui5)
```

## 386. xvssrlni.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRLNS(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRLNS(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRLNS(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRLNS(XR[xd].D[i+2], ui6)
```

## 387. xvssrlni.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRLNS(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRLNS(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRLNS(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRLNS(XR[xd].Q[i+1], ui7)
```

## 388. xvssrani.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRANS(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRANS(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRANS(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRANS(XR[xd].H[i+8], ui4)
```

## 389. xvssrani.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRANS(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRANS(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRANS(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRANS(XR[xd].W[i+4], ui5)
```

## 390. xvssrani.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRANS(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRANS(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRANS(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRANS(XR[xd].D[i+2], ui6)
```

## 391. xvssrani.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRANS(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRANS(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRANS(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRANS(XR[xd].Q[i+1], ui7)
```

## 392. xvssrlni.bu.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRLNU(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRLNU(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRLNU(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRLNU(XR[xd].H[i+8], ui4)
```

## 393. xvssrlni.hu.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRLNU(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRLNU(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRLNU(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRLNU(XR[xd].W[i+4], ui5)
```

## 394. xvssrlni.wu.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRLNU(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRLNU(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRLNU(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRLNU(XR[xd].D[i+2], ui6)
```

## 395. xvssrlni.du.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRLNU(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRLNU(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRLNU(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRLNU(XR[xd].Q[i+1], ui7)
```

## 396. xvssrani.bu.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRANU(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRANU(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRANU(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRANU(XR[xd].H[i+8], ui4)
```

## 397. xvssrani.hu.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRANU(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRANU(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRANU(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRANU(XR[xd].W[i+4], ui5)
```

## 398. xvssrani.wu.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRANU(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRANU(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRANU(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRANU(XR[xd].D[i+2], ui6)
```

## 399. xvssrani.du.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRANU(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRANU(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRANU(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRANU(XR[xd].Q[i+1], ui7)
```

## 400. xvssrlrn.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRLRNS(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRLRNS(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 401. xvssrlrn.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRLRNS(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRLRNS(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 402. xvssrlrn.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRLRNS(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRLRNS(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 403. xvssrarn.b.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRARNS(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRARNS(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 404. xvssrarn.h.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRARNS(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRARNS(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 405. xvssrarn.w.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRARNS(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRARNS(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 406. xvssrlrn.bu.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRLRNU(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRLRNU(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 407. xvssrlrn.hu.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRLRNU(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRLRNU(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 408. xvssrlrn.wu.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRLRNU(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRLRNU(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 409. xvssrarn.bu.h

```asm
for i in range(8) :
VR[vd].B[i] = SSRARNU(VR[vj].H[i], VR[vk].H[i]%16)
VR[vd].B[i+16] = SSRARNU(VR[vj].H[i+8], VR[vk].H[i+8]%16)
for i in range(8, 16, 1) :
VR[vd].B[i] = 0
VR[vd].B[i+16] = 0
```

## 410. xvssrarn.hu.w

```asm
for i in range(4) :
VR[vd].H[i] = SSRARNU(VR[vj].W[i], VR[vk].W[i]%32)
VR[vd].H[i+8] = SSRARNU(VR[vj].W[i+4], VR[vk].W[i+4]%32)
for i in range(4, 8, 1) :
VR[vd].H[i] = 0
VR[vd].H[i+8] = 0
```

## 411. xvssrarn.wu.d

```asm
for i in range(2) :
VR[vd].W[i] = SSRARNU(VR[vj].D[i], VR[vk].D[i]%64)
VR[vd].W[i+4] = SSRARNU(VR[vj].D[i+2], VR[vk].D[i+2]%64)
for i in range(2, 4, 1) :
VR[vd].W[i] = 0
VR[vd].W[i+4] = 0
```

## 412. xvssrlrni.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRLRNS(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRLRNS(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRLRNS(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRLRNS(XR[xd].H[i+8], ui4)
```

## 413. xvssrlrni.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRLRNS(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRLRNS(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRLRNS(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRLRNS(XR[xd].W[i+4], ui5)
```

## 414. xvssrlrni.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRLRNS(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRLRNS(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRLRNS(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRLRNS(XR[xd].D[i+2], ui6)
```

## 415. xvssrlrni.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRLRNS(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRLRNS(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRLRNS(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRLRNS(XR[xd].Q[i+1], ui7)
```

## 416. xvssrarni.b.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRARNS(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRARNS(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRARNS(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRARNS(XR[xd].H[i+8], ui4)
```

## 417. xvssrarni.h.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRARNS(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRARNS(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRARNS(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRARNS(XR[xd].W[i+4], ui5)
```

## 418. xvssrarni.w.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRARNS(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRARNS(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRARNS(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRARNS(XR[xd].D[i+2], ui6)
```

## 419. xvssrarni.d.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRARNS(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRARNS(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRARNS(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRARNS(XR[xd].Q[i+1], ui7)
```

## 420. xvssrlrni.bu.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRLRNU(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRLRNU(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRLRNU(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRLRNU(XR[xd].H[i+8], ui4)
```

## 421. xvssrlrni.hu.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRLRNU(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRLRNU(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRLRNU(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRLRNU(XR[xd].W[i+4], ui5)
```

## 422. xvssrlrni.wu.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRLRNU(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRLRNU(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRLRNU(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRLRNU(XR[xd].D[i+2], ui6)
```

## 423. xvssrlrni.du.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRLRNU(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRLRNU(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRLRNU(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRLRNU(XR[xd].Q[i+1], ui7)
```

## 424. xvssrarni.bu.h

```asm
for i in range(8) :
XR[xd].B[i] = SSRARNU(XR[xj].H[i], ui4)
XR[xd].B[i+8] = SSRARNU(XR[xd].H[i], ui4)
XR[xd].B[i+16] = SSRARNU(XR[xj].H[i+8], ui4)
XR[xd].B[i+24] = SSRARNU(XR[xd].H[i+8], ui4)
```

## 425. xvssrarni.hu.w

```asm
for i in range(4) :
XR[xd].H[i] = SSRARNU(XR[xj].W[i], ui5)
XR[xd].H[i+4] = SSRARNU(XR[xd].W[i], ui5)
XR[xd].H[i+8] = SSRARNU(XR[xj].W[i+4], ui5)
XR[xd].H[i+12] = SSRARNU(XR[xd].W[i+4], ui5)
```

## 426. xvssrarni.wu.d

```asm
for i in range(2) :
XR[xd].W[i] = SSRARNU(XR[xj].D[i], ui6)
XR[xd].W[i+2] = SSRARNU(XR[xd].D[i], ui6)
XR[xd].W[i+4] = SSRARNU(XR[xj].D[i+2], ui6)
XR[xd].W[i+6] = SSRARNU(XR[xd].D[i+2], ui6)
```

## 427. xvssrarni.du.q

```asm
for i in range(1) :
XR[xd].D[i] = SSRARNU(XR[xj].Q[i], ui7)
XR[xd].D[i+1] = SSRARNU(XR[xd].Q[i], ui7)
XR[xd].D[i+2] = SSRARNU(XR[xj].Q[i+1], ui7)
XR[xd].D[i+3] = SSRARNU(XR[xd].Q[i+1], ui7)
```

## 428. xvclo.b

```asm
for i in range(32) :
XR[xd].B[i] = CLO(XR[xj].B[i])
```

## 429. xvclo.h

```asm
for i in range(16) :
XR[xd].H[i] = CLO(XR[xj].H[i])
```

## 430. xvclo.w

```asm
for i in range(8) :
XR[xd].W[i] = CLO(XR[xj].W[i])
```

## 431. xvclo.d

```asm
for i in range(4) :
XR[xd].D[i] = CLO(XR[xj].D[i])
```

## 432. xvclz.b

```asm
for i in range(32) :
XR[xd].B[i] = CLZ(XR[xj].B[i])
```

## 433. xvclz.h

```asm
for i in range(16) :
XR[xd].H[i] = CLZ(XR[xj].H[i])
```

## 434. xvclz.w

```asm
for i in range(8) :
XR[xd].W[i] = CLZ(XR[xj].W[i])
```

## 435. xvclz.d

```asm
for i in range(4) :
XR[xd].D[i] = CLZ(XR[xj].D[i])
```

## 436. xvpcnt.b

```asm
for i in range(32) :
XR[xd].B[i] = PCNT(XR[xj].B[i])
```

## 437. xvpcnt.h

```asm
for i in range(16) :
XR[xd].H[i] = PCNT(XR[xj].H[i])
```

## 438. xvpcnt.w

```asm
for i in range(8) :
XR[xd].W[i] = PCNT(XR[xj].W[i])
```

## 439. xvpcnt.d

```asm
for i in range(4) :
XR[xd].D[i] = PCNT(XR[xj].D[i])
```

## 440. xvbitclr.b

```asm
for i in range(32) :
XR[xd].B[i] = Bit_Clear(XR[xj].B[i], XR[xk].B[i]%8)
```

## 441. xvbitclr.h

```asm
for i in range(16) :
XR[xd].H[i] = Bit_Clear(XR[xj].H[i], XR[xk].H[i]%16)
```

## 442. xvbitclr.w

```asm
for i in range(8) :
XR[xd].W[i] = Bit_Clear(XR[xj].W[i], XR[xk].W[i]%32)
```

## 443. xvbitclr.d

```asm
for i in range(4) :
XR[xd].D[i] = Bit_Clear(XR[xj].D[i], XR[xk].D[i]%64)
```

## 444. xvbitclri.b

```asm
for i in range(32) :
XR[xd].B[i] = Bit_Clear(XR[xj].B[i], ui3)
```

## 445. xvbitclri.h

```asm
for i in range(16) :
XR[xd].H[i] = Bit_Clear(XR[xj].H[i], ui4)
```

## 446. xvbitclri.w

```asm
for i in range(8) :
XR[xd].W[i] = Bit_Clear(XR[xj].W[i], ui5)
```

## 447. xvbitclri.d

```asm
for i in range(4) :
XR[xd].D[i] = Bit_Clear(XR[xj].D[i], ui6)
```

## 448. xvbitset.b

```asm
for i in range(32) :
XR[xd].B[i] = Bit_Set(XR[xj].B[i], XR[xk].B[i]%8)
```

## 449. xvbitset.h

```asm
for i in range(16) :
XR[xd].H[i] = Bit_Set(XR[xj].H[i], XR[xk].H[i]%16)
```

## 450. xvbitset.w

```asm
for i in range(8) :
XR[xd].W[i] = Bit_Set(XR[xj].W[i], XR[xk].W[i]%32)
```

## 451. xvbitset.d

```asm
for i in range(4) :
XR[xd].D[i] = Bit_Set(XR[xj].D[i], XR[xk].D[i]%64)
```

## 452. xvbitseti.b

```asm
for i in range(32) :
XR[xd].B[i] = Bit_Set(XR[xj].B[i], ui3)
```

## 453. xvbitseti.h

```asm
for i in range(16) :
XR[xd].H[i] = Bit_Set(XR[xj].H[i], ui4)
```

## 454. xvbitseti.w

```asm
for i in range(8) :
XR[xd].W[i] = Bit_Set(XR[xj].W[i], ui5)
```

## 455. xvbitseti.d

```asm
for i in range(4) :
XR[xd].D[i] = Bit_Set(XR[xj].D[i], ui6)
```

## 456. xvbitrev.b

```asm
for i in range(32) :
XR[xd].B[i] = Bit_Reverse(XR[xj].B[i], XR[xk].B[i]%8)
```

## 457. xvbitrev.h

```asm
for i in range(16) :
XR[xd].H[i] = Bit_Reverse(XR[xj].H[i], XR[xk].H[i]%16)
```

## 458. xvbitrev.w

```asm
for i in range(8) :
XR[xd].W[i] = Bit_Reverse(XR[xj].W[i], XR[xk].W[i]%32)
```

## 459. xvbitrev.d

```asm
for i in range(4) :
XR[xd].D[i] = Bit_Reverse(XR[xj].D[i], XR[xk].D[i]%64)
```

## 460. xvbitrevi.b

```asm
for i in range(32) :
XR[xd].B[i] = Bit_Reverse(XR[xj].B[i], ui3)
```

## 461. xvbitrevi.h

```asm
for i in range(16) :
XR[xd].H[i] = Bit_Reverse(XR[xj].H[i], ui4)
```

## 462. xvbitrevi.w

```asm
for i in range(8) :
XR[xd].W[i] = Bit_Reverse(XR[xj].W[i], ui5)
```

## 463. xvbitrevi.d

```asm
for i in range(4) :
XR[xd].D[i] = Bit_Reverse(XR[xj].D[i], ui6)
```

## 464. xvfrstp.b

```asm
idx0 = 0
for i in range(16):
if XR[xj].B[i]<0: break
idx0 = idx0 + 1
m0 = XR[xk].B[0]%16
XR[xd].B[m0] = idx0
idx1 = 0
for i in range(16,32,1):
if XR[xj].B[i]<0: break
idx1 = idx1 + 1
m1 = XR[xk].B[16]%16
XR[xd].B[m1] = idx1
```

## 465. xvfrstp.h

```asm
idx0 = 0
for i in range(8):
if XR[xj].H[i]<0: break
idx0 = idx0 + 1
m0 = XR[xk].H[0]%8
XR[xd].H[m0] = idx0
idx1 = 0
for i in range(8, 16, 1):
if XR[xj].H[i]<0: break
idx1 = idx1 + 1
m1 = XR[xk].H[8]%8
XR[xd].H[m1] = idx1
```

## 466. xvfrstpi.b

```asm
m = ui5%16
idx0 = 0
for i in range(16):
if XR[xj].B[i]<0: break
idx0 = idx0 + 1
XR[xd].B[m] = idx0
idx1 = 0
for i in range(16,32,1):
if XR[xj].B[i]<0: break
idx1 = idx1 + 1
XR[xd].B[m] = idx1
```

## 467. xvfrstpi.h

```asm
m = ui5%8
idx0 = 0
for i in range(8):
if XR[xj].H[i]<0: break
idx0 = idx0 + 1
XR[xd].H[m] = idx0
idx1 = 0
for i in range(8, 16, 1):
if XR[xj].H[i]<0: break
idx1 = idx1 + 1
XR[xd].H[m] = idx1
```

## 468. xvfadd.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_addition(XR[xj].S[i], XR[xk].S[i])
```

## 469. xvfadd.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_addition(XR[xj].D[i], XR[xk].D[i])
```

## 470. xvfsub.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_subtraction(XR[xj].S[i], XR[xk].S[i])
```

## 471. xvfsub.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_subtraction(XR[xj].D[i], XR[xk].D[i])
```

## 472. xvfmul.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_multiplication(XR[xj].S[i], XR[xk].S[i])
```

## 473. xvfmul.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_multiplication(XR[xj].D[i], XR[xk].D[i])
```

## 474. xvfdiv.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_division(XR[xj].S[i], XR[xk].S[i])
```

## 475. xvfdiv.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_division(XR[xj].D[i], XR[xk].D[i])
```

## 476. xvfmadd.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_fusedMultiplyAdd(XR[xj].S[i], XR[xk].S[i], XR[xa].S
[i])
```

## 477. xvfmadd.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_fusedMultiplyAdd(XR[xj].D[i], XR[xk].D[i], XR[xa].D
[i])
```

## 478. xvfmsub.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_fusedMultiplyAdd(XR[xj].S[i], XR[xk].S[i], -XR[xa].S
[i])
```

## 479. xvfmsub.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_fusedMultiplyAdd(XR[xj].D[i], XR[xk].D[i], -XR[xa].D
[i])
```

## 480. xvfnmadd.s

```asm
for i in range(8) :
XR[xd].S[i] = -FP32_fusedMultiplyAdd(XR[xj].S[i], XR[xk].S[i], XR[xa].S
[i])
```

## 481. xvfnmadd.d

```asm
for i in range(4) :
XR[xd].D[i] = -FP64_fusedMultiplyAdd(XR[xj].D[i], XR[xk].D[i], XR[xa].D
[i])
```

## 482. xvfnmsub.s

```asm
for i in range(8) :
XR[xd].S[i] = -FP32_fusedMultiplyAdd(XR[xj].S[i], XR[xk].S[i], -XR[xa].
S[i])
```

## 483. xvfnmsub.d

```asm
for i in range(4) :
XR[xd].D[i] = -FP64_fusedMultiplyAdd(XR[xj].D[i], XR[xk].D[i], -XR[xa].
D[i])
```

## 484. xvfmax.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_maxNum(XR[xj].S[i], XR[xk].S[i])
```

## 485. xvfmax.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_maxNum(XR[xj].D[i], XR[xk].D[i])
```

## 486. xvfmin.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_minNum(XR[xj].S[i], XR[xk].S[i])
```

## 487. xvfmin.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_minNum(XR[xj].D[i], XR[xk].D[i])
```

## 488. xvfmaxa.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_maxNumMag(XR[xj].S[i], XR[xk].S[i])
```

## 489. xvfmaxa.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_maxNumMag(XR[xj].D[i], XR[xk].D[i])
```

## 490. xvfmina.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_minNumMag(XR[xj].S[i], XR[xk].S[i])
```

## 491. xvfmina.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_minNumMag(XR[xj].D[i], XR[xk].D[i])
```

## 492. xvflogb.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_logB(XR[xj].S[i])
```

## 493. xvflogb.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_logB(XR[xj].D[i])
```

## 494. xvfclass.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_class(XR[xj].S[i])
```

## 495. xvfclass.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_class(XR[xj].D[i])
```

## 496. xvfsqrt.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_squareRoot(XR[xj].S[i])
```

## 497. xvfsqrt.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_squareRoot(XR[xj].D[i])
```

## 498. xvfrecip.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_division(1.0, XR[xj].S[i])
```

## 499. xvfrecip.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_division(1.0, XR[xj].D[i])
```

## 500. xvfrsqrt.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_division(1.0, FP_squareRoot(XR[xj].S[i]))
```

## 501. xvfrsqrt.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_division(1.0, FP_squareRoot(XR[xj].D[i]))
```

## 502. xvfrecipe.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_reciprocal_estimate(XR[xj].S[i])
```

## 503. xvfrecipe.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_reciprocal_estimate(XR[xj].D[i])
```

## 504. xvfrsqrte.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_reciprocal_squareroot_estimate(XR[xj].S[i])
```

## 505. xvfrsqrte.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_reciprocal_squareroot_estimate(XR[xj].D[i])
```

## 506. xvfcvtl.s.h

```asm
for i in range(4) :
XR[xd].S[i+0] = FP32_convertFormat(XR[xj].H[i], FP16)
XR[xd].S[i+4] = FP32_convertFormat(XR[xj].H[i+8], FP16)
```

## 507. xvfcvtl.d.s

```asm
for i in range(2) :
XR[xd].D[i+0] = FP64_convertFormat(XR[xj].S[i], FP32)
XR[xd].D[i+2] = FP64_convertFormat(XR[xj].S[i+4], FP32)
```

## 508. xvfcvth.s.h

```asm
for i in range(4) :
XR[xd].S[i+0] = FP32_convertFormat(XR[xj].H[i+4], FP16)
XR[xd].S[i+4] = FP32_convertFormat(XR[xj].H[i+12], FP16)
```

## 509. xvfcvth.d.s

```asm
for i in range(2) :
XR[xd].D[i+0] = FP64_convertFormat(XR[xj].S[i+2], FP32)
XR[xd].D[i+2] = FP64_convertFormat(XR[xj].S[i+6], FP32)
```

## 510. xvfcvt.h.s

```asm
for i in range(4) :
XR[xd].H[i+4] = FP16_convertFormat(XR[xj].S[i], FP32)
XR[xd].H[i+0] = FP16_convertFormat(XR[xk].S[i], FP32)
XR[xd].H[i+12] = FP16_convertFormat(XR[xj].S[i+4], FP32)
XR[xd].H[i+8] = FP16_convertFormat(XR[xk].S[i+4], FP32)
```

## 511. xvfcvt.s.d

```asm
for i in range(2) :
XR[xd].S[i+2] = FP32_convertFormat(XR[xj].D[i], FP64)
XR[xd].S[i+0] = FP32_convertFormat(XR[xk].D[i], FP64)
XR[xd].S[i+6] = FP32_convertFormat(XR[xj].D[i+2], FP64)
XR[xd].S[i+4] = FP32_convertFormat(XR[xk].D[i+2], FP64)
```

## 512. xvfrintrne.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_roundToInteger(XR[xj].S[i], FCSR.Enables.I, 0)
```

## 513. xvfrintrne.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_roundToInteger(XR[xj].D[i], FCSR.Enables.I, 0)
```

## 514. xvfrintrz.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_roundToInteger(XR[xj].S[i], FCSR.Enables.I, 1)
```

## 515. xvfrintrz.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_roundToInteger(XR[xj].D[i], FCSR.Enables.I, 1)
```

## 516. xvfrintrp.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_roundToInteger(XR[xj].S[i], FCSR.Enables.I, 2)
```

## 517. xvfrintrp.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_roundToInteger(XR[xj].D[i], FCSR.Enables.I, 2)
```

## 518. xvfrintrm.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_roundToInteger(XR[xj].S[i], FCSR.Enables.I, 3)
```

## 519. xvfrintrm.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_roundToInteger(XR[xj].D[i], FCSR.Enables.I, 3)
```

## 520. xvfrint.s

```asm
for i in range(8) :
XR[xd].S[i] = FP32_roundToInteger(XR[xj].S[i], FCSR.Enables.I, FCSR.RM)
```

## 521. xvfrint.d

```asm
for i in range(4) :
XR[xd].D[i] = FP64_roundToInteger(XR[xj].D[i], FCSR.Enables.I, FCSR.RM)
```

## 522. xvftintrne.w.s

```asm
for i in range(8) :
XR[xd].W[i] = Sint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 0)
```

## 523. xvftintrne.l.d

```asm
for i in range(4) :
XR[xd].L[i] = Sint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 0)
```

## 524. xvftintrz.w.s

```asm
for i in range(8) :
XR[xd].W[i] = Sint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 1)
```

## 525. xvftintrz.l.d

```asm
for i in range(4) :
XR[xd].L[i] = Sint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 1)
```

## 526. xvftintrp.w.s

```asm
for i in range(8) :
XR[xd].W[i] = Sint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 2)
```

## 527. xvftintrp.l.d

```asm
for i in range(4) :
XR[xd].L[i] = Sint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 2)
```

## 528. xvftintrm.w.s

```asm
for i in range(8) :
XR[xd].W[i] = Sint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 3)
```

## 529. xvftintrm.l.d

```asm
for i in range(4) :
XR[xd].L[i] = Sint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 3)
```

## 530. xvftint.w.s

```asm
for i in range(8) :
XR[xd].W[i] = Sint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, FCSR.RM)
```

## 531. xvftint.l.d

```asm
for i in range(4) :
XR[xd].L[i] = Sint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, FCSR.RM)
```

## 532. xvftintrz.wu.s

```asm
for i in range(8) :
XR[xd].W[i] = Uint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 1)
```

## 533. xvftintrz.lu.d

```asm
for i in range(4) :
XR[xd].L[i] = Uint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 1)
```

## 534. xvftint.wu.s

```asm
for i in range(8) :
XR[xd].W[i] = Uint32_convertToInteger(XR[xj].S[i], FCSR.Enables.I, FCSR.RM)
```

## 535. xvftint.lu.d

```asm
for i in range(4) :
XR[xd].L[i] = Uint64_convertToInteger(XR[xj].D[i], FCSR.Enables.I, FCSR.RM)
```

## 536. xvftintrne.w.d

```asm
for i in range(2) :
XR[xd].W[i+2] = Sint32_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 0)
XR[xd].W[i+0] = Sint32_convertToInteger(XR[xk].D[i], FCSR.Enables.I, 0)
XR[xd].W[i+6] = Sint32_convertToInteger(XR[xj].D[i+2], FCSR.Enables.I, 0)
XR[xd].W[i+4] = Sint32_convertToInteger(XR[xk].D[i+2], FCSR.Enables.I, 0)
```

## 537. xvftintrz.w.d

```asm
for i in range(2) :
XR[xd].W[i+2] = Sint32_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 1)
XR[xd].W[i+0] = Sint32_convertToInteger(XR[xk].D[i], FCSR.Enables.I, 1)
XR[xd].W[i+6] = Sint32_convertToInteger(XR[xj].D[i+2], FCSR.Enables.I, 1)
XR[xd].W[i+4] = Sint32_convertToInteger(XR[xk].D[i+2], FCSR.Enables.I, 1)
```

## 538. xvftintrp.w.d

```asm
for i in range(2) :
XR[xd].W[i+2] = Sint32_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 2)
XR[xd].W[i+0] = Sint32_convertToInteger(XR[xk].D[i], FCSR.Enables.I, 2)
XR[xd].W[i+6] = Sint32_convertToInteger(XR[xj].D[i+2], FCSR.Enables.I, 2)
XR[xd].W[i+4] = Sint32_convertToInteger(XR[xk].D[i+2], FCSR.Enables.I, 2)
```

## 539. xvftintrm.w.d

```asm
for i in range(3) :
XR[xd].W[i+2] = Sint32_convertToInteger(XR[xj].D[i], FCSR.Enables.I, 3)
XR[xd].W[i+0] = Sint32_convertToInteger(XR[xk].D[i], FCSR.Enables.I, 3)
XR[xd].W[i+6] = Sint32_convertToInteger(XR[xj].D[i+2], FCSR.Enables.I, 3)
XR[xd].W[i+4] = Sint32_convertToInteger(XR[xk].D[i+2], FCSR.Enables.I, 3)
```

## 540. xvftint.w.d

```asm
for i in range(2) :
XR[xd].W[i+2] = Sint32_convertToInteger(XR[xj].D[i], FCSR.Enables.I, FC
SR.RM)
XR[xd].W[i+0] = Sint32_convertToInteger(XR[xk].D[i], FCSR.Enables.I, FC
SR.RM)
XR[xd].W[i+6] = Sint32_convertToInteger(XR[xj].D[i+2], FCSR.Enables.I, FCSR.RM)
XR[xd].W[i+4] = Sint32_convertToInteger(XR[xk].D[i+2], FCSR.Enables.I, FCSR.RM)
```

## 541. xvftintrnel.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 0)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+4], FCSR.Enables.I, 0)
```

## 542. xvftintrneh.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i+2], FCSR.Enables.I, 0)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+6], FCSR.Enables.I, 0)
```

## 543. xvftintrzl.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 1)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+4], FCSR.Enables.I, 1)
```

## 544. xvftintrzh.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i+2], FCSR.Enables.I, 1)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+6], FCSR.Enables.I, 1)
```

## 545. xvftintrpl.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 2)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+4], FCSR.Enables.I, 2)
```

## 546. xvftintrph.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i+2], FCSR.Enables.I, 2)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+6], FCSR.Enables.I, 2)
```

## 547. xvftintrml.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i], FCSR.Enables.I, 3)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+4], FCSR.Enables.I, 3)
```

## 548. xvftintrmh.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i+2], FCSR.Enables.I, 3)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+6], FCSR.Enables.I, 3)
```

## 549. xvftintl.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i], FCSR.Enables.I, FCSR.RM)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+4], FCSR.Enables.I, FCSR.RM)
```

## 550. xvftinth.l.s

```asm
for i in range(2) :
XR[xd].D[i+0] = Sint64_convertToInteger(XR[xj].S[i+2], FCSR.Enables.I, FCSR.RM)
XR[xd].D[i+2] = Sint64_convertToInteger(XR[xj].S[i+6], FCSR.Enables.I, FCSR.RM)
```

## 551. xvffint.s.w

```asm
for i in range(8) :
XR[xd].S[i] = FP32_convertFromInt(XR[xj].W[i], SINT32)
```

## 552. xvffint.d.l

```asm
for i in range(4) :
XR[xd].D[i] = FP64_convertFromInt(XR[xj].L[i], SINT64)
```

## 553. xvffint.s.wu

```asm
for i in range(8) :
XR[xd].S[i] = FP32_convertFromInt(XR[xj].W[i], UINT32)
```

## 554. xvffint.d.lu

```asm
for i in range(4) :
XR[xd].D[i] = FP64_convertFromInt(XR[xj].L[i], UINT64)
```

## 555. xvffint.s.l

```asm
for i in range(2) :
XR[xd].S[i+2] = FP32_convertFromInt(XR[xj].L[i], UINT64)
XR[xd].S[i+0] = FP32_convertFromInt(XR[xk].L[i], UINT64)
XR[xd].S[i+6] = FP32_convertFromInt(XR[xj].L[i+2], UINT64)
XR[xd].S[i+4] = FP32_convertFromInt(XR[xk].L[i+2], UINT64)
```

## 556. xvffintl.d.w

```asm
for i in range(2) :
XR[xd].D[i+0] = FP64_convertFromInt(XR[xj].W[i], SINT32)
XR[xd].D[i+2] = FP64_convertFromInt(XR[xj].W[i+4], SINT32)
```

## 557. xvffinth.d.w

```asm
for i in range(2) :
XR[xd].D[i+0] = FP64_convertFromInt(XR[xj].W[i+2], SINT32)
XR[xd].D[i+2] = FP64_convertFromInt(XR[xj].W[i+6], SINT32)
```

## 558. xvseq.b

```asm
for i in range(32) :
XR[xd].B[i] = (XR[xj].B[i]==XR[xk].B[i]) ? {8{1'b1}} : 0
```

## 559. xvseq.h

```asm
for i in range(16) :
XR[xd].H[i] = (XR[xj].H[i]==XR[xk].H[i]) ? {16{1'b1}} : 0
```

## 560. xvseq.w

```asm
for i in range(8) :
XR[xd].W[i] = (XR[xj].W[i]==XR[xk].W[i]) ? {32{1'b1}} : 0
```

## 561. xvseq.d

```asm
for i in range(4) :
XR[xd].D[i] = (XR[xj].D[i]==XR[xk].D[i]) ? {64{1'b1}} : 0
```

## 562. xvseqi.b

```asm
for i in range(32) :
XR[xd].B[i] = (XR[xj].B[i]==SignExtend(si5, 8)) ? {8{1'b1}} : 0
```

## 563. xvseqi.h

```asm
for i in range(16) :
XR[xd].H[i] = (XR[xj].H[i]==SignExtend(si5, 16)) ? {16{1'b1}} : 0
```

## 564. xvseqi.w

```asm
for i in range(8) :
XR[xd].W[i] = (XR[xj].W[i]==SignExtend(si5, 32)) ? {32{1'b1}} : 0
```

## 565. xvseqi.d

```asm
for i in range(4) :
XR[xd].D[i] = (XR[xj].D[i]==SignExtend(si5, 64)) ? {64{1'b1}} : 0
```

## 566. xvsle.b

```asm
for i in range(32) :
XR[xd].B[i] = (signed(XR[xj].B[i])<=signed(XR[xk].B[i])) ? {8{1'b1}} : 0
```

## 567. xvsle.h

```asm
for i in range(16) :
XR[xd].H[i] = (signed(XR[xj].H[i])<=signed(XR[xk].H[i])) ? {16{1'b1}} : 0
```

## 568. xvsle.w

```asm
for i in range(8) :
XR[xd].W[i] = (signed(XR[xj].W[i])<=signed(XR[xk].W[i])) ? {32{1'b1}} : 0
```

## 569. xvsle.d

```asm
for i in range(4) :
XR[xd].D[i] = (signed(XR[xj].D[i])<=signed(XR[xk].D[i])) ? {64{1'b1}} : 0
```

## 570. xvslei.b

```asm
for i in range(32) :
XR[xd].B[i] = (signed(XR[xj].B[i])<=SignExtend(si5, 8)) ? {8{1'b1}} : 0
```

## 571. xvslei.h

```asm
for i in range(16) :
XR[xd].H[i] = (signed(XR[xj].H[i])<=SignExtend(si5, 16)) ? {16{1'b1}} : 0
```

## 572. xvslei.w

```asm
for i in range(8) :
XR[xd].W[i] = (signed(XR[xj].W[i])<=SignExtend(si5, 32)) ? {32{1'b1}} : 0
```

## 573. xvslei.d

```asm
for i in range(4) :
XR[xd].D[i] = (signed(XR[xj].D[i])<=SignExtend(si5, 64)) ? {64{1'b1}} : 0
```

## 574. xvsle.bu

```asm
for i in range(32) :
XR[xd].B[i] = (unsigned(XR[xj].B[i])<=unsigned(XR[xk].B[i])) ? {8{1'b1}} : 0
```

## 575. xvsle.hu

```asm
for i in range(16) :
XR[xd].H[i] = (unsigned(XR[xj].H[i])<=unsigned(XR[xk].H[i])) ? {16{1'b1}} : 0
```

## 576. xvsle.wu

```asm
for i in range(8) :
XR[xd].W[i] = (unsigned(XR[xj].W[i])<=unsigned(XR[xk].W[i])) ? {32{1'b1}} : 0
```

## 577. xvsle.du

```asm
for i in range(4) :
XR[xd].D[i] = (unsigned(XR[xj].D[i])<=unsigned(XR[xk].D[i])) ? {64{1'b1}} : 0
```

## 578. xvslei.bu

```asm
for i in range(32) :
XR[xd].B[i] = (unsigned(XR[xj].B[i])<=ZeroExtend(ui5, 8)) ? {8{1'b1}} : 0
```

## 579. xvslei.hu

```asm
for i in range(16) :
XR[xd].H[i] = (unsigned(XR[xj].H[i])<=ZeroExtend(ui5, 16)) ? {16{1'b1}} : 0
```

## 580. xvslei.wu

```asm
for i in range(8) :
XR[xd].W[i] = (unsigned(XR[xj].W[i])<=ZeroExtend(ui5, 32)) ? {32{1'b1}} : 0
```

## 581. xvslei.du

```asm
for i in range(4) :
XR[xd].D[i] = (unsigned(XR[xj].D[i])<=ZeroExtend(ui5, 64)) ? {64{1'b1}} : 0
```

## 582. xvslt.b

```asm
for i in range(32) :
XR[xd].B[i] = (signed(XR[xj].B[i])<signed(XR[xk].B[i])) ? {8{1'b1}} : 0
```

## 583. xvslt.h

```asm
for i in range(16) :
XR[xd].H[i] = (signed(XR[xj].H[i])<signed(XR[xk].H[i])) ? {16{1'b1}} : 0
```

## 584. xvslt.w

```asm
for i in range(8) :
XR[xd].W[i] = (signed(XR[xj].W[i])<signed(XR[xk].W[i])) ? {32{1'b1}} : 0
```

## 585. xvslt.d

```asm
for i in range(4) :
XR[xd].D[i] = (signed(XR[xj].D[i])<signed(XR[xk].D[i])) ? {64{1'b1}} : 0
```

## 586. xvslti.b

```asm
for i in range(32) :
XR[xd].B[i] = (signed(XR[xj].B[i])<SignExtend(si5, 8)) ? {8{1'b1}} : 0
```

## 587. xvslti.h

```asm
for i in range(16) :
XR[xd].H[i] = (signed(XR[xj].H[i])<SignExtend(si5, 16)) ? {16{1'b1}} : 0
```

## 588. xvslti.w

```asm
for i in range(8) :
XR[xd].W[i] = (signed(XR[xj].W[i])<SignExtend(si5, 32)) ? {32{1'b1}} : 0
```

## 589. xvslti.d

```asm
for i in range(4) :
XR[xd].D[i] = (signed(XR[xj].D[i])<SignExtend(si5, 64)) ? {64{1'b1}} : 0
```

## 590. xvslt.bu

```asm
for i in range(32) :
XR[xd].B[i] = (unsigned(XR[xj].B[i])<unsigned(XR[xk].B[i])) ? {8{1'b1}} : 0
```

## 591. xvslt.hu

```asm
for i in range(16) :
XR[xd].H[i] = (unsigned(XR[xj].H[i])<unsigned(XR[xk].H[i])) ? {16{1'b1}} : 0
```

## 592. xvslt.wu

```asm
for i in range(8) :
XR[xd].W[i] = (unsigned(XR[xj].W[i])<unsigned(XR[xk].W[i])) ? {32{1'b1}} : 0
```

## 593. xvslt.du

```asm
for i in range(4) :
XR[xd].D[i] = (unsigned(XR[xj].D[i])<unsigned(XR[xk].D[i])) ? {64{1'b1}} : 0
```

## 594. xvslti.bu

```asm
for i in range(32) :
XR[xd].B[i] = (unsigned(XR[xj].B[i])<ZeroExtend(ui5, 8)) ? {8{1'b1}} : 0
```

## 595. xvslti.hu

```asm
for i in range(16) :
XR[xd].H[i] = (unsigned(XR[xj].H[i])<ZeroExtend(ui5, 16)) ? {16{1'b1}} : 0
```

## 596. xvslti.wu

```asm
for i in range(8) :
XR[xd].W[i] = (unsigned(XR[xj].W[i])<ZeroExtend(ui5, 32)) ? {32{1'b1}} : 0
```

## 597. xvslti.du

```asm
for i in range(4) :
XR[xd].D[i] = (unsigned(XR[xj].D[i])<ZeroExtend(ui5, 64)) ? {64{1'b1}} : 0
```

## 598. xvfcmp.cond.s

```asm
for i in range(8) :
XR[xd].S[i] = Floating_Point_Compare(XR[xj].S[i], XR[xk].S[i], Single, cond) ? {32{1'b1}} : 0
```

## 599. xvfcmp.cond.d

```asm
for i in range(4) :
XR[xd].D[i] = Floating_Point_Compare(XR[xj].D[i], XR[xk].D[i], Double, cond) ? {64{1'b1}} : 0
```

## 600. xvbitsel.v

```asm
for i in range(256):
XR[xd].V[i] = XR[xa].V[i] ? XR[xk].V[i] : XR[xj].V[i]
```

## 601. xvbitseli.b

```asm
for i in range(32) :
XR[xd].B[i] = (~XR[xd].B[i] & XR[xj].B[i]) | (XR[xd].B[i] & ui8)
```

## 602. xvseteqz.v

```asm
CFR[cd] = (XR[xj]==0)
```

## 603. xvsetnez.v

```asm
CFR[cd] = (XR[xj]!=0)
```

## 604. xvsetanyeqz.b

```asm
bool_res = 0
for i in range(32) :
bool_res |= (XR[xj].B[i]==0)
CFR[cd] = bool_res
```

## 605. xvsetanyeqz.h

```asm
bool_res = 0
for i in range(16) :
bool_res |= (XR[xj].H[i]==0)
CFR[cd] = bool_res
```

## 606. xvsetanyeqz.w

```asm
bool_res = 0
for i in range(8) :
bool_res |= (XR[xj].W[i]==0)
CFR[cd] = bool_res
```

## 607. xvsetanyeqz.d

```asm
bool_res = 0
for i in range(4) :
bool_res |= (XR[xj].D[i]==0)
CFR[cd] = bool_res
```

## 608. xvsetallnez.b

```asm
bool_res = 1
for i in range(32) :
bool_res &= (XR[xj].B[i]!=0)
CFR[cd] = bool_res
```

## 609. xvsetallnez.h

```asm
bool_res = 1
for i in range(16) :
bool_res &= (XR[xj].H[i]!=0)
CFR[cd] = bool_res
```

## 610. xvsetallnez.w

```asm
bool_res = 1
for i in range(8) :
bool_res &= (XR[xj].W[i]!=0)
CFR[cd] = bool_res
```

## 611. xvsetallnez.d

```asm
bool_res = 1
for i in range(4) :
bool_res &= (XR[xj].D[i]!=0)
CFR[cd] = bool_res
```

## 612. xvinsgr2vr.w

```asm
XR[xd].W[ui3] = GR[rj][31:0]
```

## 613. xvinsgr2vr.d

```asm
XR[xd].D[ui2] = GR[rj][63:0]
```

## 614. xvpickve2gr.w

```asm
GR[rd] = SignExtend(XR[xj].W[ui3])
```

## 615. xvpickve2gr.d

```asm
GR[rd] = SignExtend(XR[xj].D[ui2])
```

## 616. xvpickve2gr.wu

```asm
GR[rd] = ZeroExtend(XR[xj].W[ui3])
```

## 617. xvpickve2gr.du

```asm
GR[rd] = ZeroExtend(XR[xj].D[ui2])
```

## 618. xvreplgr2vr.b

```asm
for i in range(32) :
XR[xd].B[i] = GR[rj][7:0]
```

## 619. xvreplgr2vr.h

```asm
for i in range(16) :
XR[xd].H[i] = GR[rj][15:0]
```

## 620. xvreplgr2vr.w

```asm
for i in range(8) :
XR[xd].W[i] = GR[rj][31:0]
```

## 621. xvreplgr2vr.d

```asm
for i in range(4) :
XR[xd].D[i] = GR[rj][63:0]
```

## 622. xvreplve.b

```asm
t = GR[rk]%16
for i in range(16) :
XR[xd].B[i] = XR[xj].B[t]
XR[xd].B[i+16] = XR[xj].B[t+16]
```

## 623. xvreplve.h

```asm
t = GR[rk]%8
for i in range(8) :
XR[xd].H[i] = XR[xj].H[t]
XR[xd].H[i+8] = XR[xj].H[t+8]
```

## 624. xvreplve.w

```asm
t = GR[rk]%4
for i in range(4) :
XR[xd].W[i] = XR[xj].W[t]
XR[xd].W[i+4] = XR[xj].W[t+4]
```

## 625. xvreplve.d

```asm
t = GR[rk]%2
for i in range(2) :
XR[xd].D[i] = XR[xj].D[t]
XR[xd].D[i+2] = XR[xj].D[t+2]
```

## 626. xvrepl128vei.b

```asm
t = ui4
for i in range(16) :
XR[xd].B[i] = XR[xj].B[t]
XR[xd].B[i+16] = XR[xj].B[t+16]
```

## 627. xvrepl128vei.h

```asm
t = ui3
for i in range(8) :
XR[xd].H[i] = XR[xj].H[t]
XR[xd].H[i+8] = XR[xj].H[t+8]
```

## 628. xvrepl128vei.w

```asm
t = ui2
for i in range(4) :
XR[xd].W[i] = XR[xj].W[t]
XR[xd].W[i+4] = XR[xj].W[t+4]
```

## 629. xvrepl128vei.d

```asm
t = ui1
for i in range(2) :
XR[xd].D[i] = XR[xj].D[t]
XR[xd].D[i+2] = XR[xj].D[t+2]
```

## 630. xvreplve0.b

```asm
for i in range(16) :
XR[vd].B[i] = XR[vj].B[0]
```

## 631. xvreplve0.h

```asm
for i in range(8) :
XR[vd].H[i] = XR[vj].H[0]
```

## 632. xvreplve0.w

```asm
for i in range(4) :
XR[vd].W[i] = XR[vj].W[0]
```

## 633. xvreplve0.d

```asm
for i in range(2) :
XR[vd].D[i] = XR[vj].D[0]
```

## 634. xvreplve0.q

```asm
for i in range(1) :
XR[vd].Q[i] = XR[vj].Q[0]
```

## 635. xvinsve0.w

```asm
XR[xd].W[ui3] = XR[xj].W[0]
```

## 636. xvinsve0.d

```asm
XR[xd].D[ui2] = XR[xj].D[0]
```

## 637. xvpickve.w

```asm
XR[xd].W[0] = XR[xj].W[ui3]
XR[xd][255:32] = 0
```

## 638. xvpickve.d

```asm
XR[xd].D[0] = XR[xj].D[ui2]
XR[xd][255:64] = 0
```

## 639. xvbsll.v

```asm
idx = ui5[3:0]*8
XR[xd][127:0] = {XR[xj][127-idx:0], {idx{8'b0}}}
XR[xd][255:128] = {XR[xj][255-idx:128], {idx{8'b0}}}
```

## 640. xvbsrl.v

```asm
idx = ui5[3:0]*8
XR[xd][127:0] = {{idx{8'b0}}, XR[xj][127:idx]}
XR[xd][255:128] = {{idx{8'b0}}, XR[xj][255:idx]}
```

## 641. xvpackev.b

```asm
for i in range(16) :
XR[xd].B[2i+1] = XR[xj].B[2i]
XR[xd].B[2i+0] = XR[xk].B[2i]
```

## 642. xvpackev.h

```asm
for i in range(8) :
XR[xd].H[2i+1] = XR[xj].H[2i]
XR[xd].H[2i+0] = XR[xk].H[2i]
```

## 643. xvpackev.w

```asm
for i in range(4) :
XR[xd].W[2i+1] = XR[xj].W[2i]
XR[xd].W[2i+0] = XR[xk].W[2i]
```

## 644. xvpackev.d

```asm
for i in range(2) :
XR[xd].D[2i+1] = XR[xj].D[2i]
XR[xd].D[2i+0] = XR[xk].D[2i]
```

## 645. xvpackod.b

```asm
for i in range(16) :
XR[xd].B[2i+1] = XR[xj].B[2i+1]
XR[xd].B[2i+0] = XR[xk].B[2i+1]
```

## 646. xvpackod.h

```asm
for i in range(8) :
XR[xd].H[2i+1] = XR[xj].H[2i+1]
XR[xd].H[2i+0] = XR[xk].H[2i+1]
```

## 647. xvpackod.w

```asm
for i in range(4) :
XR[xd].W[2i+1] = XR[xj].W[2i+1]
XR[xd].W[2i+0] = XR[xk].W[2i+1]
```

## 648. xvpackod.d

```asm
for i in range(2) :
XR[xd].D[2i+1] = XR[xj].D[2i+1]
XR[xd].D[2i+0] = XR[xk].D[2i+1]
```

## 649. xvpickev.b

```asm
for i in range(8) :
VR[vd].B[i+8] = VR[vj].B[2i]
VR[vd].B[i+0] = VR[vk].B[2i]
VR[vd].B[i+24] = VR[vj].B[2i+16]
VR[vd].B[i+16] = VR[vk].B[2i+16]
```

## 650. xvpickev.h

```asm
for i in range(4) :
VR[vd].H[i+4] = VR[vj].H[2i]
VR[vd].H[i+0] = VR[vk].H[2i]
VR[vd].H[i+12] = VR[vj].H[2i+8]
VR[vd].H[i+8] = VR[vk].H[2i+8]
```

## 651. xvpickev.w

```asm
for i in range(2) :
VR[vd].W[i+2] = VR[vj].W[2i]
VR[vd].W[i+0] = VR[vk].W[2i]
VR[vd].W[i+6] = VR[vj].W[2i+4]
VR[vd].W[i+4] = VR[vk].W[2i+4]
```

## 652. xvpickev.d

```asm
for i in range(1) :
VR[vd].D[i+1] = VR[vj].D[2i]
VR[vd].D[i+0] = VR[vk].D[2i]
VR[vd].D[i+3] = VR[vj].D[2i+2]
VR[vd].D[i+2] = VR[vk].D[2i+2]
```

## 653. xvpickod.b

```asm
for i in range(8) :
VR[vd].B[i+8] = VR[vj].B[2i+1]
VR[vd].B[i+0] = VR[vk].B[2i+1]
VR[vd].B[i+24] = VR[vj].B[2i+17]
VR[vd].B[i+16] = VR[vk].B[2i+17]
```

## 654. xvpickod.h

```asm
for i in range(4) :
VR[vd].H[i+4] = VR[vj].H[2i+1]
VR[vd].H[i+0] = VR[vk].H[2i+1]
VR[vd].H[i+12] = VR[vj].H[2i+9]
VR[vd].H[i+8] = VR[vk].H[2i+9]
```

## 655. xvpickod.w

```asm
for i in range(2) :
VR[vd].W[i+2] = VR[vj].W[2i+1]
VR[vd].W[i+0] = VR[vk].W[2i+1]
VR[vd].W[i+6] = VR[vj].W[2i+5]
VR[vd].W[i+4] = VR[vk].W[2i+5]
```

## 656. xvpickod.d

```asm
for i in range(1) :
VR[vd].D[i+1] = VR[vj].D[2i+1]
VR[vd].D[i+0] = VR[vk].D[2i+1]
VR[vd].D[i+3] = VR[vj].D[2i+3]
VR[vd].D[i+2] = VR[vk].D[2i+3]
```

## 657. xvilvl.b

```asm
for i in range(8) :
VR[vd].B[2i+1] = VR[vj].B[i]
VR[vd].B[2i+0] = VR[vk].B[i]
VR[vd].B[2i+17] = VR[vj].B[i+16]
VR[vd].B[2i+16] = VR[vk].B[i+16]
```

## 658. xvilvl.h

```asm
for i in range(4) :
VR[vd].H[2i+1] = VR[vj].H[i]
VR[vd].H[2i+0] = VR[vk].H[i]
VR[vd].H[2i+9] = VR[vj].H[i+8]
VR[vd].H[2i+8] = VR[vk].H[i+8]
```

## 659. xvilvl.w

```asm
for i in range(2) :
VR[vd].W[2i+1] = VR[vj].W[i]
VR[vd].W[2i+0] = VR[vk].W[i]
VR[vd].W[2i+5] = VR[vj].W[i+4]
VR[vd].W[2i+4] = VR[vk].W[i+4]
```

## 660. xvilvl.d

```asm
for i in range(1) :
VR[vd].D[2i+1] = VR[vj].D[i]
VR[vd].D[2i+0] = VR[vk].D[i]
VR[vd].D[2i+3] = VR[vj].D[i+2]
VR[vd].D[2i+2] = VR[vk].D[i+2]
```

## 661. xvilvh.b

```asm
for i in range(8) :
VR[vd].B[2i+1] = VR[vj].B[i+8]
VR[vd].B[2i+0] = VR[vk].B[i+8]
VR[vd].B[2i+17] = VR[vj].B[i+24]
VR[vd].B[2i+16] = VR[vk].B[i+24]
```

## 662. xvilvh.h

```asm
for i in range(4) :
VR[vd].H[2i+1] = VR[vj].H[i+4]
VR[vd].H[2i+0] = VR[vk].H[i+4]
VR[vd].H[2i+9] = VR[vj].H[i+12]
VR[vd].H[2i+8] = VR[vk].H[i+12]
```

## 663. xvilvh.w

```asm
for i in range(2) :
VR[vd].W[2i+1] = VR[vj].W[i+2]
VR[vd].W[2i+0] = VR[vk].W[i+2]
VR[vd].W[2i+5] = VR[vj].W[i+6]
VR[vd].W[2i+4] = VR[vk].W[i+6]
```

## 664. xvilvh.d

```asm
for i in range(1) :
VR[vd].D[2i+1] = VR[vj].D[i+1]
VR[vd].D[2i+0] = VR[vk].D[i+1]
VR[vd].D[2i+3] = VR[vj].D[i+3]
VR[vd].D[2i+2] = VR[vk].D[i+3]
```

## 665. xvshuf.b

```asm
vec0 = {XR[xj][127:0], XR[xk][127:0]}
vec1 = {XR[xj][255:128], XR[xk][255:128]}
for i in range(16) :
t0 = XR[xa].B[i][4:0]
XR[xd].B[i] = vec0.B[t0]
t1 = XR[xa].B[i+16][4:0]
XR[xd].B[i+16] = vec1.B[t1]
```

## 666. xvshuf.h

```asm
vec0 = {XR[xj][127:0], XR[xk][127:0]}
vec1 = {XR[xj][255:128], XR[xk][255:128]}
for i in range(8) :
t0 = XR[xd].H[i][3:0]
XR[xd].H[i] = vec0.H[t0]
t1 = XR[xd].H[i+8][3:0]
XR[xd].H[i+16] = vec1.H[t1]
```

## 667. xvshuf.w

```asm
vec0 = {XR[xj][127:0], XR[xk][127:0]}
vec1 = {XR[xj][255:128], XR[xk][255:128]}
for i in range(4) :
t0 = XR[xd].W[i][2:0]
XR[xd].W[i] = vec0.W[t0]
t1 = XR[xd].W[i+4][2:0]
XR[xd].W[i+16] = vec1.W[t1]
```

## 668. xvshuf.d

```asm
vec0 = {XR[xj][127:0], XR[xk][127:0]}
vec1 = {XR[xj][255:128], XR[xk][255:128]}
for i in range(2) :
t0 = XR[xd].D[i][1:0]
XR[xd].D[i] = vec0.D[t0]
t1 = XR[xd].D[i+2][1:0]
XR[xd].D[i+16] = vec1.D[t1]
```

## 669. xvperm.w

```asm
for i in range(8) :
t = XR[xk].W[i]%8
XR[xd].W[i] = XR[xj].W[t]
```

## 670. xvshuf4i.b

```asm
for i in range(4) :
vec0 = {XR[xj].B[4i+3], XR[xj].B[4i+2], XR[xj].B[4i+1], XR[xj].B[4i]}
XR[xd].B[4i+0] = vec0.B[ui8[1:0]]
XR[xd].B[4i+1] = vec0.B[ui8[3:2]]
XR[xd].B[4i+2] = vec0.B[ui8[5:4]]
XR[xd].B[4i+3] = vec0.B[ui8[7:6]]
vec1 = {XR[xj].B[4i+19], XR[xj].B[4i+18], XR[xj].B[4i+17], XR[xj].B[4i+16]}
XR[xd].B[4i+16] = vec1.B[ui8[1:0]]
XR[xd].B[4i+17] = vec1.B[ui8[3:2]]
XR[xd].B[4i+18] = vec1.B[ui8[5:4]]
XR[xd].B[4i+19] = vec1.B[ui8[7:6]]
```

## 671. xvshuf4i.h

```asm
for i in range(2) :
vec0 = {XR[xj].H[4i+3], XR[xj].H[4i+2], XR[xj].H[4i+1], XR[xj].H[4i]}
XR[xd].H[4i+0] = vec0.H[ui8[1:0]]
XR[xd].H[4i+1] = vec0.H[ui8[3:2]]
XR[xd].H[4i+2] = vec0.H[ui8[5:4]]
XR[xd].H[4i+3] = vec0.H[ui8[7:6]]
vec1 = {XR[xj].H[4i+11], XR[xj].H[4i+10], XR[xj].H[4i+9], XR[xj].H[4i+8]}
XR[xd].H[4i+8] = vec1.H[ui8[1:0]]
XR[xd].H[4i+9] = vec1.H[ui8[3:2]]
XR[xd].H[4i+10] = vec1.H[ui8[5:4]]
XR[xd].H[4i+11] = vec1.H[ui8[7:6]]
```

## 672. xvshuf4i.w

```asm
for i in range(1) :
vec0 = {XR[xj].W[4i+3], XR[xj].W[4i+2], XR[xj].W[4i+1], XR[xj].W[4i]}
XR[xd].W[4i+0] = vec0.W[ui8[1:0]]
XR[xd].W[4i+1] = vec0.W[ui8[3:2]]
XR[xd].W[4i+2] = vec0.W[ui8[5:4]]
XR[xd].W[4i+3] = vec0.W[ui8[7:6]]
vec1 = {XR[xj].W[4i+7], XR[xj].W[4i+6], XR[xj].W[4i+5], XR[xj].W[4i+4]}
XR[xd].W[4i+4] = vec1.W[ui8[1:0]]
XR[xd].W[4i+5] = vec1.W[ui8[3:2]]
XR[xd].W[4i+6] = vec1.W[ui8[5:4]]
XR[xd].W[4i+7] = vec1.W[ui8[7:6]]
```

## 673. xvshuf4i.d

```asm
vec0 = {XR[xj][127:0], XR[xd][127:0]}
XR[xd].D[0] = vec0.D[ui8[1:0]]
XR[xd].D[1] = vec0.D[ui8[3:2]]
vec1 = {XR[xj][255:128], XR[xd][255:128]}
XR[xd].D[2] = vec1.D[ui8[1:0]]
XR[xd].D[3] = vec1.D[ui8[3:2]]
```

## 674. xvpermi.w

```asm
XR[xd].W[0] = XR[xj].W[ui8[1:0]]
XR[xd].W[1] = XR[xj].W[ui8[3:2]]
XR[xd].W[2] = XR[xd].W[ui8[5:4]]
XR[xd].W[3] = XR[xd].W[ui8[7:6]]
XR[xd].W[4] = XR[xj].W[ui8[1:0]+4]
XR[xd].W[5] = XR[xj].W[ui8[3:2]+4]
XR[xd].W[6] = XR[xd].W[ui8[5:4]+4]
XR[xd].W[7] = XR[xd].W[ui8[7:6]+4]
```

## 675. xvpermi.d

```asm
XR[xd].D[0] = XR[xj].D[ui8[1:0]]
XR[xd].D[1] = XR[xj].D[ui8[3:2]]
XR[xd].D[2] = XR[xj].D[ui8[5:4]]
XR[xd].D[3] = XR[xj].D[ui8[7:6]]
```

## 676. xvpermi.q

```asm
vec = {XR[xd], XR[xj]}
XR[xd].Q[0] = vec.Q[ui8[1:0]]
XR[xd].Q[1] = vec.Q[ui8[5:4]]
```

## 677. xvextrins.b

```asm
XR[xd].B[ui8[7:4]] = XR[xj].B[ui8[3:0]]
XR[xd].B[ui8[7:4]+16] = XR[xj].B[ui8[3:0]+16]
```

## 678. xvextrins.h

```asm
XR[xd].H[ui8[6:4]] = XR[xj].H[ui8[2:0]]
XR[xd].H[ui8[6:4]+8] = XR[xj].H[ui8[2:0]+8]
```

## 679. xvextrins.w

```asm
XR[xd].W[ui8[5:4]] = XR[xj].W[ui8[1:0]]
XR[xd].W[ui8[5:4]+4] = XR[xj].W[ui8[1:0]+4]
```

## 680. xvextrins.d

```asm
XR[xd].D[ui8[4]] = XR[xj].D[ui8[0]]
XR[xd].D[ui8[4]+2] = XR[xj].D[ui8[0]+2]
```

## 681. xvld

```asm
addr = GR[rj] + SignExtend(si12)
for i in range(32):
    XR[vd].B[i] = MemB(addr + i)
```

## 682. xvst

```asm
addr = GR[rj] + SignExtend(si12)
for i in range(32):
    MemB(addr + i) = XR[vd].B[i]
```

## 683. xvldx

```asm
addr = GR[rj] + GR[rk]
for i in range(32):
    XR[vd].B[i] = MemB(addr + i)
```

## 684. xvstx

```asm
addr = GR[rj] + GR[rk]
for i in range(32):
    MemB(addr + i) = XR[vd].B[i]
```

## 685. xvldrepl.b

```asm
addr = GR[rj] + SignExtend(si12)
data = MemB(addr)
for i in range(32):
    XR[vd].B[i] = data
```

## 686. xvldrepl.h

```asm
addr = GR[rj] + SignExtend(si11 << 1)
data = MemH(addr)
for i in range(16):
    XR[vd].H[i] = data
```

## 687. xvldrepl.w

```asm
addr = GR[rj] + SignExtend(si10 << 2)
data = MemW(addr)
for i in range(8):
    XR[vd].W[i] = data
```

## 688. xvldrepl.d

```asm
addr = GR[rj] + SignExtend(si9 << 3)
data = MemD(addr)
for i in range(4):
    XR[vd].D[i] = data
```

## 689. xvstelm.b

```asm
addr = GR[rj] + SignExtend(si8)
MemB(addr) = XR[vd].B[si8]
```

## 690. xvstelm.h

```asm
addr = GR[rj] + SignExtend(si8 << 1)
MemH(addr) = XR[vd].H[si8]
```

## 691. xvstelm.w

```asm
addr = GR[rj] + SignExtend(si8 << 2)
MemW(addr) = XR[vd].W[si8]
```

## 692. xvstelm.d

```asm
addr = GR[rj] + SignExtend(si8 << 3)
MemD(addr) = XR[vd].D[si8]
```

# LASX 底层函数定义

## 1. 饱和算术运算

### SSat_Add_S - 有符号饱和加法
```
bits(N) SSat_Add_S(bits(N) x, bits(N) y):
    sum = signed(x) + signed(y)
    if sum > 2**(N-1)-1:
        return {1'b0, {N-1{1'b1}}}     # 正溢出 -> 最大正数
    elif sum < -2**(N-1):
        return {1'b1, {N-1{1'b0}}}     # 负溢出 -> 最小负数
    else:
        return sum[N-1:0]
```

### USat_Add_U - 无符号饱和加法
```
bits(N) USat_Add_U(bits(N) x, bits(N) y):
    sum = unsigned(x) + unsigned(y)
    if sum > 2**N-1:
        return {N{1'b1}}               # 溢出 -> 最大无符号数
    else:
        return sum[N-1:0]
```

### SSat_Sub_S - 有符号饱和减法
```
bits(N) SSat_Sub_S(bits(N) x, bits(N) y):
    diff = signed(x) - signed(y)
    if diff > 2**(N-1)-1:
        return {1'b0, {N-1{1'b1}}}
    elif diff < -2**(N-1):
        return {1'b1, {N-1{1'b0}}}
    else:
        return diff[N-1:0]
```

### USat_Sub_U - 无符号饱和减法
```
bits(N) USat_Sub_U(bits(N) x, bits(N) y):
    diff = unsigned(x) - unsigned(y)
    if diff < 0:
        return 0                       # 负结果 -> 0
    else:
        return diff[N-1:0]
```

---

## 2. 极值运算

### Max_S / Min_S - 有符号极值
```
bits(N) Max_S(bits(N) x, bits(N) y):
    return signed(x) > signed(y) ? x : y

bits(N) Min_S(bits(N) x, bits(N) y):
    return signed(x) < signed(y) ? x : y
```

### Max_U / Min_U - 无符号极值
```
bits(N) Max_U(bits(N) x, bits(N) y):
    return unsigned(x) > unsigned(y) ? x : y

bits(N) Min_U(bits(N) x, bits(N) y):
    return unsigned(x) < unsigned(y) ? x : y
```

---

## 3. 绝对值与差值

### Abs - 绝对值
```
bits(N) Abs(bits(N) x):
    return signed(x) < 0 ? -x : x
```

### Abs_Diff_S - 有符号绝对值差
```
bits(N) Abs_Diff_S(bits(N) x, bits(N) y):
    svalx = SignExtend(x, N+1)
    svaly = SignExtend(y, N+1)
    diff[N:0] = svalx - svaly
    result[N:0] = Abs(diff[N:0])
    return result[N-1:0]
```

### Abs_Diff_U - 无符号绝对值差
```
bits(N) Abs_Diff_U(bits(N) x, bits(N) y):
    uvalx = ZeroExtend(x, N+1)
    uvaly = ZeroExtend(y, N+1)
    diff[N:0] = uvalx - uvaly
    result[N:0] = Abs(diff[N:0])
    return result[N-1:0]
```

---

## 4. 基本移位操作

### SLL / SRL / SRA - 逻辑/算术移位
```
bits(N) SLL(bits(N) x, integer sa):           # 逻辑左移
    return sa == 0 ? x : {x[N-sa-1:0], {sa{1'b0}}}

bits(N) SRL(bits(N) x, integer sa):           # 逻辑右移
    return sa == 0 ? x : {{sa{1'b0}}, x[N-1:sa]}

bits(N) SRA(bits(N) x, integer sa):           # 算术右移(符号扩展)
    return sa == 0 ? x : {{sa{x[N-1]}}, x[N-1:sa]}
```

### ROTR / ROTL - 循环移位
```
bits(N) ROTR(bits(N) x, integer sa):          # 循环右移
    return sa == 0 ? x : {x[sa-1:0], x[N-1:sa]}

bits(N) ROTL(bits(N) x, integer sa):          # 循环左移
    return sa == 0 ? x : {x[N-sa-1:sa], x[N-1:N-sa]}
```

### SLLW_U / SLLW_S - 双宽度左移
```
bits(2N) SLLW_U(bits(N) x, integer sa):      # 零扩展后左移
    dx = ZeroExtend(x, 2N)
    return sa == 0 ? dx : {dx[2N-sa-1:0], {sa{1'b0}}}

bits(2N) SLLW_S(bits(N) x, integer sa):      # 符号扩展后左移
    dx = SignExtend(x, 2N)
    return sa == 0 ? dx : {dx[2N-sa-1:0], {sa{1'b0}}}
```

### SRLR / SRAR - 舍入移位（向右）
```
bits(N) SRLR(bits(N) x, integer sa):          # 舍入逻辑右移
    return sa == 0 ? x : {{sa{1'b0}}, x[N-1:sa]} + x[sa-1]

bits(N) SRAR(bits(N) x, integer sa):          # 舍入算术右移
    return sa == 0 ? x : {{sa{x[N-1]}}, x[N-1:sa]} + x[sa-1]
```

---

## 5. 双宽度移位操作（输入2N位，输出N位）

### SRLN / SRAN - 普通双宽移位
```
bits(N) SRLN(bits(2N) x, integer sa):         # 双宽逻辑右移
    shft_res = sa == 0 ? x : {{sa{1'b0}}, x[2N-1:sa]}
    return shft_res[N-1:0]

bits(N) SRAN(bits(2N) x, integer sa):         # 双宽算术右移
    shft_res = sa == 0 ? x : {{sa{x[2N-1]}}, x[2N-1:sa]}
    return shft_res[N-1:0]
```

### SRLRN / SRARN - 舍入双宽移位
```
bits(N) SRLRN(bits(2N) x, integer sa):        # 舍入双宽逻辑右移
    shft_res = sa == 0 ? x : {{sa{1'b0}}, x[2N-1:sa]} + x[sa-1]
    return shft_res[N-1:0]

bits(N) SRARN(bits(2N) x, integer sa):       # 舍入双宽算术右移
    shft_res = sa == 0 ? x : {{sa{x[2N-1]}}, x[2N-1:sa]} + x[sa-1]
    return shft_res[N-1:0]
```

### SSRLNU / SSRLNS - 饱和双宽逻辑右移
```
bits(N) SSRLNU(bits(2N) x, integer sa):       # 饱和无符号
    shft_res = sa == 0 ? x : {{sa{1'b0}}, x[2N-1:sa]}
    return shft_res > 2**N-1 ? {N{1'b1}} : shft_res[N-1:0]

bits(N) SSRLNS(bits(2N) x, integer sa):       # 饱和有符号
    shft_res = sa == 0 ? x : {{sa{1'b0}}, x[2N-1:sa]}
    if unsigned(shft_res) > 2**(N-1)-1:
        return {1'b0, {N-1{1'b1}}}
    else:
        return shft_res[N-1:0]
```

### SSRANU / SSRANS - 饱和双宽算术右移
```
bits(N) SSRANU(bits(2N) x, integer sa):       # 饱和无符号
    if sa == 0:
        shft_res = x
    elif signed(x) < 0:
        shft_res = 0
    else:
        shft_res = {{sa{x[2N-1]}}, x[2N-1:sa]}
    return shft_res > 2**N-1 ? {N{1'b1}} : shft_res[N-1:0]

bits(N) SSRANS(bits(2N) x, integer sa):       # 饱和有符号
    shft_res = sa == 0 ? x : {{sa{x[2N-1]}}, x[2N-1:sa]}
    if shft_res > 2**(N-1)-1:
        return {1'b0, {N-1{1'b1}}}
    elif shft_res < -2**(N-1):
        return {1'b1, {N-1{1'b0}}}
    else:
        return shft_res[N-1:0]
```

### SSRLRNU / SSRLRNS - 饱和舍入双宽逻辑右移
```
bits(N) SSRLRNU(bits(2N) x, integer sa):      # 饱和舍入无符号
    shft_res = sa == 0 ? x : {{sa{1'b0}}, x[2N-1:sa]} + x[sa-1]
    return shft_res > 2**N-1 ? {N{1'b1}} : shft_res[N-1:0]

bits(N) SSRLRNS(bits(2N) x, integer sa):     # 饱和舍入有符号
    shft_res = sa == 0 ? x : {{sa{1'b0}}, x[2N-1:sa]} + x[sa-1]
    if unsigned(shft_res) > 2**(N-1)-1:
        return {1'b0, {N-1{1'b1}}}
    else:
        return shft_res[N-1:0]
```

### SSRARNU / SSRARNS - 饱和舍入双宽算术右移
```
bits(N) SSRARNU(bits(2N) x, integer sa):     # 饱和舍入无符号
    if sa == 0:
        shft_res = x
    elif signed(x) < 0:
        shft_res = 0
    else:
        shft_res = {{sa{x[2N-1]}}, x[2N-1:sa]} + x[sa-1]
    return shft_res > 2**N-1 ? {N{1'b1}} : shft_res[N-1:0]

bits(N) SSRARNS(bits(2N) x, integer sa):     # 饱和舍入有符号
    shft_res = sa == 0 ? x : {{sa{x[2N-1]}}, x[2N-1:sa]} + x[sa-1]
    if shft_res > 2**(N-1)-1:
        return {1'b0, {N-1{1'b1}}}
    elif shft_res < -2**(N-1):
        return {1'b1, {N-1{1'b0}}}
    else:
        return shft_res[N-1:0]
```

---

## 6. 饱和函数

### Sat_S / Sat_U - 定点饱和
```
bits(N) Sat_S(bits(N) x, bits(M) y):          # 有符号饱和到y位
    if x > 2**y-1:
        return {{(N-y){1'b0}}, {y{1'b1}}}
    elif x < -2**y:
        return {{(N-y){1'b1}}, {y{1'b0}}}
    else:
        return x[N-1:0]

bits(N) Sat_U(bits(N) x, bits(M) y):         # 无符号饱和到y位
    if x > 2**(y+1)-1:
        return {{(N-y-1){1'b0}}, {(y+1){1'b1}}}
    else:
        return x[N-1:0]
```

---

## 7. 位计数操作

### CLO - 前导1计数（Count Leading Ones）
```
bits(N) CLO(bits(N) x):
    cnt = 0
    for i in range(N):
        if x[N-1-i] == 1'b0:
            return cnt
        cnt = cnt + 1
```

### CLZ - 前导0计数（Count Leading Zeros）
```
bits(N) CLZ(bits(N) x):
    cnt = 0
    for i in range(N):
        if x[N-1-i] == 1'b1:
            return cnt
        cnt = cnt + 1
```

### PCNT - 位计数（Population Count）
```
bits(N) PCNT(bits(N) x):
    cnt = 0
    for i in range(N):
        cnt = cnt + x[i]
    return cnt
```

---

## 8. 位操作

### Bit_Clear - 清除指定位
```
bits(N) Bit_Clear(bits(N) x, bit(M) y):
    for i in range(N):
        res_bits[i] = (i == y) ? 1'b0 : x[i]
    return res_bits
```

### Bit_Set - 置位指定位
```
bits(N) Bit_Set(bits(N) x, bit(M) y):
    for i in range(N):
        res_bits[i] = (i == y) ? 1'b1 : x[i]
    return res_bits
```

### Bit_Toggle - 取反指定位
```
bits(N) Bit_Toggle(bits(N) x, bit(M) y):
    for i in range(N):
        res_bits[i] = (i == y) ? ~x[i] : x[i]
    return res_bits
```

---

## 9. 符号转换

### Sign_Convert - 符号转换
```
bits(N) Sign_Convert(bits(N) x, bits(N) y):
    # x==0 返回0, x<0 返回-y, 否则返回y
    return x == 0 ? 0 : (x < 0 ? -y : y)
```

---

## 10. 浮点转换

### FP32_roundToInteger / FP64_roundToInteger
```
bits(32) FP32_roundToInteger(bits(N) x, bits(1) I_en, bits(2) rm):
    if I_en:
        return FP32_roundToIntegralExact(x)
    elif rm == 0: return FP32_roundToIntegerTiesToEven(x)
    elif rm == 1: return FP32_roundToIntegerTowardZero(x)
    elif rm == 2: return FP32_roundToIntegerTowardPositive(x)
    elif rm == 3: return FP32_roundToIntegerTowardNegative(x)

bits(64) FP64_roundToInteger(bits(N) x, bits(1) I_en, bits(2) rm):
    # 同上，仅返回64位结果
```

### 整数转换函数

```
# 有符号整数转换
bits(32) Sint32_convertToInteger(bits(N) x, bits(1) I_en, bits(2) rm):
    case {I_en, rm}:
        {1'b1, 2'd0}: return Sint32_convertToIntegerExactTiesToEven(x)
        {1'b1, 2'd1}: return Sint32_convertToIntegerExactTowardZero(x)
        {1'b1, 2'd2}: return Sint32_convertToIntegerExactTowardPositive(x)
        {1'b1, 2'd3}: return Sint32_convertToIntegerExactTowardNegative(x)
        {1'b0, 2'd0}: return Sint32_convertToIntegerTiesToEven(x)
        {1'b0, 2'd1}: return Sint32_convertToIntegerTowardZero(x)
        {1'b0, 2'd2}: return Sint32_convertToIntegerTowardPositive(x)
        {1'b0, 2'd3}: return Sint32_convertToIntegerTowardNegative(x)

bits(64) Sint64_convertToInteger(bits(N) x, bits(1) I_en, bits(2) rm):
    # 同上格式，仅返回64位结果

# 无符号整数转换
bits(32) Uint32_convertToInteger(bits(N) x, bits(1) I_en, bits(2) rm):
    # 同Sint32格式，函数名替换为Uint

bits(64) Uint64_convertToInteger(bits(N) x, bits(1) I_en, bits(2) rm):
    # 同上格式
```

---

## 11. 立即数设置

### Set_Value - 64位立即数解码
```
bits(64) Set_Value(bits(13) x):
    # 根据x[12]和x[11:10]/x[11:8]解码生成64位数据
    if x[12] == 1'b0:
        case x[11:10]:
            2'b00: data = {8{x[7:0]}}                           # 重复8字节
            2'b01: data = {4{SignExtend(x[9:0], 16)}}            # 符号扩展x4
            2'b10: data = {2{SignExtend(x[9:0], 32)}}          # 符号扩展x2
            2'b11: data = SignExtend(x[9:0], 64)                # 符号扩展x1
    else:
        case x[11:8]:
            4'b0000: data = {2{24'b0, x[7:0]}}                  # 填充0
            4'b0001: data = {2{16'b0, x[7:0], 8'b0}}           # 穿插0
            4'b0010: data = {2{8'b0, x[7:0], 16'b0}}
            4'b0011: data = {2{x[7:0], 24'b0}}
            4'b0100: data = {4{8'b0, x[7:0]}}
            4'b0101: data = {4{x[7:0], 8'b0}}
            4'b0110: data = {2{16'b0, x[7:0], 8'hFF}}           # 填充FF
            4'b0111: data = {2{8'b0, x[7:0], 16'hFFFF}}
            4'b1000: data = {8{x[7:0]}}                         # 重复8字节
            4'b1001: data = {8{x[7]}, 8{x[6]}, ..., 8{x[0]}}   # 位广播
            4'b1010: data = {2{x[7], ~x[6], {5{x[6]}}, x[5:0], 19'b0}}
            4'b1011: data = {32'b0, x[7], ~x[6], {5{x[6]}}, x[5:0], 19'b0}
            4'b1100: data = {x[7], ~x[6], {8{x[6]}}, x[5:0], 48'b0}
            default: Raise_Exception(INE)
    return data
```
