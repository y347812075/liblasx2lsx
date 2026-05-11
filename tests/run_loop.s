.text
    .globl run_loop
    .type run_loop, @function
run_loop:
    .cfi_startproc
    xvld $xr10, $a2, 0
    beq $t6, $zero, 32
    xvld $xr0, $a0, 0
    xvld $xr1, $a1, 0
    xvfmadd.s $xr10, $xr0, $xr1, $xr10
    addi.d $t6, $t6, -1
    addi.d $a0, $a0, 32
    addi.d $a1, $a1, 32
    b -28
    xvst $xr10, $a2, 0
    jr $ra
    .cfi_endproc
    .size run_loop, .-run_loop
