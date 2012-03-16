#ifndef PROCESS_H
#define PROCESS_H

#include "type.h"
#include "protect.h"

#define LDT_SIZE 2
#define NR_TASKS 2

#define STACK_SIZE_TESTA 0x8000
#define TASK_STACK 0x8000
#define STACK_SIZE_TOTAL (0x8000*NR_TASKS)
#define SELECTOR_LDT_FIRST (INDEX_LDT_FIRST*8)

#define PRIVILEGE_TASK 1

#define RPL_TASK 1


typedef struct StackFrame_
{
  u32 gs;
  u32 fs;
  u32 es;
  u32 ds;
  u32 edi;
  u32 esi;
  u32 ebp;
  u32 kernel_esp;
  u32 ebx;
  u32 edx;
  u32 ecx;
  u32 eax;
  u32 retaddr;
  u32 eip;
  u32 cs;
  u32 eflags;
  u32 esp;
  u32 ss;
}StackFrame;

typedef struct Process_
{
  StackFrame regs;
  u16 ldt_sel;
  Descriptor ldt[LDT_SIZE];
  u32 pid;
  const char *p_name;
}Process;

typedef void (*TaskAddr)(void);

#define TASK_NAME_LEN 32
typedef struct Task_
{
  TaskAddr init_eip;
  u32 stack_size;
  char name[TASK_NAME_LEN];
}Task;

void proc_a(void);
void proc_b(void);


typedef struct Tss_
{
  u32 backlink;
  u32 esp0;
  u32 ss0;
  u32 esp1;
  u32 ss1;
  u32 esp2;
  u32 ss2;
  u32 cr3;
  u32 eip;
  u32 flags;
  u32 eax;
  u32 ecx;
  u32 edx;
  u32 ebx;
  u32 esp;
  u32 ebp;
  u32 esi;
  u32 edi;
  u32 es;
  u32 cs;
  u32 ss;
  u32 ds;
  u32 fs;
  u32 gs;
  u32 ldt;
  u32 trap;
  u32 iobase;
}Tss;

extern Process proc_table[];
extern u8 task_stack[];
extern Process *ready_process;
extern Tss tss;

void init_proc(void);

#endif
