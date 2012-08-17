#include "tty.h"
#include "k_stdio.h"
#include "console.h"
#include "keyboard.h"

#define TRUE 1
#define FALSE (!TRUE)

static Tty tty_table[TTY_NUM];

static int cur_tty_index = 0;

int select_tty(int tty_index)
{ 
  int ret =0;
  
  if ( 0 <= cur_tty_index && cur_tty_index < TTY_NUM)
    cur_tty_index = tty_index;
  else
    ret = -1;

  s32_print_int(tty_table[cur_tty_index].console->vm_start, (u8*)(0xb8000+160*1), 16);

  int org_x, org_y;
  get_xy(tty_table[cur_tty_index].console, &org_x, &org_y);

  set_xy(tty_table[cur_tty_index].console, 0, 0);
  s32_console_print_str(tty_table[cur_tty_index].console, "simple tty ");

  switch (cur_tty_index)
  {
    case 0:
      s32_console_print_char(tty_table[cur_tty_index].console, '1');
      break;
    case 1:
      s32_console_print_char(tty_table[cur_tty_index].console, '2');
      break;
    case 2:
      s32_console_print_char(tty_table[cur_tty_index].console, '3');
      break;
    default:
      break;
  }
  s32_console_print_str(tty_table[cur_tty_index].console, "\r\n\r\n");
  set_xy(tty_table[cur_tty_index].console, org_x, org_y);
  u16 cur_console_vm_start = (tty_table[cur_tty_index].console->vm_start-0xb8000)/2;
  set_video_start_addr(cur_console_vm_start);


  //set_cursor(cur_console_vm_start + tty_table[cur_tty_index].console->cur_x + tty_table[cur_tty_index].console->cur_y*80);
  set_cursor(cur_console_vm_start + org_x + org_y*80);
  return ret;
}

int init_tty(Tty *tty)
{
  tty->inbuf_count = 0;
  tty->inbuf_head = tty->inbuf_tail = tty->inbuf;

  u8 tty_index = tty - tty_table;
  tty->console = console_table + tty_index;

  select_tty(0);
  return 0;
}

int is_current_tty(Tty *tty)
{
  return (tty == &tty_table[cur_tty_index]);
#if 0
  if (cur_tty_index == (tty - tty_table)/sizeof(Tty))
    return TRUE;
  else
    return FALSE;
#endif
}

int get_data_from_tty_buf(Tty *tty)
{
  if (tty->inbuf_count)
  {
    u8 ch = *(tty->inbuf_tail);
    if (tty->inbuf_tail == tty->inbuf + TTY_IN_BYTES_NUM)
      tty->inbuf_tail = tty->inbuf;
    --tty->inbuf_count;
    return ch;
  }
  else
    return -1;
}

int parse_key(Tty *tty, KeyStatus *key_status)
{
  static u8 keymap_col=0;

  int scan_code = get_data_from_tty_buf(tty);
  switch (scan_code)
  {
    case -1:
      break;
    case 0xe1:
      break;
    case 0xe0:
      scan_code = get_data_from_tty_buf(tty);
      key_status->key = lookup_key(scan_code & 0x7f, keymap_col);
      key_status->press = ((scan_code & 0x80) ? RELEASE: PRESS);
      break;
    default:
      key_status->key = lookup_key(scan_code & 0x7f, keymap_col);
      key_status->press = ((scan_code & 0x80) ? RELEASE: PRESS);
      break;
  }
}

void tty_do_read(Tty *tty)
{
  if(is_current_tty(tty))
  {
    keyboard_read(tty); // call put_key
    //KeyStatus key_status;

    //parse_key(tty, &key_status);
  }
}

void tty_do_write(Tty *tty)
{
  if (tty->inbuf_count)
  {
    u32 ch = *(tty->inbuf_tail);
    ++(tty->inbuf_tail);
    if (tty->inbuf_tail == tty->inbuf + TTY_IN_BYTES_NUM)
      tty->inbuf_tail = tty->inbuf;

    --tty->inbuf_count;
    //s32_console_print_char(tty->console, ch);
    switch (ch)
    {
      case KEY_ENTER:
        s32_console_print_char(tty_table[cur_tty_index].console, '\r');
        s32_console_print_char(tty_table[cur_tty_index].console, '\n');
        break;
      default:
        s32_console_print_char(tty_table[cur_tty_index].console, ch);
        break;
    }
  }
}

void task_tty(void)
{
  Tty *tty;

  init_console();
  for (tty = tty_table ; tty < tty_table + TTY_NUM ; ++tty)
  {
    init_tty(tty);
  }

  static int i=0;
  while(1)
  {
    s32_print_int(i, (u8*)(0xb8000+160*23), 10);
    ++i;
    for (tty = tty_table ; tty < tty_table + TTY_NUM ; ++tty)
    {
      tty_do_read(tty);
      tty_do_write(tty);
    }
  }
}

int put_key(Tty *tty, u32 key)
{
  if (tty->inbuf_count < TTY_IN_BYTES_NUM)
  {
    s32_put_char(key, (u8*)(0xb8000+160*20));
    *(tty->inbuf_head) = key;
    ++tty->inbuf_head;
    if (tty->inbuf_head == tty->inbuf+TTY_IN_BYTES_NUM)
      tty->inbuf_head = tty->inbuf;
    ++tty->inbuf_count;
  }
}
