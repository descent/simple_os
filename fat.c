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

// copy from write_os/src/chapter2/2/boot.S
__asm__ ("BS_OEMName:         .ascii  \"descent\" \n");    /* OEM String, 8 bytes required */
__asm__ ("BPB_BytsPerSec:     .2byte  512          \n");   /* Bytes per sector */
__asm__ ("BPB_SecPerCluster:  .byte   1            \n");   /* Sector per cluster */
__asm__ ("BPB_ResvdSecCnt:    .2byte  1            \n");   /* Reserved sector count */
__asm__ ("BPB_NumFATs:        .byte   2            \n");   /* Number of FATs */
__asm__ ("BPB_RootEntCnt:     .2byte  224          \n");   /* Root entries count */
__asm__ ("BPB_TotSec16:       .2byte  2880         \n");   /* Total sector number */
__asm__ ("BPB_Media:          .byte   0xf0         \n");   /* Media descriptor */
__asm__ ("BPB_FATSz16:        .2byte  9            \n");   /* FAT size(sectors) */
__asm__ ("BPB_SecPerTrk:      .2byte  18           \n");   /* Sector per track */
__asm__ ("BPB_NumHeads:       .2byte  2            \n");   /* Number of magnetic heads */
__asm__ ("BPB_HiddSec:        .4byte  0            \n");   /* Number of hidden sectors */
__asm__ ("BPB_TotSec32:       .4byte  0            \n");   /* If TotSec16 equal 0, this works */
__asm__ ("BS_DrvNum:          .byte   0            \n");   /* Driver number of interrupt 13 */
__asm__ ("BS_Reserved1:       .byte   0            \n");   /* Reserved */
__asm__ ("BS_BootSig:         .byte   0x29         \n");   /* Boot signal */
__asm__ ("BS_VolID:           .4byte  0            \n");   /* Volume ID */
__asm__ ("BS_VolLab:          .ascii  \"c-loader0.1\"\n"); /* Volume label, 11 bytes required */
__asm__ ("BS_FileSysType:     .ascii  \"FAT12   \"   \n"); /* File system type, 8 bytes required */


#define LEN 512

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

char* itoa(int n, u8* str, int radix);
void    __NOINLINE __REGPARM print(const char   *s);

void print_num(int n, u8 *sy)
{
  u8 str[10];
  u8 *s = str;
  s = itoa(n, s, 10);
  print("\r\n");
  print(sy);
  print(":");
  print(s);
}

#if 0
void dump_u8(u8 *buff, u16 count)
{
  void h2c(u8 hex, u8 ch[2]);

    for (int i=0 ; i < count ; ++i)
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
#endif

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

char* itoa(int n, u8* str, int radix)
{
  char digit[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p=str;
  char* head=str;

  if(!p || radix < 2 || radix > 36)
    return p;
  if (n==0)
  {
    *p++='0';
    *p=0;
    return p;
  }
  if (radix == 10 && n < 0)
  {
    *p++='-';
    n= -n;
  }
  while(n)
  {
    *p++=digit[n%radix];
    n/=radix;
  }
  *p=0;
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    *head=*p;
    *p=temp;
  }
  return str;
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

//ref: http://forum.osdev.org/viewtopic.php?f=1&t=7762
  u8 sector_no = 1; // cl, 1 - 18
  u8 track_no=0; // ch
  u8 head_no=0; // dh
  u8 disk_no=0; // dl

  u16 byte_per_sector = 0;
  u16 root_entry_count = 0;
  u8 *buff = (u8*)IMAGE_LMA;
  int r = read_sector(buff, sector_no, 0, 0, 0);
  byte_per_sector = ((buff[12] << 8) | buff[11]);
  root_entry_count = ((buff[18] << 8) | buff[17]);
  print_num(byte_per_sector, "byte_per_sector");
  print_num(root_entry_count, "root_entry_count");

#if 0
  //if (argc >= 2)
  {
    //sector_no = argv[1] - 0x30 ; // cl, 1 - 18
  }
  print("\r\nsector:\r\n");
  print(argv[1][0]);
  sector_no = argv[1] - 0x30 ; // cl, 1 - 18
#endif

#if 0
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
#endif


#endif


  // 回到 DOS
  __asm__ ("mov     $0x4c00, %ax\n");
  __asm__ ("int     $0x21\n");
}
#endif
