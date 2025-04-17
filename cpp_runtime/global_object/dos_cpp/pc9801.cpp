__asm__(".code16gcc\n");
#include "io.h"
#include "obj.h"

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

extern "C" void print_string(const char* str);

extern "C" int cpp_main(void)
{
  //BOCHS_MB
  //u32 esp = get_sp();

#if 0
  while (val == 3)
  {
  }
#endif
  print_string("hello pc9801/c++\r\n$");

  __asm__ volatile ("mov  $0x4c00, %ax\t\n");
  __asm__ volatile ("int  $0x21\t\n");   // 回到 DOS

  return 0;
}
