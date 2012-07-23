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
  clear_line(4);
  s32_print("*", (u8*)(0xb8000+160*22));


}
