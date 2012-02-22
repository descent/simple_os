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
  mov $0xfffb, %sp # why not setting 0xffff to %sp

#  call disp_str
  call init_bss_asm
# for bss
#  movw $0x0, %ax
#  movw %ax, %ds
  call p
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

.data
#LABEL_STACK:
#.space  512, 0


#.section ss
#  mov %ax, %es
#.org 510
#.word 0xaa55

