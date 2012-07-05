#include "type.h"
#include "include/endian.h"

//#include <stdio.h>

#define swap(a, b) \
{\
  a^=b; \
  b^=a; \
  a^=b;\
}



// ref:
// #include <endian.h>
// man endian
u32 htobe32(u32 host_32bits)
{
  union 
  {
    struct 
    {
      u8 c0;
      u8 c1;
      u8 c2;
      u8 c3;
    } ch;
    u32 ui;
  }u;

  u.ui = host_32bits;
  #if 1
  swap(u.ch.c0, u.ch.c3);
  swap(u.ch.c1, u.ch.c2);
  //printf("u: %x\n", u.ui);
  #endif

}

#if 0
int main()
{
  htobe32(0x12345678);

  return 0;
}
#endif
