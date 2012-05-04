#include "clock.h"
#include "type.h"
#include "process.h"

char* s32_itoa(int n, char* str, int radix);
void s32_print(const u8 *s, u8 *vb);
void clear_line(u8 line_no);
void loop_delay(int time);

extern int k_reenter;
extern u8 *cur_vb;

u8 ticks;

void clock_handler(int irq)
{
  ++ticks;
  ready_process->ticks--;
  static u16 p = 4;
  //s32_print("@", (u8*)(0xb8000+p*2));
  s32_print("@", cur_vb);

  ++p;
  if (k_reenter != 0)
  {
    //s32_print("!", (u8*)(0xb8000+p*2));
    s32_print("!", cur_vb);
    return;
  }


  void schedule(void);

  schedule();
#if 0
  ++ready_process;
  if (ready_process >= &proc_table[NR_TASKS])
      ready_process = proc_table;
#endif
}

void schedule(void)
{
  Process *p;
  int greatest_ticks = 0;

  while(!greatest_ticks)
  {
    for (p = proc_table ; p < proc_table + NR_TASKS ; ++p)
    {
      if (p->ticks > greatest_ticks)
      {
        greatest_ticks = p->ticks;
	ready_process = p;
      }
    }
    if (!greatest_ticks)
    {
      for (p = proc_table ; p < proc_table + NR_TASKS ; ++p)
      {
        p->ticks = p->priority;
      }
    }
  }


}
