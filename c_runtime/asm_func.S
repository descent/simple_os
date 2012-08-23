.global asm_func
asm_func:
   push   %ebp
   mov    %esp,%ebp
#   sub    $0x28,%esp

#   movl   $addr,0xb(%esp)
#   movl   $56,0x8(%esp)
#   movl   $'z',0x4(%esp)
#   movl   $fmt, (%esp)

   movl %esp, esp_value
   pushl $addr
   pushl $56
   pushl $'z'
   pushl $fmt
   call print_esp
   call printf
   movl %esp, esp_value
   call print_esp


#   leave  
   mov %ebp, %esp
   pop %ebp
   ret    

print_esp:

  pushl esp_value
  pushl $esp_fmt
  call printf
  addl $8, %esp
  ret

.data
fmt: .asciz "%c %d %p\n"
addr: .asciz "str addr"
esp_fmt: .asciz "func esp: %x\n"
esp_value: .int
