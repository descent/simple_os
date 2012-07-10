#ifndef ROMFS_H
#define ROMFS_H

typedef struct RomFsHeader_
{
  union
  {
    u8 id_str[8];
    struct
    {
      u32 nextfh;
      u32 spec;
    }header8;
  }u;
  u32 size;
  u32 checksum;
}RomFsHeader;

static inline u32 get_next_16_boundary(int cur_offset)
{
  u32 len = cur_offset;

  if (len&15)
  {
    len += 16-(len&15);
  }
  //len+=16; // next 16 boundary
  return len;
}


#endif
