#include "vfs.h"

SuperBlock *fs_type[MAX_SUPER_BLOCK];

int register_file_system(SuperBlock *type, u32 id)
{
  if (fs_type[id] == 0)
  {
    fs_type[id] = type;
    return 0;
  }
  return -1;
  
}

int unregister_file_system(SuperBlock *type, u32 id)
{
  fs_type[id] = 0;
}
