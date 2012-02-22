/*
 * let bootloader load to memory
 */
asm(".code16gcc\n");

typedef unsigned char u8;
typedef unsigned int u32;

int i;
char* itoa(int n, char* str);

void print(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7)); 
    s++;
  }
}

int p()
{
  int c=i+1;
  char arr[10]="a";
  char *s = arr;
  
  //const char *str="test";
  //volatile u8 *video_addr = (u8*)0xB8000;
  //asm("movw $0xb000 %gs");
  //*video_addr = 'a';
  s = itoa(c, s);
  print(s);
  return c;
}

#if 1
char* itoa(int n, char* str)
{
  char digit[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p=str;
  char* head=str;
  int radix = 10;

//  if(!p || radix < 2 || radix > 36)
//    return p;
  if (n==0)
  {
    *p++='0';
    *p=0;
    return str;
  }
#if 0
  if (radix == 10 && n < 0)
  {
    *p++='-';
    n= -n;
  }
#endif
  while(n)
  {
    *p++=digit[n%radix];
    n/=radix;
  }
  *p=0;
#if 1
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    *head=*p;
    *p=temp;
  }
#endif
  return str;
}
#endif

#if 0
extern u32 __bss_start__;
extern u32 __bss_end__;
void init_bss()
{
  u8 *bss=(u8*)__bss_start__;
  *bss = 0x1;
  *(bss+1) = 0x1;
  *(bss+2) = 0x1;
  *(bss+3) = 0x1;
#if 0
  while (i!=(u8*)__bss_end__)
  {
    *i = 0x1;
    ++i;
  }
#endif
}
#endif
