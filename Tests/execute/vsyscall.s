.set noreorder
.text

li $a0, 19
li $v0, 1	#printf 19
syscall

li $a0,  0x4000
li $v0, 4
syscall		#printf "youpi"

li $v0, 5
syscall		#scanf $v0 as integer

li $a1, 6
li $v0, 8
syscall		#scanf asciiz chaine

li $a0,  0x4000
li $v0, 4
syscall		#printf "your chain"

li $v0, 10
syscall

.data 
chaine: .asciiz "Youpi"
