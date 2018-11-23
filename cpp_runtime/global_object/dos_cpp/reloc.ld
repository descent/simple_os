/* for cb.c */
ENTRY(_start);
SECTIONS
{

    . = 0x100;
    __image_copy_start = .;
    .text :
    {
        *(.text)
        *(.gnu.linkonce.t*)
    }
    .rodata :
    {
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .data :
    {
        *(.data.*)
        *(.gnu.linkonce.d*)
    }
    __image_copy_end = .;

 . = ALIGN(4);

__rel_dyn_start = .;
 .rel.dyn : {
  *(.rel.dyn*)
 }
__rel_dyn_end = .;

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
