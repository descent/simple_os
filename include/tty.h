#ifndef TTY_H
#define TTY_H

#include "type.h"
#include "console.h"

#define TTY_IN_BYTES_NUM 256
#define TTY_NUM 3

typedef struct Tty_
{
  u32 inbuf[TTY_IN_BYTES_NUM];
  u32 * inbuf_head;
  u32 * inbuf_tail;
  int inbuf_count;
  struct Console_ *console;
}Tty;

void task_tty(void);
int init_tty(Tty *tty);

#endif
