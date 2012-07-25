#include "irq.h"
#include "type.h"
#include "vga.h"
#include "io.h"

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
