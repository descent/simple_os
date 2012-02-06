// read fat floppy disk

//#define DOS_COM
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
#ifdef DOS_COM
__asm__ ("jmp main\n");
#else
__asm__ ("jmp  main\n");
//__asm__ ("jmpl  $0, $main\n");
#endif

// copy from write_os/src/chapter2/2/boot.S
__asm__ ("BS_OEMName:         .ascii  \"descent \" \n");    /* OEM String, 8 bytes required */
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

#ifdef DOS_COM
void print_num(int n, u8 *sy)
{
  u8 str[10];
  u8 *s = str;
  s = itoa(n, s, 10);
  //print("\r\n");
  print(sy);
  print(":");
  print(s);
}
#endif

#ifdef DOS_COM
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


/* BIOS interrupts must be done with inline assembly */
void    __NOINLINE __REGPARM print(const char   *s){
        while(*s){
                __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
                s++;
        }
}

#ifdef DOS_COM
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
#endif

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

#if 0
int s_strncmp(const char *s1, const char *s2, u32 n);
{
  char *p1 = s1;
  char *p2 = s2;

  while (*p1 == *p2)
    ++p1;
    ++p2

}
#endif

#ifdef DOS_COM
int main(int argc, char **argv)
#else
void __NORETURN main(void)
#endif
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


#if 1

//ref: http://forum.osdev.org/viewtopic.php?f=1&t=7762
  u8 sector_no = 1; // cl, 1 - 18
  u8 track_no=0; // ch
  u8 head_no=0; // dh
  u8 disk_no=0; // dl

  u16 byte_per_sector = 0;
  u16 root_entry_count = 0;
  volatile u8 *buff = (u8*)IMAGE_LMA;

//#ifdef DOS_COM
#if 1
  //int r = read_sector(buff, sector_no, 0, 0, 0);

  // logic sector no 19 is root directory
  sector_no = 1; // cl, 1 - 18
  track_no=0; // ch
  head_no=0; // dh
  disk_no=0; // dl

  int r = read_sector(buff, sector_no, track_no, head_no, disk_no);
  byte_per_sector = ((buff[12] << 8) | buff[11]);
  root_entry_count = ((buff[18] << 8) | buff[17]);

#ifdef DOS_COM
  print_num(byte_per_sector, "byte_per_sector");
  print_num(root_entry_count, "root_entry_count");
#endif

  u16 root_dir_secotrs = 0;
  if (root_entry_count * 32 % byte_per_sector != 0)
    root_dir_secotrs = (root_entry_count * 32 / byte_per_sector) + 1;
  else
    root_dir_secotrs = (root_entry_count * 32 / byte_per_sector);

#ifdef DOS_COM
  print_num(root_dir_secotrs, "root_dir_secotrs"); // root dir occupy how many sectors
#endif


  int root_sec_no = 19;
  //int root_sec_no = 21;

  int cur_sec_no = root_sec_no;

  u16 read_sec = 0;

  for (int i=0 ; i <= root_dir_secotrs ; ++i, ++cur_sec_no)
  //for (int i=0 ; i < 1 ; ++i, ++cur_sec_no)
  {
  #if 0
    track_no = ((root_sec_no/18) >> 1);
    head_no = ((root_sec_no/18) & 1);
    sector_no = ((root_sec_no%18) + 1);
  #endif
#ifdef DOS_COM
    print_num(i + cur_sec_no, "cur sec no"); // root dir occupy how many sectors
#endif
    track_no = (((i + cur_sec_no)/18) >> 1);
    head_no = (((i + cur_sec_no)/18) & 1);
    sector_no = (((i + cur_sec_no)%18) + 1);
    r = read_sector(buff, sector_no, track_no, head_no, disk_no);
    
    for (u16 j=0 ; j < 512/32 ; ++j)
    //for (u16 j=0 ; j < 2 ; ++j)
    {
      u16 f_c = ((buff[0x1b+j*32] << 8) | buff[0x1a+j*32]); // first cluster
      u32 file_size = (( buff[0x1f + (j*32)] << 24) | (buff[0x1e + (j*32)] << 16) | (buff[0x1d + (j*32)] << 8) | buff[0x1c + (j*32)]);
      //u32 file_size = 0;
      //u32 file_size = ( buff[0x1f+(j*32)] << 24); 

      u8 filename[12]="";
      u8 attr = 0;
      for (int i=0 ; i < 11 ; ++i)
        filename[i] = buff[i+(j*32)];
      #if 1
      if (filename[0] == 0xe5) // del file
        continue;
      if (filename[0] == 0)
      {
        print("\r\nsearch end\r\n");
        goto search_end;
      }
      #endif


      attr = buff[0x0b + (j*32)];

      r = 0;
      for (int i=0 ; i < 11 ; ++i)
      {
        const u8 *ln = "LOADER  BIN";
        if (filename[i] != ln[i])
	{
	  r = 0;
	  break;
	}
	r = 1;

      }

      if (r)
      {
        read_sec = f_c - 2 + 33;
#ifdef DOS_COM
        print("\r\n");
        print("load it\r\n");
        print("\r\n");
        if (((attr >> 3) & 1) == 1) // label
        {
          print("label:");
          print(filename);
          continue;
        }
        else
        {
          print("\r\n");
          print(filename);
          print_num(f_c, " f_c");
          print_num(file_size, " file_size");
          print("\r\n");
        }
#endif
	goto search_end;
      }



    }


  }

  search_end:
  if (read_sec != 0)
  {
#ifdef DOS_COM
    print_num(read_sec, "read_sec");
#endif
    track_no = ((read_sec/18) >> 1);
    head_no = ((read_sec/18) & 1);
    sector_no = ((read_sec%18) + 1);
    #if 0
    print_num(track_no, "track_no");
    print_num(head_no, "head_no");
    #endif
    // if no the line, buff will get wrong data, very strange.
    print('s');
    r = read_sector(buff, sector_no, track_no, head_no, disk_no);

#ifdef DOS_COM
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
#endif
    //void*   e = (void*)IMAGE_ENTRY;
    void*   e = buff;
    //__asm__ __volatile__("" : : "d"(bios_drive));
    goto    *e;
  }
  else
  {
  }


#if 0
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
#endif

#endif

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
  #ifndef DOS_COM
  while(1);
  #endif
#endif


#endif


#ifdef DOS_COM
  // 回到 DOS
  __asm__ ("mov     $0x4c00, %ax\n");
  __asm__ ("int     $0x21\n");
#endif
}
