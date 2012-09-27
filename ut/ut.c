/*
 * test some function
 *
 */

#include "k_stdio.h"
#include <stdio.h>

void panic(const char *fmt, ...)
{
  char buf[256];
  char *arg = (char *)(&fmt+1);

  u32 arg_content = *((int *)arg);

  int len=s32_vsprintf(buf, fmt, arg);
  //int len=s32_sprintf(buf, fmt, arg);
  printf("%d ## %s", len, buf);
  //printf("%d\n", len);
}


int main(int argc, const char *argv[])
{
  char buf[256];
  for (int i=0 ; i < 256 ; ++i)
    buf[i] = 'A';
  int hi=0x98ef;
  int i=53;
  int len = s32_sprintf(buf, "%d %x %s\n", i, hi, "testcc");
  //printf("%d ## %s", len, buf);
  //panic("%d %x %s\n", i, hi, "panic");
  panic("%d %x %s\n", i, hi, "panic");
  //printf("%d ## %s", len, buf);
  return 0;
}
