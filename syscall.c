#include "syscall.h"
#include "asm_syscall.h"
#include "k_assert.h"
#include "k_stdlib.h"
#include "process.h"
#include "type.h"
#include "vga.h"
#include "tty.h"
#include "irq.h"

typedef void* SystemCall;

#if 0
char* s32_itoa(int n, char* str, int radix);
void s32_print(const u8 *s, u8 *vb);
void s32_print_int(int i, u8 *vb, int radix);
void clear_line(u8 line_no);
void loop_delay(int time);
#endif

#ifdef IPC
int get_ticks(void)
{
  Message msg;
  reset_msg(&msg);
  msg.type = GET_TICKS;
  send_recv(BOTH, TASK_SYS, &msg);
  return msg.RETVAL;
}

int fork(void)
{
  Message msg;
  msg.type = FORK;
  send_recv(BOTH, TASK_MM, &msg);

  return msg.PID;
}

void exit(int s)
{
  Message msg;
  msg.type = EXIT;
  msg.STATUS = s;
  send_recv(BOTH, TASK_MM, &msg);
  assert(msg.type == SYSCALL_RET);
}

int wait(int *s)
{
  Message msg;
  msg.type = WAIT;
  send_recv(BOTH, TASK_MM, &msg);
  *s = msg.STATUS;
  return (msg.PID == NO_TASK ? -1 : msg.PID);
}

int execl(const char *path)
{
  Message msg;
  msg.type        = EXEC;
  msg.PATHNAME    = (void*)path;
  msg.NAME_LEN    = s_strlen(path);
  msg.BUF         = 0;
  msg.BUF_LEN     = 0;

  send_recv(BOTH, TASK_MM, &msg);
  assert(msg.type == SYSCALL_RET);

  return msg.RETVAL;
}

#endif

int sys_get_ticks(void)
{
  extern u32 ticks;

#if 0
  static int p=0;
  s32_print("+", (u8*)(0xb8000+p));
  p += 2;
  p = (p % 160);
#endif
  return ticks;
}

int sys_set_vga_mode(void)
{
  switch_vga_mode();
  return 0;
}

int sys_write(char *buf, int len, int useless, Process *proc)
{
  tty_write(&tty_table[proc->tty_index], buf, len);
  return 0;
}

int sys_sendrec(int function, int src_dest, Message *m, Process *p)
{
  assert(k_reenter == 0);
  assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) || src_dest == ANY || src_dest == INTERRUPT);

  int ret = 0;
  int caller = proc2pid(p);
  Message *msg = (Message *)va2la(caller, m);
  msg->source = caller;

  assert(msg->source != src_dest);

  if (function == SEND)
  {
    ret = msg_send(p, src_dest, m);
    if (ret != 0)
      return ret;
  }
  else if (function == RECEIVE)
       {
         ret = msg_receive(p, src_dest, m);
           if (ret != 0)
             return ret;
       }
       else
       {
         panic("invalid function:");
       }
  return 0;
}

int sys_printk(int unused1, int unused2, char *s, Process *proc)
{
  const char *p;
  char ch;
  char reenter_err[] = "?k_reenter is incorrect for unknown reason";
  reenter_err[0] = MAG_CH_PANIC;

  if (k_reenter == 0) // ring 1 ~ 3
    p = va2la(proc2pid(proc), s);
  else if (k_reenter > 0) // ring 0
         p = s;
       else
         p = reenter_err;

  if ((*p == MAG_CH_PANIC) || (*p == MAG_CH_ASSERT && ready_process < &proc_table[NR_TASKS]))
  {
    disable_int();
    char *v = (char*)V_MEM_BASE;
    const char *q=p+1;

    while(v <(char *)(V_MEM_BASE + V_MEM_SIZE))
    {
      *v++ = *q++;
      *v++ = RED_CHAR;
      if (!*q)
      {
        while(((int)v - V_MEM_BASE) % (SCR_WIDTH * 16))
        {
          ++v;
          *v++ = GRAY_CHAR;
        }
        q = p+1;
      }

    }
    __asm__ __volatile__("hlt");
  }
  //int x=0, y=5;
  while((ch = *p++) != 0)
  {
    if (ch == MAG_CH_PANIC || ch == MAG_CH_ASSERT)
      continue;
    //s32_console_print_char_xy(tty_table[proc->tty_index].console, ch, x, y);
    s32_console_print_char(tty_table[proc->tty_index].console, ch);
    #if 0
    ++x;
    if (x > 80)
    {
      ++y;
      x=0;
    }
    #endif
  }
  return 0;

}

SystemCall sys_call_table[NR_SYS_CALL] = {sys_get_ticks, sys_set_vga_mode, sys_write, sys_sendrec, sys_printk};

