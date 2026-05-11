.text
.globl run_loop5
.type run_loop5, @function
run_loop5:
    .cfi_startproc
    # $a0=a, $a1=b, $a2=c, $a3=n, $a4=stride
    xvld $xr10, $a2, 0
    xvld $xr11, $a2, 16
    addi.d $a5, $zero, 16
loop:
    blez $a3, end
    xvld $xr0, $a0, 0
    xvldx $xr1, $a0, $a5
    xvld $xr2, $a1, 0
    xvldx $xr3, $a1, $a5
    add.d $a0, $a0, $a4
    add.d $a1, $a1, $a4
    addi.d $a3, $a3, -1
    xvfmadd.s $xr10, $xr0, $xr2, $xr10
    xvfmadd.s $xr11, $xr1, $xr2, $xr11
    xvfnmsub.s $xr10, $xr1, $xr3, $xr10
    xvfmadd.s $xr11, $xr0, $xr3, $xr11
    b loop
end:
    xvst $xr10, $a2, 0
    xvst $xr11, $a2, 16
    jr $ra
    .cfi_endproc
.size run_loop5, .-run_loop5
