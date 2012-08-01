#include "irq.h"
#include "type.h"
#include "vga.h"
#include "io.h"
#include "console.h"
#include "k_stdio.h"

u8 *text_vb = (u8*)0xb8000;
u8 text_fg;
u8 text_bg;
u16 cur_pos=0;
u32 cur_x;
u32 cur_y;

Console console_table[TTY_NUM];

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
