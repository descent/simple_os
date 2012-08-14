#include "io.h"
Io::Io():str_("data member\r\n")
{
  print("Io ctor\r\n");
  print(str_);
}
Io::~Io()
{
  print("dtor\r\n");
}

void Io::print(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}

Ab::Ab(int i)
{
  print("Ab::ctor\r\n");
  i_ = i ; 
}

Ab::~Ab()
{
  print("dtor\r\n");
}

void Ab::print(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}
