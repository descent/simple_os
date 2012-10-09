#ifndef PROCESS_H
#define PROCESS_H

#include "type.h"
#include "protect.h"

#define LDT_SIZE 2
#define NR_TASKS 1
#define NR_PROCS 3

#define STACK_SIZE_TESTA 0x8000
#define TASK_STACK 0x8000
#define STACK_SIZE_TOTAL (0x8000*NR_TASKS)
#define SELECTOR_LDT_FIRST (INDEX_LDT_FIRST*8)

#define PRIVILEGE_TASK 1

#define RPL_TASK 1
#define RPL_USER 3

extern int k_reenter;

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

typedef struct Message_
{
  int source;
  int type;
}Message;

typedef struct Process_
{
  StackFrame regs;
  u16 ldt_sel;
  Descriptor ldt[LDT_SIZE];
  u32 pid;
  const char *p_name;
  int tty_index; // use which tty

  int p_flags;

  Message *msg;
  int p_recvfrom;
  int p_sendto;
  int has_int_msg;
  struct Process_ *q_sending;
  struct Process_ *next_sending;

}Process;

typedef void (*TaskAddr)(void);

#define TASK_NAME_LEN 32
typedef struct Task_
{
  TaskAddr init_eip;
  u32 stack_size;
  char name[TASK_NAME_LEN];
}Task;


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
void schedule(void);

int ldt_seg_linear(Process *p, int ldt_index);

#define proc2pid(x) (x - proc_table)

void* va2la(int pid, void* va);

//int msg_send(struct Process_* current, int dest, struct Message_* m);
//int msg_receive(struct Process_* current, int src, struct Message_* m);
int msg_send(Process* current, int dest, Message* m);
int msg_receive(Process* current, int src, Message* m);

#define SEND            1
#define RECEIVE         2
#define BOTH            3       /* BOTH = (SEND | RECEIVE) */

#define ANY             (NR_TASKS + NR_PROCS + 10)
#define NO_TASK         (NR_TASKS + NR_PROCS + 20)
#define INTERRUPT       -10

/* ipc */
#define SEND            1
#define RECEIVE         2
#define BOTH            3       /* BOTH = (SEND | RECEIVE) */

/* Process */
#define SENDING   0x02  /* set when proc trying to send */
#define RECEIVING 0x04  /* set when proc trying to recv */



#endif
