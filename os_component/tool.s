	.file	"tool.cpp"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.cfi_sections	.debug_frame
.globl vb
	.bss
	.align 4
	.type	vb, @object
	.size	vb, 4
vb:
	.zero	4
	.text
.globl _Z6get_vbv
	.type	_Z6get_vbv, @function
_Z6get_vbv:
.LFB0:
	.file 1 "tool.cpp"
	.loc 1 7 0
	.cfi_startproc
	pushl	%ebp
.LCFI0:
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
.LCFI1:
	.cfi_def_cfa_register 5
	.loc 1 10 0
	popl	%ebp
	ret
	.cfi_endproc
.LFE0:
	.size	_Z6get_vbv, .-_Z6get_vbv
.globl _Z5printc
	.type	_Z5printc, @function
_Z5printc:
.LFB1:
	.loc 1 13 0
	.cfi_startproc
	pushl	%ebp
.LCFI2:
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
.LCFI3:
	.cfi_def_cfa_register 5
	subl	$20, %esp
	movl	8(%ebp), %eax
	movb	%al, -20(%ebp)
.LBB2:
	.loc 1 17 0
#APP
# 17 "tool.cpp" 1
	xchg %bx, %bx
# 0 "" 2
	.loc 1 19 0
#NO_APP
	call	_Z6get_vbv
	movl	%eax, -4(%ebp)
	.loc 1 20 0
	movzbl	-20(%ebp), %edx
	movl	-4(%ebp), %eax
	movb	%dl, (%eax)
	addl	$1, -4(%ebp)
	.loc 1 21 0
	movl	-4(%ebp), %eax
	movb	$9, (%eax)
	addl	$1, -4(%ebp)
.LBE2:
	.loc 1 22 0
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	_Z5printc, .-_Z5printc
.globl _Z5printPKc
	.type	_Z5printPKc, @function
_Z5printPKc:
.LFB2:
	.loc 1 25 0
	.cfi_startproc
	pushl	%ebp
.LCFI4:
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
.LCFI5:
	.cfi_def_cfa_register 5
	subl	$20, %esp
.LBB3:
	.loc 1 26 0
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	.loc 1 27 0
	jmp	.L6
.L7:
	.loc 1 28 0
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	addl	$1, -4(%ebp)
	movl	%eax, (%esp)
	call	_Z5printc
.L6:
	.loc 1 27 0
	movl	-4(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	setne	%al
	testb	%al, %al
	jne	.L7
.LBE3:
	.loc 1 29 0
	leave
	ret
	.cfi_endproc
.LFE2:
	.size	_Z5printPKc, .-_Z5printPKc
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB3:
	.loc 1 29 0
	.cfi_startproc
	pushl	%ebp
.LCFI6:
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
.LCFI7:
	.cfi_def_cfa_register 5
	.loc 1 29 0
	cmpl	$1, 8(%ebp)
	jne	.L11
	cmpl	$65535, 12(%ebp)
	jne	.L11
	.loc 1 5 0
	movl	load_addr, %eax
	movl	$753824, %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, vb
.L11:
	.loc 1 29 0
	popl	%ebp
	ret
	.cfi_endproc
.LFE3:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__I_vb, @function
_GLOBAL__I_vb:
.LFB4:
	.loc 1 29 0
	.cfi_startproc
	pushl	%ebp
.LCFI8:
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
.LCFI9:
	.cfi_def_cfa_register 5
	subl	$8, %esp
	.loc 1 29 0
	movl	$65535, 4(%esp)
	movl	$1, (%esp)
	call	_Z41__static_initialization_and_destruction_0ii
	leave
	ret
	.cfi_endproc
.LFE4:
	.size	_GLOBAL__I_vb, .-_GLOBAL__I_vb
	.section	.ctors,"aw",@progbits
	.align 4
	.long	_GLOBAL__I_vb
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.long	.LFB0-.Ltext0
	.long	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI0-.Ltext0
	.long	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI1-.Ltext0
	.long	.LFE0-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST1:
	.long	.LFB1-.Ltext0
	.long	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI2-.Ltext0
	.long	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI3-.Ltext0
	.long	.LFE1-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST2:
	.long	.LFB2-.Ltext0
	.long	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI4-.Ltext0
	.long	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI5-.Ltext0
	.long	.LFE2-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST3:
	.long	.LFB3-.Ltext0
	.long	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI6-.Ltext0
	.long	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI7-.Ltext0
	.long	.LFE3-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST4:
	.long	.LFB4-.Ltext0
	.long	.LCFI8-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI8-.Ltext0
	.long	.LCFI9-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI9-.Ltext0
	.long	.LFE4-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
	.file 2 "type.h"
	.section	.debug_info
	.long	0x177
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF12
	.byte	0x4
	.long	.LASF13
	.long	.LASF14
	.long	.Ltext0
	.long	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF1
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x4
	.string	"u8"
	.byte	0x2
	.byte	0x7
	.long	0x44
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF3
	.uleb128 0x4
	.string	"u32"
	.byte	0x2
	.byte	0x9
	.long	0x5d
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF4
	.uleb128 0x5
	.byte	0x1
	.long	.LASF15
	.byte	0x1
	.byte	0x6
	.long	.LASF16
	.long	0x80
	.long	.LFB0
	.long	.LFE0
	.long	.LLST0
	.uleb128 0x6
	.byte	0x4
	.long	0x3a
	.uleb128 0x7
	.byte	0x1
	.long	.LASF6
	.byte	0x1
	.byte	0xc
	.long	.LASF7
	.long	.LFB1
	.long	.LFE1
	.long	.LLST1
	.long	0xc6
	.uleb128 0x8
	.string	"c"
	.byte	0x1
	.byte	0xc
	.long	0xc6
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x9
	.long	.LBB2
	.long	.LBE2
	.uleb128 0xa
	.string	"vb"
	.byte	0x1
	.byte	0x13
	.long	0x80
	.byte	0x2
	.byte	0x91
	.sleb128 -12
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF5
	.uleb128 0x7
	.byte	0x1
	.long	.LASF6
	.byte	0x1
	.byte	0x18
	.long	.LASF8
	.long	.LFB2
	.long	.LFE2
	.long	.LLST2
	.long	0x10c
	.uleb128 0x8
	.string	"s"
	.byte	0x1
	.byte	0x18
	.long	0x10c
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x9
	.long	.LBB3
	.long	.LBE3
	.uleb128 0xa
	.string	"p"
	.byte	0x1
	.byte	0x1a
	.long	0x10c
	.byte	0x2
	.byte	0x91
	.sleb128 -12
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.byte	0x4
	.long	0x112
	.uleb128 0xb
	.long	0xc6
	.uleb128 0xc
	.long	.LASF17
	.byte	0x1
	.long	.LFB3
	.long	.LFE3
	.long	.LLST3
	.long	0x14a
	.uleb128 0xd
	.long	.LASF9
	.byte	0x1
	.byte	0x1d
	.long	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xd
	.long	.LASF10
	.byte	0x1
	.byte	0x1d
	.long	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 4
	.byte	0x0
	.uleb128 0xe
	.long	.LASF18
	.byte	0x1
	.long	.LFB4
	.long	.LFE4
	.long	.LLST4
	.uleb128 0xf
	.long	.LASF11
	.byte	0x1
	.byte	0x3
	.long	0x52
	.byte	0x1
	.byte	0x1
	.uleb128 0x10
	.string	"vb"
	.byte	0x1
	.byte	0x5
	.long	0x80
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	vb
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x34
	.value	0x2
	.long	.Ldebug_info0
	.long	0x17b
	.long	0x64
	.string	"get_vb"
	.long	0x86
	.string	"print"
	.long	0xcd
	.string	"print"
	.long	0x169
	.string	"vb"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0x0
	.value	0x0
	.value	0x0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0x0
	.long	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF4:
	.string	"unsigned int"
.LASF2:
	.string	"unsigned char"
.LASF6:
	.string	"print"
.LASF15:
	.string	"get_vb"
.LASF7:
	.string	"_Z5printc"
.LASF12:
	.string	"GNU C++ 4.4.3"
.LASF17:
	.string	"__static_initialization_and_destruction_0"
.LASF5:
	.string	"char"
.LASF9:
	.string	"__initialize_p"
.LASF13:
	.string	"tool.cpp"
.LASF14:
	.string	"/home/descent/test/simple_os.test/cpp_runtime/pc"
.LASF11:
	.string	"load_addr"
.LASF3:
	.string	"short unsigned int"
.LASF0:
	.string	"signed char"
.LASF8:
	.string	"_Z5printPKc"
.LASF10:
	.string	"__priority"
.LASF1:
	.string	"short int"
.LASF18:
	.string	"_GLOBAL__I_vb"
.LASF16:
	.string	"_Z6get_vbv"
	.ident	"GCC: (Ubuntu 4.4.3-4ubuntu5.1) 4.4.3"
	.section	.note.GNU-stack,"",@progbits
