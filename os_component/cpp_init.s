# 1 "cpp_init.S"
# 1 "/home/descent/test/simple_os.test/cpp_runtime/pc//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "cpp_init.S"
# init c++ runtime



# How to make G++ preprocessor output a newline in a macro?
# ref: http://stackoverflow.com/questions/2271078/how-to-make-g-preprocessor-output-a-newline-in-a-macro
# 30 "cpp_init.S"
.code32
.extern __bss_start__
.extern __bss_end__
.extern __start_ctors
.extern __end_ctors
.extern cpp_main
.extern obj_count


.text
  jmp _start
.global _start1
_start1:
retl

.global _start
_start:
  call init_bss_asm # in dos need not init bss by myself
  calll init_cpp_global_asm
  calll cpp_main
  calll g_dtors
  retl

#disp_str:
# mov $BootMessage, %ax
# mov %ax,%bp
# mov $16,%cx
# mov $0x1301,%ax
# mov $0x00c,%bx
# mov $0,%dl
# int $0x10
# ret
#BootMessage:.ascii "Hello, c-env world!"
#disp_str2:
# mov $str2, %ax
# mov %ax,%bp
# mov $5,%cx
# mov $0x1301,%ax
# mov $0x00c,%bx
# mov $0,%dl
# int $0x10
# ret
#str2:.ascii "after"

# call global object ctor
init_cpp_global_asm:




  mov $__end_ctors, %edi /* Destination */
  mov $__start_ctors, %esi /* Source */

  jmp 2f
1:
  mov %esi, %ebx
  calll *(%ebx)
  add $4, %esi
2:
  cmp %edi, %esi
  jne 1b
  ret

# init bss
init_bss_asm:
  mov $__bss_end__, %edi /* Destination */
  mov $__bss_start__, %esi /* Source */
# mov $EndOfBss, %edi /* Destination */
# mov $PM_BSS, %esi /* Source */
  mov %ds, %bx
  mov %bx, %es
  jmp 2f
1:
  mov $0, %eax
  mov %esi, %eax
  movb $0x0, %es:(%eax)
  add $1, %esi

  # wait key
# xor %eax,%eax
# int $0x16


2:
  cmp %edi, %esi
  jne 1b

  retl

# movw 16(%ebp), %ecx /* Counter */
#1:
# cmp $0, %ecx /* Loop counter */
# jz MemCpy.2
# movb %ds:(%esi), %al
# inc %esi
# movb %al, %es:(%edi)
# inc %edi
# dec %ecx
# jmp MemCpy.1
#2:
# mov 8(%ebp), %eax
# pop %ecx
# pop %edi
# pop %esi
# mov %ebp, %esp
# pop %ebp
# ret



#.bss
#_bss_start_:.word __bss_start__
#_bss_end_:.word __bss_end__

disp_al:
        pushl %ebx
        pushl %ecx
        pushl %edx

        movl (cur_pos), %ebx
        movb $0x0c, %ah # 0000: 黑底 1100: 紅字
        movb %al, %dl
        shr $4, %al
        movl $2, %ecx
.begin_1:
        andb $0x0f, %al
        cmp $9, %al
        ja .31 # cf=0, zf=0, above 9 (>9)
        #addb $'0', %al
        addb $0x30, %al
        jmp .41
.31:
        sub $0x0A, %al
        #add $'A', %al
        add $0x41, %al
.41:
        #mov [gs:edi], ax
        #mov %ax, %gs:(%edi)





        movw %ax, %gs:(%ebx)
        #mov %ax, %gs:(0)
        add $2, %ebx

        mov %ebx, cur_pos

        mov %dl, %al
        loop .begin_1
        addl $2, %ebx

        popl %edx
        popl %ecx
        popl %ebx

        ret

disp_bx:
  pushl %eax
  mov %bx, %ax
  shr $8, %ax
  call disp_al
  mov %bl, %al
  call disp_al
  popl %eax
  ret

.data
CRLF: .asciz "\r\n"
cs: .asciz "%CS: "
ds: .asciz "%DS: "
ss: .asciz "%SS: "
var: .asciz "var: "
cur_pos: .int 0
#LABEL_STACK:
#.space 512, 0


#.section ss
# mov %ax, %es
#.org 510
#.word 0xaa55
