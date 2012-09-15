#include "k_assert.h"

void assertion_failure(char *exp, char *file, char *base_file, int line)
{
  s32_printk("");

  spin("assertion_failure()");

  __asm__ __volatile__("ud2");
}


void panic(const char *fmt, ...)
{
}
