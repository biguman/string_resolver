	.file	"test.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"ptr1:\t%p\nptr2:\t%p\nptr3:\t%p\nsum:\t%lu\nsum2:\t%lu\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"sum:\t%#lX\nsum2:\t%#lX\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB51:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movl	$2, %edi
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$32, %rsp
	.cfi_def_cfa_offset 80
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	call	malloc@PLT
	movl	$2, %edi
	movq	%rax, %r12
	call	malloc@PLT
	movl	$2, %edi
	movq	%rax, %rbp
	call	malloc@PLT
	subq	$8, %rsp
	.cfi_def_cfa_offset 88
	movq	%rbp, %rbx
	movq	%rbp, %rcx
	movq	%rax, %r14
	subq	%r12, %rbx
	movq	%rax, %r8
	movq	%rax, %r13
	subq	%r12, %r14
	movq	%rbx, %r9
	leaq	.LC0(%rip), %rsi
	movq	%r12, %rdx
	pushq	%r14
	.cfi_def_cfa_offset 96
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	leaq	24(%rsp), %rsi
	movl	$56, %ecx
	movabsq	$3544956528210358286, %rax
	leaq	(%rbx,%rax), %r9
	movabsq	$3530822107858468864, %rax
	leaq	(%r14,%rax), %r8
	leaq	16(%rsp), %r14
	movq	%r14, %rsp
	.cfi_def_cfa_offset 80
	movq	%r14, %rax
	.p2align 4,,10
	.p2align 3
.L2:
	movq	%r9, %rdx
	addq	$1, %rax
	shrq	%cl, %rdx
	subl	$8, %ecx
	movb	%dl, -1(%rax)
	cmpq	%rsi, %rax
	jne	.L2
	leaq	16(%r14), %rsi
	movl	$56, %ecx
	.p2align 4,,10
	.p2align 3
.L3:
	movq	%r8, %rdx
	addq	$1, %rax
	shrq	%cl, %rdx
	subl	$8, %ecx
	movb	%dl, -1(%rax)
	cmpq	%rsi, %rax
	jne	.L3
	movq	%r8, %rcx
	movq	%r9, %rdx
	leaq	.LC1(%rip), %rsi
	xorl	%eax, %eax
	movl	$1, %edi
	call	__printf_chk@PLT
	movq	%r12, %rdi
	call	free@PLT
	movq	%rbp, %rdi
	call	free@PLT
	movq	%r13, %rdi
	call	free@PLT
	movq	%r14, %rdi
	call	puts@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L9
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	movl	$1, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE51:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
