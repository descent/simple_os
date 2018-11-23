__asm__(".code16gcc\n");
#include "io.h"
#include "obj.h"

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

#if 0
Ab ab(7);
Io io;
Obj1 g_obj1;
#endif

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

void myfree(void *ptr)
{
  print_str("myfree\r\n");
}

void *mymalloc(int size)
{
  print_str("mymalloc ## size: ");
  s16_print_int(size, 10);
  print_str("\r\n");
  static char mem[256];
  return mem;
#if 0
  // ref: http://wiki.osdev.org/C%2B%2B_Exception_Support
  static char* freeMemoryBase = reinterpret_cast<char *>(SOME_BASE_ADDRESS);
  size = (size + 7) / 8 * 8;
  freeMemoryBase += size;
  return freeMemoryBase - size;
#endif
}

// ref: http://wiki.osdev.org/C%2B%2B#The_Operators_.27new.27_and_.27delete.27
void *operator new(unsigned int s)
{
  return mymalloc(s);
}

void operator delete(void *p, unsigned int s)
{
  myfree(p);
}

// new/delete: array version

void *operator new[] (unsigned int s)
{
  return mymalloc(s);
}

void operator delete[] (void *p)
{
}

// ref: gcc-5.5.0/libstdc++-v3/libsupc++/del_opvs.cc
void operator delete[] (void *p, unsigned int t)
{
  myfree(p);
}

// new/delete: no exception version


struct nothrow_t { };

const nothrow_t nothrow;

void *operator new (unsigned int s, const nothrow_t&) 
{
  return mymalloc(s);
}


extern "C" int cpp_main(void)
{
  print_str("cpp_main\r\n");
  s16_print_int(obj_count, 10);
  print_str("\r\n");
  
  Io *io_p = new Io;
  const char *ver=__VERSION__;
  io_p->print("io_p g++ version: ");
  io_p->print(ver);
  io_p->print("\r\n");
  delete io_p;

  // test new array
  int *pia = new int[1024];
  delete [] pia;

  // test no exception new

  Io *ne_io_p =  new(nothrow) Io;

#if 0
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
#endif
  return 0;
}

