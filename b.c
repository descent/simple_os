asm(".code16gcc\n");

typedef unsigned char u8;

int i;

#if 1
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
#endif
