__asm__(".code16gcc\n");

void func(char c, int i, const char *ptr);

void c_main(void)
{
  func('a', 9, (char*)(0x1234));        
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
