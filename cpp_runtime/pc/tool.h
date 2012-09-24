#ifndef TOOL_H
#define TOOL_H

#include "type.h"

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

void print(char c);
void print(const char *s);
u8 *get_vb();

#endif
