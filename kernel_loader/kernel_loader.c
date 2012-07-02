__asm__(".code16gcc\n");

#include "../type.h"
#include "../elf.h"

#define MORE_ERR_MSG

#define NAME_VALUE(name) \
{ \
  print("\r\n"); \
  print(#name); \
  print(": "); \
  s16_print_int(name, 10); \
}

// read fat floppy disk

//#define DOS_COM

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __PACKED    __attribute__((packed))
#define __NORETURN  __attribute__((noreturn))

#define IMAGE_SIZE  8192
#define BLOCK_SIZE  512
#define READ_FAT_ADDR (0x3000) // original is 0x2000, but will overwrite bss (variable bpb), so change to 0x3000
#define IMAGE_LMA   (0x4000)
//#define IMAGE_LMA   0x8000
#define IMAGE_ENTRY 0x800c
#define buf_addr_val (*(u8 volatile*(IMAGE_LMA)))

#define BOCHS_MB __asm__ __volatile__("xchg %bx, %bx");

/* XXX these must be at top */

//u8 kernel_name[] = "KERNEL  BIN";
const u8 kernel_name[] =   "P_KERNELELF";
const u8 ramdisk_name[] =  "ROM     FS ";
//u8 kernel_name[] =   "IDT     COM";
//u8 kernel_name[] =   "IDT     ELF";
//u8 kernel_name[] = "KERNEL  ELF";
//u8 kernel_name[]   = "TEST    BIN";

void asm_memcpy(u8 *dest, u8 *src, int n);
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

// return 0: ok
// not 0: fail
// if using the function read floppy fail, suggest reset floppy disk,
// than try twice again.
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
    //BOCHS_MB
  // read sector to %es:%bx, if %bx is more than 64k, need change %es
  // to next 64k beginning address
  // ref: http://dc0d32.blogspot.tw/2010/06/real-mode-in-c-with-gcc-writing.html
  u16 num_blocks_transferred = 0;
  u8 failed = 0;

  __asm__ __volatile__ 
    (
      "movw  $0, %0\n"
      "int $0x13\n"
      "setcb %0"
      :"=m"(failed), "=a"(num_blocks_transferred)
      :"a"(0x0200|blocks), "b"(buff), "c"(track_no << 8 | sector_no), "d"(head_no << 8 | disk_no)
    ); 
#endif
  u8 ret_status = (num_blocks_transferred >> 8);
  #ifdef MORE_ERR_MSG
  NAME_VALUE(num_blocks_transferred)
  NAME_VALUE(failed)
  NAME_VALUE(ret_status)
  BOCHS_MB
  #endif
  return failed || (num_blocks_transferred != blocks);
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
  return OK;
}

int is_odd(int n)
{
  if ((n%2) == 0 )
    return -1;
  else
    return 1;
}

u16 get_next_cluster(u16 cur_cluster)
{
  u16 offset, next_cluster=0;
  volatile u8 *fat_buf = (volatile u8 *)READ_FAT_ADDR;


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
  //if (offset >= 1024) // need read fat sector
  {
    u16 fat_sector_no = (offset / 1024) + 1;
    //print_num(offset, "offset");
    //print_num(fat_sector_no, "fat_sector_no");

   read_fat(fat_buf, fat_sector_no); // FAT occupies 9 sections, sector no 1 ~ 10
  }
  if (is_odd(cur_cluster) == 1)
  {
    next_cluster = (((fat_buf[offset+1] >> 4) & 0x0f) | (fat_buf[offset+2] << 4));
  }
  else
  {
    next_cluster = ((fat_buf[offset+1] & 0x0f) << 8)| fat_buf[offset];
  }
#if 0
    print("\r\nodd");
  print_num(offset, "offset");
  print("\r\n");
  dump_u8(fat_buf+offset, 3);
  print("\r\n");
  print_num(next_cluster, "next_cluster");
#endif
  return next_cluster;
}

// fat == 1, read fat
int load_file_to_ram(int begin_cluster, int fat)
{
  int r;
  int r_sec = begin_cluster - 2 + bpb.root_dir_occupy_sector + bpb.root_dir_start_sector;
  volatile u8 *buff = (u8*)IMAGE_LMA;

  print_num(begin_cluster, "begin_cluster");
  print_num(r_sec, "cluster sector no");
  BOCHS_MB
  int track_no = ((r_sec/18) >> 1);
  int head_no = ((r_sec/18) & 1);
  int sector_no = ((r_sec%18) + 1);
  int disk_no = 0;

  // read the 1st sector
  r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);

  if (fat) // need read FAT
  {
    //dump_u8(fat_buf, 32);
    u16 offset, next_cluster, cur_cluster=begin_cluster;

#if 1
    while ((next_cluster=get_next_cluster(cur_cluster)) != 0xfff)
    {
        r_sec=next_cluster - 2 + bpb.root_dir_occupy_sector + bpb.root_dir_start_sector;
        print_num(next_cluster, "next_cluster");
        print_num(r_sec, "r_sec");
  BOCHS_MB

        track_no = ((r_sec/18) >> 1);
        head_no = ((r_sec/18) & 1);
        sector_no = ((r_sec%18) + 1);
        buff += 0x200;
        r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);

        //print("\r\n");
        cur_cluster = next_cluster;
	//if (i >= 2) break;
	//++i;
    }
#endif


    }
    else
    {
      //r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
    }

}


void start_c()
{
  int first_kernel_cluster = -1;
  int first_ramdisk_cluster = -1;
  
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
  u8 *buff = (u8*)IMAGE_LMA;

  //BOCHS_MB
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
  *buff = 0x1;

  int r = read_sector(buff, sector_no, track_no, head_no, disk_no, 1);
  init_bpb(buff, &bpb);

  if (bpb.root_entry_count * 32 % bpb.byte_per_sector != 0)
    bpb.root_dir_occupy_sector = (bpb.root_entry_count * 32 / bpb.byte_per_sector) + 1;
  else
    bpb.root_dir_occupy_sector = (bpb.root_entry_count * 32 / bpb.byte_per_sector);

  print_bpb(&bpb);

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
      //BOCHS_MB
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
#if 1
      r = s_strncmp(filename, kernel_name, 11);

      if (r == 0)
      {
        first_kernel_cluster = f_c;
        print("\r\n");
        print("load it\r\n");
      }
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

  print("\r\nload kernel: ");
  print(kernel_name);
  // load kernel
  load_file_to_ram(first_kernel_cluster, (file_size> 512) ? 1: 0);

  // copy kernel to proper position by elf information

  asm_memcpy((u8*)0x100, (u8 *)IMAGE_LMA, 512*3);

  buff = (u8*)IMAGE_LMA;
  Elf32Ehdr *elf_header = (Elf32Ehdr*)buff;
  Elf32Phdr *elf_pheader = (Elf32Phdr*)((u8 *)buff + elf_header->e_phoff);

  print("\r\nelf_header->e_entry: ");
  s16_print_int(elf_header->e_entry, 10);

  print("\r\nelf_header->e_phnum: ");
  s16_print_int(elf_header->e_phnum, 10);

  BOCHS_MB
  for (int i=0 ; i < elf_header->e_phnum; ++i)
  {
    if (CHECK_PT_TYPE_LOAD(elf_pheader))
    {
      print_num(elf_pheader->p_vaddr, "elf_pheader->p_vaddr");
      print_num(elf_pheader->p_offset, "elf_pheader->p_offset");
      print_num(elf_pheader->p_filesz, "elf_pheader->p_filesz");
      asm_absolute_memcpy((u8*)elf_pheader->p_vaddr, buff+(elf_pheader->p_offset), elf_pheader->p_filesz);
    }
    ++elf_pheader;
  }
  BOCHS_MB


  // load ramdisk
  print("\r\nload ramdisk:");
  print(ramdisk_name);
  load_file_to_ram(first_ramdisk_cluster, (file_size> 512) ? 1: 0);
  dump_u8((u8 *)IMAGE_LMA, 32);

    //while(1);
    //volatile void*   e = (void*)IMAGE_ENTRY;
    // copy 0x7000:0x100

  void init_protected_mode();

  init_protected_mode();

  #if 1
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
