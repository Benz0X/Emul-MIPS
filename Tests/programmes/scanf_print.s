	.file	1 "scanf_print.c"
	.section .mdebug.abi32
	.previous
	.rdata
	.align	2
$LC0:
	.ascii	"%s\000"
	.text
	.align	2
	.globl	main
	.ent	main
main:
	.frame	$fp,104,$31		# vars= 80, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$fp,96($sp)
	move	$fp,$sp
	sw	$4,104($fp)
	sw	$5,108($fp)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	addiu	$2,$fp,16
	move	$5,$2
	jal	scanf
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	addiu	$2,$fp,16
	move	$5,$2
	jal	printf
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	addiu	$2,$fp,16
	move	$5,$2
	jal	printf
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	addiu	$2,$fp,16
	move	$5,$2
	jal	printf
	nop

	move	$2,$0
	move	$sp,$fp
	lw	$31,100($sp)
	lw	$fp,96($sp)
	addiu	$sp,$sp,104
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.ident	"GCC: (GNU) 4.2.4"
