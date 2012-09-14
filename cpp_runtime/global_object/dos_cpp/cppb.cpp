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
void test_static_obj();

//Io io;

extern "C" int cpp_main(void)
{
  //BOCHS_MB
  //Ab ab(7);
  test_static_obj();
  test_static_obj();

  return 0;
}

void test_static_obj()
{
  static Io io;
  // for compare static object
  //Io io;
}
