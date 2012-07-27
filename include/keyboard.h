#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "type.h"

#define KB_NUM 32
#define NR_SCAN_CODES 0x80

#define PRESS 0
#define RELEASE 1

// form qt
#define KEY_ESC  0x01000000       
#define KEY_TAB     0x01000001       
#define KEY_Backtab 0x01000002       
#define KEY_BACKSPACE       0x01000003       
#define KEY_Return  0x01000004       
#define KEY_ENTER   0x01000005      //Typically located on the keypad.
#define KEY_INSERT  0x01000006       
#define KEY_DELETE  0x01000007       
#define KEY_Pause   0x01000008      //The Pause/Break key (Note: Not anything to do with pausing media)
#define KEY_Print   0x01000009       
#define KEY_SysReq  0x0100000a       
#define KEY_Clear   0x0100000b       
#define KEY_HOME    0x01000010       
#define KEY_END     0x01000011       
#define KEY_LEFT    0x01000012       
#define KEY_UP      0x01000013       
#define KEY_RIGHT   0x01000014       
#define KEY_DOWN    0x01000015       
#define KEY_PAGE_UP  0x01000016       
#define KEY_PAGE_DOWN        0x01000017       
#define KEY_SHIFT_L   0x01000020       
#define KEY_SHIFT_R   0x01000220       
#define KEY_CTRL_L 0x01000021      //On Mac OS X, this corresponds to the Command keys.
#define KEY_Meta    0x01000022      //On Mac OS X, this corresponds to the Control keys. On Windows keyboards, this key is mapped to the Windows key.
#define KEY_ALT     0x01000023       
#define KEY_AltGr   0x01001103      //On Windows, when the KeyDown event for this key is sent, the Ctrl+Alt modifiers are also set.
#define KEY_CAPSLOCK        0x01000024       
#define KEY_NUMLOCK 0x01000025       
#define KEY_SCROLL_LOCK      0x01000026       
#define KEY_F1      0x01000030       
#define KEY_F2      0x01000031       
#define KEY_F3      0x01000032       
#define KEY_F4      0x01000033       
#define KEY_F5      0x01000034       
#define KEY_F6      0x01000035       
#define KEY_F7      0x01000036       
#define KEY_F8      0x01000037       
#define KEY_F9      0x01000038       
#define KEY_F10     0x01000039       
#define KEY_F11     0x0100003a       
#define KEY_F12     0x0100003b       
#define KEY_F13     0x0100003c       
#define KEY_F14     0x0100003d       
#define KEY_F15     0x0100003e       
#define KEY_F16     0x0100003f       
#define KEY_F17     0x01000040       
#define KEY_F18     0x01000041       
#define KEY_F19     0x01000042       
#define KEY_F20     0x01000043       
#define KEY_F21     0x01000044       
#define KEY_F22     0x01000045       
#define KEY_F23     0x01000046       
#define KEY_F24     0x01000047       
#define KEY_F25     0x01000048       
#define KEY_F26     0x01000049       
#define KEY_F27     0x0100004a       
#define KEY_F28     0x0100004b       
#define KEY_F29     0x0100004c       
#define KEY_F30     0x0100004d       
#define KEY_F31     0x0100004e       
#define KEY_F32     0x0100004f       
#define KEY_F33     0x01000050       
#define KEY_F34     0x01000051       
#define KEY_F35     0x01000052       
#define KEY_Super_L 0x01000053       
#define Key_Super_R 0x01000054       
#define Key_Menu    0x01000055       
#define Key_Hyper_L 0x01000056       
#define Key_Hyper_R 0x01000057       
#define Key_Help    0x01000058       
#define Key_Direction_L     0x01000059       
#define Key_Direction_R     0x01000060
#define KEY_MINUS     0x01000160
#define KEY_MID     0x01000161
#define KEY_PLUS     0x01000162
#define KEY_DOT     0x01000163

#define SCANCODE_MODE 0
#define KEYCODE_MODE 1
#define ASCII_MODE 2

typedef struct KeyboardBuf_
{
  u8 *head;
  u8 *tail;
  u32 count; // buf count, less than KB_NUM
  u8 buf[KB_NUM];
}KeyboardBuf;

extern KeyboardBuf kb_buf;

typedef struct KeyStatus_
{
  int key;
  int scan_code[10];
  u8 press; // press or release
}KeyStatus;

int init_keyboard(void);
u8 get_byte_from_kb_buf(void);
int parse_scan_code(KeyStatus *key_status, int keyborad_mode);

#endif
