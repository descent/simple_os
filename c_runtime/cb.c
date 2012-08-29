__asm__(".code16gcc\n");

void func(char c, int i, const char *ptr);

void c_main(void)
{
  char *p=0;
  *p='a';
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
