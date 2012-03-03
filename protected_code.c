// test protected mode
#include "type.h"

int AA;

void test_p(void)
{
  u8 *vb = (u8 *)0xb8002;
  u8 *attr = (u8 *)0xb8003;

  *vb = 'M';
  *attr = 9;
#if 0
  while(1);  
#else
  __asm__ ("nop\t\n");
  __asm__ ("nop\t\n");
#endif
  void jump_kernel();
  jump_kernel();
}

void jump_kernel()
{
  __asm__ ("jmp $0x8, $0x5000\t\n");
}
