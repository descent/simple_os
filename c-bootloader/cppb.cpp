__asm__(".code16gcc\n");
/*
 * c bootloader
 */

//#define POINTER_TEST

void print(const char   *s, const char *msg="\r\ng++ test");

int bbb=0; // test bss section

extern "C" void WinMain(void)
{
  __asm__ ("mov  %cs, %ax\n");
  __asm__ ("mov  %ax, %ds\n");
  __asm__ ("mov  %ax, %ss\n");
  __asm__ ("mov  $0xfff0, %sp\n");
#ifdef POINTER_TEST
  unsigned char *vb = (unsigned char *)0xb8000;
  *vb = 'A';
  *(unsigned char *)0xb8001 = 0xc;
  *(unsigned char *)0xb8002 = 'B';
  *(unsigned char *)0xb8003 = 0xc;
#else
  print("hello cpp");
  #if 0
  unsigned char *vb = (unsigned char *)0xb8000;
  *vb = 'A';
  *(unsigned char *)0xb8001 = 0xc;
  *(unsigned char *)0xb8002 = 'B';
  *(unsigned char *)0xb8003 = 0x9;
  *(unsigned char *)0xb8004 = '@';
  *(unsigned char *)0xb8005 = 0xc;
  #endif
#endif
  while(1);
}

#ifndef POINTER_TEST
void print(const char   *s, const char *msg)
{
#if 1
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
#endif
#if 1
  while(*msg)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *msg), "b"(7));
    msg++;
  }
#endif
}
#endif

