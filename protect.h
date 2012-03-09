#ifndef PROTECT_H
#define PROTECT_H
#include "type.h"

#define GDT_SIZE 128
#define IDT_SIZE 256

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

#define	DA_386IGate	0x8E	/* 386 中斷門類型值			*/

#define	PRIVILEGE_KRNL	0


#endif
