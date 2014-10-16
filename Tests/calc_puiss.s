.set noreorder
.text

debut :
addi $a0,$zero,15 # on met 15 dans le 1er argument
addi $a1,$zero, 3 # on met 3 dans le 2e argument
jal puiss # on veut 15ˆ3
jal debut
move $v0,$t2 # on stocke le r´esultat dans $t2
J fin # on va `a la fin du programme
puiss: # proc´edure r´ecursive de calcul de puissance
bgtz $a0, else # si la puissance vaut z´ero
addi $v0,$zero,1 # on init la valeur de retour `a 1
B sortie # et on s’en va
else: # sinon
addi $a0,$a0, -1 # on d´ecr´emente la puissance
jal puiss # on relance avec la puissance moins n-1
mult $a1,$v0 # on calcul `a la puissance n
mflo $v0 # on stocke le r´esultat dans $v0
sortie:
jr $31 # on sort de la fonction
fin:


.data 
X: .byte 4
