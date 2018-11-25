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
int data1;

void (*data2)(void);

int rel_dyn_test()
{
  BOCHS_MB
  print_str("data_1: ");
  s16_print_int(data1, 16);
  print_str("\r\n");
  //data1 = 5;
  data2 = test_func;
  int i;
  i =  test_val;
  print_str("test_func_val: ");
  s16_print_int((int)test_func_val, 16);
  print_str("\r\n");
  (*test_func_val)();
    //printf("test = 0x%x\n", test_func);
    //printf("test_func = 0x%x\n", test_func_val);
  test_func();
  return i + data1;
}

extern int __image_copy_start;
extern int __image_copy_end;
extern int __rel_dyn_start;
extern int __rel_dyn_end;
extern u32 __bss_start__;
extern u32 __bss_end__;

void init_reloc_bss(u32 reloc_offset)
{
  u32 reloc_bss_b = (int)&__bss_start__ + reloc_offset;
  u32 reloc_bss_e = (int)&__bss_end__ + reloc_offset;
  print_str("reloc_bss_b: ");
  s16_print_int(reloc_bss_b, 16);
  print_str("\r\n");
  print_str("reloc_bss_e: ");
  s16_print_int(reloc_bss_e, 16);
  print_str("\r\n");
  for (u32 b = reloc_bss_b ; b < reloc_bss_e ; b++)
  {
    *(u8*)b = 0;
  }
}

void reloc(u32 reloc_addr)
{
  int from = (int)&__image_copy_start;
  int to = (int)&__image_copy_end;
  int image_size = to - from;
  u32 reloc_off = reloc_addr - from;

  print_str("reloc_addr: ");
  s16_print_int(reloc_addr, 16);
  print_str("\r\n");

  print_str("reloc_off: ");
  s16_print_int(reloc_off, 16);
  print_str("\r\n");


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
  init_reloc_bss(reloc_off);
  s16_print_int(image_size, 16);
  print_str("\r\n");

  // modify rel.dyn section
  for (int i = rel_dyn_from ; i < rel_dyn_to ; i+=8)
  {
    u32 v1 = *(u32*)i;
    u32 v2 = *(u32*)(i+4);
    #if 0
    print_str("v1: ");
    s16_print_int(v1, 16);
    print_str("\r\n");

    print_str("v2: ");
    s16_print_int(v2, 16);
    print_str("\r\n");
    #endif
    if (v2 == R_386_RELATIVE)
    {
      u32 mem_data = *(u32*)(v1 + reloc_off); // 0xa2c
      #if 0
      print_str("mem_data: ");
      s16_print_int(mem_data, 16);
      print_str("\r\n");
      #endif

      *(u32*)(v1+reloc_off) = mem_data + reloc_off; // locate offset
      mem_data = *(u32*)(v1 + reloc_off);
      #if 0
      print_str("after reloc mem_data: ");
      s16_print_int(mem_data, 16);
      print_str("\r\n");
      #endif
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
  u32 v = get_pc();
  print_str("before reloc pc: ");
  s16_print_int(v, 16);
  print_str("\r\n");

  reloc(0x1100);
  jmp_to_reloc_addr();

  print_str("after reloc to %cs:0x1100\r\n");

  v = get_pc();
  print_str("after reloc pc: ");
  s16_print_int(v, 16);
  print_str("\r\n");

  rel_dyn_test();
  
  return 0;
}

