.text
.globl run_loop1
.type run_loop1, @function
run_loop1:
    .cfi_startproc
    xvld $xr10, $a2, 0
    beq $a3, $zero, 32
    xvld $xr0, $a0, 0
    xvld $xr1, $a1, 0
    xvfmadd.s $xr10, $xr0, $xr1, $xr10
    addi.d $a3, $a3, -1
    addi.d $a0, $a0, 32
    addi.d $a1, $a1, 32
    b -28
    xvst $xr10, $a2, 0
    jr $ra
    .cfi_endproc
.size run_loop1, .-run_loop1
