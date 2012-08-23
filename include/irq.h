#ifndef IRQ_H
#define IRQ_H

#define CLOCK_IRQ 0
#define KEYBOARD_IRQ 1

#define NR_IRQ 16

typedef void (*IntHandler)();
typedef void (*IrqHandler)();

extern IrqHandler irq_table[];

int disable_irq(int irq_no);
int enable_irq(int irq_no);

static inline void disable_int(void)
{
  __asm__ __volatile__("cli\t\n");
}

static inline void enable_int(void)
{
  __asm__ __volatile__("sti\t\n");
}

static inline void put_irq_handler(int irq, IrqHandler handler)
{
  disable_irq(irq);
  irq_table[irq] = handler;
}


#endif
