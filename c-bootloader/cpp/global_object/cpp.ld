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

    .rodata :
    {
        start_ctors = .;
        *(SORT(.ctors.*))  /* Note the "SORT" */
        end_ctors = .;

        start_dtors = .;
        *(SORT(.dtors.*))
        end_dtors = .;

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
