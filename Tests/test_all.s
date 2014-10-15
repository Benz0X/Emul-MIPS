.set noreorder
.text
ADD $t1, $zero,$t2
ADDI $t1, $zero,8
ADDIU $t1, $zero,10
ADDU $t1, $zero,$t2
AND $t1, $zero,$t2
ANDI $t1, $zero,20
BEQ $t1, $t0, 20
BGEZ $t1, 20
BGTZ $t1, 20
BLEZ $t1, 20
BNE $t1, $t0, 20
