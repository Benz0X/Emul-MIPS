	.file	1 "simple.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 1
	.abicalls
	.rdata
	.align	2
$LC0:
	.ascii	"AAAAAAAAZZEEEEE\000"
	.space	5
	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.set	nomips16
	.frame	$fp,64,$31		# vars= 32, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$fp,56($sp)
	move	$fp,$sp
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	.cprestore	16
	li	$2,21			# 0x15
	sw	$2,28($fp)
	li	$2,21			# 0x15
	sw	$2,24($fp)
	lui	$3,%hi($LC0)
	lw	$5,%lo($LC0)($3)
	addiu	$2,$3,%lo($LC0)
	lw	$6,4($2)
	addiu	$2,$3,%lo($LC0)
	lw	$4,8($2)
	addiu	$2,$3,%lo($LC0)
	lw	$2,12($2)
	sw	$5,32($fp)
	sw	$6,36($fp)
	sw	$4,40($fp)
	sw	$2,44($fp)
	sw	$0,48($fp)
	sb	$0,52($fp)
	li	$2,101
	sb	$2,33($fp)
	lw	$4,28($fp)
	lw	$5,24($fp)
	lw	$25,%call16(multiply)($28)
	nop
	jalr	$25
	nop

	lw	$28,16($fp)
	move	$3,$2
	lw	$2,28($fp)
	nop
	addu	$3,$3,$2
	lw	$2,24($fp)
	nop
	addu	$2,$3,$2
	move	$sp,$fp
	lw	$31,60($sp)
	lw	$fp,56($sp)
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
