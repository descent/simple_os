#include "mm.h"
#include "protect.h"
#include "k_stdio.h"
#include "k_string.h"
#include "k_assert.h"

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

int do_fork(void)
{
  extern Message mm_msg;

  Process *p = proc_table;
  int i;

  for (i=0 ; i < NR_TASKS + NR_PROCS; i++,p++)
    if (p->p_flags == FREE_SLOT)
      break;

  int child_pid = i;

  assert(p == &proc_table[child_pid]);
  assert(child_pid >= NR_TASKS + NR_NATIVE_PROCS);

  if (i == NR_TASKS + NR_PROCS) /* no free slot */
    return -1;
  assert(i < NR_TASKS + NR_PROCS);

  int pid = mm_msg.source;
  u16 child_ldt_sel = p->ldt_sel;
  *p = proc_table[pid];
  p->ldt_sel = child_ldt_sel;
  p->p_parent = pid;
  s32_sprintf(p->name, "%s_%d", proc_table[pid].name, child_pid);

  Descriptor *ppd;

  ppd = &proc_table[pid].ldt[LDT_CODE];

  int caller_T_base  = reassembly(ppd->base_high, 24, ppd->base_mid, 16, ppd->base_low);
  int caller_T_limit = reassembly(0, 0, (ppd->limit_high_attr2 & 0xF), 16, ppd->limit_low);
  int caller_T_size  = ((caller_T_limit + 1) * ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?  4096 : 1));

  ppd = &proc_table[pid].ldt[LDT_DATA];
  int caller_D_S_base  = reassembly(ppd->base_high, 24, ppd->base_mid, 16, ppd->base_low);
  int caller_D_S_limit = reassembly(0, 0, (ppd->limit_high_attr2 & 0xF), 16, ppd->limit_low);
  int caller_D_S_size  = ((caller_T_limit + 1) * ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?  4096 : 1));

  assert((caller_T_base  == caller_D_S_base ) && (caller_T_limit == caller_D_S_limit) && (caller_T_size  == caller_D_S_size ));

  u8* child_base = (u8*)alloc_mem();
  p_asm_memcpy(child_base, (void*)caller_T_base, caller_T_size);

  init_descriptor(&p->ldt[LDT_CODE], (u32)child_base, (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT, DA_LIMIT_4K | DA_32 | DA_C | PRIVILEGE_USER << 5);

  init_descriptor(&p->ldt[LDT_DATA], (u32)child_base, (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT, DA_LIMIT_4K | DA_32 | DA_DRW | PRIVILEGE_USER << 5);

  mm_msg.PID = child_pid;

  Message m;
  m.type = SYSCALL_RET;
  m.RETVAL = 0;
  m.PID = 0;
  send_recv(SEND, child_pid, &m);
  p->p_flags = 0;

  return 0;
}

