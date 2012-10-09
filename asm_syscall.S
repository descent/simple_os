#include "asm_syscall.h"
#include "process_const.h"

.code32
.extern save

.text

#ifndef IPC
.global get_ticks
get_ticks:
  mov $_NR_GET_TICKS, %eax
  int $INT_VECTOR_SYS_CALL
  ret
#endif

.global set_vga_mode
set_vga_mode:
  mov $_NR_SET_VGA_MODE, %eax
  int $INT_VECTOR_SYS_CALL
  ret

.global write
write:
  mov $_NR_WRITE, %eax
  mov 4(%esp), %ebx
  mov 8(%esp), %ecx
  int $INT_VECTOR_SYS_CALL
  ret

.global sendrec
sendrec:
  mov $_NR_SENDREC, %eax
  mov 4(%esp), %ebx
  mov 8(%esp), %ecx
  mov 12(%esp), %edx
  int $INT_VECTOR_SYS_CALL
  ret

.global printk
printk:
  mov $_NR_PRINTK, %eax
  mov 4(%esp), %edx
  int $INT_VECTOR_SYS_CALL
  ret

.global sys_call
sys_call:
  call save
  sti
  push %esi # why push %esi ??

  pushl ready_process
  push %edx
  push %ecx
  push %ebx
  call *sys_call_table(, %eax, 4)
  add $16, %esp
  pop %esi # why pop %esi ??
  mov %eax, P_EAX_OFFSET(%esi) # return value save to eax offset in process structure
  cli
  ret

