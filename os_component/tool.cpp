#include "tool.h"

extern u32 load_addr;

Screen::Screen()
{
  vb_ = get_vb();
}

void Screen::print(char c)
{
}

void Screen::print(const char *s)
{
}

u8 *g_vb = 0;

int x=0;
int y=0;

u8 *get_vb()
{
  static u8 *vb = (u8*)(0xb8000+160 - load_addr);  
  if (g_vb == 0)
  {
    g_vb = vb;
    return vb;
  }
  else
    return g_vb;
}

#if 0
void print(char c)
{
//  void _GLOBAL__I_vb();
//  _GLOBAL__I_vb();
  //vb -= load_addr;
  BOCHS_MB

  u8 *vb = get_vb();
  if (c=='\n');
  {
    x=0;
    ++y;
  }

  *(vb+x) = c;
  *(vb+x+1) = 9;
  ++x;
}
#endif

void print(int i, int base)
{
  char str[10]="";
  char *p=str;
  p=s32_itoa(i, str, base);
  print(p);
}

void print(char c)
{
//  void _GLOBAL__I_vb();
//  _GLOBAL__I_vb();
  //vb -= load_addr;
  BOCHS_MB

  u8 *vb = get_vb();
  if (c=='\n')
  {
    x=0;
    ++y;
  }
  else
  {
    *(vb+x+y*160) = c;
    *(vb+x+1+y*160)= 9;
    x+=2;
  }
  //g_vb = vb;
}

void print(const char *s)
{
  const char *p=s;
  while(*p)
    print(*p++);
}

// sign version
char* s32_itoa_s(int n, char* str, int radix)
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
    //s32_put_char(*(p-1), (u8*)(0xb8000+80*2));
    n/=radix;
  }
  *p=0;
  #if 1
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  #endif
  return str;
}

// no sign version
char* s32_itoa(u32 n, char* str, int radix)
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
    //s32_put_char(*(p-1), (u8*)(0xb8000+80*2));
    n/=radix;
  }
  *p=0;
  #if 1
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  #endif
  return str;
}
