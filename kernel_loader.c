#include "type.h"
__asm__(".code16gcc\n");

// read fat floppy disk

//#define DOS_COM

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __PACKED    __attribute__((packed))
#define __NORETURN  __attribute__((noreturn))

#define IMAGE_SIZE  8192
#define BLOCK_SIZE  512
#define READ_FAT_ADDR (0x2000)
#define IMAGE_LMA   (0x4000)
//#define IMAGE_LMA   0x8000
#define IMAGE_ENTRY 0x800c
#define buf_addr_val (*(u8 volatile*(IMAGE_LMA)))


/* XXX these must be at top */

//u8 kernel_name[] = "KERNEL  BIN";
u8 kernel_name[] = "KERNEL  ELF";

/* BIOS interrupts must be done with inline assembly */
//void    __NOINLINE __REGPARM print(const char   *s){
void print_ch(const char c)
{
  __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | c), "b"(7));
}

void printn(const char *s, int num)
{
  for (int i=0 ; i < num; ++i)   
    print_ch(s[i]);
}
void print(const char   *s)
{
  while(*s)
  {
    __asm__ __volatile__ ("int  $0x10" : : "a"(0x0E00 | *s), "b"(7));
    s++;
  }
}


char* itoa(int n, char* str, int radix)
{
  char digit[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* p=str;
  char* head=str;
  //int radix = 10;

//  if(!p || radix < 2 || radix > 36)
//    return p;
  if (n==0)
  {
    *p++='0';
    *p=0;
    return str;
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
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  return str;
}

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



void dump_u8(volatile u8 *buff, u16 count)
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

int __REGPARM read_sector(volatile u8 *buff, u8 sector_no, u8 track_no, u8 head_no, u8 disk_no, u8 blocks)
{
#if 0
  sector_no=3; // cl
  track_no=0; // ch
  head_no=0; // dh
  disk_no=0; // dl
#endif

#if 0
  print_num(sector_no, "sector_no");
  print_num(track_no, "track_no");
  print_num(head_no, "head_no");
  print_num(disk_no, "disk_no");
#endif
//  __asm__ __volatile__("movb $2, %ah\n"); 
//  __asm__ __volatile__("movb $1, %al\n"); 
#if 1
  __asm__ __volatile__ ("int $0x13\n"
                        :
                        :"a"(0x0200|blocks), "b"(buff), "c"(track_no << 8 | sector_no), "d"(head_no << 8 | disk_no)
	               ); 
#endif
  //dump_u8(buff, 32*2);
  return 0;
}

u8 s_strlen(const char *s1)
{
  u8 len=0;
  
  while(*s1)
    ++len;
  return len;
}

// -1: not equal
// 0: equal
int s_strncmp(const char *s1, const char *s2, u8 n)
{
  for (int i=0 ; i < n ; ++i)
  {
    //print("\r\n");
    //print_ch(*s2++);
    if ((*s1) && (*s2))
      if ((*s1++) != (*s2++))
      {
        //print_ch(*s2);
        //print("\r\n");
	//print("not match");
        return -1;
      }
  }
  //print("\r\n");
  //print("match");
  return 0;
}

// -1: not equal
// 0: equal
int s_strcmp(const char *s1, const char *s2)
{
  u8 len1 = s_strlen(s1);
  u8 len2 = s_strlen(s2);
  if (len1 != len2) 
    return -1;
  else
    while(*s1 || *s2)
    {
      if (*s1 != *s2)
        return  -1;
    }
  return 0;
}

#define OK 1
#define FAIL -1

// FAT occupies 9 sections, sector no 1 ~ 10
int read_fat(volatile u8 *fat_buf, u16 fat_sector_no)
{
  u8 track_no = ((fat_sector_no/18) >> 1);
  u8 head_no = ((fat_sector_no/18) & 1);
  u8 sector_no = ((fat_sector_no%18) + 1);
  u8 disk_no = 0;

#if 0
  print_num(track_no, "fat track_no");
  print_num(head_no, "fat head_no");
  print_num(sector_no, "fat sector_no");
  print_num(disk_no, "fat disk_no");
  print("\r\n");
#endif
  u8 r = read_sector(fat_buf, sector_no, track_no, head_no, disk_no, 2);
  //dump_u8(fat_buf, 48);

  return FAIL;
}

int is_odd(int n)
{
  if ((n%2) == 0 )
    return -1;
  else
    return 1;
}

u16 get_next_cluster(volatile u8 *fat_buf, u16 cur_cluster)
{
  u16 offset, next_cluster=0;

  if (is_odd(cur_cluster) == 1)
  {
    offset = (cur_cluster-1) * 3 / 2;
    next_cluster = ((fat_buf[offset+1] >> 4) & 0x0f) | (fat_buf[offset+2] << 8);
    //offset = f_c * 1.5; // if f_c : 12 because of  24, 12 * 2??
    //offset = f_c >> 1;
  }
  else
  {
    offset = cur_cluster /2 *3; // if f_c : 12 -> 18
    next_cluster = ((fat_buf[offset+1] & 0x0f) << 8)| fat_buf[offset];
  }

#if 0
  print("\r\n");
  print_num(offset, "offset");
  print("\r\n");
  print_num(next_cluster, "next_cluster");
  print("\r\n");
#endif
  return next_cluster;
}

void start_c()
{
  print("\r\n");
  print("start c");
  print("\r\n");
  //const *str="zxc";
  //print_ch(*str);

  //while(1);

//ref: http://forum.osdev.org/viewtopic.php?f=1&t=7762
  u8 sector_no = 1; // cl, 1 - 18
  u8 track_no=0; // ch
  u8 head_no=0; // dh
  u8 disk_no=0; // dl

  u16 byte_per_sector = 512;
  u16 root_entry_count = 0;
  volatile u8 *buff = (u8*)IMAGE_LMA;

//#ifdef DOS_COM
  //int r = read_sector(buff, sector_no, 0, 0, 0);

#if 0
  // logic sector no 19 is root directory
  sector_no = 1; // cl, 1 - 18
  track_no=0; // ch
  head_no=0; // dh
  disk_no=0; // dl
#endif
  *buff = 0x1;

  int r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
  byte_per_sector = ((buff[12] << 8) | buff[11]);
  //byte_per_sector = 512;
  //root_entry_count = ((buff[18] << 8) | buff[17]);
//  root_entry_count = 224;
//  print_num(byte_per_sector, "byte_per_sector");
  byte_per_sector = ((buff[12] << 8) | buff[11]);
  root_entry_count = ((buff[18] << 8) | buff[17]);
  print_num(byte_per_sector, "byte_per_sector");
  print_num(root_entry_count, "root_entry_count");

  //dump_u8(buff, 48);
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


  u16 root_dir_secotrs = 0;
  if (root_entry_count * 32 % byte_per_sector != 0)
    root_dir_secotrs = (root_entry_count * 32 / byte_per_sector) + 1;
  else
    root_dir_secotrs = (root_entry_count * 32 / byte_per_sector);

  print_num(root_dir_secotrs, "root_dir_secotrs"); // root dir occupy how many sectors

  u16 root_sec_no = 19;
  u16 f_c = 0;
  u32 file_size = 0;

  //for (int i=0 ; i < 1 ; ++i, ++cur_sec_no)
  //for (int i=0 ; i <= root_dir_secotrs ; ++i, ++cur_sec_no)
  for (int i=root_sec_no ; i <= root_dir_secotrs+root_sec_no ; ++i)
  {
    print("\r\n");
    print_num(i, "cur sec no"); // root dir occupy how many sectors
    track_no = (((i)/18) >> 1);
    head_no = (((i)/18) & 1);
    sector_no = (((i)%18) + 1);
    disk_no = 0;
    r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
    //dump_u8(buff, 32*2);
    //while(1);
    
    //for (u16 j=0 ; j < 2 ; ++j)
    //for (u16 j=0 ; j < 16 ; ++j)
    for (u16 j=0 ; j < 512/32 ; ++j)
    {
      f_c = ((buff[0x1b+j*32] << 8) | buff[0x1a+j*32]); // first cluster
      file_size = (( buff[0x1f + (j*32)] << 24) | (buff[0x1e + (j*32)] << 16) | (buff[0x1d + (j*32)] << 8) | buff[0x1c + (j*32)]);
      //u32 file_size = 0;
      //u32 file_size = ( buff[0x1f+(j*32)] << 24); 

      u8 filename[12]="";

      for (int i=0 ; i < 11 ; ++i)
      {
        filename[i] = buff[j*32+i];
	//print_ch(filename[i]);
      }


      //filename = buff + (j*32);

      if (filename[0] == 0xe5) // del file
        continue;
      if (filename[0] == 0)
      {
        print("\r\nsearch end\r\n");
        goto search_end;
      }


      u8 attr = buff[0x0b + (j*32)];
      if (((attr >> 3) & 1) == 1) // label
      {
        print("\r\n");
        print("label:");
        print(filename);
        continue;
      }

      print("\r\n");
      print(filename);
      r = s_strncmp(filename, kernel_name, 11);
      //print("\r\n");
      //print_num(r, "r");
        print_num(f_c, " f_c");
        print_num(file_size, " file_size");

      //if (s_strcmp(filename, kernel_name) == 0) // find kernel
      if (r == 0)
      {
        print("\r\n");
        print("load it\r\n");
	goto search_end;
      }




    }


  }

  u16 r_sec=0;
  search_end:
  if (r==0)
  {
    r_sec=f_c - 2 + root_dir_secotrs + 19;
    print_num(r_sec, "r_sec");
    track_no = ((r_sec/18) >> 1);
    head_no = ((r_sec/18) & 1);
    sector_no = ((r_sec%18) + 1);
    #if 0
    print_num(track_no, "track_no");
    print_num(head_no, "head_no");
    #endif
    // if no the line, buff will get wrong data, very strange.
    print("s");
    if (file_size > 512) // need read FAT
    {
      r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);

      volatile u8 *fat_buf = (volatile u8 *)READ_FAT_ADDR;
      u16 fat_sector_no = 1;
      read_fat(fat_buf, fat_sector_no); // FAT occupies 9 sections, sector no 1 ~ 10
      //dump_u8(fat_buf, 32);
      u16 offset, next_cluster, cur_cluster=f_c;

#if 0
      next_cluster=get_next_cluster(fat_buf, cur_cluster);
  print_num(next_cluster, "xx next_cluster");
  print("\r\n");
  #if 1
        cur_cluster = next_cluster;
      next_cluster=get_next_cluster(fat_buf, cur_cluster);
  print_num(next_cluster, "xx next_cluster");
  print("\r\n");
  #endif
  while(1);
  #endif
#if 1
      while ((next_cluster=get_next_cluster(fat_buf, cur_cluster)) != 0xfff)
      {
        r_sec=next_cluster + - 2 + root_dir_secotrs + 19;

        print_num(r_sec, "r_sec");
        //print("\r\n");
        cur_cluster = next_cluster;
	//if (i >= 2) break;
	//++i;
      }
#endif

#if 0
      if (is_odd(f_c) == 1)
      {
        offset = (f_c-1) * 3 / 2;
	next_cluster = ((fat_buf[offset+1] >> 4) & 0x0f) | (fat_buf[offset+2] << 8);
        //offset = f_c * 1.5; // if f_c : 12 because of  24, 12 * 2??
        //offset = f_c >> 1;
      }
      else
      {
        offset = f_c /2 *3; // if f_c : 12 -> 18
	next_cluster = ((fat_buf[offset+1] & 0x0f) << 8)| fat_buf[offset];
      }

      print("\r\n");
      print_num(offset, "offset");
      print("\r\n");
      print_num(next_cluster, "next_cluster");
      print("\r\n");

      f_c=next_cluster;
      if (is_odd(f_c) == 1)
      {
        offset = (f_c-1) * 3 / 2;
	next_cluster = ((fat_buf[offset+1] >> 4) & 0x0f) | (fat_buf[offset+2] << 4);
        //offset = f_c * 1.5; // if f_c : 12 because of  24, 12 * 2??
        //offset = f_c >> 1;
      }
      else
      {
        offset = f_c /2 *3; // if f_c : 12 -> 18
	next_cluster = ((fat_buf[offset+1] & 0x0f) << 8)| fat_buf[offset];
      }
      print("\r\n");
      print_num(offset, "offset");
      print("\r\n");
      print_num(next_cluster, "next_cluster");
      print("\r\n");
#endif

    }
    else
    {
      r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
    }
    while(1);
    //void*   e = (void*)IMAGE_ENTRY;
    volatile void*   e = buff;
    //__asm__ __volatile__("" : : "d"(bios_drive));
    goto    *e;
  }








#ifdef DOS_COM
  // 回到 DOS
  __asm__ ("mov     $0x4c00, %ax\n");
  __asm__ ("int     $0x21\n");
#else
  while(1);
#endif
}
