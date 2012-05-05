__asm__(".code16gcc\n");

void print(const char   *s);

void main(void)
{
  print("hello world");
  while(1);
}


void print(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}


