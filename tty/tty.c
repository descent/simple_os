#include "tty.h"
#include "k_stdio.h"
#include "console.h"

Tty tty_table[TTY_NUM];

void init_tty(Tty *tty)
{
  tty->inbuf_count = 0;
  tty->inbuf_head = tty->inbuf_tail = tty->inbuf;

  u8 tty_index = tty - tty_table;
  tty->console = console_table + tty_index;
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
