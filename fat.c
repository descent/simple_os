// read fat floppy disk

#define NOCLIB

#ifndef NOCLIB
#include <stdio.h>
#include <string.h>
#endif

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __PACKED    __attribute__((packed))
#define __NORETURN  __attribute__((noreturn))

#define IMAGE_SIZE  8192
#define BLOCK_SIZE  512
#define IMAGE_LMA   (0x2000)
//#define IMAGE_LMA   0x8000
#define IMAGE_ENTRY 0x800c
#define buf_addr_val (*(u8 volatile*(IMAGE_LMA)))

/* XXX these must be at top */
#include "code16gcc.h"
//__asm__ ("jmpl  $0, $main\n");
__asm__ ("jmp main\n");

#define LEN 512

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
        unsigned char   spt;
        unsigned char   numh;
}drive_params_t;
 
int __REGPARM __NOINLINE get_drive_params(drive_params_t    *p, unsigned char   bios_drive){
        unsigned short  failed = 0;
        unsigned short  tmp1, tmp2;
        __asm__ __volatile__
            (
             "movw  $0, %0\n"
             "int   $0x13\n"
             "setcb %0\n"
             : "=m"(failed), "=c"(tmp1), "=d"(tmp2)
             : "a"(0x0800), "d"(bios_drive), "D"(0)
             : "cc", "bx"
            );
        if(failed)
                return failed;
        p->spt = tmp1 & 0x3F;
        p->numh = tmp2 >> 8;
        return failed;
}
 
int __REGPARM __NOINLINE lba_read(const void    *buffer, unsigned int   lba, unsigned char  blocks, unsigned char   bios_drive, drive_params_t  *p){
        unsigned char   c, h, s;
        c = lba / (p->numh * p->spt);
        unsigned short  t = lba % (p->numh * p->spt);
        h = t / p->spt;
        s = (t % p->spt) + 1;
        unsigned char   failed = 0;
        unsigned char   num_blocks_transferred = 0;

        __asm__ __volatile__
            (
             "movw  $0, %0\n"
             "int   $0x13\n"
             "setcb %0"
             : "=m"(failed), "=a"(num_blocks_transferred)
             : "a"(0x0200 | blocks), "c"((s << 8) | s), "d"((h << 8) | bios_drive), "b"(buffer)
            );
        return failed || (num_blocks_transferred != blocks);
}

#ifndef NOCLIB

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

#else

/* BIOS interrupts must be done with inline assembly */
void    __NOINLINE __REGPARM print(const char   *s){
        while(*s){
                __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
                s++;
        }
}

void h2c(u8 hex, u8 ch[2])
{
  u8 l = hex >> 4;

  if ( 0<= l && l <= 9)
  {
    ch[0]=l+0x30;
  }
  else
  {
    ch[0]=l+0x41-0xa; //a
    ch[0]=l+0x61-0xa; // A
  }

  l = hex & 0x0f;

  if ( 0<= l && l <= 9)
  {
    ch[1]=l+0x30;
  }
  else
  {
    ch[1]=l+0x41-0xa; //a
    ch[1]=l+0x61-0xa; // A
  }
}

//ref: http://forum.osdev.org/viewtopic.php?f=1&t=7762
  // read a sector
  // 1.44 MB floppy
  // HeadNumber: 0, 1
  // CylNum: 0 - 79
  // sector number: 1 - 18
  #if 0
  // directory assign value in inline assembly
  __asm__ __volatile__("movb $0, %ch\n"); 
  __asm__ __volatile__("movb $2, %cl\n"); // set the sector number, bits 0-5
  __asm__ __volatile__("movb $0, %dh\n"); 
  __asm__ __volatile__("movb $0, %dl\n"); // disk no, 0 -> A disk
  #endif

int read_sector(u8 *buff, u8 sector_no, u8 track_no, u8 head_no, u8 disk_no)
{
#if 0
  sector_no=3; // cl
  track_no=0; // ch
  head_no=0; // dh
  disk_no=0; // dl
#endif

  __asm__ __volatile__("movb $2, %ah\n"); 
  __asm__ __volatile__("movb $1, %al\n"); 
#if 1
  __asm__ ("int $0x13\n"
           :
	   :"b"(buff), "c"(track_no << 8 | sector_no), "d"(head_no << 8 | disk_no)
	  ); 
#endif
  return 0;
}

//void __NORETURN main(void)
int main(int argc, char **argv)
{
/*
    __asm__ ("mov  %cs, %ax\n");
    __asm__ ("mov  %ax, %ds\n");
    __asm__ ("mov  $0x07a0, %ax\n");
    __asm__ ("mov  %ax, %ss\n");
    __asm__ ("mov  $0, %esp\n");
*/
    print("woo hoo!\r\n:)");
//void h2c(u8 hex, u8 ch[2])

    //while(1);

#if 1

#if 0
  // reset A disk
  __asm__ __volatile__("movb $0, %ah\n"); 
  __asm__ __volatile__("movb $0, %dl\n"); // disk no, 0 -> A disk
  __asm__ ("int $0x13\n");
#endif

//ref: http://forum.osdev.org/viewtopic.php?f=1&t=7762
  u8 sector_no = 1; // cl, 1 - 18
  u8 track_no=0; // ch
  u8 head_no=0; // dh
  u8 disk_no=0; // dl

#if 0
  //if (argc >= 2)
  {
    //sector_no = argv[1] - 0x30 ; // cl, 1 - 18
  }
  print("\r\nsector:\r\n");
  print(argv[1][0]);
  sector_no = argv[1] - 0x30 ; // cl, 1 - 18
#endif

  u8 *buff = (u8*)IMAGE_LMA;
  for (int i=1 ; i <= 5 ; ++i)
  {
    sector_no = i;
    int r = read_sector(buff, sector_no, 0, 0, 0);

    for (int i=0 ; i < 32 ; ++i)
    {
      if (i%16==0)
        print("\r\n");
      u8 c[4]="";
      u8 h=*(buff+i);
      c[3]=0;
      c[2]=0x20;
      h2c(h, c);
      print(c);
    }
  }



  //void    *buff = (void*)IMAGE_LMA;
#if 0
  u8 *buff = (u8*)IMAGE_LMA;
  #if 0
  for (int i=0 ; i < 16 ; ++i)
    *(buff+i) = 0x90+i;
  #endif
  //buf_addr_val=0x9a;
  
  __asm__ __volatile__("movb $2, %ah\n"); 
  __asm__ __volatile__("movb $1, %al\n"); 
#if 1
  __asm__ ("int $0x13\n"
           :
	   :"b"(buff), "c"(track_no << 8 | sector_no), "d"(head_no << 8 | disk_no)
	  ); 
#endif
#endif

  
  //print("\r\n");
#endif

#if 0
  u8 h=0x3e;
  u8 c[3]="";
  h2c(h, c);
  print("\r\n");
  print(c);
  print("\r\n");
#endif

#if 0
        unsigned char   bios_drive = 0;
        __asm__ __volatile__("movb  %%dl, %0" : "=r"(bios_drive));      /* the BIOS drive number of the device we booted from is passed in dl register */
 
        drive_params_t  p = {};
        get_drive_params(&p, bios_drive);
 
        void    *buff = (void*)IMAGE_LMA;
        unsigned short  num_blocks = ((IMAGE_SIZE / BLOCK_SIZE) + (IMAGE_SIZE % BLOCK_SIZE == 0 ? 0 : 1));
#if 1
        if(lba_read(buff, 1, num_blocks, bios_drive, &p) != 0){
            print("read error :(\r\n");
            while(1);
        }
#endif
        print("Running next image...\r\n");
        //void*   e = (void*)IMAGE_ENTRY;
        //__asm__ __volatile__("" : : "d"(bios_drive));
        //goto    *e;
#endif

  // 回到 DOS
  __asm__ ("mov     $0x4c00, %ax\n");
  __asm__ ("int     $0x21\n");


}
#endif
