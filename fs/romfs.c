#include "type.h"
#include "endian.h"
#include "romfs.h"
#include "vfs.h"
#include "k_string.h"
#include "k_stdio.h"
#include "k_stdlib.h"
#include "mm.h"

INode *romfs_namei(SuperBlock *sb, char *dir);
u32 romfs_get_daddr(INode *node);

SuperBlock romfs_sb =
{
  .namei = romfs_namei,
  .get_daddr = romfs_get_daddr,
  .fs_name = "romfs",
};

int romfs_init(void)
{
  int ret;

  //romfs_sb.namei = romfs_namei;
  //romfs_sb.get_daddr = romfs_get_daddr;

  ret = register_file_system(&romfs_sb, ROMFS);
  romfs_sb.device = storage[RAMDISK];
  return ret;
}

RomFsHeader* print_romfs_entry(int begin_print_line, u32 *offset, u8 *buf, u8 is_print)
{
  int line = begin_print_line;
  u8 next_offset = *offset;

  RomFsHeader *rom_fs_header; 
  rom_fs_header = (RomFsHeader*)(buf + next_offset);

  if (is_print)
  {
  clear_line(line);
  s32_print("nextfh: ", (u8*)(0xb8000+160*line));
  s32_print_int(be32tole32(rom_fs_header->u.header8.nextfh), (u8*)(0xb8000+160*line + 10*2), 16);
  ++line;

  clear_line(line);
  s32_print("spec: ", (u8*)(0xb8000+160*line));
  s32_print_int(be32tole32(rom_fs_header->u.header8.spec), (u8*)(0xb8000+160*line + 10*2), 16);
  ++line;

  clear_line(line);
  s32_print("size: ", (u8*)(0xb8000+160*line));
  s32_print_int(be32tole32(rom_fs_header->size), (u8*)(0xb8000+160*line + 10*2), 16);
  ++line;

  clear_line(line);
  s32_print("checksum: ", (u8*)(0xb8000+160*line));
  s32_print_int(be32tole32(rom_fs_header->checksum), (u8*)(0xb8000+160*line + 10*2), 16);
  ++line;
  }
  next_offset +=16; // file name offset, skip rom_fs_header
  u32 fn_len = s_strlen(buf+next_offset);
  //u32 fn_len = s_strlen(rom_fs_header->fn);

  if (is_print)
  {
  clear_line(line);
  s32_print("fn_len: ", (u8*)(0xb8000+160*line));
  s32_print_int(fn_len, (u8*)(0xb8000+160*line + 10*2), 10);
  ++line;

  clear_line(line);
  s32_print("fn: ", (u8*)(0xb8000+160*line));
  s32_print(buf+next_offset, (u8*)(0xb8000+160*line+10*2));

  switch (get_file_type(be32tole32(rom_fs_header->u.header8.nextfh)))
  {
    case HARD_LINK:
      s32_print("hard link", (u8*)(0xb8000+160*line)+ 15*2);
      break;
    case DIRECTORY:
      s32_print("dir", (u8*)(0xb8000+160*line)+ 15*2);
      break;
    case REGULAR_FILE:
      s32_print("reg file", (u8*)(0xb8000+160*line)+ 15*2);
      break;
  }
  ++line;
  }

  next_offset = get_next_16_boundary(next_offset + fn_len);
  //clear_line(line);
  //s32_print_int(next_offset, (u8*)(0xb8000+160*line), 16);
  //return next_offset;
  *offset = (be32tole32(rom_fs_header->u.header8.nextfh) & 0xfffffff0);
  return rom_fs_header;
}

//INode return_inode; // because not yet malloc function, use global variable for return.

INode *romfs_namei(SuperBlock *sb, char *dir)
{
  u8 *buf = alloc_mem();
  u32 read_count;
  u32 all_read_count=0;

  #if 1
  while(1)
  {
    //read_count = sb->device->dout(sb->device, buf, 0, sb->device->sector_size);
    read_count = sb->device->dout(sb->device, buf, 0, sb->device->storage_size); // read all ramdisk at once
    if (read_count == 0) return 0; // read nothing
    all_read_count += read_count;



  RomFsHeader *rom_fs_header;
  rom_fs_header = (RomFsHeader*)buf;
  u8 rom_fs_identify[9]="";
  p_asm_memcpy(rom_fs_identify, rom_fs_header->u.id_str, 8);

  int line;
  clear_line(2);
  s32_print(rom_fs_identify, (u8*)(0xb8000+160*2));
  line=3;
  clear_line(line);
  s32_print_int(rom_fs_header->size, (u8*)(0xb8000+160*line), 16);
  line=4;
  clear_line(line);

  s32_print_int(rom_fs_header->checksum, (u8*)(0xb8000+160*line), 16);
  u32 volume_len = s_strlen(buf+16);

  line=5;
  clear_line(line);
  s32_print_int(volume_len, (u8*)(0xb8000+160*line), 10);
  line=6;
  clear_line(line);
  s32_print(buf+16, (u8*)(0xb8000+160*line));

  // get volume name, 16 byte alignment
  // romfs content: use get_next_16_boundary to get next 16 byte boundary.
  u32 next_offset = get_next_16_boundary(volume_len+0x10); 
  line = 7;
  do
  {
    RomFsHeader *rom_fs_header; 

    rom_fs_header = print_romfs_entry(line, &next_offset, buf, 0);
    rom_fs_header = (RomFsHeader*)(buf+next_offset);
    u32 fn_offset = next_offset +16; // file name offset, skip rom_fs_header
    u32 fn_len = s_strlen(buf+fn_offset);

    if (s_strcmp(buf+fn_offset, dir) == 0)
    {
      INode *return_inode = (INode*)alloc_mem();
      clear_line(24);
      s32_print("find fn:", (u8*)(0xb8000+160*24));
      s32_print(dir, (u8*)(0xb8000+160*24 + 13*2));

      // fill inode
      return_inode->fn_name=buf+fn_offset; // fixed me: vary danger, because buf will to release or corrupt by other data.
      return_inode->dsize = be32tole32(rom_fs_header->size);
      return_inode->daddr = next_offset;
      return_inode->sb = sb;
      free_mem(buf);
      return return_inode;
    }

    u32 fn_content_offset = 0;
    u32 file_size = be32tole32(rom_fs_header->size);
    if (file_size)
      fn_content_offset = get_next_16_boundary(fn_offset+file_size);
#if 0
    clear_line(line);
    s32_print("file size:", (u8*)(0xb8000+160*line));
    s32_print_int(file_size, (u8*)(0xb8000+160*line+12*2), 16);
    ++line;
#endif
    clear_line(line);
    s32_print("next_offset:", (u8*)(0xb8000+160*line));
    s32_print_int(next_offset, (u8*)(0xb8000+160*line+12*2), 16);
    ++line;
    if (next_offset!=0 && fn_content_offset != 0)
    {
    clear_line(line);
    s32_print("fn_con_off:", (u8*)(0xb8000+160*line));
    s32_print_int(fn_content_offset, (u8*)(0xb8000+160*line+12*2), 16);
    ++line;
    clear_line(0);
    clear_line(1);
    p_dump_u8(buf+fn_content_offset, file_size);
  BOCHS_MB
    //dump_u8(buf+fn_content_offset, 7);
    }
  }while(next_offset);

  line = 25;
  clear_line(line);
  s32_print("scan romfs ok", (u8*)(0xb8000+160*line));
  #endif

    if (all_read_count >= sb->device->storage_size)
      return 0;
  } // end while(1)
}

u32 romfs_get_daddr(INode *inode)
{
  u32 fn_len = 2;//s_strlen(inode->fn_name);
  u32 fn_content_offset = 0;
  u32 file_size = inode->dsize;
  if (file_size)
    fn_content_offset = get_next_16_boundary(inode->daddr + fn_len);
  return (fn_content_offset+16);
}

char * k_readdir(const char *dirname)
{
  SuperBlock *sb = fs_type[ROMFS];
  static u8 buf[512];

  if (sb->device->dout(sb->device, buf, 0, sb->device->sector_size) != 0)
  {
    return 0; // error
  }

  RomFsHeader *rom_fs_header;
  rom_fs_header = (RomFsHeader*)buf;
  u8 rom_fs_identify[9]="";
  p_asm_memcpy(rom_fs_identify, rom_fs_header->u.id_str, 8);

  int line;
#if 0
  clear_line(2);
  s32_print(rom_fs_identify, (u8*)(0xb8000+160*2));
  line=3;
  clear_line(line);
  s32_print_int(rom_fs_header->size, (u8*)(0xb8000+160*line), 16);
  line=4;
  clear_line(line);

  s32_print_int(rom_fs_header->checksum, (u8*)(0xb8000+160*line), 16);
#endif
  u32 volume_len = s_strlen(buf+16);
#if 0

  line=5;
  clear_line(line);
  s32_print_int(volume_len, (u8*)(0xb8000+160*line), 10);
  line=6;
  clear_line(line);
  s32_print(buf+16, (u8*)(0xb8000+160*line));
#endif

  // get volume name, 16 byte alignment
  // romfs content: use get_next_16_boundary to get next 16 byte boundary.
  u32 next_offset = get_next_16_boundary(volume_len+0x10); 
  line = 7;
  do
  {
    RomFsHeader *rom_fs_header; 

    rom_fs_header = print_romfs_entry(line, &next_offset, buf, 0);
    rom_fs_header = (RomFsHeader*)(buf+next_offset);
    u32 fn_offset = next_offset +16; // file name offset, skip rom_fs_header
    u32 fn_len = s_strlen(buf+fn_offset);

    s32_print_str(buf+fn_offset);
    s32_print_str("\r\n");

    u32 fn_content_offset = 0;
    u32 file_size = be32tole32(rom_fs_header->size);
    if (file_size)
      fn_content_offset = get_next_16_boundary(fn_offset+file_size);
#if 0
    clear_line(line);
    s32_print("file size:", (u8*)(0xb8000+160*line));
    s32_print_int(file_size, (u8*)(0xb8000+160*line+12*2), 16);
    ++line;
    clear_line(line);
    s32_print("next_offset:", (u8*)(0xb8000+160*line));
    s32_print_int(next_offset, (u8*)(0xb8000+160*line+12*2), 16);
    ++line;
    if (next_offset!=0 && fn_content_offset != 0)
    {
    clear_line(line);
    s32_print("fn_con_off:", (u8*)(0xb8000+160*line));
    s32_print_int(fn_content_offset, (u8*)(0xb8000+160*line+12*2), 16);
    ++line;
    clear_line(0);
    clear_line(1);
    p_dump_u8(buf+fn_content_offset, file_size);
    //dump_u8(buf+fn_content_offset, 7);
    }
#endif
  }while(next_offset);

#if 0
  line = 25;
  clear_line(line);
  s32_print("scan romfs ok", (u8*)(0xb8000+160*line));
#endif
}
