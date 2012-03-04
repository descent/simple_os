# kernel loader should enters protected mode
# the code will use selector:offset to access address
.code32
.text
.global _start
_start:
  call startc
  mov $0xc,%ah
  mov $'K',%al
  mov %ax,%gs:((80*0+39)*2)
  jmp .
