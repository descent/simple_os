__asm__(".code16gcc\n");

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

int a=0xaabbccdd;
int b=0;

extern "C" int cpp_main(void)
{
  BOCHS_MB
  a+=2;
  b+=5;
  return 0;
}
