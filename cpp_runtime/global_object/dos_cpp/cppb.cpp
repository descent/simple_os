__asm__(".code16gcc\n");
#include "io.h"
#include "obj.h"
#include "type.h"

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

void s16_print_int(int i, int radix);
void print_str(const char   *s);
void test_static_obj();


extern "C" int cpp_main(void)
{
  //BOCHS_MB
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
