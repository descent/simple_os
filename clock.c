#include "clock.h"
#include "type.h"
#include "process.h"

char* s32_itoa(int n, char* str, int radix);
void s32_print_int(int i, u8 *vb, int radix);
void s32_print(const u8 *s, u8 *vb);
void clear_line(u8 line_no);
void loop_delay(int time);


extern int k_reenter;

void clock_handler(int irq)
{
  static u16 p = 4;
  s32_print("@", (u8*)(0xb8000+p*2));
  ++p;
  if (k_reenter != 0)
  {
    s32_print("&", (u8*)(0xb8000+p*2));
    ++p;
    s32_print_int(k_reenter, (u8*)(0xb8000+160*20), 10);
  }

  ++ready_process;
  if (ready_process >= &proc_table[NR_TASKS])
      ready_process = proc_table;
}
