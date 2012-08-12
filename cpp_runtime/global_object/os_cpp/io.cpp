#include "io.h"
#include "cstdio"

using namespace std;

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
  printf("%s\n", s);
}
