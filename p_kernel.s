# kernel loader should enters protected mode
# the code will use selector:offset to access address
# for c runtime, maybe initialize bss and other c runtime environment again.
# reset gdt ptr
# setup page
# setup idt

.equ SELECTOR_KERNEL_CS, 8
.equ ZERO, 0

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


.extern gdt_ptr
.extern idt_ptr
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
#  lgdt gdt_ptr

  jmp csinit
csinit:
  call init_bss_asm
  #call asm_init_8259a
  call init_8259a
  call init_idt_by_c
  lidt idt_ptr

#  movl $spurious_handler, %eax
#  movw %ax, (IDT6)
  

#  call startc
  #jmp .
  sti
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


# init bss
init_bss_asm:
  movw $__bss_end__, %di    /* Destination */
  movw $__bss_start__, %si   /* Source */
#  movw $0x0, %ax
#  movw %ax, %gs
  jmp 2f
1:
  movw %si, %ax
  movb $0x0, (%eax)
#  movb $0x1, (%eax)
#  movb $0x1, %ds:(%eax)
  add $1, %si

2:
  cmpw %di, %si
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
  HW_INT_MASTER $0

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
