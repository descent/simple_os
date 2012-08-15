# init c++ runtime

#define DOS

# How to make G++ preprocessor output a newline in a macro?
# ref: http://stackoverflow.com/questions/2271078/how-to-make-g-preprocessor-output-a-newline-in-a-macro
#define PRINT_REG(REG)  /*
*/  pushl $REG /*
*/  calll _Z9print_strPKc /*
*/  addl $4, %esp /*
*/  pushl $16 /*
*/  pushl %REG /*
*/  calll _Z13s16_print_intii /*
*/  addl $8, %esp /*
*/  pushl $CRLF /*
*/  calll _Z9print_strPKc /*
*/  addl $4, %esp

.code16
.extern __bss_start__
.extern __bss_end__
.extern __start_ctors
.extern __end_ctors
.extern cpp_main
.extern obj_count


.text
  jmp _start
.global _start
_start:
  xchg %bx, %bx
#ifndef DOS
  mov %cs, %ax
  mov %ax, %ds
  mov %ax, %es

#endif
#  movl $0, obj_count

  #calll _Z9print_strPKc

# setup stack
#  mov %ax, %ss
#  mov $0xffff, %sp

#  call disp_str

  pushl $cs
  calll _Z9print_strPKc
  addl $4, %esp

  pushl $16
  pushl %cs
  calll _Z13s16_print_intii
  addl $8, %esp

  pushl $CRLF
  calll _Z9print_strPKc
  addl $4, %esp

  PRINT_REG(ds)
  PRINT_REG(ss)

 call init_bss_asm # in dos need not init bss by myself

  # call _GLOBAL__I_XX invoke global object ctor, 
  # in dos environment use 16 or 32 bit address? look like 32 bit
  mov $__start_ctors, %ebx 
  calll *(%ebx)
  calll cpp_main
  calll g_dtors
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
  movw %ds, %ax
  movw %ax, %gs
  jmp 2f
1:
  movw %si, %ax
  movb $0x0, %gs:(%eax)
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
CRLF: .asciz "\r\n"
cs: .asciz "%CS: "
ds: .asciz "%DS: "
ss: .asciz "%SS: "
#LABEL_STACK:
#.space  512, 0


#.section ss
#  mov %ax, %es
#.org 510
#.word 0xaa55

