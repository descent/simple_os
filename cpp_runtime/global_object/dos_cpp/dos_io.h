__asm__(".code16gcc\n");

#ifndef DOS_IO_H
#define DOS_IO_H
void print_str(const char   *s);
char* itoa(int n, char* str, int radix);
void s16_print_int(int i, int radix);

#endif
