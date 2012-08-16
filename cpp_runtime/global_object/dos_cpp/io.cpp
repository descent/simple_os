#include "io.h"

void print_str(const char   *s);

Io::Io():str_("data member\r\n")
{
  print("Io ctor: ");
  print(str_);
}
Io::~Io()
{
  print("io dtor\r\n");
}

void Io::print(const char   *s)
{
  print_str(s);
}

Ab::Ab(int i)
{
  print("Ab::ctor\r\n");
  i_ = i ; 
}

Ab::~Ab()
{
  print("ab dtor\r\n");
}

void Ab::print(const char   *s)
{
  print_str(s);
}
