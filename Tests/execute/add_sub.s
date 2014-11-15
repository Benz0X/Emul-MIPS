.set noreorder
.text


#Test des ADD
ADDI $a0,$0,-20
ADDI $t6,$0,10
ADDI $t9, $0, 10
ADDI $t1, $zero, 0x1
ADDI $t2, $t1, 0x1 		
ADD $t3, $t1, $t2 		#Assert t1=1,t2=2,t3=3
ADDI $t6,$t0,0x00FF #Assert t6=0
ADDI $t7,$t0,0xFFFF
ADDI $t9,$t8,0xFFFF

SUB $t6,$t8,$t6
SUB $t9, $t0, $a0

#Test des subs
SUB $t4, $t3,$t1  #Assert t4=2
SUB $t5, $t4,$t3 #Assert t5=-1
