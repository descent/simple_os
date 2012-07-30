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
void s32_print_char(u8 ch)
{
  if (ch=='\r')
  {
    text_vb = (u8*)((u32)text_vb - (((u32)text_vb-0xb8000) % 160));
    cur_pos = (((u32)text_vb - 0xb8000)/2);
    set_cursor(cur_pos);
    s32_print_int(cur_pos, (u8*)(0xb8000+160*1 + 18*2+20), 16);
    return;
  }

  if (ch=='\n')
  {
    text_vb+=160;
    cur_pos += 80;
    set_cursor(cur_pos);
    s32_print_int(cur_pos, (u8*)(0xb8000+160*1 + 18*2+40), 16);
    return;
  }
  *text_vb = ch;
  text_vb+=2;
  set_cursor(++cur_pos);
  s32_print_int(cur_pos, (u8*)(0xb8000+160*1 + 18*2+60), 16);
}

void s32_set_text_color(u8 fg, u8 bg)
{
  text_fg=fg;
  text_bg=bg;
}
