#include "draw_func.h"
#include "type.h"
#include "font_rawdata.h"

void draw_box()
{
  u8 *vb = (u8*)0xa0000;

  for (int x=0 ; x < 320 ; ++x)
    for (int y=0 ; y < 200 ; ++y)
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
  void draw_str();
  draw_str();
}

// current only for 320X240
void draw_str()
{
  u8 *vb = (u8*)0xa0000;
  u8 line=1;

  const u8* image_addr = str;
  int cur_x=0;

  for (int y = 0 ; y < hb ; ++y)
  {    
    for (int x = 0 ; x < wb ; ++x)
    { 
      u8 c = *image_addr;
          

      for (int i=7 ; i>=0 ; --i)
      {
        if (((c >> i) & 0x1) == 1)
          *vb = 60;
        else
          ;//printf("|");
        ++cur_x;
        ++vb;
      }

      ++image_addr;
    }
    cur_x = 0;
   vb = (u8*)(0xa0000+(320*line));
    ++line;
  }

}
