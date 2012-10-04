#include "k_assert.h"

void assertion_failure(char *exp, char *file, char *base_file, int line)
{
  printx("%c asert(%s) failed: file: %s, base_file: %s, ln%d", MAG_CH_ASSERT, exp, file, base_file, line);

  spin("assertion_failure()");
  __asm__ __volatile__("ud2");
}


void panic(const char *fmt, ...)
{
  int i;
  char buf[256];

  char *arg = (char *)(&fmt+1);
  int len=s32_vsprintf(buf, fmt, arg);

  printx("%c !!panic!! %s", MAG_CH_PANIC, buf);

  __asm__ __volatile__("ud2");
}
