/* for cb.c */
ENTRY(WinMain);
SECTIONS
{

    . = 0x100;
    .text : AT(0x7C00)
    {
        *(.text)
        *(.gnu.linkonce.t*)
    }
    .ctors :
    {
      start_ctors = .; _start_ctors = .; __start_ctors = .;
      *(.ctor*)
      end_ctors = .; _end_ctors = .; __end_ctors = .;
/*      . = ALIGN(0x1000); */
     }
    .dtors :
    {
      start_dtors = .; _start_dtors = .; __start_dtors = .;
      *(.dtor*)
      end_dtors = .; _end_dtors = .; __end_dtors = .;
      . = ALIGN(0x1000);
     }

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
        sbss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        ebss = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. */
    }
}
