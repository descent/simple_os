/*
 * c++11 nullptr test
 */
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

extern "C" int cpp_main(void)
{
  __asm__ ("mov $0, %ax");
  __asm__ ("mov %ax, %ss");
  __asm__ ("mov %ax, %ds");
  //char *p=0;
  char *p=nullptr;
  *p='a';
  __asm__ ("mov %cs, %ax");
  __asm__ ("mov %ax, %ss");
  __asm__ ("mov %ax, %ds");

  return 0;
}

