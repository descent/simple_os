#ifndef STORAGE_H
#define STORAGE_H

#include "type.h"

#define MAX_STORAGE_DEVICE (2)
#define RAMDISK (0)

typedef unsigned int size_t;

typedef struct StorageDevice_
{
  u32 start_pos;
  u32 sector_size;
  u32 storage_size;
  int (*dout)(struct StorageDevice_ *sd, void *dest, u32 bias, u32 size);
  int (*din)(struct StorageDevice_ *sd, void *dest, u32 bias, u32 size);
}StorageDevice;


extern StorageDevice *storage[];
int register_storage_device(StorageDevice *sd, u32 num);
int ramdisk_driver_init(void);

#endif
