#include "io.h"
#include "type.h"
#include "tool.h"

int Io::count_ = 0;

Io::Io():str_("data member")
{
#if 1

  print("Io ctor: ");
  print(str_);
  print('\n');
  num_ = count_;
  //s16_print_int(num_, 10);
  //print("\r\n");
  ++count_;
  #endif
}
Io::~Io()
{
#if 1
  print("io dtor:\n");
  //s16_print_int(num_, 10);
  //print("\r\n");
#endif
}


Ab::Ab(int i)
{
  print("Ab::ctor #\n");
#if 0
  i_ = 2 ; 
  s16_print_int(i_, 10);
  print("\r\n");
#endif
}

Ab::~Ab()
{
  print("Ab::dtor #\n");
#if 0
  s16_print_int(i_, 10);
  print("\r\n");
#endif
}


#if 0
int main()
{
  Io io;
  return 0;
}
#endif
