__asm__(".code16gcc\n");
#include "io.h"
#include "obj.h"

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

extern "C" void jmp_to_reloc_addr();
extern "C" u32 get_pc();

#define R_386_RELATIVE 0x00000008

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

extern int _start_ctors;
extern int _end_ctors;

void s16_print_int(int i, int radix);
void print_str(const char   *s);

void s32_memcpy(u8 *dest, const u8 *src, u32 n)
{
  for (u32 i=0; i < n ; ++i)
    *dest++ = *src++;
}

void test_func(void)
{
  print_str("test func\r\n");
  u32 v = get_pc();
  s16_print_int(v, 16);
  print_str("\r\n");
}

static auto test_func_val = test_func;
static int test_val = 10;

int rel_dyn_test()
{
  BOCHS_MB
  int i;
  i =  test_val;
  print_str("test_func_val: ");
  s16_print_int((int)test_func_val, 16);
  print_str("\r\n");
  (*test_func_val)();
    //printf("test = 0x%x\n", test_func);
    //printf("test_func = 0x%x\n", test_func_val);
  test_func();
  return i;
}

extern int __image_copy_start;
extern int __image_copy_end;
extern int __rel_dyn_start;
extern int __rel_dyn_end;



void reloc(u32 reloc_addr)
{
  int from = (int)&__image_copy_start;
  int to = (int)&__image_copy_end;
  int image_size = to - from;

  int rel_dyn_from = (int)&__rel_dyn_start;
  int rel_dyn_to = (int)&__rel_dyn_end;

  s16_print_int(from, 16);
  print_str("\r\n");
  s16_print_int(to, 16);
  print_str("\r\n");
  s16_print_int(rel_dyn_from, 16);
  print_str("\r\n");
  s16_print_int(rel_dyn_to, 16);
  print_str("\r\n");

  s32_memcpy((u8*)reloc_addr, (u8*)from, image_size);
  s16_print_int(image_size, 16);
  print_str("\r\n");

  // modify rel.dyn section
  for (int i = rel_dyn_from ; i < rel_dyn_to ; i+=8)
  {
    u32 v1 = *(u32*)i;
    u32 v2 = *(u32*)(i+4);
    print_str("v1: ");
    s16_print_int(v1, 16);
    print_str("\r\n");

    print_str("v2: ");
    s16_print_int(v2, 16);
    print_str("\r\n");
    if (v2 == R_386_RELATIVE)
    {
      u32 reloc_off = 0x1000;
      u32 mem_data = *(u32*)(v1 + reloc_off); // 0xa2c
    print_str("mem_data: ");
    s16_print_int(mem_data, 16);
    print_str("\r\n");

      *(u32*)(v1+reloc_off) = mem_data + reloc_off; // locate offset
       mem_data = *(u32*)(v1 + reloc_off);
    print_str("after reloc mem_data: ");
    s16_print_int(mem_data, 16);
    print_str("\r\n");
    }
    print_str("\r\n");
  }

  //s16_print_int(rel_dyn_to, 16);
  //print_str("\r\n");
  //s16_print_int(v, 16);
  //print_str("\r\n");

}

extern "C" int cpp_main(void)
{
  print_str("cpp_main\r\n");
  //s16_print_int(obj_count, 10);
  rel_dyn_test();

  reloc(0x1100);
  jmp_to_reloc_addr();

  print_str("after reloc to %cs:0x1100\r\n");

  u32 v = get_pc();
  s16_print_int(v, 16);
  print_str("\r\n");

  rel_dyn_test();
  
  return 0;
}

