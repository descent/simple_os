/* for cb.c */
ENTRY(_start);
SECTIONS
{

    . = 0x100;
    .text :
    {
        *(.text)
        *(.gnu.linkonce.t*)
    }
    /* for g++ 4.7 */
    .init_array :
    {
      __start_global_ctor__ = .;
      __end_globa_ctor__ = .;
    }
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
    }

    .data :
    {
        *(.data)
        *(.gnu.linkonce.d*)
    }

    .bss :
    {
        sbss = .; __bss_start__ = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        ebss = .; __bss_end__ = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. */
    }
}
