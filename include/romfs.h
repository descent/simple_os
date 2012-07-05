#ifndef ROMFS_H
#define ROMFS_H

typedef struct RomFsHeader_
{
  union
  {
    struct
    {
      u8 str[8];
    }id_str;
    u32 nextfh;
    u32 spec;
  }u;
  u32 size;
  u32 checksum;
}RomFsHeader;



#endif
