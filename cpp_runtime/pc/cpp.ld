MEMORY
{
  rom : ORIGIN = 0x00000100, LENGTH = 0x40000
  ram : ORIGIN = 0x100000, LENGTH = 30K
}
ENTRY(begin);
SECTIONS
{

    . = 0x100;
    .text :
    {
        *(.text)
        *(.gnu.linkonce.t*)
    } > rom
    /* for g++ 4.7 */
    .init_array :
    {
      __start_global_ctor__ = .;
    } 
    __end_global_ctor__ = .;
    .ctors :
    {
      start_ctors = .; _start_ctors = .; __start_ctors = .;
      *(.ctor*)
      end_ctors = .; _end_ctors = .; __end_ctors = .;
/*      . = ALIGN(0x1000); */
     }
     /*
    .dtors :
    {
      start_dtors = .; _start_dtors = .; __start_dtors = .;
      *(.dtor*)
      end_dtors = .; _end_dtors = .; __end_dtors = .;
      . = ALIGN(0x1000);
     }
     */

    .rodata :
    {
        *(.rodata*)
        *(.gnu.linkonce.r*)
    } > rom

    .data :
    {
        *(.data)
        *(.gnu.linkonce.d*)
    } > rom

    .bss :
    {
        __bss_start__ = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        __bss_end__ = .;
   } > ram

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. */
    }
}
