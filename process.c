#include "process.h"

Process proc_table[NR_TASKS];
u8 task_stack[STACK_SIZE_TOTAL];
Process *ready_process;
Tss tss;
