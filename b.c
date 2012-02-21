asm(".code16gcc\n");

typedef unsigned char u8;
typedef unsigned int u32;

int i;

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
  const char *str="test";
  //volatile u8 *video_addr = (u8*)0xB8000;
  //asm("movw $0xb000 %gs");
  //*video_addr = 'a';
  print(str);
  return c;
}

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
