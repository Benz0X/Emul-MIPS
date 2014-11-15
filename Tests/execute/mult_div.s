.set noreorder
.text


#Test de mult
MULT $t8, $t1
NOP
NOP
MFHI $a0
MFLO $a1
NOP
NOP
#div
DIV $t0, $t1
NOP
NOP
MFHI $a2
MFLO $a3
