#include "mm.h"
#include "protect.h"
#include "k_stdio.h"
#include "k_string.h"
#include "k_assert.h"
#include "elf.h"
#include "vfs.h"

extern Message mm_msg;

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

  u32 caller_T_base  = reassembly(ppd->base_high, 24, ppd->base_mid, 16, ppd->base_low);
  u32 caller_T_limit = reassembly(0, 0, (ppd->limit_high_attr2 & 0xF), 16, ppd->limit_low);
  u32 caller_T_size  = ((caller_T_limit + 1) * ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?  4096 : 1));

  ppd = &proc_table[pid].ldt[LDT_DATA];
  u32 caller_D_S_base  = reassembly(ppd->base_high, 24, ppd->base_mid, 16, ppd->base_low);
  u32 caller_D_S_limit = reassembly(0, 0, (ppd->limit_high_attr2 & 0xF), 16, ppd->limit_low);
  u32 caller_D_S_size  = ((caller_T_limit + 1) * ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?  4096 : 1));

  assert((caller_T_base  == caller_D_S_base ) && (caller_T_limit == caller_D_S_limit) && (caller_T_size  == caller_D_S_size ));

  u8* child_base = (u8*)alloc_mem();
  child_base = (u8*)(0xa00000);
  caller_T_size = 0x100000;
  //p_asm_memcpy(child_base, (void*)caller_T_base, caller_T_size);
  //p_asm_memcpy(child_base+0x100000, (void*)caller_T_base+0x100000, 0x100000);
  p_asm_memcpy(child_base+0x100000, (void*)caller_T_base+0x100000, caller_T_size);

#if 0
  init_descriptor(&p->ldt[LDT_CODE], (u32)child_base, (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT, DA_LIMIT_4K | DA_32 | DA_C | PRIVILEGE_USER << 5);
  init_descriptor(&p->ldt[LDT_DATA], (u32)child_base, (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT, DA_LIMIT_4K | DA_32 | DA_DRW | PRIVILEGE_USER << 5);
#else
  u32 test_base = (u32)child_base;
  u32 test_limit = 0x400-1;
  init_descriptor(&p->ldt[LDT_CODE], test_base, test_limit, DA_LIMIT_4K | DA_32 | DA_C | PRIVILEGE_USER << 5);
  init_descriptor(&p->ldt[LDT_DATA], test_base, test_limit, DA_LIMIT_4K | DA_32 | DA_DRW | PRIVILEGE_USER << 5);
#endif
  mm_msg.PID = child_pid;

#if 1
  // invoke child process
  Message m;
  m.type = SYSCALL_RET;
  m.RETVAL = 0;
  m.PID = 0;
  send_recv(SEND, child_pid, &m);
#endif
  int x=0; // for debug

  return 0;
}

void cleanup(Process * proc)
{
  Message msg2parent;
  msg2parent.type = SYSCALL_RET;
  msg2parent.PID = proc2pid(proc);
  msg2parent.STATUS = proc->exit_status;
  send_recv(SEND, proc->p_parent, &msg2parent);

  proc->p_flags = FREE_SLOT;

}

#define INIT 3
void do_exit(int status)
{
  int pid = mm_msg.source;
  int parent_pid = proc_table[pid].p_parent;
  Process * p = &proc_table[pid];

#if 0
  MESSAGE msg2fs;
  msg2fs.type = EXIT;
  msg2fs.PID = pid;
  send_recv(BOTH, TASK_FS, &msg2fs);
#endif
  //free_mem(pid);

  p->exit_status = status;
  //p->exit_status = 9;

  if (proc_table[parent_pid].p_flags & WAITING)
  {
    proc_table[parent_pid].p_flags &= ~WAITING;
    cleanup(&proc_table[pid]);
    
  }
  else
  {
    proc_table[parent_pid].p_flags |= HANGING;
  }

  for (int i=0 ; i < (NR_TASKS + NR_PROCS); i++)
  {
    if (proc_table[i].p_parent == pid)
    {
      proc_table[i].p_parent == INIT;
      if ((proc_table[INIT].p_flags & WAITING) && (proc_table[i].p_flags & HANGING)) 
      {
        proc_table[INIT].p_flags &= ~WAITING;
        cleanup(&proc_table[i]);

      }

    }
  }
}

void do_wait(void)
{
  int pid = mm_msg.source;

  int i;
  int children = 0;
  Process* p_proc = proc_table;
  for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) 
  {
    if (p_proc->p_parent == pid) 
    {
      children++;
      if (p_proc->p_flags & HANGING) 
      {
        cleanup(p_proc);
        return;
      }
    }
  }

  if (children) 
  {
    /* has children, but no child is HANGING */
    proc_table[pid].p_flags |= WAITING;
  }
  else 
  {
    /* no child at all */
    Message msg;
    msg.type = SYSCALL_RET;
    msg.PID = NO_TASK;
    send_recv(SEND, pid, &msg);
  }
}


void do_execl(void)
{
  /* get parameters from the message */
  int name_len = mm_msg.NAME_LEN; /* length of filename */
  int src = mm_msg.source;        /* caller proc nr. */
  assert(name_len < MAX_PATH);
        
#if 0
  char pathname[MAX_PATH];
  p_asm_memcpy((void*)va2la(TASK_MM, pathname), (void*)va2la(src, mm_msg.PATHNAME), name_len);
  pathname[name_len] = 0; /* terminate the string */
#endif

  INode *inode = fs_type[ROMFS]->namei(fs_type[ROMFS], "echo"); // get super block infomation
  int addr = fs_type[ROMFS]->get_daddr(inode);
  u8 *buf = (u8*)alloc_mem();
  fs_type[ROMFS]->device->dout(fs_type[ROMFS]->device, buf, addr, inode->dsize);

  Elf32Ehdr *elf_header = (Elf32Ehdr*)buf;
  Elf32Phdr *elf_pheader = (Elf32Phdr*)((u8 *)buf + elf_header->e_phoff);
  u32 entry = elf_header->e_entry;

  for (int i=0 ; i < elf_header->e_phnum; ++i)
  {
    if (CHECK_PT_TYPE_LOAD(elf_pheader))
    {
      p_asm_memcpy((u8*)va2la(src,elf_pheader->p_vaddr), va2la(TASK_MM, buf+(elf_pheader->p_offset)), elf_pheader->p_filesz);
    }
    ++elf_pheader;
  }
  free_mem(buf);

  /* setup eip & esp */
  proc_table[src].regs.eip = entry;
  //proc_table[src].regs.esp = PROC_IMAGE_SIZE_DEFAULT - PROC_ORIGIN_STACK;

}

