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


Ab ab(7);
Io io;
Obj1 g_obj1;

extern int _start_ctors;
extern int _end_ctors;

void s16_print_int(int i, int radix);
void print_str(const char   *s);

extern int obj_count;

u16 asm_get_ds(void)
{
  u16 v=0;
  __asm__ __volatile__
    (
      "mov %%ds, %%ax\n"
      : "=a"(v)// output
    );
  return v;
}

u16 asm_get_cs(void)
{
  u16 v=0;
  __asm__ __volatile__
    (
      "mov %%cs, %%ax\n"
      : "=a"(v)// output
    );
  return v;
}

const int SOME_BASE_ADDRESS = 0x500;

void *mymalloc(int size)
{
  // ref: http://wiki.osdev.org/C%2B%2B_Exception_Support
  static char* freeMemoryBase = reinterpret_cast<char *>(SOME_BASE_ADDRESS);
  size = (size + 7) / 8 * 8;
  freeMemoryBase += size;
  return freeMemoryBase - size;
}

// ref: http://wiki.osdev.org/C%2B%2B#The_Operators_.27new.27_and_.27delete.27
void *operator new(unsigned int s)
{
  return mymalloc(s);
}

extern "C" int cpp_main(void)
{
  print_str("cpp_main\r\n");
  s16_print_int(obj_count, 10);
  print_str("\r\n");
  
  Io *io_p = new Io;

  u16 cs = asm_get_cs();
  u16 ds = asm_get_ds();
  io.print("cs: ");
  s16_print_int(cs, 16);
  io.print("\r\n");
  io.print("ds: ");
  s16_print_int(ds, 16);
  io.print("\r\n");
  //BOCHS_MB  
  const char *ver=__VERSION__;
  io.print("io hello cpp class\r\n");
  io.print("io g++ version: ");
  io.print(ver);
  io.print("\r\n");

  Obj1 obj1;
  Obj1 obj2;
  obj1 = obj2;
  obj1.test_obj1();

  return 0;
}

