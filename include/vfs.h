/*
 * ref 一步步寫嵌入式操作系統, chapter 7
 *
 */
#ifndef VFS_H
#define VFS_H

#include "type.h"
#include "storage.h"

#define MAX_SUPER_BLOCK 8
#define ROMFS 0


struct SuperBlock_;

typedef struct INode_
{
  char *fn_name;
  u32 flags;
  u32 dsize;
  u32 daddr;
  struct SuperBlock_ *sb;
}INode;

typedef struct SuperBlock_
{
  INode *(*namei)(struct SuperBlock_ *sb, char *p); 
  u32 (*get_daddr)(INode *);
  StorageDevice *device;
  char *fs_name;
}SuperBlock;

int register_file_system(SuperBlock *type, u32 id);
int unregister_file_system(SuperBlock *type, u32 id);

extern SuperBlock *fs_type[];
#endif
