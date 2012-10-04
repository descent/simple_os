#ifndef PROTECT_H
#define PROTECT_H
#include "type.h"

#define GDT_SIZE 128
#define IDT_SIZE 256

#define INDEX_TSS               4
#define INDEX_LDT_FIRST         5


// 8 byte
typedef struct Descriptor_
{
  u16 limit_low;
  u16 base_low;
  u8 base_mid;
  u8 attr1;
  u8 limit_high_attr2;
  u8 base_high;
}Descriptor;

// 8 byte
typedef struct Gate_
{
  u16 offset_low;
  u16 selector;
  u8 dcount;
  u8 attr;
  u16 offset_high;
}Gate;

#define SELECTOR_KERNEL_CS 8
#define SELECTOR_KERNEL_DS 16 // (8*2)
#define SELECTOR_KERNEL_GS 24+3 // (8*3) , RPL: 3

// ldt index
#define LDT_CODE 0
#define LDT_DATA 1

#define SELECTOR_TSS 8*4

#define	DA_386IGate	0x8E	/* 386 中斷門類型值			*/
#define DA_C 0x98
#define DA_DRW 0x92
#define DA_386TSS               0x89    /* 可用 386 任務狀態段類型值            */
#define DA_LDT                  0x82    /* 局部描述符表段類型值                 */

#define	PRIVILEGE_KRNL 0
#define	PRIVILEGE_USER 3

#define SEL_USE_LDT 4
#define SEL_USE_GDT 0

static inline void init_descriptor(Descriptor *desc, u32 base, u32 limit, u16 attr)
{
  desc->limit_low = (limit & 0xffff);
  desc->base_low = (base & 0xffff);
  desc->base_mid = ( (base >> 16) & 0xff);
  desc->attr1 = (attr & 0xff);
  desc->limit_high_attr2 = ((limit << 16) & 0x0f)| (attr >> 8) & 0xf0;
  desc->base_high = ((base >> 24) & 0xff);
}

static inline u32 seg2base(u16 seg)
{
  extern Descriptor gdt[];

  Descriptor *desc = &gdt[seg >> 3];
  return ( (desc->base_high << 24) | (desc->base_mid << 16) | desc->base_low );
}

static inline u32 linear2phy(u32 base, u32 offset)
{
  return (base+offset);
}

#endif
