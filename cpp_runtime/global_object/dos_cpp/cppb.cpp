/*
 * c++ runtime test
 */

__asm__(".code16gcc\n");
#include "io.h"
#include "obj.h"
#include "dos_io.h"

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

extern "C" int cpp_main(void)
{
  //BOCHS_MB  
  const char *ver=__VERSION__;
  print("io hello cpp class\r\n");
  print("io g++ version: ");
  print(ver);
  print("\r\n");
  Io io;
  DeriveIo d_io;
  Io *p_io = &io;
  Io *p_d_io = &d_io;
  p_io->fun1();
  p_d_io->fun1();

  return 0;
}

