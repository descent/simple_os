#include "storage.h"
#include "kl_global.h"
#include "k_string.h"

#define RAMDISK_SECTOR_SIZE 512
#define RAMDISK_SECTOR_MASK (~(RAMDISK_SECTOR_SIZE-1))
#define RAMDISK_SECTOR_OFFSET ((RAMDISK_SECTOR_SIZE-1))

extern int __romfs_start__, __romfs_end__;

int ramdisk_dout(StorageDevice *sd, void *dest, u32 addr, u32 size)
{
  p_asm_memcpy((u8*)dest, (u8 *)(addr+sd->start_pos), size);
  //dump_u8((u8 *)(addr+sd->start_pos) , 16);
  //dump_u8((u8 *)(dest) , 16);
  //*(u8 *)(dest) = 0xff;

  return size;
}

#define RAMFS_ADDR (16*RAMDISK_ES + LOAD_KERNEL_OFFSET)



// c99
StorageDevice ramdisk_sd=
{
  .dout = ramdisk_dout,
  .sector_size = RAMDISK_SECTOR_SIZE,
  .storage_size = 2*1024*1024,
  //.storage_size = (int)&__romfs_end__ - (int)&__romfs_start__,
  //.start_pos = 0x40800000,
  .start_pos = (int)&__romfs_start__,
};

int ramdisk_driver_init(void)
{
  int ret;
  ramdisk_sd.storage_size = (int)&__romfs_end__ - (int)&__romfs_start__;
  ret = register_storage_device(&ramdisk_sd, RAMDISK);
  return ret;
}

StorageDevice *storage[MAX_STORAGE_DEVICE];

int register_storage_device(StorageDevice *sd, u32 num)
{
  if (storage[num])
    return -1;
  else
  {
    storage[num] = sd;
    return 0;
  }
}
