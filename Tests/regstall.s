

.set noreorder    # ne pas modifier l'ordre des instructions ne fait meme pas des optimisations de boucle (les NOP sont mis de manière explicite)

_start:
.global _start
addi $t1,$zero,8
addi $t1,$zero,8	#no stall needed
NOP
NOP
addi $t1,$zero,8
addi $t2,$t1,8		#stall needed
NOP
NOP
addi $t2,$t2,8		#no stall needed

KO:
ADDI $v0,$zero,10  	#appel explicite à exit
syscall

