# enter protect mode

.code16
.text
.global begin
  jmp begin

/* Segment Descriptor data structure.
   Usage: Descriptor Base, Limit, Attr
    Base:  4byte 
    Limit: 4byte (low 20 bits available)
    Attr:  2byte (lower 4 bits of higher byte are always 0) */
.macro Descriptor Base, Limit, Attr
    .2byte  \Limit & 0xFFFF
    .2byte  \Base & 0xFFFF
    .byte   (\Base >> 16) & 0xFF
    .2byte  ((\Limit >> 8) & 0xF00) | (\Attr & 0xF0FF)
    .byte   (\Base >> 24) & 0xFF
.endm

.set    DA_32,  0x4000  /* 32-bit segment */
.set    DA_LIMIT_4K, 0x8000     /* 4K */
.set    DA_CR,   0x9A   /* Execute/Read */
.set    DA_DRW,  0x92   /* Read/Write */

gdt0: Descriptor  0, 0, 0
gdt1: Descriptor  0, 0xfffff, DA_CR | DA_32 | DA_LIMIT_4K
gdt2: Descriptor  0, 0xfffff, DA_DRW | DA_32 | DA_LIMIT_4K
.set gdt_len, (. - gdt0)
gdt_ptr: .2byte (gdt_len -1 ) # limit
         .4byte 0             # base

.set selector_code, (gdt1-gdt0)
.set selector_data, (gdt2-gdt0)



begin:
  mov     %cs,%ax
  mov     %ax,%ds
  mov     %ax,%es
  mov     %ax,%ss

#  mov $0xb8000, %edi   
#  movb $'R', (%edi)
#  mov $0xb8001, %edi  
#  movb $0xc, (%edi)
#  jmp .

    xor     %eax, %eax
    mov     %cs, %ax
    shl     $4, %eax
    add     $gdt0, %eax      /* eax <- gdt base*/
    movl    %eax, (gdt_ptr + 2)

  lgdt gdt_ptr

  cli
  # open a20 line
  inb $0x92, %al
  orb $0b00000010, %al
  outb %al, $0x92

  # enable protect mode
  movl %cr0, %eax
  orl $1, %eax
  movl %eax, %cr0

  ljmpl $selector_code, $seg_code32
#  ljmpl $selector_code, $0


  movw    $0xb800, %ax
  movw    %ax, %gs

  mov    $0, %edi    /* Destination */
  mov    $msg, %esi   /* Source */

1:
  #cmp $0, %ecx
  cmpb $0, (%esi)
  jz 2f
  movb %ds:(%esi), %al
  inc %esi
  movb %al, %gs:(%edi)
  inc %edi
  movb $0xc, %gs:(%edi)
  inc %edi
  dec %ecx
  jmp 1b
2:
  movb $'E', %gs:(160)
  jmp .
#msg:.ascii "Hello GAS"
msg:
  .asciz "Hello GAS"
  #.asciz "Hello World"
.code32
seg_code32:
  mov $selector_data, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss
  mov $0xb8002, %edi    /* Destination */
  movb $'P', (%edi)
  mov $0xb8003, %edi    /* Destination */
  movb $0xc, (%edi)
  jmp .

.org 510
.word 0xaa55
