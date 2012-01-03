// read fat floppy disk

#include <stdio.h>
#include <string.h>

#define LEN 512

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

int main()
{
  char buf[LEN];
  FILE *fs=fopen("/home/descent/git/myprogs/c-bootloader/Dos6.22.img", "r");

  int len=fread(buf, 1, LEN, fs);
  int byte_per_sector=0;
  int root_entry_count=0;
  int root_dir_sector=0; 

  printf("len: %d\n", len);
  printf("buf[11]: %x\n", buf[11]);
  printf("buf[12]: %x\n", buf[12]);
  memcpy(&byte_per_sector, buf+11, 2);

  printf("buf[17]: %x\n", buf[17]);
  printf("buf[18]: %x\n", buf[18]);
  memcpy(&root_entry_count, buf+17, 2);
  printf("byte_per_sector: %d\n", byte_per_sector);
  printf("root_entry_count: %d\n", root_entry_count);

  root_dir_sector = (root_entry_count * 32 / byte_per_sector);
  if ((root_entry_count * 32 % byte_per_sector) != 0)
  ++root_dir_sector;
  printf("root_dir_sector: %d\n", root_dir_sector);

  // root derectory from 19ed sector.
  fseek(fs, 19*byte_per_sector, SEEK_SET);

  int cur_pos=0;
  for (int i=0 ; i < root_entry_count ; ++i)
  {
    printf("item ## %d\n", i);
    fread(buf, 1, 32, fs); // a root directory item needs 32 byte
    char fname[8+1+3+1]="";
    memcpy(fname, buf, 11);
    printf("fname:%s\n", fname);
    int fa=buf[12];
    printf("file attr:%d\n", fa);
  
    u16 w_time, w_data;
    memcpy(&w_time, buf+0x16, 2);
    printf("w_time: %x\n", w_time);
    
    memcpy(&w_data, buf+0x18, 2);
    printf("w_data: %x\n", w_data);
  
    u16 first_cluster;
    memcpy(&first_cluster, buf+0x1a, 2);
    printf("first_cluster: %x\n", first_cluster);
  
    u32 f_size;
    memcpy(&f_size, buf+0x1c, 4);
    printf("f_size: %x\n", f_size);
    printf("f_size: %d\n", f_size);
    if (f_size <= 512)
    {
        int data_cluster=root_dir_sector+19+first_cluster-2;
	int f_offset=byte_per_sector*data_cluster;
	cur_pos = ftell(fs);
        fseek(fs, f_offset, SEEK_SET);
        int len=fread(buf, 1, f_size, fs);
        fseek(fs, cur_pos, SEEK_SET);
	printf("write save file len: %d\n", len);
        FILE *save_fs=fopen("/home/descent/git/myprogs/c-bootloader/config.sys", "w");
	fwrite(buf, 1, len, save_fs);
	fclose(save_fs);

    }
    else
    {
      // find fat cluster
    }
    if (strcmp("CONFIG  SYS", fname)==0)
        break;
  }



  fclose(fs);
  return 0;
}
