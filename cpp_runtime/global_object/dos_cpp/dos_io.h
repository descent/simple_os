__asm__(".code16gcc\n");

#ifndef DOS_IO_H
#define DOS_IO_H

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void print(const char   *s);
void print(int i, int radix);
void print_str(const char   *s);
char* itoa(int n, char* str, int radix);
void s16_print_int(int i, int radix);

namespace 
{

inline u16 asm_get_ds(void)
{
  u16 v=0;
  __asm__ __volatile__
    (
      "mov %%ds, %%ax\n"
      : "=a"(v)// output
    );
  return v;
}

inline u16 asm_get_cs(void)
{
  u16 v=0;
  __asm__ __volatile__
    (
      "mov %%cs, %%ax\n"
      : "=a"(v)// output
    );
  return v;
}
}
#endif
