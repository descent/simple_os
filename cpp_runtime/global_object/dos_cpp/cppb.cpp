__asm__(".code16gcc\n");
#include "io.h"
#include "obj.h"

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


/*
 * c bootloader
 */

//#define POINTER_TEST


#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");


void s16_print_int(int i, int radix);
void print_str(const char   *s);

//Io io;

extern "C" int cpp_main(void)
{
  BOCHS_MB
  static Io io;

  return 0;
}

