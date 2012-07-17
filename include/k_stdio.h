#ifndef K_STDIO_H
#define K_STDIO_H

// for kernel_loader.c
// prefix s16 means Simple 16 bit code
void s16_print_int(int i, int radix);
void print(const char   *s);

// =========================
// for p_kernel.elf
void s32_print_int(int i, u8 *vb, int radix);
void s32_print(const u8 *s, u8 *vb);
void clear_line(u8 line_no);
void clear();

void dump_u8(u8 *buff, int len);
void h2c(u8 hex, u8 ch[2]);

//bios int 16-0, Wait for Keypress and Read Character
//so all previous bios print messages may be seen
static inline void bios_wait_key()
{
  __asm__ __volatile__("xor %eax,%eax\n");
  __asm__ __volatile__("int $0x16\n");
}

// reset disk
void bios_reset_disk()
{
  __asm__ __volatile__("mov $0, %ah\n");
  __asm__ __volatile__("mov 0x0, %dl\n");
  __asm__ __volatile__("int $0x13\n");
}

// bochs magic break point
#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

#endif
