#ifndef ROMFS_H
#define ROMFS_H

#define HARD_LINK 0
#define DIRECTORY 1
#define REGULAR_FILE 2
#define SYMBOLIC_LINK 3
#define BLOCK_DEVICE 4
#define CHAR_DEVICE 5
#define SOCKET 6
#define FIFO 7

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
  //u8 *fn; // not fixed length
  //u32 fn_len; // file name length, not RomFsHeader part, only for convenience.
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

static inline u8 get_file_type(u8 t)
{
  return (t & 7);
}


#endif
