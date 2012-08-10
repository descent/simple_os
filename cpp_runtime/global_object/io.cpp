#include "io.h"
Io::Io():str_("data member\r\n")
{
  print("ctor\r\n");
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
