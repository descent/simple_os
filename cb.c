// read fat floppy disk

__asm__(".code16gcc\n");


//#define DOS_COM
#define NOCLIB

#ifndef NOCLIB
#include <stdio.h>
#include <string.h>
#endif

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __PACKED    __attribute__((packed))
#define __NORETURN  __attribute__((noreturn))

#define IMAGE_SIZE  8192
#define BLOCK_SIZE  512
#define IMAGE_LMA   (0x2000)
//#define IMAGE_LMA   0x8000
#define IMAGE_ENTRY 0x800c
#define buf_addr_val (*(u8 volatile*(IMAGE_LMA)))



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void    __NOINLINE __REGPARM print(const char   *s);

void main(void)
{
  print("ab");
  while(1);
}


void    __NOINLINE __REGPARM print(const char   *s){
        while(*s){
                __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
                s++;
        }
}


