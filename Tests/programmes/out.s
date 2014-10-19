	.file	1 "simple.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 1
	.abicalls
	.rdata
	.align	2
$LC0:
	.asciiz	"BoTableau\000"
	.space	2
	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.set	nomips16
	.frame	$fp,64,$31		# vars= 24, regs= 3/0, args= 16, gp= 8
	.mask	0xc0010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$fp,56($sp)
	sw	$16,52($sp)
	move	$fp,$sp
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	.cprestore	16
	li	$2,21			# 0x15
	sw	$2,28($fp)
	li	$2,42			# 0x2a
	sw	$2,24($fp)
	lui	$3,%hi($LC0)
	lw	$4,%lo($LC0)($3)
	addiu	$2,$3,%lo($LC0)
	lw	$2,4($2)
	sw	$4,32($fp)
	sw	$2,36($fp)
	addiu	$2,$3,%lo($LC0)
	lhu	$2,8($2)
	nop
	sh	$2,40($fp)
	sh	$0,42($fp)
	li	$2,10
	sb	$2,37($fp)
	lw	$3,28($fp)
	lw	$2,24($fp)
	nop
	addu	$16,$3,$2
	lw	$4,28($fp)
	lw	$5,24($fp)
	lw	$25,%call16(multiply)($28)
	nop
	jalr	$25
	nop

	lw	$28,16($fp)
	addu	$2,$16,$2
	move	$sp,$fp
	lw	$31,60($sp)
	lw	$fp,56($sp)
	lw	$16,52($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.align	2
	.globl	multiply
	.ent	multiply
	.type	multiply, @function
multiply:
	.set	nomips16
	.frame	$fp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$fp,4($sp)
	move	$fp,$sp
	sw	$4,8($fp)
	sw	$5,12($fp)
	lw	$3,8($fp)
	lw	$2,12($fp)
	nop
	mult	$3,$2
	mflo	$2
	move	$sp,$fp
	lw	$fp,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	multiply
	.ident	"GCC: (Debian 4.3.5-4) 4.3.5"
