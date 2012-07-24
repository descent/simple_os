#ifndef IRQ_H
#define IRQ_H

typedef void (*IntHandler)();
typedef void (*IrqHandler)();

extern IrqHandler irq_table[];

int disable_irq(int irq_no);
int enable_irq(int irq_no);


static inline void put_irq_handler(int irq, IrqHandler handler)
{
  disable_irq(irq);
  irq_table[irq] = handler;
}


#endif
