	.text
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-1024
	sd	ra,1016(sp)
	sd	s0,1008(sp)
	addi	s0,sp,1024
	li	a5,3
	sw	a5,-1024(s0)
	lw	a5,-1024(s0)
	sw	a5,-1020(s0)
	lw	a0,-1020(s0)
	call	print_int
	sw	a0,-1016(s0)
	li	a5,0
	sw	a5,-1012(s0)
	lw	a0,-1012(s0)
	ld	ra,1016(sp)
	ld	s0,1008(sp)
	addi	sp,sp,1024
	jr	ra
	.size	main, .-main
