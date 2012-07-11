#include "type.h"
#include "endian.h"
#include "romfs.h"
#include "vfs.h"

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

  ret = register_file_system(&romfs_sb, ROMFS);
  romfs_sb.device = storage[RAMDISK];
  return ret;
}

INode *romfs_namei(SuperBlock *sb, char *dir)
{
}

u32 romfs_get_daddr(INode *node)
{
}
