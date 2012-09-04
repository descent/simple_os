#include "io.h"
#include "irq.h"
#include "vga.h"
#include "k_stdio.h"
#include "k_ctype.h"
#include "keyboard.h"

static KeyboardBuf kb_buf;
static u8 capslock=0;
static u8 shift_r=0;
static u8 shift_l=0;
static u8 alt_r=0;
static u8 alt_l=0;

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
  //if (lookup_key(scan_code & 0x7f, 0) == 'g')
    //switch_vga_mode();
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

static int keymap[NR_SCAN_CODES*KEYMAP_COLS] =
{
0, 0,
KEY_ESC, KEY_ESC,
'1', '!',
'2', '@',
'3', '#',
'4', '$',
'5', '%',
'6', '^',
'7', '&',
'8', '*',
'9', '(',
'0', ')',
'-', '_',
'=', '+',
KEY_BACKSPACE, KEY_BACKSPACE,
KEY_TAB, KEY_TAB,
'q', 'Q',
'w', 'W',
'e', 'E',
'r', 'R',
't', 'T',
'y', 'Y',
'u', 'U',
'i', 'I',
'o', 'O',
'p', 'P',
'[', '{',
']', '}',
KEY_ENTER, KEY_ENTER,
KEY_CTRL_L, KEY_CTRL_L,
'a', 'A',
's', 'S',
'd', 'D',
'f', 'F',
'g', 'G',
'h', 'H',
'j', 'J',
'k', 'K',
'l', 'L',
';', ':',
'\'', '\"',
'`', '~',
KEY_SHIFT_L, KEY_SHIFT_L,
'\\', '|',
'z', 'Z',
'x', 'X',
'c', 'C',
'v', 'V',
'b', 'B',
'n', 'N',
'm', 'M',
',', '<',
'.', '>',
'/', '?',
KEY_SHIFT_R, KEY_SHIFT_R,
'*', '*',
KEY_ALT_L, KEY_ALT_L,
' ', ' ',
KEY_CAPSLOCK, KEY_CAPSLOCK,
KEY_F1, KEY_F1,
KEY_F2, KEY_F2,
KEY_F3, KEY_F3,
KEY_F4, KEY_F4,
KEY_F5, KEY_F5,
KEY_F6, KEY_F6,
KEY_F7, KEY_F7,
KEY_F8, KEY_F8,
KEY_F9, KEY_F9,
KEY_F10, KEY_F10,
KEY_NUMLOCK, KEY_NUMLOCK,
KEY_SCROLL_LOCK, KEY_SCROLL_LOCK,
KEY_HOME, KEY_HOME,
/* 0x48 */ KEY_UP, KEY_UP,
/* 0x49 - PgUp          */      KEY_PAGE_UP, KEY_PAGE_UP,
/* 0x4A - '-'           */      KEY_MINUS, KEY_MINUS,
/* 0x4B - Left          */      KEY_LEFT, KEY_LEFT,
/* 0x4C - MID           */      KEY_MID, KEY_MID,
/* 0x4D - Right         */      KEY_RIGHT, KEY_RIGHT,
/* 0x4E - '+'           */      KEY_PLUS, KEY_PLUS,
/* 0x4F - End           */      KEY_END, KEY_END,
/* 0x50 - Down          */      KEY_DOWN, KEY_DOWN,
KEY_PAGE_DOWN, KEY_PAGE_DOWN,
KEY_INSERT, KEY_INSERT,
KEY_DOT, KEY_DOT,
0, 0,
0, 0,
0, 0,
KEY_F11, KEY_F11,
KEY_F12, KEY_F12,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
/*0x6b*/0, 0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
0,0,
/*0x7f*/0, 0
};

int lookup_key(u8 scan_code, u8 col)
{
  return keymap[scan_code*KEYMAP_COLS+col];
  //return KEY_UP;
}

typedef struct Pair_
{
  s16 key1;
  s16 key2;
}Pair;

Pair shift_map[]=
{
  '`', '~',
  '1', '!',
  '2', '@',
  '3', '#',
  '4', '$',
  '5', '%',
  '6', '^',
  '7', '&',
  '8', '*',
  '9', '(',
  '0', ')',
  '-', '_',
  '=', '+',
  '[', '{',
  ']', '}',
  '\\', '|',
  ';', ':',
  '\'', '\"',
  ',', '<',
  '.', '>',
  '/', '?'
};

int to_shift_key(int key)
{
  int map_key=key;

  if (('a' <= key) && (key <= 'z'))
  {

  if (capslock==0)
  {
    if (shift_r==1 || shift_l==1)
      map_key = toupper(key);
    else
      map_key = tolower(key);
  }
  else
  {
    if (shift_r==1 || shift_l==1)
      map_key = tolower(key);
    else
      map_key = toupper(key);
  }

  }
  else
  {
  }
  return map_key;
}

int parse_scan_code(KeyStatus *key_status, int keyborad_mode)
{
  u8 make;
  int ret=0;
  static u8 keymap_col=0;

  if(kb_buf.count > 0)
  {
    u8 scan_code = get_byte_from_kb_buf();
    switch (scan_code)
    {
      case 0xe1:
        break;
      case 0xe0:
        scan_code = get_byte_from_kb_buf();
        key_status->key = lookup_key(scan_code & 0x7f, keymap_col);
        key_status->press = ((scan_code & 0x80) ? RELEASE: PRESS);
        break;
      default:
        if (keyborad_mode==ASCII_MODE && key_status->press == PRESS)
        {
          switch (key_status->key)
          {
            case KEY_SHIFT_R:
              shift_r=1;
              break;
            case KEY_SHIFT_L:
              shift_l=1;
              break;
          }
        }
        if (keyborad_mode==ASCII_MODE && key_status->press == RELEASE)
        {
          switch (key_status->key)
          {
            case KEY_SHIFT_R:
              shift_r=0;
              break;
            case KEY_SHIFT_L:
              shift_l=0;
              break;
            case KEY_CAPSLOCK:
              capslock = (!capslock);
              break;
          }
        }


        //key_status->key = to_shift_key(key_status->key);

        if (capslock==0)
        {
          if (shift_r==1 || shift_l==1)
            keymap_col = 1;
          else
            keymap_col = 0;
        }
        else
        {
          if (shift_r==1 || shift_l==1)
            keymap_col = 0;
          else
            keymap_col = 1;
        }
        key_status->key = lookup_key(scan_code & 0x7f, keymap_col);
        key_status->press = ((scan_code & 0x80) ? RELEASE: PRESS);
        break;
    }

  }
  else
  {
    ret=1;
  }

  return ret;
}

int keyboard_read(Tty *tty)
{
  u8 make;
  int ret=0;
  static u8 keymap_col=0;
  static u8 alt_l=0;

  KeyStatus key_status;
  if(kb_buf.count > 0)
  {
    u8 scan_code = get_byte_from_kb_buf();
    //put_key(tty, get_byte_from_kb_buf() );
    key_status.key = lookup_key(scan_code & 0x7f, keymap_col);
    key_status.press = ((scan_code & 0x80) ? RELEASE: PRESS);
    // alt+F1, alt+F2, alt+F3 switch tty
    // else pass to tty inbuf
    switch (key_status.key)
    {
      case KEY_F1:
      case KEY_F2:
      case KEY_F3:
        if (alt_l == 1 && key_status.press == PRESS)
          select_tty(key_status.key - KEY_F1);
        alt_l = 0;
        break;
      case KEY_F6:
        if (alt_l == 1 && key_status.press == PRESS)
        {
          int set_vga_mode(void);
          select_tty(3);

          set_vga_mode();  // system call
          draw_256_grid();
          //draw_box();
          draw_bg();
          draw_str(20, 100, 15);
          #if 0
          draw_box_1(40, 0, 3);
          draw_box_1(40, 0, 3);
          draw_box_1(40, 50, 5);
          draw_box_1(100, 100, 10);
          #endif
        }
        alt_l = 0;
        break;
      case KEY_ALT_L:
        alt_l = 1;
        break;
      case 0x20 ... 0x7e: // ascii printable char. gnu extension: I don't want use gnu extension, but it is very convenience.
        if (key_status.press == PRESS)
          put_key(tty, key_status.key);
        alt_l = 0;
        break;
      default:
        if (key_status.press == PRESS)
          put_key(tty, key_status.key);
        alt_l = 0;
        break;

    }
  }

}
