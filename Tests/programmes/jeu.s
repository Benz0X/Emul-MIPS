	.file	1 "jeu.c"
	.section .mdebug.abi32
	.previous
	.rdata
	.align	2
$LC0:
	.ascii	"Entrez votre nom :\000"
	.align	2
$LC1:
	.ascii	"%s\000"
	.align	2
$LC2:
	.ascii	"Entrez un max\000"
	.align	2
$LC3:
	.ascii	"%d\000"
	.align	2
$LC4:
	.ascii	"Trouvez le nombre entre 0 et %d\012\000"
	.align	2
$LC5:
	.ascii	"C'est +\000"
	.align	2
$LC6:
	.ascii	"C'est -\000"
	.align	2
$LC7:
	.ascii	"Vous avez trouv\303\251 en %d coups !\012\000"
	.text
	.align	2
	.globl	main
	.ent	main
main:
	.frame	$fp,80,$31		# vars= 56, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$fp,72($sp)
	move	$fp,$sp
	sw	$4,80($fp)
	sw	$5,84($fp)
	sw	$0,28($fp)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	jal	puts
	nop

	addiu	$3,$fp,32
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	jal	scanf
	nop

	addiu	$2,$fp,32
	move	$4,$2
	jal	strlen
	nop

	sw	$2,28($fp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	jal	puts
	nop

	addiu	$3,$fp,64
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	jal	scanf
	nop

	sw	$0,16($fp)
	j	$L2
	nop

$L3:
	lw	$3,16($fp)
	addiu	$2,$fp,16
	addu	$2,$2,$3
	lb	$2,16($2)
	nop
	move	$3,$2
	lw	$2,24($fp)
	nop
	addu	$2,$2,$3
	sw	$2,24($fp)
	lw	$2,16($fp)
	nop
	addiu	$2,$2,1
	sw	$2,16($fp)
$L2:
	lw	$2,28($fp)
	nop
	addiu	$3,$2,-2
	lw	$2,16($fp)
	nop
	slt	$2,$2,$3
	bne	$2,$0,$L3
	nop

	lw	$3,24($fp)
	li	$2,419627008			# 0x19030000
	ori	$2,$2,0xa459
	mult	$3,$2
	mflo	$4
	li	$2,-1989148672			# 0xffffffff89700000
	ori	$2,$2,0x5f5b
	mult	$4,$2
	mfhi	$2
	addu	$2,$2,$4
	sra	$3,$2,29
	sra	$2,$4,31
	subu	$3,$3,$2
	li	$2,999948288			# 0x3b9a0000
	ori	$2,$2,0xc9f5
	mult	$3,$2
	mflo	$2
	subu	$2,$4,$2
	sw	$2,24($fp)
	lw	$3,64($fp)
	lw	$2,24($fp)
	nop
	bne	$3,$0,1f
	div	$0,$2,$3
	break	7
1:
	mfhi	$2
	move	$3,$2
	lw	$2,64($fp)
	nop
	addu	$3,$3,$2
	srl	$2,$3,31
	addu	$2,$2,$3
	sra	$2,$2,1
	sw	$2,24($fp)
	lw	$3,64($fp)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	jal	printf
	nop

	sw	$0,16($fp)
$L5:
	addiu	$3,$fp,68
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	jal	scanf
	nop

	lw	$2,16($fp)
	nop
	addiu	$2,$2,1
	sw	$2,16($fp)
	lw	$3,68($fp)
	lw	$2,24($fp)
	nop
	slt	$2,$3,$2
	beq	$2,$0,$L6
	nop

	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	jal	puts
	nop

$L6:
	lw	$2,68($fp)
	lw	$3,24($fp)
	nop
	slt	$2,$3,$2
	beq	$2,$0,$L8
	nop

	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	jal	puts
	nop

$L8:
	lw	$3,68($fp)
	lw	$2,24($fp)
	nop
	bne	$3,$2,$L10
	nop

	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lw	$5,16($fp)
	jal	printf
	nop

	j	$L12
	nop

$L10:
	lw	$3,68($fp)
	lw	$2,24($fp)
	nop
	bne	$3,$2,$L5
	nop

$L12:
	move	$2,$0
	move	$sp,$fp
	lw	$31,76($sp)
	lw	$fp,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.ident	"GCC: (GNU) 4.2.4"
