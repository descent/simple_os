/*
 * A simple bootloader skeleton for x86, using gcc.
 *
 * Prashant Borole (boroleprashant at Google mail)
 * */
 
/* XXX these must be at top */
#include "code16gcc.h"
__asm__ ("jmpl  $0, $main\n");
 
 
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
 
void __NORETURN main(){
    print("woo hoo!\r\n:)");
    while(1);
}
