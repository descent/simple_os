#include "io_port.h"

.global disable_irq
disable_irq:
  mov 4(%esp), %ecx
  pushf
  cli
  mov $1, %ah
  rol %cl, %ah
  cmp $8, %cl
  jae disable_8
disable_0:
  in $INT_M_CTLMASK, %al
  test %ah, %al
  jnz dis_already
  or %ah, %al
  outb %al, $INT_M_CTLMASK
  popf
  mov $1, %eax
  retl
disable_8:
  in $INT_S_CTLMASK, %al
  test %ah, %al
  jnz dis_already
  or %ah, %al
  outb %al, $INT_M_CTLMASK
  popf
  mov $1, %eax
  retl
dis_already:
  popf
  xor %eax, %eax
  retl

.global enable_irq
enable_irq:
  mov 4(%esp), %ecx
  pushf
  cli
  mov $~1, %ah
  rol %cl, %ah
  cmp $8, %cl
  jae enable_8
enable_0:
  in $INT_M_CTLMASK, %al
  and %ah, %al
  outb %al, $INT_M_CTLMASK
  popf
  retl
enable_8:
  in $INT_S_CTLMASK, %al
  and %ah, %al
  outb %al, $INT_M_CTLMASK
  popf
  retl
