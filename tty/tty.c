#include "tty.h"
#include "k_stdio.h"
#include "console.h"

static Tty tty_table[TTY_NUM];

static int cur_tty_index = 0;

int select_tty(int tty_index)
{ 
  int ret =0;
  
  if ( 0 <= cur_tty_index && cur_tty_index < TTY_NUM)
    cur_tty_index = tty_index;
  else
    ret = -1;
  set_video_start_addr((tty_table[cur_tty_index].console->vm_start-0xb8000)/2);
  //s32_print_int(tty_table[cur_tty_index].console->vm_start, (u8*)(0xb8000+160*1), 16);

  switch (cur_tty_index)
  {
    case 0:
      s32_console_print_char(tty_table[cur_tty_index].console, '0');
      break;
    case 1:
      s32_console_print_char(tty_table[cur_tty_index].console, '1');
      break;
    case 2:
      s32_console_print_char(tty_table[cur_tty_index].console, '2');
      break;
    default:
      break;

  }
  return ret;
}

int init_tty(Tty *tty)
{
  tty->inbuf_count = 0;
  tty->inbuf_head = tty->inbuf_tail = tty->inbuf;

  u8 tty_index = tty - tty_table;
  tty->console = console_table + tty_index;

  init_console();
  return 0;
}

void task_tty(void)
{
  Tty *tty;

  for (tty = tty_table ; tty < tty_table + TTY_NUM ; ++tty)
  {
    init_tty(tty);
  }

  static int i=0;
  while(1)
  {
    s32_print_int(i, (u8*)(0xb8000+160*23), 10);
    ++i;
  }
}

void tty_do_read(Tty *tty)
{
}
