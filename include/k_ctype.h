#ifndef K_CTYPE_H
#define K_CTYPE_H


// a : 0x61
// A : 0x41
// z : 0x7a
// Z : 0x5a
static inline int toupper(int c)
{
  if (('a' <= c) && (c <= 'z'))
    return (c-0x20);
  else
    return c;
}

static inline int tolower(int c)
{
  if (('A' <= c) && (c <= 'Z'))
    return (c+0x20);
  else
    return c;
}

#endif
