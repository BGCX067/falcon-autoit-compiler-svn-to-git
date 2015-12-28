	.file	"cinvoke.c"
	.text
	.globl	_context_set_error
	.def	_context_set_error;	.scl	2;	.type	32;	.endef
_context_set_error:
LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	L2
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_arch_free_errstr
L2:
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	movl	8(%ebp), %eax
	movl	16(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	movl	20(%ebp), %edx
	movl	%edx, 8(%eax)
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE9:
	.globl	_context_set_nomem
	.def	_context_set_nomem;	.scl	2;	.type	32;	.endef
_context_set_nomem:
LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$8, (%esp)
	call	_GetWin32ErrMsg
	movl	$1, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$8, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE10:
	.section .rdata,"dr"
LC0:
	.ascii "call succeeded\0"
	.text
	.globl	_context_clear_error
	.def	_context_clear_error;	.scl	2;	.type	32;	.endef
_context_clear_error:
LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$0, 12(%esp)
	movl	$LC0, 8(%esp)
	movl	$0, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE11:
	.globl	_cinv_context_create
	.def	_cinv_context_create;	.scl	2;	.type	32;	.endef
_cinv_context_create:
LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$12, (%esp)
	call	_malloc
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L6
	movl	$0, %eax
	jmp	L7
L6:
	movl	-12(%ebp), %eax
	movl	$0, 8(%eax)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-12(%ebp), %eax
L7:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE12:
	.globl	_cinv_context_delete
	.def	_cinv_context_delete;	.scl	2;	.type	32;	.endef
_cinv_context_delete:
LFB13:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	L9
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_arch_free_errstr
L9:
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$1, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE13:
	.globl	_cinv_context_geterrormsg
	.def	_cinv_context_geterrormsg;	.scl	2;	.type	32;	.endef
_cinv_context_geterrormsg:
LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE14:
	.globl	_cinv_context_geterrorcode
	.def	_cinv_context_geterrorcode;	.scl	2;	.type	32;	.endef
_cinv_context_geterrorcode:
LFB15:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE15:
	.globl	_cinv_library_create
	.def	_cinv_library_create;	.scl	2;	.type	32;	.endef
_cinv_library_create:
LFB16:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$4, (%esp)
	call	_malloc
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L13
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L14
L13:
	movl	-12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_library_create
	testl	%eax, %eax
	jne	L15
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, %eax
	jmp	L14
L15:
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-12(%ebp), %eax
L14:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE16:
	.globl	_cinv_library_load_entrypoint
	.def	_cinv_library_load_entrypoint;	.scl	2;	.type	32;	.endef
_cinv_library_load_entrypoint:
LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	12(%ebp), %eax
	leal	-12(%ebp), %edx
	movl	%edx, 12(%esp)
	movl	16(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_library_get_entrypoint
	testl	%eax, %eax
	jne	L17
	movl	$0, %eax
	jmp	L18
L17:
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-12(%ebp), %eax
L18:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE17:
	.globl	_cinv_library_delete
	.def	_cinv_library_delete;	.scl	2;	.type	32;	.endef
_cinv_library_delete:
LFB18:
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
	call	_arch_library_delete
	testl	%eax, %eax
	jne	L20
	movl	$0, %eax
	jmp	L21
L20:
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
L21:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE18:
	.data
__typeformats:
	.byte	99
	.byte	115
	.byte	105
	.byte	108
	.byte	101
	.byte	102
	.byte	100
	.byte	112
	.text
	.globl	_parse_type
	.def	_parse_type;	.scl	2;	.type	32;	.endef
_parse_type:
LFB19:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$56, %esp
	movl	8(%ebp), %eax
	movb	%al, -28(%ebp)
	movsbl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_tolower
	movb	%al, -13(%ebp)
	cmpb	$50, -13(%ebp)
	jne	L23
	movl	12(%ebp), %eax
	movl	$1, (%eax)
	movl	$1, %eax
	jmp	L24
L23:
	cmpb	$52, -13(%ebp)
	jne	L25
	movl	12(%ebp), %eax
	movl	$2, (%eax)
	movl	$1, %eax
	jmp	L24
L25:
	cmpb	$56, -13(%ebp)
	jne	L26
	movl	12(%ebp), %eax
	movl	$4, (%eax)
	movl	$1, %eax
	jmp	L24
L26:
	movl	$0, -12(%ebp)
	jmp	L27
L29:
	movl	-12(%ebp), %eax
	addl	$__typeformats, %eax
	movb	(%eax), %al
	cmpb	-13(%ebp), %al
	jne	L28
	movl	-12(%ebp), %edx
	movl	12(%ebp), %eax
	movl	%edx, (%eax)
	movl	$1, %eax
	jmp	L24
L28:
	incl	-12(%ebp)
L27:
	cmpl	$7, -12(%ebp)
	jle	L29
	movl	$0, %eax
L24:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE19:
	.data
	.align 32
__sizefuncs:
	.long	_arch_size_char
	.long	_arch_size_short
	.long	_arch_size_int
	.long	_arch_size_long
	.long	_arch_size_extralong
	.long	_arch_size_float
	.long	_arch_size_double
	.long	_arch_size_ptr
	.section .rdata,"dr"
	.align 4
LC1:
	.ascii "cinvoke developer error: unknown type %d\12\0"
	.text
	.globl	_get_size
	.def	_get_size;	.scl	2;	.type	32;	.endef
_get_size:
LFB20:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	20(%ebp), %eax
	movl	$1, (%eax)
	cmpl	$7, 8(%ebp)
	jbe	L31
	movl	8(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$LC1, 4(%esp)
	movl	__imp___iob, %eax
	addl	$64, %eax
	movl	%eax, (%esp)
	call	_fprintf
L31:
	movl	8(%ebp), %eax
	movl	__sizefuncs(,%eax,4), %eax
	movl	%eax, -12(%ebp)
	movl	20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	movl	-12(%ebp), %eax
	call	*%eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE20:
	.data
	.align 32
__pullfuncs:
	.long	_arch_getval_char
	.long	_arch_getval_short
	.long	_arch_getval_int
	.long	_arch_getval_long
	.long	_arch_getval_extralong
	.long	_arch_getval_float
	.long	_arch_getval_double
	.long	_arch_getval_ptr
	.text
	.globl	_pull_value
	.def	_pull_value;	.scl	2;	.type	32;	.endef
_pull_value:
LFB21:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	cmpl	$7, 12(%ebp)
	jbe	L33
	movl	12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$LC1, 4(%esp)
	movl	__imp___iob, %eax
	addl	$64, %eax
	movl	%eax, (%esp)
	call	_fprintf
L33:
	movl	12(%ebp), %eax
	movl	__pullfuncs(,%eax,4), %eax
	movl	%eax, -12(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	movl	-12(%ebp), %eax
	call	*%eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE21:
	.globl	_set_value
	.def	_set_value;	.scl	2;	.type	32;	.endef
_set_value:
LFB22:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	cmpl	$7, 12(%ebp)
	ja	L35
	movl	12(%ebp), %eax
	sall	$2, %eax
	addl	$L44, %eax
	movl	(%eax), %eax
	jmp	*%eax
	.section .rdata,"dr"
	.align 4
L44:
	.long	L36
	.long	L37
	.long	L38
	.long	L39
	.long	L40
	.long	L41
	.long	L42
	.long	L43
	.text
L36:
	movl	16(%ebp), %eax
	movb	(%eax), %al
	movsbl	%al, %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_char
	jmp	L34
L37:
	movl	16(%ebp), %eax
	movw	(%eax), %ax
	cwtl
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_short
	jmp	L34
L38:
	movl	16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_int
	jmp	L34
L39:
	movl	16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_long
	jmp	L34
L40:
	movl	16(%ebp), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	%edx, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_extralong
	jmp	L34
L41:
	movl	16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_float
	jmp	L34
L42:
	movl	16(%ebp), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	%edx, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_double
	jmp	L34
L43:
	movl	16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_setval_ptr
	jmp	L34
L35:
	movl	12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$LC1, 4(%esp)
	movl	__imp___iob, %eax
	addl	$64, %eax
	movl	%eax, (%esp)
	call	_fprintf
	nop
L34:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE22:
	.section .rdata,"dr"
	.align 4
LC2:
	.ascii "The return value format was not recognized\0"
LC3:
	.ascii "Too many parameters\0"
	.align 4
LC4:
	.ascii "The parameter format was not recognized\0"
	.text
	.globl	_cinv_function_create
	.def	_cinv_function_create;	.scl	2;	.type	32;	.endef
_cinv_function_create:
LFB23:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%ebx
	subl	$64, %esp
	movl	$36, (%esp)
	.cfi_offset 3, -16
	.cfi_offset 7, -12
	call	_malloc
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	jne	L47
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L48
L47:
	movl	-16(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	movl	-16(%ebp), %eax
	movw	$0, 32(%eax)
	movl	16(%ebp), %eax
	movb	(%eax), %al
	testb	%al, %al
	jne	L49
	movl	-16(%ebp), %eax
	movl	$0, 4(%eax)
	movl	-16(%ebp), %eax
	movl	$-1, 8(%eax)
	jmp	L50
L49:
	movl	-16(%ebp), %eax
	movl	$1, 4(%eax)
	movl	-16(%ebp), %eax
	leal	8(%eax), %edx
	movl	16(%ebp), %eax
	movb	(%eax), %al
	movsbl	%al, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_parse_type
	testl	%eax, %eax
	jne	L50
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, 12(%esp)
	movl	$LC2, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L48
L50:
	movl	20(%ebp), %eax
	movl	$-1, -44(%ebp)
	movl	%eax, %edx
	movb	$0, %al
	movl	-44(%ebp), %ecx
	movl	%edx, %edi
	repne scasb
	movl	%ecx, %eax
	notl	%eax
	decl	%eax
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movl	%edx, 12(%eax)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	jns	L51
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, 12(%esp)
	movl	$LC3, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L48
L51:
	movl	-16(%ebp), %eax
	movl	$0, 16(%eax)
	movl	-16(%ebp), %eax
	movl	$0, 20(%eax)
	movl	-16(%ebp), %eax
	movl	$0, 24(%eax)
	movl	-16(%ebp), %eax
	movl	$0, 28(%eax)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	jle	L52
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movl	%edx, 16(%eax)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movl	%edx, 20(%eax)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movl	%edx, 24(%eax)
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movl	%edx, 28(%eax)
	movl	-16(%ebp), %eax
	movl	16(%eax), %eax
	testl	%eax, %eax
	je	L53
	movl	-16(%ebp), %eax
	movl	20(%eax), %eax
	testl	%eax, %eax
	je	L53
	movl	-16(%ebp), %eax
	movl	24(%eax), %eax
	testl	%eax, %eax
	jne	L54
L53:
	movl	-16(%ebp), %eax
	movl	16(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	20(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	24(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	28(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L48
L54:
	movl	$0, -12(%ebp)
	jmp	L55
L57:
	movl	-12(%ebp), %eax
	addl	20(%ebp), %eax
	movb	(%eax), %al
	movsbl	%al, %eax
	leal	-32(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_parse_type
	testl	%eax, %eax
	jne	L56
	movl	-16(%ebp), %eax
	movl	16(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	20(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	24(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	28(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, 12(%esp)
	movl	$LC4, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L48
L56:
	movl	-32(%ebp), %eax
	leal	-28(%ebp), %edx
	movl	%edx, 12(%esp)
	leal	-24(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	-20(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_get_size
	movl	-16(%ebp), %eax
	movl	16(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	-20(%ebp), %eax
	movl	%eax, (%edx)
	movl	-16(%ebp), %eax
	movl	20(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	-24(%ebp), %eax
	movl	%eax, (%edx)
	movl	-16(%ebp), %eax
	movl	24(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%eax, %edx
	movl	-32(%ebp), %eax
	movl	%eax, (%edx)
	incl	-12(%ebp)
L55:
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	cmpl	-12(%ebp), %eax
	jg	L57
	movl	$0, -12(%ebp)
	jmp	L58
L61:
	movl	-16(%ebp), %eax
	movl	28(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	leal	(%eax,%edx), %ebx
	movl	-16(%ebp), %eax
	movl	24(%eax), %ecx
	movl	-16(%ebp), %eax
	movl	12(%eax), %edx
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	-12(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_arch_is_register_parm
	movl	%eax, (%ebx)
	movl	-16(%ebp), %eax
	movl	28(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L62
L59:
	movl	-16(%ebp), %eax
	movl	32(%eax), %eax
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movl	16(%eax), %eax
	movl	-12(%ebp), %ecx
	sall	$2, %ecx
	addl	%ecx, %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	movl	-16(%ebp), %edx
	movw	%ax, 32(%edx)
	movl	-16(%ebp), %eax
	movl	32(%eax), %eax
	testw	%ax, %ax
	jns	L60
	movl	-16(%ebp), %eax
	movl	16(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	20(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	24(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	28(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	$0, 12(%esp)
	movl	$LC3, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L48
L62:
	nop
L60:
	incl	-12(%ebp)
L58:
	movl	-16(%ebp), %eax
	movl	12(%eax), %eax
	cmpl	-12(%ebp), %eax
	jg	L61
L52:
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-16(%ebp), %eax
L48:
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
LFE23:
	.globl	_cinv_function_delete
	.def	_cinv_function_delete;	.scl	2;	.type	32;	.endef
_cinv_function_delete:
LFB24:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	16(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	12(%ebp), %eax
	movl	20(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	12(%ebp), %eax
	movl	24(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	12(%ebp), %eax
	movl	28(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE24:
	.globl	_cinv_function_invoke
	.def	_cinv_function_invoke;	.scl	2;	.type	32;	.endef
_cinv_function_invoke:
LFB25:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$100, %esp
	movl	12(%ebp), %eax
	movl	32(%eax), %eax
	cwtl
	movl	%eax, -16(%ebp)
	movl	12(%ebp), %eax
	movl	24(%eax), %ecx
	movl	12(%ebp), %eax
	movl	12(%eax), %edx
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%ecx, 16(%esp)
	movl	24(%ebp), %ecx
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	-64(%ebp), %eax
	movl	%eax, (%esp)
	.cfi_offset 3, -12
	call	_arch_set_register_parms
	cmpl	$0, -16(%ebp)
	je	L65
/APP
 # 363 "cinvoke.c" 1
	sub %ebx, %esp
 # 0 "" 2
/NO_APP
	movl	%ebx, -16(%ebp)
/APP
 # 364 "cinvoke.c" 1
	mov %esp, %ebx
 # 0 "" 2
/NO_APP
	movl	%ebx, -20(%ebp)
	movl	$0, -12(%ebp)
	jmp	L66
L73:
	movl	12(%ebp), %eax
	movl	16(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -36(%ebp)
	movl	12(%ebp), %eax
	movl	20(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -24(%ebp)
	movl	12(%ebp), %eax
	movl	28(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -40(%ebp)
	movl	-24(%ebp), %eax
	movl	-36(%ebp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -28(%ebp)
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	24(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -32(%ebp)
	cmpl	$0, -40(%ebp)
	je	L69
	jmp	L68
L70:
	movl	-32(%ebp), %eax
	movb	(%eax), %dl
	movl	-20(%ebp), %eax
	movb	%dl, (%eax)
	incl	-20(%ebp)
	incl	-32(%ebp)
	decl	-24(%ebp)
L69:
	cmpl	$0, -24(%ebp)
	jne	L70
	jmp	L71
L72:
	movl	-20(%ebp), %eax
	movb	$0, (%eax)
	incl	-20(%ebp)
	decl	-28(%ebp)
L71:
	cmpl	$0, -28(%ebp)
	jne	L72
L68:
	incl	-12(%ebp)
L66:
	movl	12(%ebp), %eax
	movl	12(%eax), %eax
	cmpl	-12(%ebp), %eax
	jg	L73
L65:
/APP
 # 413 "cinvoke.c" 1
	mov -64(%ebp), %ecx; 			mov -60(%ebp), %edx; 			call *16(%ebp)
 # 0 "" 2
 # 414 "cinvoke.c" 1
	mov %eax, -56(%ebp); 			mov %edx, -52(%ebp)
 # 0 "" 2
/NO_APP
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	cmpl	$5, %eax
	je	L74
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	cmpl	$6, %eax
	jne	L75
L74:
/APP
 # 414 "cinvoke.c" 1
	fstp -48(%ebp)
 # 0 "" 2
/NO_APP
L75:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$1, %eax
	je	L76
	cmpl	$0, -16(%ebp)
	je	L76
/APP
 # 417 "cinvoke.c" 1
	add %ebx, %esp
 # 0 "" 2
/NO_APP
	movl	%ebx, -16(%ebp)
L76:
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	je	L77
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	movl	20(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	-56(%ebp), %eax
	movl	%eax, (%esp)
	call	_pull_value
L77:
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
	addl	$100, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE25:
	.globl	_cinv_cbthunk
	.def	_cinv_cbthunk;	.scl	2;	.type	32;	.endef
_cinv_cbthunk:
LFB26:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$108, %esp
/APP
 # 435 "cinvoke.c" 1
	mov %ecx, -88(%ebp); 			mov %edx, -84(%ebp)
 # 0 "" 2
	.cfi_offset 3, -20
	.cfi_offset 6, -16
	.cfi_offset 7, -12
 # 437 "cinvoke.c" 1
	mov %ebp, %ebx
 # 0 "" 2
/NO_APP
	movl	%ebx, -32(%ebp)
	movl	$0, -28(%ebp)
	addl	$96, -32(%ebp)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	je	L79
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	12(%eax), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, -28(%ebp)
	cmpl	$0, -28(%ebp)
	je	L91
L80:
	movl	$0, -36(%ebp)
	jmp	L82
L88:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	20(%eax), %eax
	movl	-36(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -44(%ebp)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%eax), %eax
	movl	-36(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -48(%ebp)
	movl	-44(%ebp), %eax
	movl	-48(%ebp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -52(%ebp)
	movl	-36(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %ebx
	addl	-28(%ebp), %ebx
	movl	-44(%ebp), %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, (%ebx)
	movl	-36(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L83
	movl	$0, -40(%ebp)
	jmp	L84
L85:
	movl	-40(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_free
	incl	-40(%ebp)
L84:
	movl	-40(%ebp), %eax
	cmpl	-36(%ebp), %eax
	jl	L85
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	jmp	L78
L83:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	28(%eax), %eax
	movl	-36(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L92
L86:
	movl	-44(%ebp), %ebx
	movl	-36(%ebp), %eax
	sall	$2, %eax
	addl	-28(%ebp), %eax
	movl	(%eax), %edx
	movl	-32(%ebp), %ecx
	movl	%ebx, %eax
	movl	%ecx, %ebx
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsb
	movl	-48(%ebp), %eax
	addl	%eax, -32(%ebp)
	jmp	L87
L92:
	nop
L87:
	incl	-36(%ebp)
L82:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	12(%eax), %eax
	cmpl	-36(%ebp), %eax
	jg	L88
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	24(%eax), %ecx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	12(%eax), %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%ecx, 16(%esp)
	movl	-28(%ebp), %ecx
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	-88(%ebp), %eax
	movl	%eax, (%esp)
	call	_arch_get_register_parms
L79:
	movl	8(%ebp), %eax
	movl	8(%eax), %ecx
	movl	8(%ebp), %eax
	movl	4(%eax), %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%edx, 12(%esp)
	leal	-64(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	-28(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	*%ecx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	4(%eax), %eax
	testl	%eax, %eax
	je	L78
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	8(%eax), %eax
	leal	-64(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	-80(%ebp), %eax
	movl	%eax, (%esp)
	call	_set_value
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	8(%eax), %eax
	cmpl	$5, %eax
	je	L89
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	8(%eax), %eax
	cmpl	$6, %eax
	jne	L90
L89:
	fldl	-72(%ebp)
/APP
 # 509 "cinvoke.c" 1
	fld %st
 # 0 "" 2
/NO_APP
	fstp	%st(0)
L90:
/APP
 # 509 "cinvoke.c" 1
	mov -80(%ebp), %eax; 			mov -76(%ebp), %edx
 # 0 "" 2
/NO_APP
	jmp	L78
L91:
	nop
L78:
	addl	$108, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE26:
	.globl	_cinv_callback_create
	.def	_cinv_callback_create;	.scl	2;	.type	32;	.endef
_cinv_callback_create:
LFB27:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$52, %esp
	movl	$16, (%esp)
	.cfi_offset 3, -12
	call	_malloc
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L94
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L95
L94:
	movl	12(%ebp), %eax
	movl	12(%eax), %ebx
	movl	12(%ebp), %eax
	movl	24(%eax), %ecx
	movl	12(%ebp), %eax
	movl	(%eax), %edx
	movl	12(%ebp), %eax
	movl	32(%eax), %eax
	cwtl
	movl	%ebx, 20(%esp)
	movl	%ecx, 16(%esp)
	movl	%edx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$_cinv_cbthunk, (%esp)
	call	_arch_callback_stub
	movl	-12(%ebp), %edx
	movl	%eax, 12(%edx)
	movl	-12(%ebp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	jne	L96
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L95
L96:
	movl	-12(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	movl	-12(%ebp), %eax
	movl	16(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	-12(%ebp), %eax
	movl	20(%ebp), %edx
	movl	%edx, 8(%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-12(%ebp), %eax
L95:
	addl	$52, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE27:
	.globl	_cinv_callback_getentrypoint
	.def	_cinv_callback_getentrypoint;	.scl	2;	.type	32;	.endef
_cinv_callback_getentrypoint:
LFB28:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	12(%ebp), %eax
	movl	12(%eax), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE28:
	.globl	_cinv_callback_delete
	.def	_cinv_callback_delete;	.scl	2;	.type	32;	.endef
_cinv_callback_delete:
LFB29:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, (%esp)
	call	_arch_free_stub
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE29:
	.def	_arch_free_errstr;	.scl	2;	.type	32;	.endef
	.def	_GetWin32ErrMsg;	.scl	2;	.type	32;	.endef
	.def	_malloc;	.scl	2;	.type	32;	.endef
	.def	_free;	.scl	2;	.type	32;	.endef
	.def	_arch_library_create;	.scl	2;	.type	32;	.endef
	.def	_arch_library_get_entrypoint;	.scl	2;	.type	32;	.endef
	.def	_arch_library_delete;	.scl	2;	.type	32;	.endef
	.def	_tolower;	.scl	2;	.type	32;	.endef
	.def	_arch_size_char;	.scl	2;	.type	32;	.endef
	.def	_arch_size_short;	.scl	2;	.type	32;	.endef
	.def	_arch_size_int;	.scl	2;	.type	32;	.endef
	.def	_arch_size_long;	.scl	2;	.type	32;	.endef
	.def	_arch_size_extralong;	.scl	2;	.type	32;	.endef
	.def	_arch_size_float;	.scl	2;	.type	32;	.endef
	.def	_arch_size_double;	.scl	2;	.type	32;	.endef
	.def	_arch_size_ptr;	.scl	2;	.type	32;	.endef
	.def	_fprintf;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_char;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_short;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_int;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_long;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_extralong;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_float;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_double;	.scl	2;	.type	32;	.endef
	.def	_arch_getval_ptr;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_char;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_short;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_int;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_long;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_extralong;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_float;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_double;	.scl	2;	.type	32;	.endef
	.def	_arch_setval_ptr;	.scl	2;	.type	32;	.endef
	.def	_arch_is_register_parm;	.scl	2;	.type	32;	.endef
	.def	_arch_set_register_parms;	.scl	2;	.type	32;	.endef
	.def	_arch_get_register_parms;	.scl	2;	.type	32;	.endef
	.def	_arch_callback_stub;	.scl	2;	.type	32;	.endef
	.def	_arch_free_stub;	.scl	2;	.type	32;	.endef
