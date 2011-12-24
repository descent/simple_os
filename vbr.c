/*
 * A simple bootloader skeleton for x86, using gcc.
 *
 * Prashant Borole (boroleprashant at Google mail)
 * */
 
/* XXX these must be at top */
#include "code16gcc.h"
__asm__ ("jmpl  $0, $main\n");
//unsigned char stack[]={0x1,2,3,4};
 
#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __NORETURN  __attribute__((noreturn))
 
/* BIOS interrupts must be done with inline assembly */
void    __NOINLINE __REGPARM print(const char   *s){
        while(*s){
                __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
                s++;
        }
}
/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
 
// setup stack address to 0x7c00 - 0x200 = 0x7a00
// %ss = 0x7a00, %esp = 0
void __NORETURN main(){
    __asm__ ("mov  %cs, %ax\n");
    __asm__ ("mov  %ax, %ds\n");
    __asm__ ("mov  $0x07a0, %ax\n");
    __asm__ ("mov  %ax, %ss\n");
    __asm__ ("mov  $0, %esp\n");
    print("woo hoo!\r\n:)");
    while(1);
}
