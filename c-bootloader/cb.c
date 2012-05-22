__asm__(".code16gcc\n");
/*
 * c bootloader
 */

//#define POINTER_TEST

void main(const char   *s);

int bbb=0; // test bss section

void WinMain(void)
{
#ifdef POINTER_TEST
  unsigned char *vb = (unsigned char *)0xb8000;
  *vb = 'A';
  *(unsigned char *)0xb8001 = 0xc;
  *(unsigned char *)0xb8002 = 'B';
  *(unsigned char *)0xb8003 = 0xc;
#else
  main("hello world");
#endif
  while(1);
}

#ifndef POINTER_TEST
void main(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}
#endif

