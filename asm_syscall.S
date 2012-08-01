#include "syscall.h"
#include "process_const.h"

.code32
.extern save

.text
.global get_ticks
get_ticks:
  mov $_NR_GET_TICKS, %eax
  int $INT_VECTOR_SYS_CALL
  ret

.global set_vga_mode
set_vga_mode:
  mov $_NR_SET_VGA_MODE, %eax
  int $INT_VECTOR_SYS_CALL
  ret

.global sys_call
sys_call:
  call save
  sti
  call *sys_call_table(, %eax, 4)
  mov %eax, P_EAX_OFFSET(%esi) # return value save to eax offset in process structure
  cli
  ret

