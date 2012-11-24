.code32
.text

.extern main
.extern exit

.global _start
_start:
  call main
#  call exit
