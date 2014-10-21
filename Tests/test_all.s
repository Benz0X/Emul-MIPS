.set noreorder
.text
JAL plop
ADD $t1, $zero,$t2
ADDI $t1, $zero,8
ADDIU $t1, $zero,10
plop: ADDU $t1, $zero,$t2
AND $t1, $zero,$t2
ANDI $t1, $zero,10
BEQ $t1, $t0, 20
BGEZ $t1, 12
BGTZ $t1, 32
BLEZ $t1, 40
NOP
BLT $t1, $t2, 200
BNE $t1, $t0, 48
BREAK
DIV $t1, $t2
J 1020
JAL 2040
JALR $t1, $t2
JR $t1
LB $t1, 20($zero)
LBU $t1, 20($zero)
LI $t1,0xAAAAAAAA
LUI $t1, 32
LW $t1, 20($zero)
MFHI $t1
MFLO $t1
MOVE $t1, $t2
MULT $t1, $t2
NEG $t1,$t2
NOP
OR $t1, $t3,$t2
ORI $t1, $zero,123
SB $t1, 20($zero)
SEB $t1, $t2
SLL $1, $1, 0 #l'instr' a l'air d'être 0x00000004
SLT $t1, $t2, $t3
SLTI $t1, $t2, 321
SLTIU $t1, $t2, 321
SLTU $t1, $t2, $t3
SRA  $t1, $t2, 2 #l'instr' a l'air d'etre 0x0000007
SRL  $t1, $t2, 2 #0x00000006
SUB $t1, $t2, $t3
SUBU $t1, $t2, $t3
SW $t1, 20($zero)
SYSCALL
XOR $t1, $t2, $t3
