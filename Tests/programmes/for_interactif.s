	.file	1 "for_interactif.c"
	.section .mdebug.abi32
	.previous
	.rdata
	.align	2
$LC0:
	.ascii	"Entrez des notes (moins de 10,<0 pour terminer) :\000"
	.align	2
$LC1:
	.ascii	"%d\000"
	.align	2
$LC2:
	.ascii	"Vous avez entr\303\251 %d\012\000"
	.align	2
$LC3:
	.ascii	"Votre moyenne est : %d\012\000"
	.align	2
$LC4:
	.ascii	"notes tri\303\251es : \000"
	.align	2
$LC5:
	.ascii	" %d \012\000"
	.text
	.align	2
	.globl	main
	.ent	main
main:
	.frame	$fp,88,$31		# vars= 64, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$fp,80($sp)
	move	$fp,$sp
	sw	$4,88($fp)
	sw	$5,92($fp)
	sw	$0,72($fp)
	sw	$0,28($fp)
	sw	$0,24($fp)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	jal	puts
	nop

	sw	$0,24($fp)
	j	$L2
	nop

$L3:
	addiu	$2,$fp,72
	lui	$3,%hi($LC1)
	addiu	$4,$3,%lo($LC1)
	move	$5,$2
	jal	scanf
	nop

	lw	$2,72($fp)
	nop
	blez	$2,$L4
	nop

	lw	$3,72($fp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	jal	printf
	nop

	lw	$2,24($fp)
	lw	$4,72($fp)
	sll	$2,$2,2
	addiu	$3,$fp,16
	addu	$2,$2,$3
	sw	$4,16($2)
	lw	$3,72($fp)
	lw	$2,28($fp)
	nop
	addu	$2,$2,$3
	sw	$2,28($fp)
$L4:
	lw	$2,24($fp)
	nop
	addiu	$2,$2,1
	sw	$2,24($fp)
$L2:
	lw	$2,24($fp)
	nop
	slt	$2,$2,9
	bne	$2,$0,$L3
	nop

	lw	$3,28($fp)
	lw	$2,24($fp)
	nop
	bne	$2,$0,1f
	div	$0,$3,$2
	break	7
1:
	mflo	$2
	sw	$2,16($fp)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lw	$5,16($fp)
	jal	printf
	nop

	lw	$2,24($fp)
	nop
	addiu	$3,$2,-1
	addiu	$2,$fp,32
	move	$4,$2
	move	$5,$3
	jal	tri_tab
	nop

	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	jal	puts
	nop

	sw	$0,20($fp)
	j	$L7
	nop

$L8:
	lw	$2,20($fp)
	nop
	sll	$2,$2,2
	addiu	$3,$fp,16
	addu	$2,$2,$3
	lw	$3,16($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	jal	printf
	nop

	lw	$2,20($fp)
	nop
	addiu	$2,$2,1
	sw	$2,20($fp)
$L7:
	lw	$2,20($fp)
	lw	$3,24($fp)
	nop
	slt	$2,$2,$3
	bne	$2,$0,$L8
	nop

	move	$2,$0
	move	$sp,$fp
	lw	$31,84($sp)
	lw	$fp,80($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.align	2
	.globl	tri_tab
	.ent	tri_tab
tri_tab:
	.frame	$fp,16,$31		# vars= 8, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-16
	sw	$fp,8($sp)
	move	$fp,$sp
	sw	$4,16($fp)
	sw	$5,20($fp)
	sw	$0,4($fp)
	j	$L12
	nop

$L13:
	lw	$2,4($fp)
	nop
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($fp)
	nop
	addu	$2,$3,$2
	addiu	$2,$2,4
	lw	$4,0($2)
	lw	$2,4($fp)
	nop
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($fp)
	nop
	addu	$2,$3,$2
	lw	$2,0($2)
	nop
	slt	$2,$4,$2
	beq	$2,$0,$L14
	nop

	lw	$2,4($fp)
	nop
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($fp)
	nop
	addu	$2,$3,$2
	addiu	$2,$2,4
	lw	$2,0($2)
	nop
	sw	$2,0($fp)
	lw	$2,4($fp)
	nop
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($fp)
	nop
	addu	$2,$3,$2
	addiu	$4,$2,4
	lw	$2,4($fp)
	nop
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($fp)
	nop
	addu	$2,$3,$2
	lw	$2,0($2)
	nop
	sw	$2,0($4)
	lw	$2,4($fp)
	nop
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($fp)
	nop
	addu	$3,$3,$2
	lw	$2,0($fp)
	nop
	sw	$2,0($3)
	sw	$0,4($fp)
	j	$L12
	nop

$L14:
	lw	$2,4($fp)
	nop
	addiu	$2,$2,1
	sw	$2,4($fp)
$L12:
	lw	$2,4($fp)
	lw	$3,20($fp)
	nop
	slt	$2,$2,$3
	bne	$2,$0,$L13
	nop

	move	$2,$0
	move	$sp,$fp
	lw	$fp,8($sp)
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	tri_tab
	.size	tri_tab, .-tri_tab
	.ident	"GCC: (GNU) 4.2.4"
