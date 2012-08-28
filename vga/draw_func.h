#ifndef DRAW_FUNC_H
#define DRAW_FUNC_H

#include "type.h"

void draw_box();
void draw_box_1(int draw_x, int draw_y, int c);
void draw_char(int x, int y, char ch);
void draw_bg(void);
void draw_256_grid(void);
void draw_point(int x, int y, u8 c);

#endif
