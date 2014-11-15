.set noreorder
.text


#Test des ADD
ADDI $t1, $zero, 0x1
ADDI $t2, $t1, 0x1 		
ADD $t3, $t1, $t2 		#Assert t1=1,t2=2,t3=3


#Test des subs
SUB $t4, $t3,$t1  #Assert t4=2
SUB $t5, $t4,$t3 #Assert t5=-1
