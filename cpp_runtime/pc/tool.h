#ifndef TOOL_H
#define TOOL_H

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

void print(char c);
void print(const char *s);

#endif
