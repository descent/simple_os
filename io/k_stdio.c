#include "type.h"
#include "k_stdio.h"
#include "console.h"

u8 *cur_vb = (u8*)0xb8000+160;


// prefix s32 means simple, 32bit code (in x86 protected mode)
void s32_put_char(u8 ch, u8 *vb)
{
  *vb = ch;
}

// text mode screen 25 * 80
void s32_print(const u8 *s, u8 *vb)
{
  while(*s)
  {
    s32_put_char(*s, vb);
    ++s;
    vb+=2;
  }
  cur_vb = vb;
  if ((int)cur_vb >= 0xb8000+160*24)
    cur_vb = (u8*)0xb8000+160;
}

// sign version
char* s32_itoa_s(int n, char* str, int radix)
{
  char digit[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p=str;
  char* head=str;
  //int radix = 10;

//  if(!p || radix < 2 || radix > 36)
//    return p;
  if (n==0)
  {
    *p++='0';
    *p=0;
    return str;
  }
  if (radix == 10 && n < 0)
  {
    *p++='-';
    n= -n;
  }
  while(n)
  {
    *p++=digit[n%radix];
    //s32_put_char(*(p-1), (u8*)(0xb8000+80*2));
    n/=radix;
  }
  *p=0;
  #if 1
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  #endif
  return str;
}

// no sign version
char* s32_itoa(u32 n, char* str, int radix)
{
  char digit[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p=str;
  char* head=str;
  //int radix = 10;

//  if(!p || radix < 2 || radix > 36)
//    return p;
  if (n==0)
  {
    *p++='0';
    *p=0;
    return str;
  }
  if (radix == 10 && n < 0)
  {
    *p++='-';
    n= -n;
  }
  while(n)
  {
    *p++=digit[n%radix];
    //s32_put_char(*(p-1), (u8*)(0xb8000+80*2));
    n/=radix;
  }
  *p=0;
  #if 1
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  #endif
  return str;
}

void s32_print_int(int i, u8 *vb, int radix)
{
  u8 str[12]="";
  u8 *str_ptr = str;

  str_ptr = s32_itoa(i, str_ptr, radix);
  s32_print(str_ptr, vb);
}

void clear_line(u8 line_no)
{
  u8* vb = (u8*)0xb8000 + 160*line_no;

  for (int x = 0; x < 80; ++x)
  {
    *vb++ = 0x20;
    *vb++ = WHITE;
  }
}

void clear()
{
  u8* vb = (u8*)0xb8000;
  const u8 row=25;
  
  for (int x = 0; x < 80*row ; ++x)
  {
    *vb++ = 0x20;
    *vb++ = WHITE;
  }
}

// preifx p means protected
void p_dump_u8(u8 *buff, int len)
{
  void h2c(u8 hex, u8 ch[2]);

  u8 *vb = (u8*)0xb8000;
  int line=24;

    for (int i=0 ; i < len ; ++i)
    {
      u8 c[4]="";
      //if (i%16==0)
      //  s32_print("\r\n", vb);
      u8 h=*(buff+i);
      c[3]=0;
      c[2]=0x20; // space
      h2c(h, c);
      s32_print(c, vb);
      vb+=6;
    }
    #if 0
    clear_line(line);
    s32_print("len:", (u8*)(0xb8000+160*line));
    s32_print_int(len, (u8*)(0xb8000+160*line+12*2), 16);
    #endif
}

int s32_printf(const char *fmt, ...)
{
  char buf[256];
  int i=1;
  char *p = buf;

  for (; *fmt ; ++fmt)
  {
    if (*fmt != '%')
    {
      *p++ = *fmt;
      continue;
    }
    ++fmt;
    u8 *arg = (u8 *)(&fmt + i); // nst argument address
    u32 arg_content = *((u32*)arg);

    switch (*fmt)
    {
      case 'd':
      {
        u8 str[12]="";
        s32_itoa(arg_content, str, 10);

        char *str_ptr = str;
        while(*str_ptr)
          *p++ = *str_ptr++;
        break;
      }
      case 'x':
      {
        u8 str[12]="";
        s32_itoa(arg_content, str, 16);

        char *str_ptr = str;
        while(*str_ptr)
          *p++ = *str_ptr++;
        break;
      }
      case 's':
      {
        char *str_ptr = (char *)arg_content;
        while(*str_ptr)
          *p++ = *str_ptr++;
        break;
      }
      default:
        break;
    }
    ++i; // point to next argument
 
  } // end for (char *p = buf; *fmt ; ++fmt, ++i)
  int len = p-buf;
  //buf[len]='\0';

  int write(char *buf, int len);
  write(buf, len);

  return 0;
}
