.set noreorder
.text


#Test des ADD
ADDI $sp,$sp,-32
ADDI $t0,$zero,123
SW $t0,4($sp) 
LW $t1,4($sp)
NOP
ADDI $t2,$zero,321
NOP
SW $t2,8($sp)
NOP
NOP
LW $t3,8($sp) 
