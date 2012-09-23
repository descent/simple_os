#ifndef K_ASSERT_H
#define K_ASSERT_H

#include "k_stdio.h"

void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp) if (exp) ;\
                    else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)

void panic(const char *fmt, ...);

static inline void spin(char *func_name)
{
  s32_printk("\nspinning in %s ...\n", func_name);
  while(1);
}


#endif
