#include "k_stdio.h"

int main(int argc, const char *argv[])
{
#if 0
  *((unsigned char *)(0xb8000+160*22+0)) = 'e';
  *((unsigned char *)(0xb8000+160*22+1)) = 0x14;
  *((unsigned char *)(0xb8000+160*22+2)) = 'c';
  *((unsigned char *)(0xb8000+160*22+3)) = 0x94;
  *((unsigned char *)(0xb8000+160*22+4)) = 'h';
  *((unsigned char *)(0xb8000+160*22+5)) = 0x0f;
  *((unsigned char *)(0xb8000+160*22+6)) = 'o';
  *((unsigned char *)(0xb8000+160*22+7)) = 0x0c;
#endif
  s32_printf("I am app\r\n");
  return 2;
}
