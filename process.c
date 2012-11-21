#include "process.h"
#include "keyboard.h"
#include "console.h"
#include "tty.h"
#include "romfs.h"
#include "syscall.h"
#include "vga.h"
#include "k_assert.h"
#include "k_stdio.h"
#include "k_string.h"
#include "k_stdlib.h"
#include "systask.h"
#include "vfs.h"
#include "storage.h"

Process proc_table[NR_TASKS + NR_PROCS];
u8 task_stack[STACK_SIZE_TOTAL];
//u8 task_stack[0x9000];
Process *ready_process;
Tss tss;

extern u8 *cur_vb;

//int k_reenter = -1;
int k_reenter = 0;

void s32_print(const u8 *s, u8 *vb);
void s32_print_int(int i, u8 *vb, int radix);
void clear_line(u8 line_no);
void loop_delay(int time);
void milli_delay(int milli_sec);

int get_ticks(void);

u8 get_privilege(void)
{
  u16 cs_reg;
  __asm__ __volatile__ ("mov %%cs, %0\t\n"
                          :"=a"(cs_reg)
                       ); 
  return (cs_reg & 0x03);
}

void init(void)
{
#if 0
  while(1);
#else
  int pid = fork();
  int a=5;

  if (pid != 0) // parent
  {
    while(1)
    {
      *((u8*)0xb8000+160) = 'P';
      *((u8*)0xb8000+160+1) = 0x94;
      *((u8*)0xb8000+160+2) = 'a';
      *((u8*)0xb8000+160+3) = 0x94;
      *((u8*)0xb8000+160+4) = 'r';
      *((u8*)0xb8000+160+5) = 0x94;
    }
    //spin("parent");
  }
  else // child
  {
    while(1)
    {
      *((u8*)0xb8000+160*3) = 'C';
      *((u8*)0xb8000+160*3+1) = 0x0f;
      *((u8*)0xb8000+160*3+2) = 'h';
      *((u8*)0xb8000+160*3+3) = 0x0f;
      *((u8*)0xb8000+160*3+4) = 'i';
    }
    //spin("child");
  }
  #endif
}

void proc_a(void)
{
  //assert(0);
  //char buf[1] = {'1'};
  //char buf[] = "ws";
  //BOCHS_MB
  static i=0;




  while(1)
  {

    //write(buf, 1);
#ifdef TEST_IPC
    //s32_printf("a <Ticks: %d>", i++);
    s32_printf("a <Ticks: %d>", get_ticks() );
    milli_delay(200);
#endif
  }
#if 0
#if 1
  u16 l=10;
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  u8 privilege = get_privilege();

  s32_set_text_color(HRED, HRED);
  //s32_print_str("press any key\r\n");
  s32_print("tty 1", (u8*)(0xb8000));

  static int ll=0;
  static u8 readdir = 0;
  static u8 alt_l=0;
  while(1)
  {
    //u8 key = get_byte_from_kb_buf();
    KeyStatus key_status;
    //int r=parse_//scan_code(&key_status, SCANCODE_MODE);
    int r=parse_scan_code(&key_status, ASCII_MODE);
#if 0
    __asm__ volatile ("mov $0xc,%ah\t\n");
    __asm__ volatile ("mov $'A',%al\t\n");
    __asm__ volatile ("mov %ax,%gs:((80*0+39)*2)\t\n");
#endif

#if 0
    const char* proc_a_str="proc A privilege: ";
    sp = s32_itoa(l, stack_str, 10);
    clear_line(l-1);
    s32_print(sp, (u8*)(0xb8000+160*l));
    s32_print(proc_a_str, (u8*)(0xb8000+160*l+4*2));
    sp = s32_itoa(privilege, stack_str, 10);
#endif
    //s32_print(sp, (u8*)(0xb8000+160*l + 22*2));
    if (r==0)
    {
      s32_set_text_color(WHITE, WHITE);

      //clear_line(1);
      if (key_status.press == PRESS)
      {
        //s32_print_int(key_status.key, (u8*)(0xb8000+160*1 + 22*2+20), 16);

        switch (key_status.key)
        {
          case 0x20 ... 0x7e: // ascii printable char. gnu extension: I don't want use gnu extension, but it is very convenience.
            if (key_status.key == 'l')
              readdir = 1;
            else
              readdir = 0;
            s32_print_char(key_status.key);
            break;
          case KEY_UP:
            --ll;
            if (ll <= 0 ) 
              ll = 0 ;
            set_video_start_addr(80*ll);
            break;
          case KEY_DOWN:
            ++ll;
            set_video_start_addr(80*ll);
            break;
          case KEY_F1:
            if (alt_l == 1)
              //s32_print("tty 1", (u8*)(0xb8000));
              select_tty(0);
            break;
          case KEY_F2:
            if (alt_l == 1)
              //s32_print("tty 2", (u8*)(0xb8000));
              select_tty(1);
            break;
          case KEY_F3:
            if (alt_l == 1)
              //s32_print("tty 3", (u8*)(0xb8000));
              select_tty(2);
            break;
          case KEY_ALT_L:
            alt_l = 1;
            break;
          case KEY_ENTER:
            if (readdir == 1) readdir = 2;
            else readdir = 0;
            s32_print_char('\r');
            s32_print_char('\n');
            break;
          default:
            readdir = 0;
            break;
        }
        //s32_print("key code press: ", (u8*)(0xb8000+160*1));
      }
      else // key release
      {
        switch (key_status.key)
        {
          case KEY_F12:
            //switch_vga_mode();
            set_vga_mode();  // system call
            draw_box();
            draw_box_1(40, 0, 3);
            draw_box_1(40, 0, 3);
            draw_box_1(40, 50, 5);
            draw_box_1(100, 100, 10);
            draw_str();
            break;
          case KEY_F1:
          case KEY_F2:
          case KEY_F3:
            break;
          default:
            readdir = 0;
            alt_l = 0;
            break;
        }
        //s32_print("key code release: ", (u8*)(0xb8000+160*1));
        //s32_print_int(key_status.key, (u8*)(0xb8000+160*2 + 22*2+20), 16);

      }
      //s32_put_char(key_status.key, (u8*)(0xb8000+160*1 + 22*2+20), 16);
      //s32_print(proc_a_str, (u8*)(0xb8000+160*l+4*2));
    }
    if (readdir == 2)
    {
      k_readdir("/");
      readdir=0;
    }
    ++l;
    l = ((l%10) + 10);
    loop_delay(10);
  }
#else

  int i = 0;
  
  int sys_get_ticks(void);
  int get_ticks(void);

  while(1)
  {
    u8 key = get_byte_from_kb_buf();
    //int r = sys_get_ticks();
    int r = get_ticks();
    s32_print("A", cur_vb);
    //s32_print_int(i++, cur_vb, 10);
    s32_print(".", cur_vb);
    s32_print_int(key, cur_vb, 10);
    //loop_delay(100);
    milli_delay(1000);
  }
#endif
#endif
}

void proc_b(void)
{
  char buf[1] = {'2'};
  while(1)
  {
    //s32_printf("b <Ticks: %d>", get_ticks() );
    //milli_delay(200);
    //write(buf, 1);
  }
#if 0
#if 1
  //#define VB_OFFSET (35*2)
  const u16 VB_OFFSET = (30*2);
  u16 l=12;
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  u8 privilege = get_privilege();
  while(1)
  {
    const char* proc_a_str="proc B privilege: ";
    sp = s32_itoa(l, stack_str, 10);
    clear_line(l-1);
    s32_print(sp, (u8*)(0xb8000+160*l+VB_OFFSET));
    s32_print(proc_a_str, (u8*)(0xb8000+160*l+4*2 + VB_OFFSET));
    sp = s32_itoa(privilege, stack_str, 10);
    s32_print(sp, (u8*)(0xb8000+160*l + 22*2 + VB_OFFSET));
    ++l;
    l = ((l%10) + 10);
    loop_delay(10);

  }
#else
  int i = 0;
  
  while(1)
  {
    int r = get_ticks();
    s32_print("B", cur_vb);
    //s32_print_int(i++, cur_vb, 10);
    s32_print(".", cur_vb);
    s32_print_int(r, cur_vb, 10);
    //loop_delay(100);
    milli_delay(1000);
  }
#endif
#endif
}

void proc_c(void)
{
  char buf[] = "third tty";
  int hex=0x3f;
  int dec=98;
  while(1)
  {
    //s32_printf("c <Ticks: %d>", get_ticks() );
    //milli_delay(200);
    //write(buf, 1);
    //s32_printf("test printf %d %x %s", dec, hex, buf);
    //s32_printf("q %s", buf);
  }
#if 0
#if 1
  const u16 VB_OFFSET = (50*2);
  u16 l=14;
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  while(1)
  {

    sp = s32_itoa(l, stack_str, 10);
    clear_line(l-1);
    s32_print(sp, (u8*)(0xb8000+160*l+VB_OFFSET));
    s32_print("process c", (u8*)(0xb8000+160*l+5*2+ VB_OFFSET));
    ++l;
    l = ((l%10) + 10);
    loop_delay(10);
  }
#else
  int i = 0;
  
  while(1)
  {
    int r = get_ticks();
    s32_print("C", cur_vb);
    //s32_print_int(i++, cur_vb, 10);
    s32_print(".", cur_vb);
    s32_print_int(r, cur_vb, 10);
    //loop_delay(100);
    milli_delay(1000);
  }
#endif
#endif
}

// how to add a task/process:
// add function to tasks and add 1 to NR_TASKS
Task task_table[NR_TASKS] = 
{
  {task_tty, TASK_STACK, "TTY"},
  {task_sys, TASK_STACK, "SYS"},
  {task_mm, TASK_STACK, "MM"},
};

Task user_proc_table[NR_NATIVE_PROCS] = 
{
  {init, TASK_STACK, "INIT"},
  {proc_a, TASK_STACK, "proc a"},
  {proc_b, TASK_STACK, "proc b"},
  {proc_c, TASK_STACK, "proc c"},
};

void init_proc(void)
{
  extern Descriptor gdt[];
  void p_asm_memcpy(void *dest, void *src, u16 n);
  void p_asm_memset(void *dest, int c, u16 n);

  u32 task_stack_top = 0;
  u16 selector_ldt = SELECTOR_LDT_FIRST;
  u8 privilege;
  u8 rpl;
  u32 eflags;

  Task *task;
  Process *proc = proc_table;

  for (int i = 0 ; i < NR_TASKS + NR_PROCS; ++i, proc++, task++, selector_ldt += (1 << 3) )
  {
    if (i >= NR_TASKS + NR_NATIVE_PROCS) 
    {
      proc->p_flags = FREE_SLOT;
      proc->ldt_sel = selector_ldt;
      continue;
    }

    if (i < NR_TASKS)
    {
      task = task_table + i;
      privilege = PRIVILEGE_TASK;
      rpl = RPL_TASK;
      eflags = 0x1202;
    }
    else
    {
      task = user_proc_table + (i - NR_TASKS);
      #if 1
      privilege = PRIVILEGE_USER;
      rpl = RPL_USER;
      eflags = 0x202;
      #else
      privilege = PRIVILEGE_TASK;
      rpl = RPL_TASK;
      eflags = 0x1202;
      #endif
    }
    proc->p_name = task->name;

#if 1
    if (s_strcmp(task->name, "INIT") != 0) 
    {
      proc->ldt[LDT_CODE]  = gdt[SELECTOR_KERNEL_CS >> 3];
      proc->ldt[LDT_DATA] = gdt[SELECTOR_KERNEL_DS >> 3];

      /* change the DPLs */
      proc->ldt[LDT_CODE].attr1  = DA_C   | privilege << 5;
      proc->ldt[LDT_DATA].attr1 = DA_DRW | privilege << 5;
    }
    else // init process
    {
      u32 k_base=0x100000, k_limit=0xa0000;
      //int ret = get_kernel_map(&k_base, &k_limit);
      //assert(ret == 0);
      init_descriptor(&proc->ldt[LDT_CODE],
                                  0, /* bytes before the entry point
                                      * are useless (wasted) for the
                                      * INIT process, doesn't matter
                                      */
                                  (k_base + k_limit) >> LIMIT_4K_SHIFT,
                                  DA_32 | DA_LIMIT_4K | DA_C | privilege << 5);

       init_descriptor(&proc->ldt[LDT_DATA],
                                  0, /* bytes before the entry point
                                      * are useless (wasted) for the
                                      * INIT process, doesn't matter
                                      */
                                  (k_base + k_limit) >> LIMIT_4K_SHIFT,
                                  DA_32 | DA_LIMIT_4K | DA_DRW | privilege << 5);


    }


#else
    p_asm_memcpy(&proc->ldt[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(Descriptor));
    proc->ldt[0].attr1 = (DA_C | (privilege << 5) );
    p_asm_memcpy(&proc->ldt[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(Descriptor));
    proc->ldt[1].attr1 = (DA_DRW | (privilege << 5) );
#endif

    proc->p_parent = NO_TASK;
    proc->ldt_sel = selector_ldt;

    proc->regs.cs = (0 & 0xfff8) | SEL_USE_LDT | rpl; // a ldt selector
    proc->regs.ds = (8 & 0xfff8) | SEL_USE_LDT | rpl; // a ldt selector
    proc->regs.es = (8 & 0xfff8) | SEL_USE_LDT | rpl; // a ldt selector
    proc->regs.fs = (8 & 0xfff8) | SEL_USE_LDT | rpl; // a ldt selector
    proc->regs.ss = (8 & 0xfff8) | SEL_USE_LDT | rpl; // a ldt selector
    //proc->regs.gs = (SELECTOR_KERNEL_GS & 0xfff8) | SEL_USE_LDT | RPL_TASK;
    proc->regs.gs = (SELECTOR_KERNEL_GS & 0xfff8) | rpl;
    proc->regs.eip = (u32)(task->init_eip);

    task_stack_top += (u32)task_stack+task->stack_size;
    proc->regs.esp = task_stack_top;

    proc->regs.eflags = eflags;

    proc->pid = i;
    proc->tty_index = 0;

#ifdef IPC
    proc->p_flags = 0;
    proc->msg = 0;
    proc->p_recvfrom = NO_TASK;
    proc->p_sendto = NO_TASK;
    proc->has_int_msg = 0;
    proc->q_sending = 0;
    proc->next_sending = 0;
#endif

    //selector_ldt += (1 << 3); // +8
    //++task;
    //++proc;
  }
  proc_table[NR_TASKS + 0].tty_index = 0;
  proc_table[NR_TASKS + 1].tty_index = 1;
  proc_table[NR_TASKS + 2].tty_index = 2;
}


int deadlock(int src, int dest)
{
  Process *p = proc_table + dest;
  while(1)
  {
    if (p->p_flags & SENDING)
    {
      if (p->p_sendto == src)
      {
        p = proc_table + dest;
        printx("=_=%s", p->name);
        do
        {
          assert(p->msg);
          p = proc_table + p->p_sendto;
          printx("->%s", p->name);
        }while (p != proc_table + src);
        printx("=-=");
        return 1;
      }
      p = proc_table + p->p_sendto;
    }
    else
      break;
  }

  return 0;
}

void unblock(Process *p)
{
  assert(p->p_flags == 0);
}

void schedule(void)
{
  //Process *p;
  //int greatest_ticks = 0;

  while(1)
  {
    ++ready_process;

    if (ready_process >= &proc_table[NR_TASKS + NR_PROCS])
    {
      ready_process = proc_table;
    }
    if (ready_process->p_flags == FREE_SLOT)
      continue;

    if (ready_process->p_flags == 0) // only p_flags == 0 can be selected to run
      break;
  }


}

void block(Process *p)
{
  assert(p->p_flags);
  schedule();
}

// virtual address -> linear address
void* va2la(int pid, void* va)
{
  Process *p = &proc_table[pid];
  u32 seg_base = ldt_seg_linear(p, LDT_DATA);
  u32 la = seg_base + (u32)va;

  if (pid < NR_TASKS + NR_NATIVE_PROCS)
  {
    assert(la==(u32)va);
  }
  return (void*)la;
}

int ldt_seg_linear(Process *p, int ldt_index)
{
  Descriptor *d = &(p->ldt[ldt_index]);
  return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

int msg_send(Process* current, int dest, Message* m)
{
  Process *sender = current;
  Process *p_dest = proc_table + dest;

  assert(proc2pid(sender) != dest);

  if (deadlock(proc2pid(sender), dest))
  {
    panic("");
  }

  if ((p_dest->p_flags & RECEIVING) && (p_dest->p_recvfrom == proc2pid(sender) || p_dest->p_recvfrom == ANY))
  {
    assert(p_dest->msg);
    assert(m);

    p_asm_memcpy(va2la(dest, p_dest->msg), va2la(proc2pid(sender), m), sizeof(Message) );
    p_dest->msg = 0;
    p_dest->p_flags &= ~RECEIVING;
    p_dest->p_recvfrom = NO_TASK;
    unblock(p_dest);

    assert(p_dest->p_flags == 0);
    assert(p_dest->msg == 0);
    assert(p_dest->p_recvfrom == NO_TASK);
    assert(p_dest->p_sendto == NO_TASK);

    assert(sender->p_flags == 0);
    assert(sender->msg == 0);
    assert(sender->p_recvfrom == NO_TASK);
    assert(sender->p_sendto == NO_TASK);
  }
  else
  {
    sender->p_flags |= SENDING;
    assert(sender->p_flags == SENDING);
    sender->p_sendto = dest;
    sender->msg = m;

    Process *p;
    if (p_dest->q_sending)
    {
      p = p_dest->q_sending;
      while(p->next_sending)
        p = p->next_sending;
      p->next_sending = sender;
    }
    else
    {
      p_dest->q_sending = sender;
    }
    sender->next_sending = 0;

    block(sender);

    assert(sender->p_flags == SENDING);
    assert(sender->msg != 0);
    assert(sender->p_recvfrom == NO_TASK);
    assert(sender->p_sendto == dest);
  }
  return 0;
}

int msg_receive(Process* current, int src, Message* m)
{
  Process *who_wanna_recv = current;
  Process *from = 0;
  Process *prev = 0;
  int copyok = 0;
  assert(proc2pid(who_wanna_recv) != src);

  if ((who_wanna_recv->has_int_msg) && ((src == ANY) || (src == INTERRUPT)) )
  {
    Message msg;
    reset_msg(&msg);
    msg.source = INTERRUPT;
    msg.type = HARD_INT;
    assert(m);
    p_asm_memcpy(va2la(proc2pid(who_wanna_recv), m), &msg, sizeof(Message) );
    who_wanna_recv->has_int_msg = 0;

    assert(who_wanna_recv->p_flags == 0);
    assert(who_wanna_recv->msg == 0);
    assert(who_wanna_recv->p_sendto == NO_TASK);
    assert(who_wanna_recv->has_int_msg == 0);
    return 0;
  }

  if (src == ANY)
  {
    if (who_wanna_recv->q_sending)
    {
      from = who_wanna_recv->q_sending;
      copyok = 1;

      assert(who_wanna_recv->p_flags == 0);
      assert(who_wanna_recv->msg == 0);
      assert(who_wanna_recv->p_recvfrom == NO_TASK);
      assert(who_wanna_recv->p_sendto == NO_TASK);
      assert(who_wanna_recv->q_sending != 0);

      assert(from->p_flags == SENDING);
      assert(from->msg != 0);
      assert(from->p_recvfrom == NO_TASK);
      assert(from->p_sendto == proc2pid(who_wanna_recv));
    }
  }
  else
  {
    from = &proc_table[src];

    if ((from->p_flags & SENDING) && (from->p_sendto == proc2pid(who_wanna_recv)))
    {
      copyok = 1;
      Process *p = who_wanna_recv->q_sending;
      assert(p);
      while(p)
      {
        assert(from->p_flags & SENDING);
        if (proc2pid(p) == src)
        {
          from = p;
          break;
        }
        prev = p;
        p = p->next_sending;
      }

      assert(who_wanna_recv->p_flags == 0);
      assert(who_wanna_recv->msg == 0);
      assert(who_wanna_recv->p_recvfrom == NO_TASK);
      assert(who_wanna_recv->p_sendto == NO_TASK);
      assert(who_wanna_recv->q_sending != 0);

      assert(from->p_flags == SENDING);
      assert(from->msg != 0);
      assert(from->p_recvfrom == NO_TASK);
      assert(from->p_sendto == proc2pid(who_wanna_recv));
    }
  }

  if (copyok)
  {
    if (from == who_wanna_recv->q_sending)
    {
      assert(prev == 0);
      who_wanna_recv->q_sending = from->next_sending;
      from->next_sending = 0;
    }
    else
    {
      assert(prev);
      prev->next_sending = from->next_sending;
      from->next_sending = 0;
    }
    assert(m);
    assert(from->msg);
    p_asm_memcpy(va2la(proc2pid(who_wanna_recv), m), va2la(proc2pid(from), from->msg), sizeof(Message) );

    from->msg = 0;
    from->p_sendto = NO_TASK;
    from->p_flags &= ~SENDING;
    unblock(from);
  }
  else
  {
    who_wanna_recv->p_flags |= RECEIVING;
    who_wanna_recv->msg = m;
    if (src == ANY)
      who_wanna_recv->p_recvfrom = ANY;
    else
      who_wanna_recv->p_recvfrom = proc2pid(from);
    block(who_wanna_recv);

    assert(who_wanna_recv->p_flags == RECEIVING);
    assert(who_wanna_recv->msg != 0);
    assert(who_wanna_recv->p_recvfrom != NO_TASK);
    assert(who_wanna_recv->p_sendto == NO_TASK);
    assert(who_wanna_recv->has_int_msg == 0);
  }
  return 0;
}

int send_recv(int function, int src_dest, Message *msg)
{
  void p_asm_memset(void *dest, int c, u16 n);

  int ret = 0;

  if (function == RECEIVE)
    p_asm_memset(msg, 0 , sizeof(Message));

  switch (function)
  {
    case BOTH:
    {
      ret = sendrec(SEND, src_dest, msg);
      if (ret == 0)
        ret = sendrec(RECEIVE, src_dest, msg);
      break;
    }
    case SEND:
    case RECEIVE:
    {
      ret = sendrec(function, src_dest, msg);
      break;
    }
    default:
    {
      assert((function == BOTH) || (function == SEND) || (function == RECEIVE));
      break;
    }
  }

  return ret;

}

int exec(u8 *start)
{
  goto *start;
}
