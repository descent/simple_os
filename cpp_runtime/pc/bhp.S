# bootloader enter protect mode, then call c func

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
.set    DA_C,  0x98

gdt0: Descriptor  0, 0, 0
gdt1: Descriptor  0, 0xfffff, DA_CR | DA_32 | DA_LIMIT_4K
gdt2: Descriptor  0, 0xfffff, DA_DRW | DA_32 | DA_LIMIT_4K
gdt3: Descriptor  0, 0xfffff, DA_DRW | DA_32 | DA_LIMIT_4K
LABEL_DESC_NORMAL:  Descriptor  0,         0xffff, DA_DRW    # Normal descriptor is for back to real mode.
LABEL_DESC_CODE16:  Descriptor    0,         0xffff, DA_C      # 非一致程式碼段, 16, back to dos

.set gdt_len, (. - gdt0)
gdt_ptr: .2byte (gdt_len -1 ) # limit
         .4byte 0             # base

.set selector_code, (gdt1-gdt0)
.set selector_data, (gdt2-gdt0)
.set selector_code16, (LABEL_DESC_CODE16 - gdt0)
.set SelectorNormal, (LABEL_DESC_NORMAL - gdt0)



begin:
  #xchg %bx, %bx
  mov     %cs,%ax
  mov     %ax,%ds
  mov     %ax,%es
  mov     %ax,%ss

  movw %ax, (LABEL_GO_BACK_TO_REAL+3) # modify segment value, indexed memory mode, ref  professional aeesmbly language p 102.
  movw %sp, rm_stack_val


#  mov $0xb8000, %edi   
#  movb $'R', (%edi)
#  mov $0xb8001, %edi  
#  movb $0xc, (%edi)
#  jmp .

  xor     %eax, %eax
  mov     %cs, %ax
  shl     $4, %eax

  xchg %bx, %bx
  movl %eax, load_addr
#  addl    $(begin), %eax
#  sub $0x100, %eax
  movw    %ax, (gdt1 + 2)
  shr     $16, %eax
  movb    %al, (gdt1 + 4)
  movb    %ah, (gdt1 + 7)

#  xor     %eax, %eax
#  mov     %cs, %ax
#  shl     $4, %eax
#  addl    $(begin), %eax
  movl    load_addr, %eax
  movw    %ax, (gdt2 + 2)
  shr     $16, %eax
  movb    %al, (gdt2 + 4)
  movb    %ah, (gdt2 + 7)

    /* Initialize 16-bits code segment descriptor. */
    xor     %eax, %eax
    mov     %cs, %ax
    shl     $4, %eax
    addl    $(LABEL_SEG_CODE16), %eax
    movw    %ax, (LABEL_DESC_CODE16 + 2)
    shr     $16, %eax
    movb    %al, (LABEL_DESC_CODE16 + 4)
    movb    %ah, (LABEL_DESC_CODE16 + 7)


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


#  ljmpl $selector_code, *(%eax)
  ljmpl $selector_code, $seg_code32

LABEL_REAL_ENTRY:               # 從保護模式跳回到實模式就到了這裡
        mov     %cx, %ax
        mov     %ax, %ds
        mov     %ax, %es
        mov     %ax, %ss
    
#        movw    rm_stack_val, %sp # if restore %sp will happen strange thing

        in      $0x92, %al
        and     $0b11111101, %al   #  close A20 line
        out     %al, $0x92
    
        sti                     # 開中斷
    
#  jmp .
        mov     $0x4c00, %ax
        int     $0x21             #  回到 DOS



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
  mov $24, %ax
  mov %ax, %ds
  mov %ax, %es

  mov $0xb8004, %edi    /* Destination */
  movb $'P', (%edi)
  mov $0xb8005, %edi    /* Destination */
  movb $0x9, (%edi)
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %ss
  #mov $TopOfStack, %esp
  mov $0x300000, %esp
  #calll _start1
  calll _start

  ljmpl $selector_code16,$0

#.org 510
#.word 0xaa55
.data
.global load_addr
pm_entry: .int
rm_stack_val: .int
load_addr: .int
.global PM_STACK
PM_STACK:
.space  512, 0
.set    TopOfStack, (. - PM_STACK - 1)



.code16
LABEL_SEG_CODE16:
    #jmp     .
        # back to real mode
        mov     $SelectorNormal, %ax
        mov     %ax, %ds
        mov     %ax, %es
        mov     %ax, %fs
        mov     %ax, %gs
        mov     %ax, %ss

        mov     %cr0, %eax
        and     $0b11111110, %al
        mov     %eax, %cr0


LABEL_GO_BACK_TO_REAL:
#.2byte 0xea66
#.4byte 0x00000000
#.2byte LABEL_REAL_ENTRY
    jmp     $0, $LABEL_REAL_ENTRY      # 段位址會在程序開始處被設置成正確的值


.set Code16Len, . - LABEL_SEG_CODE16


