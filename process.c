#include "process.h"
#include "keyboard.h"
#include "console.h"
#include "k_stdio.h"
#include "tty.h"
#include "romfs.h"
#include "syscall.h"
#include "vga.h"

Process proc_table[NR_TASKS];
u8 task_stack[STACK_SIZE_TOTAL];
//u8 task_stack[0x9000];
Process *ready_process;
Tss tss;

extern u8 *cur_vb;


//int k_reenter = -1;
int k_reenter = 0;

char* s32_itoa(int n, char* str, int radix);
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

int set_vga_mode(void);

void proc_a(void)
{
  while(1);
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
}

void proc_c(void)
{
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
}

// how to add a task/process:
// add function to tasks and add 1 to NR_TASKS
Task tasks[NR_TASKS] = {
                         {proc_a, TASK_STACK, "proc a"},
                         {proc_b, TASK_STACK, "proc b"},
                         {proc_c, TASK_STACK, "proc c"},
                         {task_tty, TASK_STACK, "tty"},
                       };

void init_proc(void)
{
  extern Descriptor gdt[];
  void p_asm_memcpy(void *dest, void *src, u16 n);
  void p_asm_memset(void *dest, int c, u16 n);

  u32 task_stack_top = 0;
  u16 selector_ldt = SELECTOR_LDT_FIRST;

  for (int i = 0 ; i < NR_TASKS; ++i)
  {
    Process *proc = &proc_table[i];
    proc->ldt_sel = selector_ldt;

    p_asm_memcpy(&proc->ldt[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(Descriptor));
    proc->ldt[0].attr1 = (DA_C | (PRIVILEGE_TASK << 5) );
    p_asm_memcpy(&proc->ldt[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(Descriptor));
    proc->ldt[1].attr1 = (DA_DRW | (PRIVILEGE_TASK << 5) );

    proc->regs.cs = (0 & 0xfff8) | SEL_USE_LDT | RPL_TASK; // a ldt selector
    proc->regs.ds = (8 & 0xfff8) | SEL_USE_LDT | RPL_TASK; // a ldt selector
    proc->regs.es = (8 & 0xfff8) | SEL_USE_LDT | RPL_TASK; // a ldt selector
    proc->regs.fs = (8 & 0xfff8) | SEL_USE_LDT | RPL_TASK; // a ldt selector
    proc->regs.ss = (8 & 0xfff8) | SEL_USE_LDT | RPL_TASK; // a ldt selector
    //proc->regs.gs = (SELECTOR_KERNEL_GS & 0xfff8) | SEL_USE_LDT | RPL_TASK;
    proc->regs.gs = (SELECTOR_KERNEL_GS & 0xfff8) | RPL_TASK;
    proc->regs.eip = (u32)tasks[i].init_eip;

    task_stack_top += (u32)task_stack+tasks[i].stack_size;
    proc->regs.esp = task_stack_top;

    proc->regs.eflags = 0x1202;

    proc->p_name = tasks[i].name;
    selector_ldt += (1 << 3); // +8
  }



}
