#include "draw_func.h"
#include "../type.h"

void draw_box()
{
  u8 *vb = (u8*)0xa0000;

  for (int x=0 ; x < 320 ; ++x)
    for (int y=0 ; y < 240 ; ++y)
    {
      *(vb + x + 320*y) = 1; // 0: black, 1: blue
    }
}

void draw_box_1(int draw_x, int draw_y, int c)
{
  u8 *vb = (u8*)0xa0000;

  for (int x = draw_x ; x < 40+draw_x ; ++x)
    for (int y = draw_y ; y < 40+draw_y ; ++y)
    {
      *(vb + x + 320*y) = c;
    }
}

