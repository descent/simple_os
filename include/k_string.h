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

static inline int s_strcmp(const char *s1, const char *s2)
{
  s8 res;
  while(1)
    if ((res = *s1 - *s2++) != 0 || !*s1++)
      break;
  return res;
}


#endif
