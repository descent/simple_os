	.file	"cb.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$753664, -4(%ebp)
	movl	-4(%ebp), %eax
	movb	$65, (%eax)
	movl	$753665, %eax
	movb	$12, (%eax)
	movl	$753666, %eax
	movb	$66, (%eax)
	movl	$753667, %eax
	movb	$12, (%eax)
.L2:
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Debian 4.6.3-4) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
