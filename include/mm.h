#ifndef MM_H
#define MM_H

#include "type.h"
#include "k_stdio.h"

extern u32 memory_used;
extern u32 memsize_mb;

int mm_init(void);
u32 memtest(volatile u32 start, volatile u32 end);
void *alloc_mem(void);
void free_mem(void *mem_addr);
int do_fork(void);

#endif
