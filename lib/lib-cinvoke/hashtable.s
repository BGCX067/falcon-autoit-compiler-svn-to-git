	.file	"hashtable.c"
	.section .rdata,"dr"
	.align 32
_primes:
	.long	53
	.long	97
	.long	193
	.long	389
	.long	769
	.long	1543
	.long	3079
	.long	6151
	.long	12289
	.long	24593
	.long	49157
	.long	98317
	.long	196613
	.long	393241
	.long	786433
	.long	1572869
	.long	3145739
	.long	6291469
	.long	12582917
	.long	25165843
	.long	50331653
	.long	100663319
	.long	201326611
	.long	402653189
	.long	805306457
	.long	1610612741
	.globl	_prime_table_length
	.align 4
_prime_table_length:
	.long	26
	.globl	_max_load_factor
	.align 4
_max_load_factor:
	.long	1059481190
	.text
	.globl	_create_hashtable
	.def	_create_hashtable;	.scl	2;	.type	32;	.endef
_create_hashtable:
LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%ebx
	subl	$48, %esp
	movl	_primes, %eax
	movl	%eax, -16(%ebp)
	cmpl	$1073741824, 8(%ebp)
	jbe	L2
	.cfi_offset 3, -16
	.cfi_offset 7, -12
	movl	$0, %eax
	jmp	L3
L2:
	movl	$0, -12(%ebp)
	jmp	L4
L7:
	movl	-12(%ebp), %eax
	movl	_primes(,%eax,4), %eax
	cmpl	8(%ebp), %eax
	jbe	L5
	movl	-12(%ebp), %eax
	movl	_primes(,%eax,4), %eax
	movl	%eax, -16(%ebp)
	jmp	L6
L5:
	incl	-12(%ebp)
L4:
	movl	_prime_table_length, %eax
	cmpl	%eax, -12(%ebp)
	jb	L7
L6:
	movl	$28, (%esp)
	call	_malloc
	movl	%eax, -20(%ebp)
	cmpl	$0, -20(%ebp)
	jne	L8
	movl	$0, %eax
	jmp	L3
L8:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, %edx
	movl	-20(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	-20(%ebp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	L9
	movl	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, %eax
	jmp	L3
L9:
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-20(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, %ebx
	movb	$0, %al
	movl	%ebx, %edi
	movl	%edx, %ecx
	rep stosb
	movl	-20(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax)
	movl	-20(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, 16(%eax)
	movl	-20(%ebp), %eax
	movl	$0, 8(%eax)
	movl	-20(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 20(%eax)
	movl	-20(%ebp), %eax
	movl	16(%ebp), %edx
	movl	%edx, 24(%eax)
	movl	-16(%ebp), %eax
	movl	$0, %edx
	movl	%eax, -32(%ebp)
	movl	%edx, -28(%ebp)
	fildq	-32(%ebp)
	flds	_max_load_factor
	fmulp	%st, %st(1)
	fstpl	(%esp)
	call	_ceil
	fnstcw	-34(%ebp)
	movw	-34(%ebp), %ax
	movb	$12, %ah
	movw	%ax, -36(%ebp)
	fldcw	-36(%ebp)
	fistpq	-32(%ebp)
	fldcw	-34(%ebp)
	movl	-32(%ebp), %eax
	movl	-28(%ebp), %edx
	movl	%eax, %edx
	movl	-20(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	-20(%ebp), %eax
L3:
	addl	$48, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE6:
	.globl	_str_hash
	.def	_str_hash;	.scl	2;	.type	32;	.endef
_str_hash:
LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	$0, -8(%ebp)
	jmp	L11
L12:
	movl	-8(%ebp), %eax
	movl	%eax, %edx
	sall	$6, %edx
	movl	-12(%ebp), %eax
	addl	%eax, %edx
	movl	-8(%ebp), %eax
	sall	$16, %eax
	addl	%edx, %eax
	subl	-8(%ebp), %eax
	movl	%eax, -8(%ebp)
L11:
	movl	-4(%ebp), %eax
	movb	(%eax), %al
	movsbl	%al, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	setne	%al
	incl	-4(%ebp)
	testb	%al, %al
	jne	L12
	movl	-8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE7:
	.globl	_str_eq
	.def	_str_eq;	.scl	2;	.type	32;	.endef
_str_eq:
LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_strcmp
	testl	%eax, %eax
	sete	%al
	movzbl	%al, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE8:
	.globl	_hash
	.def	_hash;	.scl	2;	.type	32;	.endef
_hash:
LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	20(%eax), %edx
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	*%edx
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE9:
	.def	_hashtable_expand;	.scl	3;	.type	32;	.endef
_hashtable_expand:
LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%ebx
	subl	$64, %esp
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	movl	_prime_table_length, %edx
	decl	%edx
	cmpl	%edx, %eax
	jne	L16
	.cfi_offset 3, -16
	.cfi_offset 7, -12
	movl	$0, %eax
	jmp	L17
L16:
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 16(%eax)
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	movl	_primes(,%eax,4), %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, -28(%ebp)
	cmpl	$0, -28(%ebp)
	je	L18
	movl	-24(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-28(%ebp), %eax
	movl	%eax, %ebx
	movb	$0, %al
	movl	%ebx, %edi
	movl	%edx, %ecx
	rep stosb
	movl	$0, -20(%ebp)
	jmp	L19
L21:
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-20(%ebp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	-12(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, (%edx)
	movl	-12(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_indexFor
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	-32(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, (%eax)
	jmp	L20
L31:
	nop
L20:
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-20(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L21
	incl	-20(%ebp)
L19:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	-20(%ebp), %eax
	ja	L31
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	-28(%ebp), %edx
	movl	%edx, 4(%eax)
	jmp	L23
L18:
	movl	-24(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_realloc
	movl	%eax, -28(%ebp)
	cmpl	$0, -28(%ebp)
	jne	L24
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	leal	-1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 16(%eax)
	movl	$0, %eax
	jmp	L17
L24:
	movl	8(%ebp), %eax
	movl	-28(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	-24(%ebp), %edx
	subl	%eax, %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, %ebx
	movb	$0, %al
	movl	%ebx, %edi
	movl	%edx, %ecx
	rep stosb
	movl	$0, -20(%ebp)
	jmp	L25
L30:
	movl	-20(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	jmp	L26
L29:
	movl	-12(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_indexFor
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	cmpl	-20(%ebp), %eax
	jne	L27
	movl	-12(%ebp), %eax
	addl	$12, %eax
	movl	%eax, -16(%ebp)
	jmp	L28
L27:
	movl	-12(%ebp), %eax
	movl	12(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, (%eax)
	movl	-32(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	-32(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, (%eax)
L28:
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
L26:
	cmpl	$0, -12(%ebp)
	jne	L29
	incl	-20(%ebp)
L25:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	-20(%ebp), %eax
	ja	L30
L23:
	movl	8(%ebp), %eax
	movl	-24(%ebp), %edx
	movl	%edx, (%eax)
	movl	-24(%ebp), %eax
	movl	$0, %edx
	movl	%eax, -48(%ebp)
	movl	%edx, -44(%ebp)
	fildq	-48(%ebp)
	flds	_max_load_factor
	fmulp	%st, %st(1)
	fstpl	(%esp)
	call	_ceil
	fnstcw	-50(%ebp)
	movw	-50(%ebp), %ax
	movb	$12, %ah
	movw	%ax, -52(%ebp)
	fldcw	-52(%ebp)
	fistpq	-48(%ebp)
	fldcw	-50(%ebp)
	movl	-48(%ebp), %eax
	movl	-44(%ebp), %edx
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	$-1, %eax
L17:
	addl	$64, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE10:
	.globl	_indexFor
	.def	_indexFor;	.scl	2;	.type	32;	.endef
_indexFor:
LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	12(%ebp), %eax
	movl	$0, %edx
	divl	8(%ebp)
	movl	%edx, %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE11:
	.globl	_hashtable_count
	.def	_hashtable_count;	.scl	2;	.type	32;	.endef
_hashtable_count:
LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE12:
	.globl	_hashtable_insert
	.def	_hashtable_insert;	.scl	2;	.type	32;	.endef
_hashtable_insert:
LFB13:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	8(%ebp), %eax
	movl	8(%eax), %edx
	movl	8(%ebp), %eax
	movl	12(%eax), %eax
	cmpl	%eax, %edx
	jbe	L35
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_hashtable_expand
	testl	%eax, %eax
	jne	L35
	movl	$0, %eax
	jmp	L36
L35:
	movl	$16, (%esp)
	call	_malloc
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L37
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	leal	-1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	$0, %eax
	jmp	L36
L37:
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_hash
	movl	-12(%ebp), %edx
	movl	%eax, 8(%edx)
	movl	-12(%ebp), %eax
	movl	8(%eax), %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_indexFor
	movl	%eax, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	movl	-12(%ebp), %eax
	movl	16(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-16(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-16(%ebp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	-12(%ebp), %eax
	movl	%eax, (%edx)
	movl	$-1, %eax
L36:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE13:
	.globl	_hashtable_search
	.def	_hashtable_search;	.scl	2;	.type	32;	.endef
_hashtable_search:
LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_hash
	movl	%eax, -16(%ebp)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_indexFor
	movl	%eax, -20(%ebp)
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-20(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	jmp	L39
L42:
	movl	-12(%ebp), %eax
	movl	8(%eax), %eax
	cmpl	-16(%ebp), %eax
	jne	L40
	movl	8(%ebp), %eax
	movl	24(%eax), %edx
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	*%edx
	testl	%eax, %eax
	je	L40
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	jmp	L41
L40:
	movl	-12(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -12(%ebp)
L39:
	cmpl	$0, -12(%ebp)
	jne	L42
	movl	$0, %eax
L41:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE14:
	.globl	_hashtable_remove
	.def	_hashtable_remove;	.scl	2;	.type	32;	.endef
_hashtable_remove:
LFB15:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$56, %esp
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_hash
	movl	%eax, -20(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_hash
	movl	8(%ebp), %edx
	movl	(%edx), %edx
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_indexFor
	movl	%eax, -24(%ebp)
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-24(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	jmp	L44
L47:
	movl	-12(%ebp), %eax
	movl	8(%eax), %eax
	cmpl	-20(%ebp), %eax
	jne	L45
	movl	8(%ebp), %eax
	movl	24(%eax), %edx
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	*%edx
	testl	%eax, %eax
	je	L45
	movl	-12(%ebp), %eax
	movl	12(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	leal	-1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -28(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-28(%ebp), %eax
	jmp	L46
L45:
	movl	-12(%ebp), %eax
	addl	$12, %eax
	movl	%eax, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -12(%ebp)
L44:
	cmpl	$0, -12(%ebp)
	jne	L47
	movl	$0, %eax
L46:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE15:
	.globl	_hashtable_destroy
	.def	_hashtable_destroy;	.scl	2;	.type	32;	.endef
_hashtable_destroy:
LFB16:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, 12(%ebp)
	je	L49
	movl	$0, -12(%ebp)
	jmp	L50
L53:
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	-20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -16(%ebp)
	jmp	L51
L52:
	movl	-16(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -16(%ebp)
	movl	-24(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L51:
	cmpl	$0, -16(%ebp)
	jne	L52
	incl	-12(%ebp)
L50:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	-12(%ebp), %eax
	ja	L53
	jmp	L54
L49:
	movl	$0, -12(%ebp)
	jmp	L55
L58:
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	-20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -16(%ebp)
	jmp	L56
L57:
	movl	-16(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -16(%ebp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L56:
	cmpl	$0, -16(%ebp)
	jne	L57
	incl	-12(%ebp)
L55:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	cmpl	-12(%ebp), %eax
	ja	L58
L54:
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE16:
	.globl	_hashtable_iterator
	.def	_hashtable_iterator;	.scl	2;	.type	32;	.endef
_hashtable_iterator:
LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	movl	%edx, (%eax)
	movl	12(%ebp), %eax
	movl	$0, 4(%eax)
	movl	12(%ebp), %eax
	movl	$0, 8(%eax)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -8(%ebp)
	movl	12(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	L65
L60:
	movl	$0, -4(%ebp)
	jmp	L62
L64:
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-4(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	L63
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	-4(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	12(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	12(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, 12(%eax)
	jmp	L59
L63:
	incl	-4(%ebp)
L62:
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jb	L64
	jmp	L59
L65:
	nop
L59:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE17:
	.globl	_hashtable_iterator_key
	.def	_hashtable_iterator_key;	.scl	2;	.type	32;	.endef
_hashtable_iterator_key:
LFB18:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	(%eax), %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE18:
	.globl	_hashtable_iterator_value
	.def	_hashtable_iterator_value;	.scl	2;	.type	32;	.endef
_hashtable_iterator_value:
LFB19:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	4(%eax), %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE19:
	.globl	_hashtable_iterator_advance
	.def	_hashtable_iterator_advance;	.scl	2;	.type	32;	.endef
_hashtable_iterator_advance:
LFB20:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	L69
	movl	$0, %eax
	jmp	L70
L69:
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	12(%eax), %eax
	movl	%eax, -8(%ebp)
	cmpl	$0, -8(%ebp)
	je	L71
	movl	8(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	$-1, %eax
	jmp	L70
L71:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	movl	$0, 8(%eax)
	movl	8(%ebp), %eax
	movl	12(%eax), %eax
	leal	1(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jb	L72
	movl	8(%ebp), %eax
	movl	$0, 4(%eax)
	movl	$0, %eax
	jmp	L70
L72:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	4(%eax), %eax
	movl	%eax, -16(%ebp)
	jmp	L73
L74:
	incl	-4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jb	L73
	movl	8(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	movl	$0, 4(%eax)
	movl	$0, %eax
	jmp	L70
L73:
	movl	-4(%ebp), %eax
	sall	$2, %eax
	addl	-16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -8(%ebp)
	cmpl	$0, -8(%ebp)
	je	L74
	movl	8(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	$-1, %eax
L70:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE20:
	.globl	_hashtable_iterator_remove
	.def	_hashtable_iterator_remove;	.scl	2;	.type	32;	.endef
_hashtable_iterator_remove:
LFB21:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	jne	L76
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	12(%eax), %eax
	sall	$2, %eax
	addl	%eax, %edx
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	12(%eax), %eax
	movl	%eax, (%edx)
	jmp	L77
L76:
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	movl	8(%ebp), %edx
	movl	4(%edx), %edx
	movl	12(%edx), %edx
	movl	%edx, 12(%eax)
L77:
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	8(%eax), %edx
	decl	%edx
	movl	%edx, 8(%eax)
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, -16(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_hashtable_iterator_advance
	movl	%eax, -20(%ebp)
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	cmpl	-12(%ebp), %eax
	jne	L78
	movl	8(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, 8(%eax)
L78:
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-20(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE21:
	.globl	_hashtable_iterator_search
	.def	_hashtable_iterator_search;	.scl	2;	.type	32;	.endef
_hashtable_iterator_search:
LFB22:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_hash
	movl	%eax, -20(%ebp)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	-20(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_indexFor
	movl	%eax, -24(%ebp)
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	movl	-24(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	$0, -16(%ebp)
	jmp	L80
L83:
	movl	-12(%ebp), %eax
	movl	8(%eax), %eax
	cmpl	-20(%ebp), %eax
	jne	L81
	movl	12(%ebp), %eax
	movl	24(%eax), %edx
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	16(%ebp), %eax
	movl	%eax, (%esp)
	call	*%edx
	testl	%eax, %eax
	je	L81
	movl	8(%ebp), %eax
	movl	-24(%ebp), %edx
	movl	%edx, 12(%eax)
	movl	8(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, 8(%eax)
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	movl	$-1, %eax
	jmp	L82
L81:
	movl	-12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -12(%ebp)
L80:
	cmpl	$0, -12(%ebp)
	jne	L83
	movl	$0, %eax
L82:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE22:
	.def	_malloc;	.scl	2;	.type	32;	.endef
	.def	_free;	.scl	2;	.type	32;	.endef
	.def	_ceil;	.scl	2;	.type	32;	.endef
	.def	_strcmp;	.scl	2;	.type	32;	.endef
	.def	_realloc;	.scl	2;	.type	32;	.endef
