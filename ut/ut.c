/*
 * test some function
 *
 */

#include "k_stdio.h"
#include <stdio.h>

int main(int argc, const char *argv[])
{
  char buf[256];
  for (int i=0 ; i < 256 ; ++i)
    buf[i] = 'A';
  int hi=0xac98;
  int i=53;
  int len=s32_sprintf(buf, "%d %x %s\n", i, hi, "test");
  printf("%d ## %s", len, buf);
  return 0;
}
