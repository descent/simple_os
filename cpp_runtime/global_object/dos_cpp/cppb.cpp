__asm__(".code16gcc\n");
#include "io.h"

/*
 * c bootloader
 */

//#define POINTER_TEST


#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");


Ab ab(7);
Io io;

extern int _start_ctors;
extern int _end_ctors;

extern "C" int cpp_main(void)
{
  //BOCHS_MB  
  const char *ver=__VERSION__;
  io.print("io hello cpp class\r\n");
  io.print("io g++ version: ");
  io.print(ver);
  io.print("\r\n");
  return 0;
}

void print_str(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}
