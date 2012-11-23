#ifndef PROCESS_H
#define PROCESS_H

#include "type.h"
#include "protect.h"
#include "k_string.h"

#define LDT_SIZE 2
#define NR_TASKS 3
#define NR_PROCS 32
#define NR_NATIVE_PROCS 4

#define TASK_TTY 0
#define TASK_SYS 1
#define TASK_MM 2


#define STACK_SIZE_TESTA 0x8000
#define TASK_STACK 0x8000
#define STACK_SIZE_TOTAL (TASK_STACK*(NR_TASKS+NR_NATIVE_PROCS))
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

typedef struct Mess1_
{
  int m1i1;
  int m1i2;
  int m1i3;
  int m1i4;
}Mess1;

typedef struct Mess2_
{
  int m2i1;
  int m2i2;
  int m2i3;
  int m2i4;
}Mess2;

typedef struct Mess3_
{
  int m3i1;
  int m3i2;
  int m3i3;
  int m3i4;
  u64 m3l1;
  u64 m3l2;
  void * m3p1;
  void * m3p2;
}Mess3;

typedef struct Message_
{
  int source;
  int type;
  union
  {
    Mess1 m1;
    Mess2 m2;
    Mess3 m3;
  }u;
}Message;

#define RETVAL u.m3.m3i1
#define PID    u.m3.m3i2
#define STATUS u.m3.m3i1

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
  u32 p_parent;
  //u32 org_esp;
  struct Process_ *q_sending;
  struct Process_ *next_sending;
  int exit_status;
  char name[16];

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
int send_recv(int function, int src_dest, Message *msg);

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
#define WAITING   0x08
#define HANGING   0x10
#define FREE_SLOT 0x20


static inline void reset_msg(Message *m)
{
  p_asm_memset(m, 0, sizeof(Message));
}

int exec(u8 *start);

#define HARD_INT 1
#define GET_TICKS 2
#define FORK 3
#define SYSCALL_RET 4
#define EXIT 5
#define WAIT 6

#endif
