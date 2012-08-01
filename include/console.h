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

typedef struct Console_
{
  u32 cur_start_addr;
  u32 org_addr;
  u32 vm_limit; // video mem
  u32 cursor;
}Console;


extern Console console_table[];

void s32_print_char(u8 ch);
void s32_print_str(const u8* str);

void s32_set_text_color(u8 fg, u8 bg);

void set_cursor(u16 pos);
void set_video_start_addr(u16 addr);

#endif
