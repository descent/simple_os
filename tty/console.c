#include "irq.h"
#include "type.h"
#include "vga.h"
#include "io.h"
#include "console.h"
#include "k_stdio.h"

static int cur_console_index=0;

u8 *text_vb = (u8*)0xb8000;
u8 text_fg;
u8 text_bg;
u16 cur_pos=0;
u32 cur_x;
u32 cur_y;

Console console_table[CONSOLE_NUM];

int init_console()
{
  Console *console;

  for (int i=0 ; i < CONSOLE_NUM ; ++i)
  {
    console = &console_table[i];
    console->cur_vm = console->vm_start = 0xb8000 + i*VIDEO_RAM_SIZE;
    console->cur_x = console->cur_y = 0;
  }
  return 0;
}

int select_console(int console_index)
{
  int ret =0;

  if ( 0 <= console_index && console_index < TTY_NUM)
    cur_console_index = console_index;
  else
    ret = -1;
  return ret;
}

void set_cursor(u16 pos)
{
  disable_int();
  io_out8(CRTC_ADDR, CURSOR_H);
  io_out8(CRTC_DATA, (pos >> 8) & 0xff);

  io_out8(CRTC_ADDR, CURSOR_L);
  io_out8(CRTC_DATA, pos & 0xff);
  enable_int();
}

void set_video_start_addr(u16 addr)
{
  disable_int();
  io_out8(CRTC_ADDR, START_ADDR_H);
  io_out8(CRTC_DATA, (addr >> 8) & 0xff);
  io_out8(CRTC_ADDR, START_ADDR_L);
  io_out8(CRTC_DATA, addr & 0xff);
  enable_int();
}

// prefix s32 means simple, 32bit code (in x86 protected mode)
// this version don't specify video address
// for console io
void s32_print_char(u8 ch)
{
  switch (ch)
  {
    case '\r':
      //cur_x = cur_x - (cur_x % 80);
      cur_x = 0;
      text_vb = (u8*)((u32)text_vb - (((u32)text_vb-0xb8000) % 160));
      cur_pos = (((u32)text_vb - 0xb8000)/2);
      s32_print_int(cur_pos, (u8*)(0xb8000+160*1 + 18*2+20), 16);
      break;
    case '\n':
      text_vb+=160;
      cur_pos += 80;
      s32_print_int(cur_pos, (u8*)(0xb8000+160*1 + 18*2+40), 16);
      ++cur_y;
      break;
    case 0x20 ... 0x7e: // ascii printable char. gnu extension: I don't want use gnu extension, but it is very convenience.
      *text_vb = ch;
      *(text_vb+1) = (text_fg|text_bg);
      text_vb+=2;
      ++cur_x;
      ++cur_pos;
      break;
    default:
      break;
  }

  set_cursor(cur_pos);
  s32_print_int(cur_pos, (u8*)(0xb8000+160*1 + 18*2+60), 16);
  clear_line(18);
  s32_print_int(cur_x, (u8*)(0xb8000+160*18 + 23*2+40), 10);
  s32_print_int(cur_y, (u8*)(0xb8000+160*18 + 23*2+60), 10);
  if (cur_y >= 25 )
    set_video_start_addr(80*(cur_y - 24));
}

void s32_print_str(const u8* str)
{
  while(*str)
    s32_print_char(*str++);
}

void s32_set_text_color(u8 fg, u8 bg)
{
  text_fg=fg;
  text_bg=bg;
}

void s32_console_print_char(Console *console, u8 ch)
{
  u8 *console_vb = (u8*)console->cur_vm;
  //u8 *console_vb = (u8*)(0xb8000);
  //s32_put_char(ch, (u8*)(0xb8000+160*19));

  switch (ch)
  {
    case '\r':
      console->cur_x = 0;
      console_vb = (u8*)((u32)text_vb - (((u32)console_vb - console->vm_start) % 160));
      break;
    case '\n':
      console_vb+=160;
      ++console->cur_y;
      break;
    case 0x20 ... 0x7e: // ascii printable char. gnu extension: I don't want use gnu extension, but it is very convenience.
      *console_vb = ch;
      //*(console_vb+1) = (text_fg|text_bg);
      console_vb+=2;
      ++console->cur_x;
      break;
    default:
      break;
  }

  if (console->cur_y >= 25 )
    set_video_start_addr(80*(console->cur_y - 24));
  set_cursor(console->cur_x + console->cur_y * 80);
  console->cur_vm = console_vb;
}
