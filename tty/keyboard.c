#include "io.h"
#include "k_stdio.h"

#define KEYBOARD_IRQ 1


void put_irq_handler(int irq, IrqHandler handler);
int enable_irq(int irq_no);



void init_keyboard(void)
{
  void keyboard_handler(int irq);

  s32_print("init keyboard", (u8*)(0xb8000+160*23));
  put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
  enable_irq(KEYBOARD_IRQ);
}

void keyboard_handler(int irq)
{
  static cur_pos=0;
  //clear_line(22);
  //s32_print("*", (u8*)(0xb8000+160*22));

  u8 scan_code = io_in8(0x60);
  s32_print_int(scan_code, (u8*)(0xb8000+160*22 + cur_pos*2), 16);
  cur_pos+=3;
}
