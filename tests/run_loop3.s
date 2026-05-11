.text
.globl run_loop3
.type run_loop3, @function
run_loop3:
    .cfi_startproc
    xvld $xr2, $a1, 0

    xvld $xr0, $a0, 0
    xvfmadd.s $xr2, $xr0, $xr0, $xr2
    addi.d $a2, $a2, -1
    addi.d $a0, $a0, 32
    bnez $a2, -16
    xvst $xr2, $a1, 0
    jr $ra
    .cfi_endproc
.size run_loop3, .-run_loop3
