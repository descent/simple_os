#include "io.h"
#include "type.h"

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");


template <typename T> 
T max(T a, T b)
{
  if (a > b)
    return a;
  else
    return b;
}

void test_static_obj();

extern "C" int cpp_main(void)
{
  //BOCHS_MB
  test_static_obj();
  test_static_obj();
  int ii = max(5, 6);
  float fi = max(7.8, 9.1);
  #if 0
  print_str("\r\n");
  s16_print_int(ii, 10);
  print_str("\r\n");
  s16_print_int(fi, 10);
  #endif

  return 0;
}

void test_static_obj()
{
  static Io io;
  // for compare static object
  //Io io;
}
