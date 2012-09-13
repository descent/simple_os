#ifndef CONSOLE_H
#define CONSOLE_H

#include "tty.h"
#include "type.h"

// char fg attribute 
#define HRED 0xc
#define HGREEN 0xa
#define HBLUE 0x9
#define HWHITE 0xf

#define RED 4
#define GREEN 2
#define BLUE 1
#define WHITE 7

// char bg attribute 

// video ram is from 0xb8000 ~ 0xc0000
// need 4000 byte for 80X25 console

#define TEXT_CONSOLE 0
#define GRAPHIC_CONSOLE 1

typedef struct Console_
{
  u32 vm_start; // video memory start
  u32 vm_end; // video memroy end
  u32 cur_vm;
  u32 cur_x, cur_y;
  u8 type; // TEXT_CONSOLE means text console, GRAPHIC_CONSOLE means graphic console
}Console;

#define CONSOLE_NUM 4
#define VIDEO_RAM_SIZE ((0xc0000-0xb8000) / (CONSOLE_NUM-1) ) // last console is graphic console

extern Console console_table[];

void s32_print_char(u8 ch);
void s32_print_str(const u8* str);

void s32_set_text_color(u8 fg, u8 bg);

int init_console();
void set_cursor(u16 pos);
void set_video_start_addr(u16 addr);
void s32_console_print_char(Console *console, u8 ch);
void s32_console_print_str(Console *console, const u8 *str);
void s32_console_print_char_xy(Console *console, u8 ch, int x, int y);
void s32_console_print_str_xy(Console *console, const u8 *str, int x, int y);
void set_xy(Console *console, int x, int y);
void get_xy(Console *console, int *x, int *y);
inline static void clear_console(Console *console)
{
  for (int y=0 ; y < 50 ; ++y)
    for (int x=0 ; x < 80 ; ++x)
      s32_console_print_char_xy(console, ' ', x, y);
  set_xy(console, 0, 2);
}

#endif
