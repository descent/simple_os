#include <stdio.h>

typedef signed int s32;
typedef unsigned int u32;

static inline u32 asm_get_esp(void)
{
  u32 v=0;
  __asm__ __volatile__
    (
      "movl %%esp, %%eax\n"
      : "=a"(v)// output
    );
  return v;
}


void func(char c, int i, const char *ptr)
{
  ++c;
  ++i;
  ++ptr;
  printf("%c %d %p\n", c, i, ptr);
}

void asm_func(char c, int i, const char *ptr);



int main(int argc, const char *argv[])
{
  u32 esp = asm_get_esp();

  //func('a', 9, (char*)(0x1234));        
  printf("esp: %x\n", esp);
  asm_func('a', 9, (char*)(0x1234));        
  printf("esp: %x\n", esp);
  return 0;
}
