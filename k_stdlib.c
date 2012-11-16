#include "k_stdlib.h"
#include "k_string.h"
#include "kl_global.h"
#include "elf.h"

int get_kernel_info(u32 * b, u32 * l)
{
  Elf32Ehdr *elf_header = (Elf32Ehdr*)KERNEL_INFO;
  u32 kaddr = KERNEL_INFO;
  if (s_memcmp(elf_header->e_ident, ELFMAG, SELFMAG) != 0)
  {
    s32_print("not elf", (u8*)(0xb8000+160*24 + 12*2));
    return -1;
  }
  else
  {
    s32_print("find elf", (u8*)(0xb8000+160*24 + 12*2));
  }

  *b = ~0;
  unsigned int t = 0;
  int i;
  for (i = 0; i < elf_header->e_shnum; i++) 
  {
    Elf32_Shdr* section_header = (Elf32_Shdr*)(KERNEL_INFO + elf_header->e_shoff + i * elf_header->e_shentsize);
        
    if (section_header->sh_flags & SHF_ALLOC) 
    {
      int bottom = section_header->sh_addr;
      int top = section_header->sh_addr + section_header->sh_size;
        
      if (*b > bottom)
        *b = bottom;
      if (t < top)
        t = top;
    }
  }
  *l = t - *b - 1;
  return 0;
}

