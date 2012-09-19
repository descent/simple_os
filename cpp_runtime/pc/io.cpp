#include "io.h"
#include "type.h"

int Io::count_ = 0;

Io::Io():str_("data member\r\n")
{
#if 1
  print("Io ctor: ");
  print(str_);
  num_ = count_;
  //s16_print_int(num_, 10);
  print("\r\n");
  ++count_;
  #endif
}
Io::~Io()
{
#if 0
  print("io dtor: ");
  s16_print_int(num_, 10);
  print("\r\n");
#endif
}

void Io::print(const char   *s)
{
  __asm__ __volatile__ ("mov $24, %ax\t\n");
  __asm__ __volatile__ ("mov %ax, %ds\t\n");
  static u8 *vb = (u8*)(0xb8000+160);
  *vb = 'I';
  ++vb;
  *vb = 9;
  ++vb;
  __asm__ __volatile__ ("mov $0x10, %ax\t\n");
  __asm__ __volatile__ ("mov %ax, %ds\t\n");
#if 0
  print_str(s);
#endif
}

Ab::Ab(int i)
{
#if 0
  print("Ab::ctor # ");
  i_ = 2 ; 
  s16_print_int(i_, 10);
  print("\r\n");
#endif
}

Ab::~Ab()
{
#if 0
  print("Ab::dtor # ");
  s16_print_int(i_, 10);
  print("\r\n");
#endif
}

void Ab::print(const char   *s)
{
#if 0
  print_str(s);
#endif
}

#if 0
int main()
{
  Io io;
  return 0;
}
#endif
