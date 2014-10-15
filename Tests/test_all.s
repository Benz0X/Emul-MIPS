.set noreorder
.text
ADD $t1, $zero,$t2
ADDI $t1, $zero,8
ADDIU $t1, $zero,10
ADDU $t1, $zero,$t2
AND $t1, $zero,$t2
ANDI $t1, $zero,10
BEQ $t1, $t0, 20
BGEZ $t1, 12
BGTZ $t1, 32
BLEZ $t1, 40
BNE $t1, $t0, 48
BREAK
DIV $t1, $t2
J 1020
JAL 2040
JALR $t1, $t2
JR $t1
LB $t1, 20($zero)
LBU $t1, 20($zero)
LUI $t1, 32
LW $t1, 20($zero)
MFHI $t1
MFLO $t1
MULT $t1, $t2
NOP
