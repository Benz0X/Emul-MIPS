.set noreorder
.text


#Test des ADD
ADDI $t1, $zero, 0xFFFF
ADDI $t1, $t1, 0xFFFF 		#Test overFlow
ADDI $t2, $t1, -0xF 		#Test signed

ADD $t1, $t2, $t3
ADD $t1, $t2, $t2			#Test overFlow



#Test des subs


#Test de mult et div
