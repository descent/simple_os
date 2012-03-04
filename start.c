#include "type.h"

void s32_put_char(u8 ch, u8 *vb)
{
  *vb = ch;
}

void s32_print(const u8 *s, u8 *vb)
{
  while(*s)
  {
    s32_put_char(*s, vb);
    ++s;
    vb+=2;
  }
}

char* s32_itoa(int n, char* str, int radix)
{
  char digit[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p=str;
  char* head=str;
  //int radix = 10;

//  if(!p || radix < 2 || radix > 36)
//    return p;
  if (n==0)
  {
    *p++='0';
    *p=0;
    return str;
  }
  if (radix == 10 && n < 0)
  {
    *p++='-';
    n= -n;
  }
  while(n)
  {
    *p++=digit[n%radix];
    n/=radix;
  }
  *p=0;
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  return str;
}

void startc()
{
  u8 *ro_str="ro_string";
  u8 *vb=(u8*)0xb8000;

  *vb = 'W';
  s32_print(ro_str, vb);
}
