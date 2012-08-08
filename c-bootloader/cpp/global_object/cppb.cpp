__asm__(".code16gcc\n");
#include "io.h"

/*
 * c bootloader
 */

//#define POINTER_TEST


#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

void *__dso_handle;
extern "C"
{
int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle)
{
  return 0;
}
}

  Io io;

extern "C" void WinMain(void)
{
  BOCHS_MB  
  #if 1
  __asm__ ("mov  %cs, %ax\n");
  __asm__ ("mov  %ax, %ds\n");
  __asm__ ("mov  %ax, %ss\n");
  //__asm__ ("mov  $0xfff0, %sp\n");
  #endif
  {
  io.print("hello cpp class\r\n");
  }
  #if 0
  unsigned char *vb = (unsigned char *)0xb8000;
  *vb = 'A';
  *(unsigned char *)0xb8001 = 0xc;
  *(unsigned char *)0xb8002 = 'B';
  *(unsigned char *)0xb8003 = 0x9;
  *(unsigned char *)0xb8004 = '@';
  *(unsigned char *)0xb8005 = 0xc;
  #endif
  while(1);
}



