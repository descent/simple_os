#ifndef DOS_PROG
__asm__(".code16gcc\n");
#endif

#include "type.h"
#include "k_string.h" // s_strlen()
#include "k_stdio.h"
#include "k_stdlib.h"
#ifdef DOS_PROG
#include "kl_g.h"
#else
#include "kl_global.h"
#endif

//#define MORE_ERR_MSG

#define NAME_VALUE(name) \
{ \
  print("\r\n"); \
  print(#name); \
  print(": "); \
  s16_print_int(name, 10); \
}

#define NAME_VALUE16(name) \
{ \
  print("\r\n"); \
  print(#name); \
  print(": "); \
  s16_print_int(name, 16); \
}

// read fat floppy disk

//#define DOS_COM

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __PACKED    __attribute__((packed))
#define __NORETURN  __attribute__((noreturn))


//#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

#define FAT_BUF_LEN 2048
static u8 fat_buffer[2048];

void dump_reg(void)
{
  u16 reg_v=0;
  __asm__ __volatile__
  (
    "mov %%dx, %%ax\n"
    : "=a"(reg_v)// output
  );
  NAME_VALUE(reg_v)
}

/* XXX these must be at top */

//u8 kernel_name[] = "KERNEL  BIN";
const u8 kernel_name[] =   "P_KERNELELF";

#ifdef LOAD_FS
const u8 ramdisk_name[] =  "ROM     FS ";
#endif

//u8 kernel_name[] =   "IDT     COM";
//u8 kernel_name[] =   "IDT     ELF";
//u8 kernel_name[] = "KERNEL  ELF";
//u8 kernel_name[]   = "TEST    BIN";

void asm_memcpy(u8 *dest, u8 *src, int n);
void asm_seg_memcpy(u8 *dest, u8 *src, int n, u16 fs);
void asm_absolute_memcpy(u8 *dest, u8 *src, int n);

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

// prefix s16 means Simple 16 bit code
void s16_print_int(int i, int radix)
{
  u8 str[12]="";
  u8 *str_ptr = str;

  str_ptr = itoa(i, str_ptr, radix);
  print(str_ptr);
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



//void dump_u8(u8 *buff, u16 count)
void dump_u8(u8 *buff, int count)
{
#ifndef DOS_PROG
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
#endif
}

#if 0
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
#endif

void print_bpb(BPB *bpb)
{
  print("\r\nbpb->vendor_name: ");
  print(bpb->vendor_name);
  #if 1
  NAME_VALUE(bpb->byte_per_sector)

  NAME_VALUE(bpb->sector_per_cluster)
  NAME_VALUE(bpb->boot_occupy_sector)
  NAME_VALUE(bpb->fat_count)
  NAME_VALUE(bpb->root_entry_count)
  NAME_VALUE(bpb->logic_sector_count)
  NAME_VALUE(bpb->fat_occupy_sector)
  NAME_VALUE(bpb->sector_per_track)
  NAME_VALUE(bpb->head_count)
  NAME_VALUE(bpb->hidden_sector_count)
  NAME_VALUE(bpb->root_dir_occupy_sector)
  #endif
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


// ref: http://dc0d32.blogspot.tw/2010/06/real-mode-in-c-with-gcc-writing.html
/* use for floppy, or as a fallback */
typedef struct {
        unsigned char   spt;
        unsigned char   numh;
}drive_params_t;
 
int __REGPARM __NOINLINE get_drive_params(drive_params_t    *p, unsigned char   bios_drive){
/*
  ch : max track no (low 8 bit)
  cl : max sector no (0~5), max track no (6~7)
  dh : max head no
  dl : disk number
  es:di  : point to params list
 */
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
        p->spt = tmp1 & 0x3F; // max track no
        p->numh = tmp2 >> 8;
        return failed;
}

// return 0: ok
// not 0: fail
// if using the function read floppy fail, suggest reset floppy disk,
// than try twice again.
int __REGPARM read_sector(u16 buff, u8 sector_no, u8 track_no, u8 head_no, u8 disk_no, u8 blocks)
{
  //bios_wait_key();
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
  // read sector to %es:%bx, if %bx is more than 64k, need change %es
  // to next 64k beginning address
  // ref: http://dc0d32.blogspot.tw/2010/06/real-mode-in-c-with-gcc-writing.html
  u16 num_blocks_transferred = 0;
  u8 failed = 0;
  //BOCHS_MB
  //__asm__ __volatile__("xchg %bx, %bx");
  __asm__ __volatile__("push %eax");
  __asm__ __volatile__("push %ebx");
  __asm__ __volatile__("push %ecx");
  __asm__ __volatile__("push %edx");

  __asm__ __volatile__ 
    (
      "movw  $0, %0\n"
      "int $0x13\n"
      "setcb %0"
      :"=m"(failed), "=a"(num_blocks_transferred)
      :"a"(0x0200|blocks), "b"(buff), "c"(track_no << 8 | sector_no), "d"(head_no << 8 | disk_no)
    ); 
#endif
  __asm__ __volatile__("pop %edx");
  __asm__ __volatile__("pop %ecx");
  __asm__ __volatile__("pop %ebx");
  __asm__ __volatile__("pop %eax");
  u8 ret_status = (num_blocks_transferred >> 8);
  #ifdef MORE_ERR_MSG
  NAME_VALUE(num_blocks_transferred)
  NAME_VALUE(failed)
  NAME_VALUE(ret_status)
  #endif
  return failed || (num_blocks_transferred != blocks);
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

#if 0
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
#endif

#define OK 1
#define FAIL -1

// FAT occupies 9 sections, sector no 1 ~ 10
int read_fat(volatile u8 *fat_buf, u16 fat_sector_no)
{
  u8 track_no = ((fat_sector_no/18) >> 1);
  u8 head_no = ((fat_sector_no/18) & 1);
  u8 sector_no = (fat_sector_no%18);
  u8 disk_no = 0;

#ifdef DEBUG_INFO
  print_num(track_no, "fat track_no");
  print_num(head_no, "fat head_no");
  print_num(sector_no, "fat sector_no");
  print_num(disk_no, "fat disk_no");
  print("\r\n");
#endif
  u8 r = read_sector(fat_buf, sector_no, track_no, head_no, disk_no, 2);
  return OK;
}

int is_odd(int n)
{
  if ((n%2) == 0 )
    return -1;
  else
    return 1;
}

#define FAT_SIZE 3 // 12 bit 

u16 get_next_cluster(u16 cur_cluster)
{
  u32 offset, next_cluster=0; // u32 is enough to offset in 1/44 MB floppy
  u8 *fat_buf = fat_buffer;


  //print_num(cur_cluster, "cur_cluster");

  if (is_odd(cur_cluster) == 1)
  {
    offset = (cur_cluster-1) / 2 * 3;
    //next_cluster = (((fat_buf[offset+1] >> 4) & 0x0f) | (fat_buf[offset+2] << 4));
    //offset = f_c * 1.5; // if f_c : 12 because of  24, 12 * 2??
    //offset = f_c >> 1;
  }
  else
  {
    //print("\r\neven");
    offset = cur_cluster /2 *3; // if f_c : 12 -> 18
    //next_cluster = ((fat_buf[offset+1] & 0x0f) << 8)| fat_buf[offset];
  }

  //u16 read_fat_sector_no = ((offset / 1024*2)+1)*2;
  u16 read_fat_sector_no = (offset/512)+1;
  u32 short_offset = offset%512;
  //if (short_offset != 0)
    ++read_fat_sector_no;

  int  pause=0;

#if 0
  #if 1
  if (offset >= 1023)
  {
    ++read_fat_sector_no;
    offset = 511;
    pause=1;
  }
  else
    offset %= 1024;
  #endif
#endif

#ifdef DEBUG_INFO
  NAME_VALUE(offset)
  NAME_VALUE(short_offset)
  NAME_VALUE(read_fat_sector_no)
#endif


  //NAME_VALUE(offset)

  u16 org_es = asm_get_es();
  u16 es = 0x9000;
  asm_set_es(es);
  read_fat(fat_buf, read_fat_sector_no); // FAT occupies 9 sections, sector no 1 ~ 10
  asm_set_es(org_es);

#ifdef DEBUG_INFO
  dump_u8(fat_buf+offset, 3);
#endif

  if (is_odd(cur_cluster) == 1)
  {
    next_cluster = (((fat_buf[short_offset+1] >> 4) & 0x0f) | (fat_buf[short_offset+2] << 4));
  }
  else
  {
    next_cluster = ((fat_buf[short_offset+1] & 0x0f) << 8)| fat_buf[short_offset];
  }
#if 0
    print("\r\nodd");
  print_num(offset, "offset");
  print("\r\n");
  dump_u8(fat_buf+offset, 3);
  print("\r\n");
  print_num(next_cluster, "next_cluster");
    NAME_VALUE(next_cluster);
    dump_u8(fat_buf+offset, 3);
#endif
    #if 0
  if (pause == 1)
    bios_wait_key();
  bios_wait_key();
  #endif
  return next_cluster;
}

// fat == 1, read fat
int load_file_to_ram(int begin_cluster, int fat, u16 org_es, u16 es)
{
  int r;
  int r_sec = begin_cluster - 2 + bpb.root_dir_occupy_sector + bpb.root_dir_start_sector;
  u16 buff = LOAD_KERNEL_OFFSET;

  print_num(begin_cluster, "begin_cluster");
  print_num(r_sec, "cluster sector no");
  //bios_wait_key();
  int track_no = ((r_sec/18) >> 1);
  int head_no = ((r_sec/18) & 1);
  int sector_no = ((r_sec%18) + 1);
  int disk_no = 0;
  int read_sector_count=1;

  // read the 1st sector
  asm_set_es(es);
  r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
  print("\r\n.");

  if (fat) // need read FAT
  {
    //dump_u8(fat_buf, 32);
    static u16 offset, next_cluster, cur_cluster;

    cur_cluster=begin_cluster;

    while ((next_cluster=get_next_cluster(cur_cluster)) != 0xfff)
    {
        r_sec=next_cluster - 2 + bpb.root_dir_occupy_sector + bpb.root_dir_start_sector;

//#ifdef DEBUG_INFO
#ifdef MORE_ERR_MSG
        print_num(next_cluster, "next_cluster");
        print_num(r_sec, "r_sec");
#endif

        track_no = ((r_sec/18) >> 1);
        head_no = ((r_sec/18) & 1);
        sector_no = ((r_sec%18) + 1);
        buff += 0x200;
#ifdef MORE_ERR_MSG
        NAME_VALUE16(es);
        NAME_VALUE16(buff);
        //print_num((u32)buff, "buff");
#endif
        if (read_sector_count == 65536/512)
        {
          buff = LOAD_KERNEL_OFFSET;
#ifdef MORE_ERR_MSG
          print("\r\nmore than 64Kb\r\n");
#endif
          es+=0x1000;
          asm_set_es(es);
          read_sector_count = 0;
        }
        else
        {
#ifdef MORE_ERR_MSG
          print("\r\nless than 64Kb\r\n");
#endif
        }
        r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
        print(".");
        ++read_sector_count;

#ifdef MORE_ERR_MSG
  u16 buf_v = (u16)buff;
  NAME_VALUE16(buf_v);
#endif

        //print("\r\n");
        cur_cluster = next_cluster;
	//if (i >= 2) break;
	//++i;
    }
  }
  else
  {
    //r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
  }
  NAME_VALUE16(es);
  asm_set_es(org_es);
}


void start_c()
{
  int first_kernel_cluster = -1;
  int first_ramdisk_cluster = -1;
#if 0  
  __asm__ __volatile__ ("mov $0x9000, %ax\n");
  __asm__ __volatile__ ("mov %ax, %ds\n");
  __asm__ __volatile__ ("mov %ax, %ss\n");
#endif
  //u8 str[]="stack c";
  //print("\r\n");

  static u8 str[]="begin c";

  u16 org_es = asm_get_es();
  drive_params_t dp;
  get_drive_params(&dp, 0);
  NAME_VALUE(dp.spt);
  NAME_VALUE(dp.numh);
  asm_set_es(org_es);
  //bios_wait_key();

  print("\r\n");
  print("begin c");
#if 0
  u16 es = asm_get_es();
  NAME_VALUE16(es);
  asm_set_es(0xa000);
  es = asm_get_es();
  NAME_VALUE16(es);
#endif
  //print(str);
  //while(1);
  //const *str="zxc";
  //print_ch(*str);

//ref: http://forum.osdev.org/viewtopic.php?f=1&t=7762
  u8 sector_no = 1; // cl, 1 - 18
  u8 track_no=0; // ch
  u8 head_no=0; // dh
  u8 disk_no=0; // dl

  u16 byte_per_sector = 512;
  u16 root_entry_count = 0;
  u8 *buff = (u8*)IMAGE_LMA;

 //__asm__ __volatile__("xchg %bx, %bx");

//#ifdef DOS_COM
  //int r = read_sector(buff, sector_no, 0, 0, 0);

#if 0
  // logic sector no 19 is root directory
  sector_no = 1; // cl, 1 - 18
  track_no=0; // ch
  head_no=0; // dh
  disk_no=0; // dl
#endif
  //*buff = 0x1;

  int r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
  init_bpb(buff, &bpb);

  if (bpb.root_entry_count * 32 % bpb.byte_per_sector != 0)
    bpb.root_dir_occupy_sector = (bpb.root_entry_count * 32 / bpb.byte_per_sector) + 1;
  else
    bpb.root_dir_occupy_sector = (bpb.root_entry_count * 32 / bpb.byte_per_sector);

  print_bpb(&bpb);
  //bios_wait_key();

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

  bpb.root_dir_start_sector = bpb.fat_count * bpb.fat_occupy_sector + bpb.boot_occupy_sector;
  
  u16 root_sec_no = bpb.root_dir_start_sector;
  u16 f_c = 0;
  u32 file_size = 0;
  print("\r\nroot dir sector starts: ");
  s16_print_int(root_sec_no, 10);
  //while(1);
  print("\r\nread root dir sector to get file name info");

  //for (int i=0 ; i < 1 ; ++i, ++cur_sec_no)
  //for (int i=0 ; i <= root_dir_secotrs ; ++i, ++cur_sec_no)
  for (int i=root_sec_no ; i <= bpb.root_dir_occupy_sector+root_sec_no ; ++i)
  {
    print("\r\nread sector no: ");
    s16_print_int(i, 10);
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
      #if 0
      print("\r\nfirst cluster:");
      s16_print_int(f_c, 10);

      print("\r\nsize:");
      s16_print_int(file_size, 10);
      #endif
      r = s_strncmp(filename, kernel_name, 11);

      if (r == 0)
      {
        first_kernel_cluster = f_c;
        print("\r\n");
        print("load it\r\n");
#ifndef LOAD_FS
	goto search_end;
#endif
      }

#ifdef LOAD_FS
      r = s_strncmp(filename, ramdisk_name, 11);
      if (r == 0)
      {
        first_ramdisk_cluster = f_c;
        print("\r\n");
        print("load it\r\n");
      }
      if ( (first_ramdisk_cluster != -1) && (first_kernel_cluster != -1) )
      {
	goto search_end;
      }
#endif



    }


  }



  u16 r_sec=0;
  search_end:
  print("\r\nfirst_kernel_cluster: ");
  s16_print_int(first_kernel_cluster, 10);

  print("\r\nfirst_ramdisk_cluster: ");
  s16_print_int(first_ramdisk_cluster, 10);

  BOCHS_MB
  print("\r\nload kernel: ");
  print(kernel_name);
  // load kernel
  org_es = asm_get_es();
  u16 es = KERNEL_ES;
  load_file_to_ram(first_kernel_cluster, (file_size> 512) ? 1: 0, org_es, es);
  print("\r\n");

#if 0
  // load ramdisk
  print("\r\nload ramdisk:");
  print(ramdisk_name);
  org_es = asm_get_es();
  es = RAMDISK_ES;
  load_file_to_ram(first_ramdisk_cluster, (file_size> 512) ? 1: 0, org_es, es);
  print("\r\n");
  //dump_u8((u8 *)IMAGE_LMA, 32);
#endif
#ifndef DOS_PROG
  void init_protected_mode();

  init_protected_mode();
#endif



    //while(1);
    //volatile void*   e = (void*)IMAGE_ENTRY;
    // copy 0x7000:0x100


  #if 0
  u8 seg=0;
  __asm__ __volatile__ ("jmp *%0\n"
                          :
                          :"m"(elf_header->e_entry)
                       ); 
  #endif

#if 0
    volatile void*   e = (void*)IMAGE_LMA;
    //volatile void*   e = buff;
    //__asm__ __volatile__("" : : "d"(bios_drive));
    print("\r\njmp it");
    //goto    *e;
    __asm__ ("jmp $0x7000,$0x100");
    //__asm__ __volatile__ ("jmp *%eax\n");
    __asm__ __volatile__ ("jmp *%%eax\n"
                          :
                          :"a"(elf_header->e_entry)
	                 ); 
    __asm__ __volatile__ ("jmp %0\n"
                          :
                          :"a"(elf_header->e_entry)
	                 ); 
    //__asm__ ("movw $0x0,%ax");
    //__asm__ ("movw %ax,%cs");
    //__asm__ ("movw %ax,%ds");
    //__asm__ ("movw %ax,%ss");
    //goto *elf_header->e_entry; // if elf_header->e_entry = 0x4000, will jmp to 0x9000:0x4000
    //__asm__ ("movw $0x0,%bx");
    //__asm__ ("movw %bx, %ds");
    __asm__ __volatile__ ("movw $0, %%bx\n"
                          "movw %%bx, %%ds\n"
                          "jmp %0\n"
                          :
                          :"m"(elf_header->e_entry)
	                 ); 
#endif







#ifdef DOS_COM
  // 回到 DOS
  __asm__ ("mov     $0x4c00, %ax\n");
  __asm__ ("int     $0x21\n");
#else
  while(1);
#endif
}

#ifdef DOS_PROG
int main(int argc, const char *argv[])
{
        
  return 0;
}
#endif
