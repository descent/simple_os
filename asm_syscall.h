#ifndef ASM_SYSCALL_H
#define ASM_SYSCALL_H

#define INT_VECTOR_SYS_CALL 0x90
#define _NR_GET_TICKS 0
#define _NR_SET_VGA_MODE 1
#define _NR_WRITE 2
#define _NR_SENDREC 3

#define NR_SYS_CALL 4

//int get_ticks(void);
//int set_vga_mode(void);

#endif
