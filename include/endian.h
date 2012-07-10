#ifndef ENDIAN_H
#define ENDIAN_H

#include "type.h"

#define swap(a, b) \
{\
  a^=b; \
  b^=a; \
  a^=b;\
}


typedef union U_
{
  struct 
  {
    u8 c0;
    u8 c1;
    u8 c2;
    u8 c3;
  } ch;
  u32 ui;
}U;

// ref:
// #include <endian.h>
// man endian

static inline u32 be32tole32(u32 big_endian_32bits)
{
  U u;
  u.ui = big_endian_32bits;
  swap(u.ch.c0, u.ch.c3);
  swap(u.ch.c1, u.ch.c2);
  return u.ui;
}

static inline u32 le32tobe32(u32 host_32bits)
{
  U u;
  u.ui = host_32bits;
  #if 1
  swap(u.ch.c0, u.ch.c3);
  swap(u.ch.c1, u.ch.c2);
  //printf("u: %x\n", u.ui);
  #endif
  return u.ui;
}


#endif
