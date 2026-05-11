.text
.globl run_loop2
.type run_loop2, @function
run_loop2:
    .cfi_startproc
    xvld $xr2, $a2, 0
    xvld $xr0, $a0, 0
    xvld $xr1, $a1, 0
    xvfmadd.s $xr2, $xr0, $xr1, $xr2
    addi.d $a3, $a3, -1
    addi.d $a0, $a0, 32
    addi.d $a1, $a1, 32
    bnez $a3, -24
    xvst $xr2, $a2, 0
    jr $ra
    .cfi_endproc
.size run_loop2, .-run_loop2
