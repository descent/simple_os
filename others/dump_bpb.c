#include "../include/type.h"

#include <stdio.h>
#include <stdlib.h>

// BIOS parameter block
typedef struct BPB_
{
  u8 vendor_name[9];
  u16 byte_per_sector;
  u8 sector_per_cluster;
  u16 boot_occupy_sector;
  u8 fat_count;
  u16 root_entry_count;
  u16 logic_sector_count;
  u8 floppy_type;
  u16 fat_occupy_sector;
  u16 sector_per_track;
  u16 head_count;
  u16 hidden_sector_count;
  u16 root_dir_occupy_sector; // not BPB part, only for convenience
  u16 root_dir_start_sector;  // not BPB part, only for convenience
}BPB;

BPB bpb;

void init_bpb(u8 *buff, BPB *bpb)
{
  // ref : http://home.educities.edu.tw/wanker742126/asm/ch18.html
  for (int i=0 ; i <9 ; ++i)
  {
    bpb->vendor_name[i] = buff[0x3+i];
  }
  bpb->byte_per_sector = ((buff[12] << 8) | buff[11]);
  bpb->sector_per_cluster = buff[0xd];
  bpb->boot_occupy_sector = ((buff[0xf] << 8) | buff[0xe]);
  bpb->fat_count = buff[0x10];
  bpb->root_entry_count = ((buff[18] << 8) | buff[17]);
  bpb->logic_sector_count = ((buff[0x14] << 8) | buff[0x13]);
  if (bpb->logic_sector_count==0)
    bpb->logic_sector_count = ((buff[0x23] << 8) | buff[0x20]); // more than 32MB
  bpb->fat_occupy_sector = ((buff[0x17] << 8) | buff[0x16]);
  bpb->sector_per_track = ((buff[0x19] << 8) | buff[0x18]);
  bpb->head_count = ((buff[0x1b] << 8) | buff[0x1a]);
  bpb->hidden_sector_count = ((buff[0x1f] << 8) | buff[0x1c]);
}

void print_bpb(BPB *bpb)
{
  printf("bpb->vendor_name: %s\n", bpb->vendor_name);
  printf("bpb->byte_per_sector : %d (%#x)\n", bpb->byte_per_sector, bpb->byte_per_sector);
  printf("bpb->sector_per_cluster : %d (%#x)\n", bpb->sector_per_cluster, bpb->sector_per_cluster);
  printf("bpb->boot_occupy_sector : %d (%#x)\n", bpb->boot_occupy_sector, bpb->boot_occupy_sector);
  printf("bpb->fat_count : %d (%#x)\n", bpb->fat_count, bpb->fat_count);
  printf("bpb->root_entry_count : %d (%#x)\n", bpb->root_entry_count, bpb->root_entry_count);
  printf("bpb->logic_sector_count : %d (%#x)\n", bpb->logic_sector_count, bpb->logic_sector_count);
  printf("bpb->fat_occupy_sector : %d (%#x)\n", bpb->fat_occupy_sector, bpb->fat_occupy_sector);
  printf("bpb->sector_per_track : %d (%#x)\n", bpb->sector_per_track, bpb->sector_per_track);
  printf("bpb->head_count : %d (%#x)\n", bpb->head_count, bpb->head_count);
  printf("bpb->hidden_sector_count : %d (%#x)\n", bpb->hidden_sector_count, bpb->hidden_sector_count);
  printf("bpb->root_dir_occupy_sector : %d (%#x)\n", bpb->root_dir_occupy_sector, bpb->root_dir_occupy_sector);
}


#define FSIZE 512

int main(int argc, const char *argv[])
{
  char buf[FSIZE];
  FILE *fs;        
  BPB bpb;

  if (argc <=1)
  {
    printf("%s fn\n",argv[0]);
    exit(-1);
  }

  fs = fopen(argv[1], "r");
  if (!fs)
  {
    printf("open file fail\n");
    exit(-2);
  }
  int count = fread(buf, 1, FSIZE, fs);
  printf("count: %d\n", count);
  if (count == FSIZE)
  {
    init_bpb(buf, &bpb);
    print_bpb(&bpb);
  }

  fclose(fs);
  return 0;
}
