/*
 * the kernel loader will initialize c runtime and enter x86 protected mode 
 */

#include "pm.h"
#include "common.h"

.code16
.extern __bss_start__
.extern __bss_end__

.text
.global _start
_start:
  mov %cs, %ax
  mov %ax, %ds
  mov %ax, %es
  #movw $0x0, %ax
  movw %ax, %ds
  mov %ax, %ss
# setup stack
  mov $0xfff0, %sp # why not setting 0xffff to %sp, in ms dos, 0xfff0 is ok, 0xffb will get core dump

#  call disp_str
  call init_bss_asm
# for bss
#  movw $0x0, %ax
#  movw %ax, %ds
#  call p
  call start_c
#  call text_test
#  call disp_str2
  jmp .
#disp_str: 
#    mov     $BootMessage, %ax
#    mov     %ax,%bp 
#    mov     $16,%cx
#    mov     $0x1301,%ax
#    mov     $0x00c,%bx
#    mov     $0,%dl
#    int     $0x10
#    ret
#BootMessage:.ascii "Hello, c-env world!"
#disp_str2: 
#    mov     $str2, %ax
#    mov     %ax,%bp 
#    mov     $5,%cx
#    mov     $0x1301,%ax
#    mov     $0x00c,%bx
#    mov     $0,%dl
#    int     $0x10
#    ret
#str2:.ascii "after"

.globl write_mem8
write_mem8:
  pushl %ebp
  mov %esp, %ebp
  #movw $0xa000, %ax # graphic mode
  movw $0xb800, %ax # text mode
  movw %ax, %gs
  movl 8(%ebp), %ecx;
#  mov $0x0, %ecx;
  movb 12(%ebp), %al;
  movb %al, %gs:(%ecx)
  #movb $'A', %gs:(%ecx)
  popl %ebp
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
  movb $0x1, (%eax)
#  movb $0x1, %ds:(%eax)
  add $1, %si
  
2:
  cmpw %di, %si
  jne 1b
  ret

# copy from: write_os/src/chapter3/8/lib.h
# ex: 
# asm_memcpy((u8*)0x100, (u8 *)IMAGE_LMA, 512*3);
# copy IMAGE_LMA to 0x7000:0x100
.globl asm_memcpy
asm_memcpy:
    pushl   %ebp
    mov     %esp, %ebp

    pushl   %esi
    pushl   %edi
    pushl   %ecx

    mov $0x7000, %ax
    mov %ax, %fs

    mov     8(%ebp), %edi    /* Destination */
    mov     12(%ebp), %esi   /* Source */
    mov     16(%ebp), %ecx   /* Counter */
MemCpy.1:
    cmp     $0, %ecx  /* Loop counter */
    jz      MemCpy.2
    movb    %ds:(%esi), %al
    inc     %esi
    movb    %al, %fs:(%edi)
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

.globl asm_absolute_memcpy
# ex: 
# asm_absolute_memcpy((u8*)0x100, (u8 *)IMAGE_LMA, 512*3);
# copy IMAGE_LMA to 0x100
asm_absolute_memcpy:
    pushl   %ebp
    mov     %esp, %ebp

    pushl   %esi
    pushl   %edi
    pushl   %ecx

    mov $0, %ax
    mov %ax, %fs

    mov     8(%ebp), %edi    /* Destination */
    mov     12(%ebp), %esi   /* Source */
    mov     16(%ebp), %ecx   /* Counter */
1:
    cmp     $0, %ecx  /* Loop counter */
    jz      2f
    movb    %ds:(%esi), %al
    inc     %esi
    movb    %al, %fs:(%edi)
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

.data
LABEL_GDT: Descriptor 0, 0, 0 # empty descriptor
LABEL_DESC_FLAT_C: Descriptor 0, 0xfffff, DA_CR | DA_32 | DA_LIMIT_4K
LABEL_DESC_FLAT_RW: Descriptor 0, 0xfffff, DA_DRW | DA_32 | DA_LIMIT_4K
LABEL_DESC_VIDEO: Descriptor 0xb8000, 0xffff, DA_DRW | DA_DPL3
.set gdt_len, (. - LABEL_GDT)
gdt_ptr: .2byte (gdt_len - 1)
         .4byte loader_base_phy_addr+LABEL_GDT

.set sel_flat_c, (LABEL_DESC_FLAT_C - LABEL_GDT)
.set sel_flat_rw, (LABEL_DESC_FLAT_RW - LABEL_GDT)
.set sel_video, (LABEL_DESC_VIDEO - LABEL_GDT + SA_RPL3)
#.set sel_video, (LABEL_DESC_VIDEO - LABEL_GDT)

LABEL_STACK:
.space 512, 0
.set top_of_stack, (. - LABEL_STACK)

.set pm_phy_addr, (loader_base_phy_addr+LABEL_PM_START)


.text
.globl init_protected_mode
init_protected_mode:
  lgdt gdt_ptr

  # clear interrupt flag
  cli

  # open a20 line
  inb $0x92, %al
  orb $0b00000010, %al
  outb %al, $0x92

  # enable protect mode
  movl %cr0, %eax
  orl $1, %eax
  movl %eax, %cr0

  ljmpl $sel_flat_c, $pm_phy_addr

#  movw     16(%ebp), %ecx   /* Counter */
#1:
#    cmp     $0, %ecx  /* Loop counter */
#    jz      MemCpy.2
#    movb    %ds:(%esi), %al
#    inc     %esi
#    movb    %al, %es:(%edi)
#    inc     %edi
#    dec     %ecx
#    jmp     MemCpy.1
#2:
#    mov     8(%ebp), %eax
#    pop     %ecx
#    pop     %edi
#    pop     %esi
#    mov     %ebp, %esp
#    pop     %ebp
#    ret



#.bss
#_bss_start_:.word   __bss_start__
#_bss_end_:.word   __bss_end__

# gdt selector

#LABEL_STACK:
#.space  512, 0


#.section ss
#  mov %ax, %es
#.org 510
#.word 0xaa55

.align 32
.code32
LABEL_PM_START: 
  mov $sel_video, %ax
  mov %ax, %gs

  mov $sel_flat_rw, %ax
  mov %ax, %ds
  mov %ax, %ss
  mov %ax, %es
  mov %ax, %fs

  mov $top_of_stack, %esp

  mov $0, %edi
  movb $0xc, %ah
  movb $'Z', %al
  mov %ax, %gs:(%edi)

  call test_p

