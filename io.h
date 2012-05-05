#ifndef IO_H
#define IO_H

#include "type.h"

static inline u8 io_in8(u16 port)
{

// ref: http://wiki.osdev.org/Inline_Assembly/Examples
  u8 ret;
  __asm__ volatile( "inb %1, %0"
               : "=a"(ret) : "Nd"(port) );
  return ret;
}


static inline
void io_out8(u16 port, u8 data)
{
//    outb    %al, $0x92
// N: "N" : Constant in range 0 to 255 (for out instruction).
// ref: http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html#s6
  __asm__ volatile("outb %0, %1"
               : : "a"(data), "Nd"(port) );
  __asm__ volatile("nop\t\n");
  __asm__ volatile("nop\t\n");
}

static inline
void io_out16(u16 port, u16 data)
{
  __asm__ volatile("outw %0, %1"
               : : "a"(data), "Nd"(port) );
  __asm__ volatile("nop\t\n");
  __asm__ volatile("nop\t\n");
}

#endif
