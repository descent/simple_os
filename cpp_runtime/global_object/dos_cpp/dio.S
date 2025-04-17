# print_string.s - AT&T syntax for real-mode DOS
# void print_string(const char* str);

    .code16                         # 16-bit code
    .globl print_string            # C 函數名 (DJGPP 會加底線)

print_string:
    push %ebp
    mov  %esp, %ebp

    mov  8(%ebp), %dx                # 將參數 str 傳給 DX
    mov  $0x09, %ah                 # AH = 09h 顯示字串
    int  $0x21                      # 呼叫 DOS 服務

    pop  %ebp
    ret
