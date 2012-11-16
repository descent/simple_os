#ifndef STRING_H
#define STRING_H

#include "type.h"

// kernel loader part function
void asm_memcpy(u8 *dest, u8 *src, int n);
void asm_seg_memcpy(u8 *dest, u8 *src, int n, u16 fs);
void asm_absolute_memcpy(u8 *dest, u8 *src, int n);

// prefix s is simple; 32 means in protoected mode
static inline u32 s_strlen(const char *s)
{
  u8 len=0;

  while(*s++)
    ++len;
  return len;
}

static inline char* s_strcpy(char *dest, const char *src)
{
  char *p = dest;
  while(*dest)
    *dest++ = *src++;
  *dest='\0';
  return p;
}

static inline int s_strcmp(const char *s1, const char *s2)
{
  s8 res;
  while(1)
    if ((res = *s1 - *s2++) != 0 || !*s1++)
      break;
  return res;
}

static inline int s_memcmp(const char *s1, const char *s2, int n)
{
  if ((s1 == 0) || (s2 == 0)) 
  { /* for robustness */
    return (s1 - s2);
  }

  const char * p1 = (const char *)s1;
  const char * p2 = (const char *)s2;
  int i;
  for (i = 0; i < n; i++,p1++,p2++) 
  {
    if (*p1 != *p2) 
    {
      return (*p1 - *p2);
    }
  }
  return 0;
}

void p_asm_memcpy(void *dest, void *src, u16 n);
void p_asm_memset(void *dest, int c, u16 n);

#endif
