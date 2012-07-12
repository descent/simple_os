#ifndef K_STDIO_H
#define K_STDIO_H

void s32_print_int(int i, u8 *vb, int radix);
void s32_print(const u8 *s, u8 *vb);
void clear_line(u8 line_no);
void clear();

void dump_u8(u8 *buff, int len);
void h2c(u8 hex, u8 ch[2]);

// bochs magic break point
#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

#endif
