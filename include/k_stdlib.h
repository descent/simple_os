#ifndef K_STDLIB_H
#define K_STDLIB_H

#include "type.h"

// bochs magic break point
#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

// sign version
char* s32_itoa_s(int n, char* str, int radix);

// unsign version
char* s32_itoa(u32 n, char* str, int radix);

static inline void asm_set_es(u16 es_v)
{
  __asm__ __volatile__ 
    (
      "mov %%ax, %%es\n"
      : // output
      : "a"(es_v)// input
    );
}

static inline u16 asm_get_es(void)
{
  u16 es_v=0;
  __asm__ __volatile__ 
    (
      "mov %%es, %%ax\n"
      : "=a"(es_v)// output
    );
  return es_v;
}

int get_kernel_info(u32 * b, u32 * l);

#endif
