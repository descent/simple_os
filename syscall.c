#include "syscall.h"
#include "type.h"
#include "vga.h"
#include "tty.h"
#include "process.h"

typedef void* SystemCall;

#if 0
char* s32_itoa(int n, char* str, int radix);
void s32_print(const u8 *s, u8 *vb);
void s32_print_int(int i, u8 *vb, int radix);
void clear_line(u8 line_no);
void loop_delay(int time);
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

int sys_write(char *buf, int len, Process *proc)
{
  tty_write(&tty_table[proc->tty_index], buf, len);
  return 0;
}

SystemCall sys_call_table[NR_SYS_CALL] = {sys_get_ticks, sys_set_vga_mode, sys_write};

