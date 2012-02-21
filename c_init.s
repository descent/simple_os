.code16
.extern __bss_start__
.extern __bss_end__

.text
.global _start
_start:
  mov %cs, %ax
  mov %ax, %ds
  mov %ax, %es

# setup stack
  mov %ax, %ss
  mov $0xffff, %sp

  call disp_str
  call p
  call disp_str2
  jmp .
disp_str: 
    mov     $BootMessage, %ax
    mov     %ax,%bp 
    mov     $16,%cx
    mov     $0x1301,%ax
    mov     $0x00c,%bx
    mov     $0,%dl
    int     $0x10
    ret
BootMessage:.ascii "Hello, c-env world!"
disp_str2: 
    mov     $str2, %ax
    mov     %ax,%bp 
    mov     $5,%cx
    mov     $0x1301,%ax
    mov     $0x00c,%bx
    mov     $0,%dl
    int     $0x10
    ret
str2:.ascii "after"


# init bss

#.bss
_bss_start_:.word   __bss_start__
_bss_end_:.word   __bss_end__

.data
#LABEL_STACK:
#.space  512, 0


#.section ss
#  mov %ax, %es
#.org 510
#.word 0xaa55

