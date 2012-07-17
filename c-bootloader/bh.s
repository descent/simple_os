.code16
.text
.global begin
begin:
  mov     %cs,%ax
  mov     %ax,%ds
  mov     %ax,%es
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
.org 510
.word 0xaa55
