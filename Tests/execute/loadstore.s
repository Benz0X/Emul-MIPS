.set noreorder
.text
ADDI $t1, $zero, 0x4000 

#Test des loads
LB $t2, 0($t1)
LBU $t3, 0($t1)
LW $t4, 0($t1)

LUI $t5, 12345

#Test des stores
SB $t5, 0($t1)
ADDi $t1, $zero, 0x4003
SW $t5, 0($t1)

.data 
chaine: .asciiz "Youpi"
