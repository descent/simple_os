#include "process.h"

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

u8 get_privilege(void)
{
  u16 cs_reg;
  __asm__ __volatile__ ("mov %%cs, %0\t\n"
                          :"=a"(cs_reg)
                       ); 
  return (cs_reg & 0x03);
}

void proc_a(void)
{
#if 0
  u16 l=10;
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  u8 privilege = get_privilege();

  while(1)
  {
#if 0
    __asm__ volatile ("mov $0xc,%ah\t\n");
    __asm__ volatile ("mov $'A',%al\t\n");
    __asm__ volatile ("mov %ax,%gs:((80*0+39)*2)\t\n");
#endif

    const char* proc_a_str="proc A privilege: ";
    sp = s32_itoa(l, stack_str, 10);
    clear_line(l-1);
    s32_print(sp, (u8*)(0xb8000+160*l));
    s32_print(proc_a_str, (u8*)(0xb8000+160*l+4*2));
    sp = s32_itoa(privilege, stack_str, 10);
    s32_print(sp, (u8*)(0xb8000+160*l + 22*2));
    ++l;
    l = ((l%10) + 10);
    loop_delay(10);
  }
#endif

  int i = 0;
  
  int sys_get_ticks(void);
  int get_ticks(void);

  while(1)
  {
    //int r = sys_get_ticks();
    int r = get_ticks();
    s32_print("A", cur_vb);
    //s32_print_int(i++, cur_vb, 10);
    s32_print(".", cur_vb);
    s32_print_int(r, cur_vb, 10);
    loop_delay(100);
  }

}

void proc_b(void)
{
#if 0
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
#endif
  int i = 0;
  
  while(1)
  {
    int r = get_ticks();
    s32_print("B", cur_vb);
    //s32_print_int(i++, cur_vb, 10);
    s32_print(".", cur_vb);
    s32_print_int(r, cur_vb, 10);
    loop_delay(100);
  }
}

void proc_c(void)
{
#if 0
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
#endif
  int i = 0;
  
  while(1)
  {
    int r = get_ticks();
    s32_print("C", cur_vb);
    //s32_print_int(i++, cur_vb, 10);
    s32_print(".", cur_vb);
    s32_print_int(r, cur_vb, 10);
    loop_delay(100);
  }
}

Task tasks[NR_TASKS] = {
                         {proc_a, TASK_STACK, "proc a"},
//                         {proc_b, TASK_STACK, "proc b"},
//                         {proc_c, TASK_STACK, "proc c"},
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
