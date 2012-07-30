// test protected mode
#include "type.h"
#include "kl_global.h"
#include "elf.h"
#include "k_stdio.h"
#include "k_stdlib.h"
#include "k_string.h"

int AA;

void test_p(void)
{
  u8 *vb = (u8 *)0xb8002;
  u8 *attr = (u8 *)0xb8003;

  *vb = 'M';
  *attr = 9;
#if 0
  while(1);  
#else
  __asm__ ("nop\t\n");
  __asm__ ("nop\t\n");
#endif
  void jump_kernel();
  jump_kernel();
}

void jump_kernel()
{
  __asm__ ("jmp $0x8, $0x5000\t\n");
  __asm__ ("jmp 0x5000\t\n");
}

void s32_memcpy(u8 *dest, const u8 *src, u32 n)
{
  for (int i=0; i < n ; ++i)
    *dest++ = *src++;
}

// in this state, cannot use global, static, bss data,
// because this address is base on 0x100, but kloaderp.bin is loaded to 0x9000:0x0100,
// so need added 0x9000X16 is global, static, bss data real address.
// I only use stack for variable.
// copy_elf_code() is run under protocted mode.
void copy_elf_code(void)
{
  u8 *vb = (u8 *)0xb8002;
  u8 *attr = (u8 *)0xb8003;

  *((u8 *)0xb8000) = 'P';
  *vb = 'M';
  *attr = 9;
  // copy kernel to proper position by elf information


  u8 *buff = (KERNEL_ES*16)+(u8*)LOAD_KERNEL_OFFSET;
  Elf32Ehdr *elf_header = (Elf32Ehdr*)buff;
  Elf32Phdr *elf_pheader = (Elf32Phdr*)((u8 *)buff + elf_header->e_phoff);
  // the load elf address maybe overlay entry ~ elf code size, so save entry first
  u32 entry = elf_header->e_entry; 

  int line=0;
  clear_line(line);
  s32_print_int(elf_header->e_entry, (u8*)(0xb8000+160*line), 16);


#if 0
  print("\r\nelf_header->e_entry: ");
  s16_print_int(elf_header->e_entry, 10);

  print("\r\nelf_header->e_phnum: ");
  s16_print_int(elf_header->e_phnum, 10);
#endif

  BOCHS_MB
  for (int i=0 ; i < elf_header->e_phnum; ++i)
  {
  ++line;
  clear_line(line);
  s32_print_int(elf_header->e_entry, (u8*)(0xb8000+160*line), 16);
    if (CHECK_PT_TYPE_LOAD(elf_pheader))
    {
      //print_num(elf_pheader->p_vaddr, "elf_pheader->p_vaddr");
      ++line;
      clear_line(line);
      s32_print_int(elf_pheader->p_vaddr, (u8*)(0xb8000+160*line), 16);

      ++line;
      clear_line(line);
      s32_print_int(elf_pheader->p_offset, (u8*)(0xb8000+160*line), 16);
      //print_num(elf_pheader->p_offset, "elf_pheader->p_offset");
      //print_num(elf_pheader->p_filesz, "elf_pheader->p_filesz");
      //asm_absolute_memcpy((u8*)elf_pheader->p_vaddr, buff+(elf_pheader->p_offset), elf_pheader->p_filesz);
      s32_memcpy((u8*)elf_pheader->p_vaddr, buff+(elf_pheader->p_offset), elf_pheader->p_filesz);
  BOCHS_MB
    }
    ++elf_pheader;
  }
  BOCHS_MB
  ++line;
  clear_line(line);
  s32_print_int(elf_header->e_entry, (u8*)(0xb8000+160*line), 16);
  goto *entry;
}
