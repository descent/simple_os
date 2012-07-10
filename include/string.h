#ifndef STRING_H
#define STRING_H

#include "type.h"

// prefix s is simple
static inline u32 s_strlen(const char *s)
{
  u8 len=0;

  while(*s++)
    ++len;
  return len;
}


#endif
