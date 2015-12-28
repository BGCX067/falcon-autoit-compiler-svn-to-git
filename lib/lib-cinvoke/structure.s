	.file	"structure.c"
	.text
	.globl	_cinv_structure_create
	.def	_cinv_structure_create;	.scl	2;	.type	32;	.endef
_cinv_structure_create:
LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$16, (%esp)
	call	_malloc
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L2
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L3
L2:
	movl	$_str_eq, 8(%esp)
	movl	$_str_hash, 4(%esp)
	movl	$16, (%esp)
	call	_create_hashtable
	movl	-12(%ebp), %edx
	movl	%eax, (%edx)
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L4
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L3
L4:
	movl	-12(%ebp), %eax
	movl	$0, 4(%eax)
	movl	-12(%ebp), %eax
	movl	$0, 8(%eax)
	movl	-12(%ebp), %eax
	movl	$1, 12(%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-12(%ebp), %eax
L3:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE3:
	.section .rdata,"dr"
	.align 4
LC0:
	.ascii "the structure is already finished\0"
	.align 4
LC1:
	.ascii "the structure already contains a member by that name\0"
	.text
	.globl	_cinv_structure_addmember_value
	.def	_cinv_structure_addmember_value;	.scl	2;	.type	32;	.endef
_cinv_structure_addmember_value:
LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$48, %esp
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	L6
	.cfi_offset 3, -16
	.cfi_offset 6, -12
	movl	$0, 12(%esp)
	movl	$LC0, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L7
L6:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_search
	testl	%eax, %eax
	je	L8
	movl	$0, 12(%esp)
	movl	$LC1, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L7
L8:
	movl	16(%ebp), %eax
	movl	%eax, (%esp)
	call	_strdup
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L9
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L7
L9:
	movl	$12, (%esp)
	call	_malloc
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	jne	L10
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L7
L10:
	leal	-24(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-20(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	-28(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	20(%ebp), %eax
	movl	%eax, (%esp)
	call	_get_size
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	movl	-24(%ebp), %ecx
	cltd
	idivl	%ecx
	movl	%edx, %eax
	testl	%eax, %eax
	je	L11
	movl	12(%ebp), %eax
	movl	4(%eax), %ecx
	movl	-24(%ebp), %ebx
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	movl	-24(%ebp), %esi
	cltd
	idivl	%esi
	movl	%edx, %eax
	movl	%ebx, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	leal	(%ecx,%eax), %edx
	movl	12(%ebp), %eax
	movl	%edx, 4(%eax)
L11:
	movl	-16(%ebp), %eax
	movl	$0, (%eax)
	movl	-16(%ebp), %eax
	movl	20(%ebp), %edx
	movl	%edx, 4(%eax)
	movl	12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	-12(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_insert
	movl	12(%ebp), %eax
	movl	12(%eax), %edx
	movl	-24(%ebp), %eax
	cmpl	%eax, %edx
	jge	L12
	movl	-24(%ebp), %edx
	movl	12(%ebp), %eax
	movl	%edx, 12(%eax)
L12:
	movl	12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-20(%ebp), %eax
	addl	%eax, %edx
	movl	12(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
L7:
	addl	$48, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE4:
	.section .rdata,"dr"
	.align 4
LC2:
	.ascii "the child structure is not finished\0"
	.text
	.globl	_cinv_structure_addmember_struct
	.def	_cinv_structure_addmember_struct;	.scl	2;	.type	32;	.endef
_cinv_structure_addmember_struct:
LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	L14
	.cfi_offset 3, -12
	movl	$0, 12(%esp)
	movl	$LC0, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L15
L14:
	movl	20(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	jne	L16
	movl	$0, 12(%esp)
	movl	$LC2, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L15
L16:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_search
	testl	%eax, %eax
	je	L17
	movl	$0, 12(%esp)
	movl	$LC1, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L15
L17:
	movl	16(%ebp), %eax
	movl	%eax, (%esp)
	call	_strdup
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L18
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L15
L18:
	movl	$12, (%esp)
	call	_malloc
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	jne	L19
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	movl	$0, %eax
	jmp	L15
L19:
	movl	20(%ebp), %eax
	movl	12(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	cltd
	idivl	-20(%ebp)
	movl	%edx, %eax
	testl	%eax, %eax
	je	L20
	movl	12(%ebp), %eax
	movl	4(%eax), %ecx
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	cltd
	idivl	-20(%ebp)
	movl	%edx, %eax
	movl	-20(%ebp), %edx
	movl	%edx, %ebx
	subl	%eax, %ebx
	movl	%ebx, %eax
	leal	(%ecx,%eax), %edx
	movl	12(%ebp), %eax
	movl	%edx, 4(%eax)
L20:
	movl	-16(%ebp), %eax
	movl	20(%ebp), %edx
	movl	%edx, (%eax)
	movl	-16(%ebp), %eax
	movl	$0, 4(%eax)
	movl	12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, 8(%eax)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	-12(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_insert
	movl	12(%ebp), %eax
	movl	12(%eax), %eax
	cmpl	-20(%ebp), %eax
	jge	L21
	movl	12(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	%edx, 12(%eax)
L21:
	movl	12(%ebp), %eax
	movl	4(%eax), %edx
	movl	20(%ebp), %eax
	movl	4(%eax), %eax
	addl	%eax, %edx
	movl	12(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
L15:
	addl	$36, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE5:
	.globl	_cinv_structure_finish
	.def	_cinv_structure_finish;	.scl	2;	.type	32;	.endef
_cinv_structure_finish:
LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	je	L23
	.cfi_offset 3, -16
	.cfi_offset 6, -12
	movl	$0, 12(%esp)
	movl	$LC0, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L24
L23:
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	movl	12(%ebp), %edx
	movl	12(%edx), %ecx
	cltd
	idivl	%ecx
	movl	%edx, %eax
	testl	%eax, %eax
	je	L25
	movl	12(%ebp), %eax
	movl	4(%eax), %ecx
	movl	12(%ebp), %eax
	movl	12(%eax), %ebx
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	movl	12(%ebp), %edx
	movl	12(%edx), %esi
	cltd
	idivl	%esi
	movl	%edx, %eax
	movl	%ebx, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	leal	(%ecx,%eax), %edx
	movl	12(%ebp), %eax
	movl	%edx, 4(%eax)
L25:
	movl	12(%ebp), %eax
	movl	$1, 8(%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
L24:
	addl	$16, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE6:
	.section .rdata,"dr"
LC3:
	.ascii "the structure is not finished\0"
	.text
	.globl	_cinv_structure_getsize
	.def	_cinv_structure_getsize;	.scl	2;	.type	32;	.endef
_cinv_structure_getsize:
LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	jne	L27
	movl	$0, 12(%esp)
	movl	$LC3, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L28
L27:
	movl	12(%ebp), %eax
	movl	4(%eax), %edx
	movl	16(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
L28:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE7:
	.globl	_cinv_structure_create_instance
	.def	_cinv_structure_create_instance;	.scl	2;	.type	32;	.endef
_cinv_structure_create_instance:
LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	12(%ebp), %eax
	movl	8(%eax), %eax
	testl	%eax, %eax
	jne	L30
	movl	$0, 12(%esp)
	movl	$LC3, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L31
L30:
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_malloc
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L32
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_nomem
	jmp	L33
L32:
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
L33:
	movl	-12(%ebp), %eax
L31:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE8:
	.section .rdata,"dr"
	.align 4
LC4:
	.ascii "cannot find member with that name\0"
	.text
	.globl	_get_member
	.def	_get_member;	.scl	2;	.type	32;	.endef
_get_member:
LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	16(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_search
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L35
	movl	$0, 12(%esp)
	movl	$LC4, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$-1, %eax
	jmp	L36
L35:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	L37
	movl	24(%ebp), %eax
	movl	$1, (%eax)
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	movl	4(%eax), %edx
	movl	20(%ebp), %eax
	movl	%edx, (%eax)
	jmp	L38
L37:
	movl	24(%ebp), %eax
	movl	$0, (%eax)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	leal	-16(%ebp), %edx
	movl	%edx, 12(%esp)
	movl	20(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	-16(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_get_size
L38:
	movl	-12(%ebp), %eax
	movl	8(%eax), %eax
L36:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE9:
	.section .rdata,"dr"
LC5:
	.ascii "value_ptr cannot be NULL\0"
	.text
	.globl	_cinv_structure_instance_setvalue
	.def	_cinv_structure_instance_setvalue;	.scl	2;	.type	32;	.endef
_cinv_structure_instance_setvalue:
LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$60, %esp
	movl	16(%ebp), %eax
	movl	%eax, -28(%ebp)
	cmpl	$0, 24(%ebp)
	jne	L40
	.cfi_offset 3, -20
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	movl	$0, 12(%esp)
	movl	$LC5, 8(%esp)
	movl	$87, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_set_error
	movl	$0, %eax
	jmp	L41
L40:
	leal	-40(%ebp), %eax
	movl	%eax, 16(%esp)
	leal	-36(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_get_member
	movl	%eax, -32(%ebp)
	cmpl	$-1, -32(%ebp)
	jne	L42
	movl	$0, %eax
	jmp	L41
L42:
	movl	-32(%ebp), %eax
	addl	%eax, -28(%ebp)
	movl	-36(%ebp), %eax
	movl	-28(%ebp), %edx
	movl	24(%ebp), %ecx
	movl	%ecx, %ebx
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsb
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	$1, %eax
L41:
	addl	$60, %esp
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
LFE10:
	.globl	_cinv_structure_instance_getvalue
	.def	_cinv_structure_instance_getvalue;	.scl	2;	.type	32;	.endef
_cinv_structure_instance_getvalue:
LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$56, %esp
	movl	16(%ebp), %eax
	movl	%eax, -12(%ebp)
	leal	-24(%ebp), %eax
	movl	%eax, 16(%esp)
	leal	-20(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_get_member
	movl	%eax, -16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L44
	movl	$0, %eax
	jmp	L45
L44:
	movl	-16(%ebp), %eax
	addl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_context_clear_error
	movl	-12(%ebp), %eax
L45:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE11:
	.globl	_cinv_structure_delete_instance
	.def	_cinv_structure_delete_instance;	.scl	2;	.type	32;	.endef
_cinv_structure_delete_instance:
LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
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
LFE12:
	.globl	_cinv_structure_delete
	.def	_cinv_structure_delete;	.scl	2;	.type	32;	.endef
_cinv_structure_delete:
LFB13:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_hashtable_count
	testl	%eax, %eax
	je	L48
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	leal	-24(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_iterator
L49:
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_hashtable_iterator_key
	movl	%eax, (%esp)
	call	_free
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_hashtable_iterator_value
	movl	%eax, (%esp)
	call	_free
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_hashtable_iterator_remove
	testl	%eax, %eax
	jne	L49
L48:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	_hashtable_destroy
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
LFE13:
	.def	_str_eq;	.scl	2;	.type	32;	.endef
	.def	_str_hash;	.scl	2;	.type	32;	.endef
	.def	_malloc;	.scl	2;	.type	32;	.endef
	.def	_context_set_nomem;	.scl	2;	.type	32;	.endef
	.def	_create_hashtable;	.scl	2;	.type	32;	.endef
	.def	_free;	.scl	2;	.type	32;	.endef
	.def	_context_clear_error;	.scl	2;	.type	32;	.endef
	.def	_context_set_error;	.scl	2;	.type	32;	.endef
	.def	_hashtable_search;	.scl	2;	.type	32;	.endef
	.def	_strdup;	.scl	2;	.type	32;	.endef
	.def	_get_size;	.scl	2;	.type	32;	.endef
	.def	_hashtable_insert;	.scl	2;	.type	32;	.endef
	.def	_hashtable_count;	.scl	2;	.type	32;	.endef
	.def	_hashtable_iterator;	.scl	2;	.type	32;	.endef
	.def	_hashtable_iterator_key;	.scl	2;	.type	32;	.endef
	.def	_hashtable_iterator_value;	.scl	2;	.type	32;	.endef
	.def	_hashtable_iterator_remove;	.scl	2;	.type	32;	.endef
	.def	_hashtable_destroy;	.scl	2;	.type	32;	.endef
