// test protected mode
#include "type.h"

void test_p(void)
{
  u8 *vb = (u8 *)0xb8002;
  u8 *attr = (u8 *)0xb8003;

  *vb = 'M';
  *attr = 9;
  while(1);  
}
