# kernel loader should enters protected mode
# the code will use selector:offset to access address
# for c runtime, maybe initialize bss and other c runtime environment again.
# reset gdt ptr
# setup page
# setup idt



.equ SELECTOR_KERNEL_CS, 8
.equ SELECTOR_FLAT_RW, 8*2
.equ LDT_SEL_OFFSET, 18*4
.equ P_STACKTOP, 18*4
.equ TSS3_S_SP0, 4
.equ SELECTOR_TSS, 4*8
.equ EOI, 0x20
.equ INT_M_CTL, 0x20 # I/O port for interrupt controller        <Master>

.equ PAGE_DIR_BASE, 0x200000
.equ PAGE_TBL_BASE, 0x201000

#----------------------------------------------------------------------------
# 分頁機制使用的常量說明
#----------------------------------------------------------------------------
.equ PG_P            ,     1       # 頁存在屬性位
.equ PG_RWR          ,     0       # R/W 屬性位值, 讀/執行
.equ PG_RWW          ,     2       # R/W 屬性位值, 讀/寫/執行
.equ PG_USS          ,     0       # U/S 屬性位值, 系統級
.equ PG_USU          ,     4       # U/S 屬性位值, 用戶級

.equ PG_ATTR , PG_P  | PG_USU | PG_RWW
.equ PG_ATTR1 , PAGE_TBL_BASE | PG_P  | PG_USU | PG_RWW



.set SelectorCode32, 8
.set DA_386IGate, 0x8E    /* 32-bit Interrupt Gate */


# Gate Descriptor data structure.
#   Usage: Gate Selector, Offset, PCount, Attr
#    Selector:  2byte
#    Offset:    4byte
#    PCount:    byte
#    Attr:      byte 
.macro Gate  Selector, Offset, PCount, Attr
    .2byte     (\Offset & 0xFFFF)
    .2byte     \Selector
    .2byte     (\PCount & 0x1F) | ((\Attr << 8) & 0xFF00)
    .2byte     ((\Offset >> 16) & 0xFFFF)
.endm

.extern k_reenter
.extern gdt_ptr
.extern idt_ptr
.extern __bss_start__ 
.extern __bss_end__ 
.extern memsize
#.extern	exception_handler

.global	divide_error
.global	single_step_exception
.global	nmi
.global	breakpoint_exception
.global	overflow
.global	bounds_check
.global	inval_opcode
.global	copr_not_available
.global	double_fault
.global	copr_seg_overrun
.global	inval_tss
.global	segment_not_present
.global	stack_exception
.global	general_protection
.global	page_fault
.global	copr_error
.global	spurious_handler

.global	hwint00
.global	hwint01
.global	hwint02
.global	hwint03
.global	hwint04
.global	hwint05
.global	hwint06
.global	hwint07
.global	hwint08
.global	hwint09
.global	hwint10
.global	hwint11
.global	hwint12
.global	hwint13
.global	hwint14
.global	hwint15

.code32
.text
.global _start
_start:
  #call switch_mode
  mov $STACK_TOP, %esp
  call init_bss_asm
  call c_test
  cli
  sgdt gdt_ptr
  call init_protected_mode_by_c
#  call asm_init_8259a
#  movw $0x8, (IDT6+2)
  #lidt asm_idt_ptr
  #jmp .
  nop
  nop
#  sidt s_idt_ptr
  lgdt gdt_ptr

  jmp csinit
csinit:
  #call asm_init_8259a
  call init_8259a
  call init_idt_by_c
  lidt idt_ptr
  call setup_paging
  call init_tss

  xor %eax, %eax
  mov $SELECTOR_TSS, %ax
  ltr %ax
  #ud2

  call kernel_main

#  movl $spurious_handler, %eax
#  movw %ax, (IDT6)
  

#  call startc
  #jmp .
  #sti
  #ud2
  #jmp $0x40,$0
  mov $0xc,%ah
  mov $'K',%al
  mov %ax,%gs:((80*0+39)*2)
  jmp .

# prefix p means protected mode
.globl p_asm_memcpy
p_asm_memcpy:
    pushl   %ebp
    mov     %esp, %ebp

    pushl   %esi
    pushl   %edi
    pushl   %ecx

#    mov $0x7000, %ax
#    mov %ax, %fs

    mov     8(%ebp), %edi    /* Destination */
    mov     12(%ebp), %esi   /* Source */
    mov     16(%ebp), %ecx   /* Counter */
MemCpy.1:
    cmp     $0, %ecx  /* Loop counter */
    jz      MemCpy.2
    movb    %ds:(%esi), %al
    inc     %esi
    movb    %al, %ds:(%edi) # note use %ds segment register
    inc     %edi
    dec     %ecx
    jmp     MemCpy.1
MemCpy.2:
    mov     8(%ebp), %eax
    pop     %ecx
    pop     %edi
    pop     %esi
    mov     %ebp, %esp
    pop     %ebp
    ret

.globl p_asm_memset
p_asm_memset:
    pushl   %ebp
    mov     %esp, %ebp

    pushl   %esi
    pushl   %edi
    pushl   %ecx

    mov     8(%ebp), %edi    /* Destination */
    mov     12(%ebp), %edx   /* char to be put */
    mov     16(%ebp), %ecx   /* Counter */
1:
    cmp     $0, %ecx  /* Loop counter */
    jz      2f

    movb    %dl, (%edi)
    inc     %edi

    dec     %ecx
    jmp     1b
2:
    mov     8(%ebp), %eax
    pop     %ecx
    pop     %edi
    pop     %esi
    mov     %ebp, %esp
    pop     %ebp
    ret

# init bss
init_bss_asm:
  movl $__bss_end__, %edi    /* Destination */
  movl $__bss_start__, %esi   /* Source */
#  movw $0x0, %ax
#  movw %ax, %gs
  jmp 2f
1:
  movl %esi, %eax
  movb $0x0, (%eax)
#  movb $0x1, (%eax)
#  movb $0x1, %ds:(%eax)
  addl $1, %esi

2:
  cmpl %edi, %esi
  jne 1b
  ret


# copy from: oranges_os/chapter5/h/kernel/kernel.asm
divide_error:
	push	$0xFFFFFFFF	
	push	$0	
	jmp	exception

single_step_exception:
	push	$0xFFFFFFFF
	push	$1	
	jmp	exception
nmi:
	push	$0xFFFFFFFF
	push	$2	
	jmp	exception
breakpoint_exception:
	push	$0xFFFFFFFF
	push	$3	
	jmp	exception
overflow:
	push	$0xFFFFFFFF
	push	$4	
	jmp	exception
bounds_check:
	push	$0xFFFFFFFF
	push	$5
	jmp	exception
inval_opcode:
	push	$0xFFFFFFFF
	push	$6
	jmp	exception
copr_not_available:
	push	$0xFFFFFFFF
	push	$7
	jmp	exception
double_fault:
	push	$8
	jmp	exception
copr_seg_overrun:
	push	$0xFFFFFFFF
	push	$9
	jmp	exception

inval_tss:
	push	$10
	jmp	exception
segment_not_present:
	push	$11
	jmp	exception
stack_exception:
	push	$12
	jmp	exception
general_protection:
	push	$13
	jmp	exception
page_fault:
	push	$14
	jmp	exception
copr_error:
	push	$0xFFFFFFFF
	push	$16
	jmp	exception


exception:
	call	exception_handler
	add	$4*2, %esp	# 讓堆疊頂指向 EIP，堆疊中從頂向下依次是：EIP、CS、EFLAGS
	hlt

io_delay:
  nop
  nop
  nop
  nop
  ret
asm_init_8259a:
  mov $0x11, %al
  outb %al, $0x20
  call io_delay

  outb %al, $0xa0
  call io_delay

  mov $0x20, %al
  outb %al, $0x21
  call io_delay

  mov $0x28, %al
  outb %al, $0xa1
  call io_delay

  mov $0x04, %al
  outb %al, $0x21
  call io_delay

  mov $0x02, %al
  outb %al, $0xa1
  call io_delay

  mov $0x01, %al
  outb %al, $0x21
  call io_delay

  out %al, $0xa1
  call io_delay

  mov $0b11111110, %al
  out %al, $0x21
  call io_delay

  mov $0b11111111, %al
  out %al, $0xa1
  call io_delay

  ret

spurious_handler:
.set spurious_handler_offset, (.)
  call exception_handler
  mov $0x0c, %ah
  mov $'%', %al
  mov %ax, %gs:((80 * 1 + 75) * 2) # (1, 75)
  jmp .
  iretl

#.align 32
##.data
#LABEL_IDT:
#.rept 32
##Gate SelectorCode32, spurious_handler_offset, 0, DA_386IGate
#.endr
##Gate SelectorCode32, spurious_handler, 0, DA_386IGate
#IDT0: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#IDT1: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#IDT2: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#IDT3: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#IDT4: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#IDT5: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#IDT6: Gate SelectorCode32, 0x000050fb, 0, DA_386IGate
#
##Gate SelectorCode32, clock_handler_offset, 0, DA_386IGate # 0x20
##Gate SelectorCode32, spurious_handler_offset, 0, DA_386IGate
#
#.rept 95
##Gate SelectorCode32, spurious_handler_offset, 0, DA_386IGate
#.endr
#
##Gate SelectorCode32, user_int_handler_offset, 0, DA_386IGate # 0x80
#
#.set asm_idt_len, (. - LABEL_IDT)  /* IDT Length */
#
#asm_idt_ptr: .2byte  (asm_idt_len - 1)  /* IDT Limit */
#         .4byte  LABEL_IDT             /* IDT Base */
#
#
#
#LABEL_TEST:
#  s_idt_ptr: .4byte 0x0
#  t1: .8byte 0x1


.macro HW_INT_MASTER irq_no
    pushl \irq_no
    call spurious_irq
    add $4, %esp
    hlt
.endm

.macro HW_INT_SLAVE irq_no
    pushl \irq_no
    call spurious_irq
    add $4, %esp
    hlt
.endm

.align 16
hwint00:
  #HW_INT_MASTER $0

  sub $4, %esp
   
  pushal
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs

  mov %ss, %dx
  mov %dx, %ds
  mov %dx, %es


  incb %gs:(0)
  mov $EOI, %al
  outb %al, $INT_M_CTL

  # check k_reenter
  incl (k_reenter)
  cmpl $0, (k_reenter)
  jne .re_enter

  mov $STACK_TOP, %esp
  sti

  pushl 0
  call clock_handler
  addl $4, %esp

  addl $2, (VB)
  pushl VB
  pushl $TIMER_STR
  call s32_print
  add $8, %esp

  pushl $10
  call loop_delay
  add $4, %esp

  cli 

  movl (ready_process), %esp
  lldt LDT_SEL_OFFSET(%esp)

  lea P_STACKTOP(%esp), %eax
  movl %eax, (tss+TSS3_S_SP0)

.re_enter:
  decl (k_reenter)

  popl %gs
  popl %fs
  popl %es
  popl %ds
  popal

  add $4, %esp

  iretl

.align 16
hwint01:
  HW_INT_MASTER $1
.align 16
hwint02:
  HW_INT_MASTER $2
.align 16
hwint03:
  HW_INT_MASTER $3
.align 16
hwint04:
  HW_INT_MASTER $4
.align 16
hwint05:
  HW_INT_MASTER $5
.align 16
hwint06:
  HW_INT_MASTER $6
.align 16
hwint07:
  HW_INT_MASTER $7
.align 16
hwint08:
  HW_INT_SLAVE $8
.align 16
hwint09:
  HW_INT_SLAVE $9
.align 16
hwint10:
  HW_INT_SLAVE $10
.align 16
hwint11:
  HW_INT_SLAVE $11
.align 16
hwint12:
  HW_INT_SLAVE $12
.align 16
hwint13:
  HW_INT_SLAVE $13
.align 16
hwint14:
  HW_INT_SLAVE $14
.align 16
hwint15:
  HW_INT_SLAVE $15

.globl restart
restart: # if timer isr run, the process switch will ok or fail ???
  nop
  nop
  movl (ready_process), %esp
  lldt LDT_SEL_OFFSET(%esp)
  lea P_STACKTOP(%esp), %eax
  #lea 0x1000(%esp), %eax
  movl %eax, (tss+TSS3_S_SP0)

  popl %gs
  popl %fs
  popl %es
  popl %ds
  popal
  add $4, %esp
  iretl

.globl io_load_eflags
io_load_eflags:
  pushfl
  popl %eax
  retl

.globl io_store_eflags
io_store_eflags:
  mov 4(%esp), %eax
  pushl %eax
  popfl
  retl

.globl load_cr0
load_cr0:
  mov %cr0, %eax
  retl
.globl store_cr0
store_cr0:
  mov 4(%esp), %eax
  mov %eax, %cr0 
  retl



.global memtest_sub
memtest_sub:	# unsigned int memtest_sub(unsigned int start, unsigned int end)
  pushl	%edi    # iEBX, ESI, EDI àg¢½¢ÌÅj
  pushl	%esi
  pushl	%ebx
  mov $0xaa55aa55, %esi			# pat0 = 0xaa55aa55;
  mov $0x55aa55aa, %edi			# pat1 = 0x55aa55aa;
  mov 12+4(%ESP), %eax			# i = start;
mts_loop:
  mov %eax, %ebx
  add 0xffc, %ebx				# p = i + 0xffc;
  mov (%ebx), %edx				# old = *p;
  mov %esi, (%ebx)
  xorl $0xffffffff, (%ebx)	# *p ^= 0xffffffff;
  cmp (%ebx), %edi   # if (*p != pat1) goto fin;
  jne mts_fin
  xorl $0xffffffff, (%ebx)	# *p ^= 0xffffffff;
  cmp (%ebx), %esi	# if (*p != pat0) goto fin;
  jne		mts_fin
  mov %edx, (%ebx)				# *p = old;
  add $0x1000, %eax				# i += 0x1000;
  cmp 12+8(%esp), %eax			# if (i <= end) goto mts_loop;
  jbe		mts_loop
  pop %ebx
  pop %esi
  pop %edi
  retl
mts_fin:
  mov %edx, (%ebx)				# *p = old;
  pop %ebx
  pop %esi
  pop %edi
  retl

# ref: oranges_os/chapter6/d/boot/loader.asm
# 启动分页机制 --------------------------------------------------------------
setup_paging:
	# 根据記憶體大小计算应初始化多少PDE以及多少页表
	xor	%edx, %edx
	mov	(memsize), %eax
	mov	$0x400000, %ebx	# 400000h = 4M = 4096 * 1024, 一个页表对应的内存大小
	div	%ebx
	mov	%eax, %ecx	# 此时 ecx 为页表的个数，也即 PDE 应该的个数
	test	%edx, %edx
	jz	.no_remainder
	inc	%ecx		# 如果余数不为 0 就需增加一个页表
.no_remainder:
	push	%ecx		# 暂存页表个数

	# 为简化处理, 所有线性位址对应相等的物理位址. 并且不考虑内存空洞.

	# 首先初始化页目录
	mov	$SELECTOR_FLAT_RW, %ax
	mov	%ax, %es

	mov	$PAGE_DIR_BASE, %edi	# 此段首位址为 PageDirBase
	xor	%eax, %eax
#	mov	eax, PageTblBase | PG_P  | PG_USU | PG_RWW
	mov	$PG_ATTR1, %eax
1:
	stosl
	add	$4096, %eax		# 为了简化, 所有页表在内存中是连续的.
	loop	1b

	# 再初始化所有页表
	pop	%eax			# 页表个数
	mov	$1024, %ebx		# 每个页表 1024 个 PTE
	mul	%ebx
	mov	%eax, %ecx		# PTE个数 = 页表个数 * 1024
	mov	$PAGE_TBL_BASE, %edi	# 此段首位址为 PageTblBase
	xor	%eax, %eax
	#mov	$PG_P  | $PG_USU | $PG_RWW, %eax
	mov	$PG_ATTR, %eax
2:
	stosl
	add	$4096, %eax		# 每一页指向 4K 的空间
	loop	2b

	mov	$PAGE_DIR_BASE, %eax
	mov	%eax, %cr3
	mov	%cr0, %eax
	or	$0x80000000, %eax
	mov	%eax, %cr0
	jmp	3f
3:
	nop

	retl
# 分页机制启动完毕 ----------------------------------------------------------

.align 32
.data
mem_size: .int 0x0
TIMER_STR: .asciz "timer"
VB: .long (0xb8006+160)
.space  2048, 0
STACK_TOP:


