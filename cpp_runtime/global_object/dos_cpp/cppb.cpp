__asm__(".code16gcc\n");
#include "io.h"


typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


/*
 * c bootloader
 */

//#define POINTER_TEST


#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");


Ab ab(7);
Io io;

extern int _start_ctors;
extern int _end_ctors;

extern "C" int cpp_main(void)
{
  //BOCHS_MB  
  const char *ver=__VERSION__;
  io.print("io hello cpp class\r\n");
  io.print("io g++ version: ");
  io.print(ver);
  io.print("\r\n");
  return 0;
}

void print_str(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}

char* itoa(int n, char* str, int radix)
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

void s16_print_int(int i, int radix)
{
  char str[12]="";
  char *str_ptr = str;

  str_ptr = itoa(i, str_ptr, radix);
  print_str(str_ptr);
}
