#include "io.h"

void print_str(const char   *s);

int Io::count_ = 0;

Io::Io():str_("data member\r\n")
{
  print("Io ctor: ");
  print(str_);
  num_ = count_;
  s16_print_int(num_, 10);
  print("\r\n");
  ++count_;
}
Io::~Io()
{
  print("io dtor: ");
  s16_print_int(num_, 10);
  print("\r\n");
}

void Io::print(const char   *s)
{
  print_str(s);
}

Ab::Ab(int i)
{
  print("Ab::ctor # ");
  i_ = 2 ; 
  s16_print_int(i_, 10);
  print("\r\n");
}

Ab::~Ab()
{
  print("Ab::dtor # ");
  s16_print_int(i_, 10);
  print("\r\n");
}

void Ab::print(const char   *s)
{
  print_str(s);
}
