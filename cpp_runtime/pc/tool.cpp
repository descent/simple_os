#include "tool.h"
#include "type.h"

extern u32 load_addr;
u8 *vb = (u8*)(0xb8000+160 - load_addr);
void print(char c)
{
  *vb++ = c;
  *vb++ = 9;
}

void print(const char *s)
{
  const char *p=s;
  while(*p)
    print(*p++);
}
