#include "tool.h"

extern u32 load_addr;

u8 *get_vb()
{
  static u8 *vb = (u8*)(0xb8000+160 - load_addr);  
  return vb;
}

void print(char c)
{
//  void _GLOBAL__I_vb();
//  _GLOBAL__I_vb();
  //vb -= load_addr;
  BOCHS_MB

  u8 *vb = get_vb();
  *vb++ = c;
  *vb++ = 9;
}

void print(const char *s)
{
  const char *p=s;
  while(*p)
    print(*p++);
}
