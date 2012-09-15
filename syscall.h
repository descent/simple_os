#ifndef SYSCALL_H
#define SYSCALL_H

#include "process.h"

// implement in asm_syscall.S
int get_ticks(void);
int set_vga_mode(void);
int write(char *buf, int len);


#endif
