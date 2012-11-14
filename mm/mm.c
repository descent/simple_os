#include "mm.h"

u32 memory_used;
// 0000 0000
// 0000 0000
// 0000 0000
// 0000 0000
// alloc 512kb, total managed memroy is 32*152kb = 16Mb

u32 mm_start;
u32 memsize_mb;
u32 memsize;

u32 io_load_eflags(void);
void io_store_eflags(u32 eflg);
u32 load_cr0(void);
void store_cr0(u32 cr0);

int mm_init(void)
{
  memsize = memtest(0x00400000, 0x2000000);
  mm_start = memsize - 16*1024*1024;
}

int find_free_area(void)
{
  for (int i=0 ; i < 32 ; ++i)
  {
    if (((memory_used >> i) & 1) == 0)
    {
      memory_used |= (1<<i);
      return i;
    }
  }
  return -1;
}

// return a 512kb free memory block
void *alloc_mem(void)
{
  int i = find_free_area();
  if (i==-1)
    return 0;
  else
    return (void*)(mm_start+(i*512*1024));
}

void free_mem(void *mem_addr)
{
  if (mem_addr == 0) return;

  int i = (((u32)mem_addr - mm_start)/(512*1024));
  memory_used &= (~(1<<i));
}

// copy from 30days_os/projects/09_day/harib06b/bootpack.c
u32 memtest(volatile u32 start, volatile u32 end)
{
  u32 memtest_sub(volatile u32 start, volatile u32 end);

	char flg486 = 0;
	u32 eflg, cr0, i;

	/* 386©A486È~ÈÌ©ÌmF */
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if ((eflg & EFLAGS_AC_BIT) != 0) { /* 386ÅÍAC=1ÉµÄà©®Å0ÉßÁÄµÜ¤ */
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
	io_store_eflags(eflg);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE; /* LbVÖ~ */
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; /* LbVÂ */
		store_cr0(cr0);
	}

	return i;
}

#if 0
u32 memtest_sub(volatile u32 start, volatile u32 end)
{
	volatile unsigned int i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
	for (i = start; i <= end; i += 0x1000) {
		p = (unsigned int *) (i + 0xffc);
		old = *p;			/* ¢¶éOÌlðo¦Ä¨­ */
		*p = pat0;			/* ½ßµÉ¢ÄÝé */
		*p ^= 0xffffffff;	/* »µÄ»êð½]µÄÝé */
		if (*p != pat1) {	/* ½]ÊÉÈÁ½©H */
not_memory:
			*p = old;
			break;
		}
		*p ^= 0xffffffff;	/* à¤êx½]µÄÝé */
		if (*p != pat0) {	/* ³ÉßÁ½©H */
			goto not_memory;
		}
		*p = old;			/* ¢¶Á½lð³Éß· */
	}
	return i;
}
#endif
