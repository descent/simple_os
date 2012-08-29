__asm__(".code16gcc\n");

void func(char c, int i, const char *ptr);

void c_main(void)
{
  __asm__ ("mov $0, %ax");
  __asm__ ("mov %ax, %ss");
  __asm__ ("mov %ax, %ds");
  char *p=0;
  *p='a';
  __asm__ ("mov %cs, %ax");
  __asm__ ("mov %ax, %ss");
  __asm__ ("mov %ax, %ds");
  //while(1);
}

void func(char c, int i, const char *ptr)
{
  c+=3;
  i+=2;
  ++ptr;
  #if 0
  char ch;
  int stack_int;
  ch = 'b';
  stack_int = 8;
  #endif
}
