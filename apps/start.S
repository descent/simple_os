.code32
.text

.extern main
.extern exit

.global cstart
cstart:
  call main
  call exit

.data
.global __romfs_start__
.global __romfs_end__
.global __bss_start__
.global __bss_end__

__romfs_start__: .int 0x0
__romfs_end__: .int 0x0
__bss_start__: .int 0x0
__bss_end__: .int 0x0

