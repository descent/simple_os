#include "io.h"

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");


Io io;
int main()
{
  io.print("hello cpp class\r\n");
  return 0;
}


