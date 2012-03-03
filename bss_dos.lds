ENTRY(_start)

SECTIONS
{
  . = 0x100;
  .text :
  {
    *(.text)
  }
  .= ALIGN(32);
  .rodata :
  {
    *(.rodata)
  }
  .= ALIGN(32);

  .data :
  {
    *(.data)
  }

/*  .= 0xeffa; */
  .= ALIGN(32);
  __bss_start__ =.;
  .bss :
  {
    *(.bss)
  }
  __bss_end__ = .;

/*
  .sig : AT(0x7DFE)
  {
    SHORT(0xaa55);
  }
  .asig : AT(0x7e50)
  {
    SHORT(0xefab);
  }
  .bsig : AT(0x7f50)
  {
    SHORT(0xefab);
  }
*/
    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
/* add any unwanted sections spewed out by your version of gcc and flags here */
    }

}
