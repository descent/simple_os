# init c++ runtime

.code16
.extern __bss_start__
.extern __bss_end__
.extern __start_ctors
.extern __end_ctors
.extern cpp_main

.text
.global _start
_start:
  xchg %bx, %bx
#  mov %cs, %ax
#  mov %ax, %ds
#  mov %ax, %es

# setup stack
#  mov %ax, %ss
#  mov $0xffff, %sp

#  call disp_str
#  call init_bss_asm # in dos need not init bss by myself
  mov $__start_ctors, %bx # call _GLOBAL__I_XX invoke global object ctor, in dos environment use 16 bit address
  calll *(%bx)
  calll cpp_main
#  call disp_str2
  mov     $0x4c00, %ax
  int     $0x21   # 回到 DOS

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

# call global object ctor

# init bss
init_bss_asm:
  movw $__bss_end__, %di    /* Destination */
  movw $__bss_start__, %si   /* Source */
  movw $0x0, %ax
  movw %ax, %gs
  jmp 2f
1:
  movw %si, %ax
  movb $0x1, %gs:(%eax)
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

