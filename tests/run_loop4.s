.text
.globl run_loop4
.type run_loop4, @function
run_loop4:
    .cfi_startproc
    vld $vr10, $a2, 0
    vld $vr11, $a2, 16
loop:
    beq $a3, $zero, end
    vld $vr0, $a0, 0
    vld $vr1, $a1, 0
    vld $vr2, $a0, 16
    vld $vr3, $a1, 16
    vfmadd.s $vr10, $vr0, $vr1, $vr10
    vfmadd.s $vr11, $vr2, $vr3, $vr11
    addi.d $a3, $a3, -1
    addi.d $a0, $a0, 32
    addi.d $a1, $a1, 32
    b loop
end:
    vst $vr10, $a2, 0
    vst $vr11, $a2, 16
    jr $ra
    .cfi_endproc
.size run_loop4, .-run_loop4
