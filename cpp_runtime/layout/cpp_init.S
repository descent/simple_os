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

#define PRINT_VAR(VAR)  /*
*/  pushl $var /*
*/  calll _Z9print_strPKc /*
*/  addl $4, %esp /*
*/  pushl $16 /*
*/  pushl VAR /*
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


.text
  jmp _start
.global _start
_start:
  xchg %bx, %bx
  call init_bss_asm # in dos need not init bss by myself
  call cpp_main

  # call _GLOBAL__I_XX invoke global object ctor, 
  # in dos environment use 16 or 32 bit address? look like 32 bit
#  call disp_str2
  mov     $0x4c00, %ax
  int     $0x21   # 回到 DOS


# call global object ctor
init_cpp_global_asm:
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
  mov $__end_global_ctor__, %edi    /* Destination */
  mov $__start_global_ctor__, %esi   /* Source */
#else
  mov $__end_ctors, %edi    /* Destination */
  mov $__start_ctors, %esi   /* Source */
#endif
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
  movw $__bss_end__, %di    /* Destination */
  movw $__bss_start__, %si   /* Source */
  movw %ds, %bx
  movw %bx, %es
  jmp 2f
1:
  mov $0, %eax
  mov %si, %ax
  movb $0x0, %es:(%eax)
  add $1, %si


  # wait key
#  xor %eax,%eax
#  int $0x16

  
2:
  cmp %di, %si
  jne 1b

  ret

#if 0
.data
CRLF: .asciz "\r\n"
cs: .asciz "%CS: "
ds: .asciz "%DS: "
ss: .asciz "%SS: "
var: .asciz "var: "
cur_pos: .int 0
#endif
