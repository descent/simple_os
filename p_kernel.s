# kernel loader should enters protected mode
# the code will use selector:offset to access address
# for c runtime, maybe initialize bss and other c runtime environment again.
# reset gdt ptr
# setup page
# setup idt

.equ  SELECTOR_KERNEL_CS, 8

.extern gdt_ptr
.extern gdt

.code32
.text
.global _start
_start:
  sgdt gdt_ptr
  call init_protected_mode_by_c
  lgdt gdt_ptr
  jmp csinit
csinit:
  call init_bss_asm
  call startc
  mov $0xc,%ah
  mov $'K',%al
  mov %ax,%gs:((80*0+39)*2)
  jmp .

# prefix p means protected mode
.globl p_asm_memcpy
p_asm_memcpy:
    pushl   %ebp
    mov     %esp, %ebp

    pushl   %esi
    pushl   %edi
    pushl   %ecx

#    mov $0x7000, %ax
#    mov %ax, %fs

    mov     8(%ebp), %edi    /* Destination */
    mov     12(%ebp), %esi   /* Source */
    mov     16(%ebp), %ecx   /* Counter */
MemCpy.1:
    cmp     $0, %ecx  /* Loop counter */
    jz      MemCpy.2
    movb    %ds:(%esi), %al
    inc     %esi
    movb    %al, %ds:(%edi) # note use %ds segment register
    inc     %edi
    dec     %ecx
    jmp     MemCpy.1
MemCpy.2:
    mov     8(%ebp), %eax
    pop     %ecx
    pop     %edi
    pop     %esi
    mov     %ebp, %esp
    pop     %ebp
    ret


# init bss
init_bss_asm:
  movw $__bss_end__, %di    /* Destination */
  movw $__bss_start__, %si   /* Source */
#  movw $0x0, %ax
#  movw %ax, %gs
  jmp 2f
1:
  movw %si, %ax
  movb $0x1, (%eax)
#  movb $0x1, %ds:(%eax)
  add $1, %si

2:
  cmpw %di, %si
  jne 1b
  ret

