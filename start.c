#include "io.h"
#include "type.h"
#include "protect.h"
#include "process.h"

#define INT_M_PORT 0x20
#define INT_S_PORT 0xa0
#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28
#define INT_M_CTLMASK 0x21
#define INT_S_CTLMASK 0xa1

#define NR_IRQ 16

//void __attribute__((aligned(16))) function() { }

//__attribute__((aligned(512))) 
u8 gdt_ptr[6];
Descriptor gdt[GDT_SIZE];

u8 idt_ptr[6];
Gate idt[IDT_SIZE];

u8 *cur_vb = (u8*)0xb8000+160;

IrqHandler irq_table[NR_IRQ];

void disable_irq(int irq);

// char fg attribute 
#define HRED 0xc
#define HGREEN 0xa
#define HBLUE 0x9
#define HWHITE 0xf

#define RED 4
#define GREEN 2
#define BLUE 1
#define WHITE 7

#define EFLAGS_AC_BIT           0x00040000
#define CR0_CACHE_DISABLE       0x60000000

u32 io_load_eflags(void);
void io_store_eflags(u32 eflg);
u32 load_cr0(void);
void store_cr0(u32 cr0);

#if 0
u32 io_load_eflags(void)
{
}

void io_store_eflags(u32 eflg)
{
}

u32 load_cr0(void)
{
}

void store_cr0(u32 cr0)
{
}
#endif

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

void clear_line(u8 line_no)
{
  u8* vb = (u8*)0xb8000 + 160*line_no;

  for (int x = 0; x < 80; ++x)
  {
    *vb++ = 0x20;
    *vb++ = WHITE;
  }
}

void clear()
{
  u8* vb = (u8*)0xb8000;
  const u8 row=25;
  
  for (int x = 0; x < 80*row ; ++x)
  {
    *vb++ = 0x20;
    *vb++ = WHITE;
  }
}

// prefix s32 means simple, 32bit code (in x86 protected mode)
void s32_put_char(u8 ch, u8 *vb)
{
  *vb = ch;
}

void s32_print(const u8 *s, u8 *vb)
{
  while(*s)
  {
    s32_put_char(*s, vb);
    ++s;
    vb+=2;
  }
  cur_vb = vb;
  if ((int)cur_vb >= 0xb8000+160*24)
    cur_vb = (u8*)0xb8000+160;
}

char* s32_itoa(int n, char* str, int radix)
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
    //s32_put_char(*(p-1), (u8*)(0xb8000+80*2));
    n/=radix;
  }
  *p=0;
  #if 1
  for (--p; head < p ; ++head, --p)
  {
    char temp=*head;
    if (*(p-1) != '-')
    {
      *head=*p;
      *p=temp;
    }
  }
  #endif
  return str;
}

void s32_print_int(int i, u8 *vb, int radix)
{
  u8 str[12]="";
  u8 *str_ptr = str;

  str_ptr = s32_itoa(i, str_ptr, radix);
  s32_print(str_ptr, vb);
}

void p_asm_memcpy(void *dest, void *src, u16 n);
void p_asm_memset(void *dest, int c, u16 n);

void c_test()
{
#if 0
{
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  u16 *p_gdt_limit = (u16*)(&gdt_ptr[0]);
  u32 *p_gdt_base = (u32*)(&gdt_ptr[2]);

  sp = s32_itoa(*p_gdt_limit, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*0));
  sp = s32_itoa(*p_gdt_base, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*1));
}
#endif
}

void init_protected_mode_by_c()
{
  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  //int i=65535;
#if 0
{
  u16 *p_gdt_limit = (u16*)(&gdt_ptr[0]);
  u32 *p_gdt_base = (u32*)(&gdt_ptr[2]);

  sp = s32_itoa(*p_gdt_limit, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*2));
  sp = s32_itoa(*p_gdt_base, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*3));
}
#endif


  p_asm_memcpy(&gdt, (void*)(*((u32*)(&gdt_ptr[2]))), *((u16*)(&gdt_ptr[0])) + 1);

  u16 *p_gdt_limit = (u16*)(&gdt_ptr[0]);
  u32 *p_gdt_base = (u32*)(&gdt_ptr[2]);
  *p_gdt_limit = GDT_SIZE * sizeof(Descriptor) - 1;
  *p_gdt_base = (u32)&gdt;

#if 0
  sp = s32_itoa(*p_gdt_limit, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*4));

  sp = s32_itoa(*p_gdt_base, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*5));
#endif

}

// vector no
#define DIVIDE_NO               0x0
#define DEBUG_NO                0x1
#define NMI_NO                  0x2
#define BREAKPOINT_NO           0x3
#define OVERFLOW_NO             0x4
#define BOUNDS_NO               0x5
#define INVAL_OP_NO             0x6
#define COPROC_NOT_NO           0x7
#define DOUBLE_FAULT_NO         0x8
#define COPROC_SEG_NO           0x9
#define INVAL_TSS_NO            0xA
#define SEG_NOT_NO              0xB
#define STACK_FAULT_NO          0xC
#define PROTECTION_NO           0xD
#define PAGE_FAULT_NO           0xE
#define COPROC_ERR_NO           0x10

void init_idt_by_c()
{
  void init_8259a();

  //s32_print("init_8259a", (u8*)(0xb8000+160*5));
  //init_8259a();

  void init_idt_desc_by_c(u8 vector_no, u8 desc_type, IntHandler handler, u8 privilege);

  u16 *idt_limit = (u16 *)(&idt_ptr[0]);
  u32 *idt_base = (u32 *)(&idt_ptr[2]);
  *idt_limit = IDT_SIZE * sizeof(Gate) - 1;
  *idt_base = (u32)&idt;
//  u8 a=0;

#if 0
  __asm__ volatile ("lidt %0\t\n"
                     :
		     :"m"(idt_ptr)
                   );
#endif
  void divide_error(void);
  void single_step_exception(void);
  void nmi(void);
  void breakpoint_exception(void);
  void overflow(void);
  void bounds_check(void);
  void inval_opcode(void);

  void copr_not_available(void);
  void double_fault(void);
  void copr_seg_overrun(void);
  void inval_tss(void);
  void segment_not_present(void);
  void stack_exception(void);
  void general_protection(void);
  void page_fault(void);
  void copr_error(void);

#if 1
  init_idt_desc_by_c(DIVIDE_NO, DA_386IGate, divide_error, PRIVILEGE_KRNL);
  init_idt_desc_by_c(DEBUG_NO, DA_386IGate, single_step_exception, PRIVILEGE_KRNL);
  init_idt_desc_by_c(NMI_NO, DA_386IGate, nmi, PRIVILEGE_KRNL);
  init_idt_desc_by_c(BREAKPOINT_NO, DA_386IGate, breakpoint_exception, PRIVILEGE_KRNL);
  init_idt_desc_by_c(OVERFLOW_NO, DA_386IGate, overflow, PRIVILEGE_KRNL);
  init_idt_desc_by_c(BOUNDS_NO, DA_386IGate, bounds_check, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INVAL_OP_NO, DA_386IGate, inval_opcode, PRIVILEGE_KRNL);

  init_idt_desc_by_c(COPROC_NOT_NO, DA_386IGate, copr_not_available, PRIVILEGE_KRNL);
  init_idt_desc_by_c(DOUBLE_FAULT_NO, DA_386IGate, double_fault, PRIVILEGE_KRNL);
  init_idt_desc_by_c(COPROC_SEG_NO, DA_386IGate, copr_seg_overrun, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INVAL_TSS_NO, DA_386IGate, inval_tss, PRIVILEGE_KRNL);
  init_idt_desc_by_c(SEG_NOT_NO, DA_386IGate, segment_not_present, PRIVILEGE_KRNL);
  init_idt_desc_by_c(STACK_FAULT_NO, DA_386IGate, stack_exception, PRIVILEGE_KRNL);
  init_idt_desc_by_c(PROTECTION_NO, DA_386IGate, general_protection, PRIVILEGE_KRNL);
  init_idt_desc_by_c(PAGE_FAULT_NO, DA_386IGate, page_fault, PRIVILEGE_KRNL);
  init_idt_desc_by_c(COPROC_ERR_NO, DA_386IGate, copr_error, PRIVILEGE_KRNL);
  #endif

  void hwint00(void);
  void hwint01(void);
  void hwint02(void);
  void hwint03(void);
  void hwint04(void);
  void hwint05(void);
  void hwint06(void);
  void hwint07(void);
  void hwint08(void);
  void hwint09(void);
  void hwint10(void);
  void hwint11(void);
  void hwint12(void);
  void hwint13(void);
  void hwint14(void);
  void hwint15(void);

  init_idt_desc_by_c(INT_VECTOR_IRQ0, DA_386IGate, hwint00, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 1, DA_386IGate, hwint01, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 2, DA_386IGate, hwint02, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 3, DA_386IGate, hwint03, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 4, DA_386IGate, hwint04, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 5, DA_386IGate, hwint05, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 6, DA_386IGate, hwint06, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 7, DA_386IGate, hwint07, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 8, DA_386IGate, hwint08, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 9, DA_386IGate, hwint09, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 10, DA_386IGate, hwint10, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 11, DA_386IGate, hwint11, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 12, DA_386IGate, hwint12, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 13, DA_386IGate, hwint13, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 14, DA_386IGate, hwint14, PRIVILEGE_KRNL);
  init_idt_desc_by_c(INT_VECTOR_IRQ0 + 15, DA_386IGate, hwint15, PRIVILEGE_KRNL);

#if 0
  void spurious_handler(void);

  for (int i=0 ; i <= 6; ++i)
    init_idt_desc_by_c(i, DA_386IGate, spurious_handler, PRIVILEGE_KRNL);
#endif

}

void init_idt_desc_by_c(u8 vector_no, u8 desc_type, IntHandler handler, u8 privilege)
{
  Gate *cur_gate = &idt[vector_no];

  u32 base = (u32)handler;
  cur_gate->offset_low      = base & 0xFFFF;
  cur_gate->selector        = SELECTOR_KERNEL_CS;
  cur_gate->dcount          = 0;
  cur_gate->attr            = desc_type | (privilege << 5);
  cur_gate->offset_high     = (base >> 16) & 0xFFFF;
}

void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags)
{
  char *err_msg[] = {
                      "#DE: Dvivde Error", 
		      "#DB: RESERVED",
		      "-- : NMI Interrupt", 
		      "#BP: Breakppoint",
		      "#OF: Overflow",
		      "#BR: BOUND Range Exceeded",
		      "#UD: Invalid Opcode (Undefined Opcode)",
		      "#NM: Device Not Available (No Math Coprocessor)",
		      "#DF: Double Fault",
		      "   : Coprocessor Segment Overrun (reserved)",
		      "#TS: Invalid TSS",
		      "#NP: Segment Not Present",
		      "#SS: Stack-Segment Fault",
		      "#GP: General Protection",
		      "#PF: Page Fault",
		      "-- : (Intel reserved. Do not use.)",
		      "#MF: x87 FPU Floating-Point Error (Math Fault)",
		      "#AC: Alignment Check",
		      "#MC: Machine Check",
		      "#XF: SIMD Floating-Point Exceprion"
                    };
  //clear();
  s32_print("exception_handler", (u8*)(0xb8000+160*24));

  u8 str[12]="";
  u8 *str_ptr = str;

  clear_line(0);
  clear_line(1);
  clear_line(2);
  clear_line(3);
  clear_line(23);

  s32_print(err_msg[vec_no], (u8*)(0xb8000+160*23));
  str_ptr = s32_itoa(eflags, str_ptr, 16);
  s32_print("eflags", (u8*)(0xb8000+160*0));
  s32_print(str_ptr, (u8*)(0xb8000+160*1));

  str_ptr = s32_itoa(cs, str_ptr, 16);
  s32_print("cs", (u8*)(0xb8000+160*2));
  s32_print(str_ptr, (u8*)(0xb8000+160*3));
  
  str_ptr = s32_itoa(eip, str_ptr, 16);
  clear_line(4);
  s32_print("eip", (u8*)(0xb8000+160*4));
  clear_line(5);
  s32_print(str_ptr, (u8*)(0xb8000+160*5));

  if (err_code != 0xffffffff)
  {
    str_ptr = s32_itoa(err_code, str_ptr, 16);
    clear_line(6);
    s32_print("err_code", (u8*)(0xb8000+160*6));
    clear_line(7);
    s32_print(str_ptr, (u8*)(0xb8000+160*7));
  }
}


void ptr_test(u8 *ch)
{
  ch[0] = 'X';
  //*ch = 'X';
}

int AA;
char data_str[]="data_string";

void startc()
{
#if 1
  clear();
  u8 *ro_str="ro_string";
  u8 *vb=(u8*)0xb8000;
  int i=65536;
  int c=AA+1;
  u8 stack_str[10]="y";
  u8 *sp = stack_str;

  *vb = 'W';
  #if 0
  while(1);
  #else
  __asm__ ("nop\n\t");
  __asm__ ("nop\n\t");
  #endif
  #if 0
  u8 ch='Q';
  *(vb+15*2) = ch;
  ptr_test(&ch);
  *(vb+17*2) = ch;
  #endif
  //s32_print(ro_str, vb+80*2);
  sp = s32_itoa(i, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160));
  //s32_put_char(sp[0], vb);
  sp = s32_itoa(c, stack_str, 10);
  s32_print(sp, (u8*)(0xb8000+160*2));
  s32_print(data_str, (u8*)(0xb8000+160*3));
  static char *s_str="static_point"; 
  s32_print(s_str, (u8*)(0xb8000+160*4));
  static char s_str_a[]="static_array"; 
  s32_print(s_str_a, (u8*)(0xb8000+160*5));
#endif
}



void init_8259a()
{
  void spurious_irq(int irq);

  // master 8259 icw1
  io_out8(INT_M_PORT, 0x11);

  // slave 8259 icw1
  io_out8(INT_S_PORT, 0x11);

  // master 8259 icw2
  io_out8(INT_M_CTLMASK, INT_VECTOR_IRQ0);

  /* Slave  8259, ICW2. 設置 '從8259' 的中斷入口地址為 0x28 */
  io_out8(INT_S_CTLMASK, INT_VECTOR_IRQ8);

  /* Master 8259, ICW3. IR2 對應 '從8259'. */
  io_out8(INT_M_CTLMASK, 0x4);

  /* Slave  8259, ICW3. 對應 '主8259' 的 IR2. */
  io_out8(INT_S_CTLMASK, 0x2);

  /* Master 8259, ICW4. */
  io_out8(INT_M_CTLMASK, 0x1);

  /* Slave  8259, ICW4. */
  io_out8(INT_S_CTLMASK, 0x1);

  /* Master 8259, OCW1.  */
  //io_out8(INT_M_CTLMASK, 0xFF);
  //io_out8(INT_M_CTLMASK, 0xfd); // keyboard irq 1
  io_out8(INT_M_CTLMASK, 0xfe); // timer irq 0

  /* Slave  8259, OCW1.  */
  io_out8(INT_S_CTLMASK, 0xFF);

  for (int i = 0 ; i < NR_IRQ ; ++i)
  {
    irq_table[i] = spurious_irq;
  }

}

void put_irq_handler(int irq, IrqHandler handler)
{
  disable_irq(irq);
  irq_table[irq] = handler;
}


void init_tss(void)
{
  p_asm_memset(&tss, 0, sizeof(tss));
  tss.ss0 = SELECTOR_KERNEL_DS;
  init_descriptor(&gdt[INDEX_TSS], linear2phy(seg2base(SELECTOR_KERNEL_DS), (u32)&tss), sizeof(tss)-1, DA_386TSS);
  tss.iobase = sizeof(tss); // ???

  for (int i = 0 ; i < NR_TASKS ; ++i)
  {
    init_descriptor(&gdt[INDEX_LDT_FIRST+i], linear2phy(seg2base(SELECTOR_KERNEL_DS), (u32)proc_table[i].ldt), LDT_SIZE * sizeof(Descriptor) - 1, DA_LDT);
  }

  
}

void spurious_irq(int irq)
{
  clear();
  s32_print("spurious_irq", (u8*)(0xb8000+160*6));

  u8 str[12]="";
  u8 *str_ptr = str;
  str_ptr = s32_itoa(irq, str_ptr, 16);
  s32_print(str_ptr, (u8*)(0xb8000+160*7));
}

void loop_delay(int time)
{
  int i, j, k;
  for (k = 0; k < time; k++) 
  {
    for (i = 0; i < 10; i++) 
    {
      for (j = 0; j < 10000; j++) 
      {
      }
    }
  }
}


u32 memsize;

void kernel_main(void)
{
  clear();
  clear_line(13);
  s32_print("memory size", (u8*)(0xb8000+160*13));
  //int memsize = memtest(0x00400000, 0xbfffffff) / (1024 * 1024);
  // 0x2000000 = 32MB
  // because of bochs will alert memory access more than the memory size,
  //   so limit address to 0x2000000, if use qemu, it works fine.
  memsize = memtest(0x00400000, 0x2000000);
  int memsize_mb = memsize / (1024 * 1024);

  u8 stack_str[10]="y";
  u8 *sp = stack_str;
  sp = s32_itoa(memsize_mb, stack_str, 10);
  
  clear();
#if 1
  clear_line(24);
  s32_print("memory size", (u8*)(0xb8000+160*24));
  s32_print(sp, (u8*)(0xb8000+160*24 + 12*2));
  s32_print("MB", (u8*)(0xb8000+160*24 + 12*2 + 4*2));
#endif

  void setup_paging(void);
  setup_paging();

  ready_process = proc_table;


  init_proc();

  cur_vb = (u8*)0xb8000+160;

  void restart(void);
  restart();
  s32_print("xxxxxxxxxxx", (u8*)(0xb8000+160*15));
  while(1);

}

