#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "type.h"

#define KB_NUM 32

int init_keyboard(void);
u8 get_byte_from_kb_buf(void);

typedef struct KeyboardBuf_
{
  u8 *head;
  u8 *tail;
  u32 count; // buf count, less than KB_NUM
  u8 buf[KB_NUM];
}KeyboardBuf;

extern KeyboardBuf kb_buf;

#endif
