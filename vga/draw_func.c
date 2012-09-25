#include "draw_func.h"
#include "font_rawdata.h"
#include "k_stdlib.h"
#include "io.h"
#include "vga.h"

const int y_res = 320;

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
  //void draw_str();
  //draw_str();
}

// current only for 320X240
void draw_str(int ox, int oy, u8 color)
{
  const u8* image_addr = str;

  for (int y = 0 ; y < hb ; ++y)
  {    
    int xx=0;          
    for (int x = 0 ; x < wb ; ++x)
    { 
      u8 c = *image_addr;

      for (int i=7 ; i>=0 ; --i, ++xx)
      {
        if (((c >> i) & 0x1) == 1)
          draw_point(ox+xx, oy+y, color);
      }
      ++image_addr;
    }
  }

}

void draw_char(int dx, int dy, char ch)
{
  BOCHS_MB
  u8 line=1;

  const u8* image_addr = ch_data;
  int org_x=dx;
  int org_y=dy;
  int cur_x=org_x;
  int cur_y=org_y;
  u8 *org_vb = (u8*)(0xa0000 + cur_x + cur_y*320);// + dx + dy*320;
  u8 *vb = org_vb;

  for (int y = 0 ; y < ch_hb ; ++y)
  {    
    for (int x = 0 ; x < ch_wb ; ++x)
    { 
      u8 c = *image_addr;
          

      for (int i=7 ; i>=0 ; --i)
      {
        if (((c >> i) & 0x1) == 1)
          *vb = 7;
        else
          ;//printf("|");
        //++cur_x;
        ++vb;
      }

      ++image_addr;
    }
    ++cur_y;
    vb = (u8*)((0xa0000+(320*cur_y)) + org_x);
    ++line;
  }
}

#if 1
void draw_bg(int ox, int oy, u8 *pic_raw_data, int w, int h, u8 *palette)
{
  u8 *vb = (u8*)0xa0000;
  int line=0;
  const u8* image_addr = pic_raw_data;
  //extern u8 PALETTE[];
  u8 *p = palette;

  for (int i=0 ; i < 256; ++i)
  {
    set_palette(i, *p, *(p+1), *(p+2));
    p+=3;
  }


  for (int y = 0 ; y < h ; ++y)
  {    
    for (int x = 0 ; x < w ; ++x)
    { 
      u8 c = *image_addr;
      draw_point(ox+x, y, c);
      //*vb++ = c;

      ++image_addr;
    }
    //vb = (u8*)(0xa0000+(320*line));
    //++line;
  }
}

void draw_20x12_grid(int x, int y, u8 c)
{
  for (int h=0; h < 12 ; ++h)
    for (int w=0 ; w < 20 ; ++w)
      draw_point(x+w, y+h, c);
}

void draw_256_grid(void)
{
  u8 c=0;
  for (int y=0 ; y < 200 ; y+=12)
    for (int x=0 ; x < 320 ; x+=20)
      draw_20x12_grid(x, y, c++);

}

// only for 320X200X256 color
void draw_point(int x, int y, u8 c)
{
  u32 vm = 0xa0000;
  *((u8*)(vm + x + y*y_res)) = c;
}

void set_palette(int index, u8 r, u8 g, u8 b)
{
  io_out8(PALETTE_MASK,0xff);
  io_out8(PALETTE_REGISTER_WR,index);
  io_out8(PALETTE_DATA, r);
  io_out8(PALETTE_DATA, g);
  io_out8(PALETTE_DATA, b);
}

void draw_8x16_ch(int ox, int oy, u8 ch, u8 color)
{
  extern char font[];
  //extern char *font; note: is wrong, so point is diff from array
  const u8* image_addr = font + ch *16;

  for (int y = 0 ; y < 16 ; ++y)
  {    
    int xx=0;          
    for (int x = 0 ; x < 1 ; ++x)
    { 
      u8 c = *image_addr;

      for (int i=7 ; i>=0 ; --i, ++xx)
      {
        if (((c >> i) & 0x1) == 1)
          draw_point(ox+xx, oy+y, color);
      }
      ++image_addr;
    }
  }
}

#endif
