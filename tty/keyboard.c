#include "io.h"
#include "irq.h"
#include "k_stdio.h"
#include "keyboard.h"

KeyboardBuf kb_buf;

int init_keyboard(void)
{
  void keyboard_handler(int irq);
  s32_print("init keyboard", (u8*)(0xb8000+160*23));

  kb_buf.count = 0;
  kb_buf.head = kb_buf.tail = kb_buf.buf;

  put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
  enable_irq(KEYBOARD_IRQ);
  return 0;
}

void keyboard_handler(int irq)
{
  static int cur_pos=0;
  //clear_line(22);
  //s32_print("*", (u8*)(0xb8000+160*22));

  u8 scan_code = io_in8(0x60);
  if (kb_buf.count < KB_NUM)
  {
    *(kb_buf.head) = scan_code;
    ++kb_buf.head;
    if (kb_buf.head == kb_buf.buf + KB_NUM)
      kb_buf.head = kb_buf.buf;
    ++kb_buf.count;
  }
  s32_print_int(scan_code, (u8*)(0xb8000+160*22 + cur_pos*2), 16);
  cur_pos+=3;
}

u8 get_byte_from_kb_buf(void)
{
  u8 scan_code;

  while(kb_buf.count <= 0);

  disable_int();
  scan_code = *(kb_buf.tail);
  ++kb_buf.tail;
  if (kb_buf.tail == kb_buf.buf + KB_NUM)
    kb_buf.tail = kb_buf.buf;
  --kb_buf.count;
  enable_int();
  return scan_code;
}

u32 keymap[] =
{
0,
KEY_ESC,
'1',
'2',
'3',
'4',
'5',
'6',
'7',
'8',
'9',
'0',
'-',
'=',
KEY_BACKSPACE,
KEY_TAB,
'q',
'w',
'e',
'r',
't',
'y',
'u',
'i',
'o',
'p',
'[',
']',
KEY_ENTER,
KEY_CTRL_L,
'a',
's',
'd',
'f',
'g',
'h',
'j',
'k',
'l',
';',
'\'',
'`',
KEY_SHIFT_L,
'\\',
'z',
'x',
'c',
'v',
'b',
'n',
'm',
',',
'.',
'/',
KEY_SHIFT_R,
'*',
KEY_ALT,
' ',
KEY_CAPSLOCK,
KEY_F1,
KEY_F2,
KEY_F3,
KEY_F4,
KEY_F5,
KEY_F6,
KEY_F7,
KEY_F8,
KEY_F9,
KEY_F10,
KEY_NUMLOCK,
KEY_SCROLL_LOCK,
KEY_HOME,
/* 0x48 */ KEY_UP,
/* 0x49 - PgUp          */      KEY_PAGE_UP,
/* 0x4A - '-'           */      KEY_MINUS,
/* 0x4B - Left          */      KEY_LEFT,
/* 0x4C - MID           */      KEY_MID,
/* 0x4D - Right         */      KEY_RIGHT,
/* 0x4E - '+'           */      KEY_PLUS,
/* 0x4F - End           */      KEY_END,
/* 0x50 - Down          */      KEY_DOWN,
KEY_PAGE_DOWN,
KEY_INSERT,
KEY_DOT,
0,
0,
0,
KEY_F11,
KEY_F12,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
/*0x6b*/0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
/*0x7f*/0,
};

int lookup_key(u8 scan_code)
{
  return keymap[scan_code];
  //return KEY_UP;
}

int parse_scan_code(KeyStatus *key_status, int keyborad_mode)
{
  u8 make;
  int ret=0;
  static u8 shift_r=0;
  static u8 shift_l=0;

  if(kb_buf.count > 0)
  {
    u8 scan_code = get_byte_from_kb_buf();
    switch (scan_code)
    {
      case 0xe1:
        break;
      case 0xe0:
        scan_code = get_byte_from_kb_buf();
        key_status->key = lookup_key(scan_code & 0x7f);
        key_status->press = ((scan_code & 0x80) ? RELEASE: PRESS);
        break;
      default:
        key_status->key = lookup_key(scan_code & 0x7f);
        key_status->press = ((scan_code & 0x80) ? RELEASE: PRESS);
        switch (key_status->key)
        {
          case KEY_SHIFT_R:
            shift_r=1;
            break;
          case KEY_SHIFT_L:
            shift_l=1;
            break;
        }

        break;
    }

  }
  else
  {
    ret=1;
  }
  return ret;
}
