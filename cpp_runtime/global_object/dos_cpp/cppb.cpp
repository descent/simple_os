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

  {
  const char *ver=__VERSION__;
  io.print("hello cpp class\r\n");
  io.print("g++ version: ");
  io.print(ver);
  io.print("\r\n");
  }
  return 0;
}

void *__dso_handle;
extern "C"
{
int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle)
{
  io.print("__cxa_atexit: register dtor here\r\n");
  return 0;
}
}
