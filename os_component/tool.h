#ifndef TOOL_H
#define TOOL_H

#include "type.h"

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

void print(int i, int base=10);
void print(char c);
void print(const char *s);
u8 *get_vb();
char* s32_itoa(u32 n, char* str, int radix);
char* s32_itoa_s(int n, char* str, int radix);

class Screen
{
  public:
    Screen();
    void print(char c);
    void print(const char *s);
  private:
    int x, y;
    u8 *vb_;
};

#endif
