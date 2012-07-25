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
