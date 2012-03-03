/* chapter3/1/pm.h

   Author: Wenbo Yang <solrex@gmail.com> <http://solrex.cn>

   This file is part of the source code of book "Write Your Own OS with Free
   and Open Source Software". Homepage @ <http://share.solrex.cn/WriteOS/>.

   This file is licensed under the GNU General Public License; either
   version 3 of the License, or (at your option) any later version. */

/* Comments below accords to "Chapter 3.4.5: Segment Descriptors" of "Intel
   64 and IA-32 Arch. SW Developer's Manual: Volume 3A: System Programming
   Guide". */

/* GDT Descriptor Attributes
     DA_  : Descriptor Attribute
     D    : Data Segment
     C    : Code Segment
     S    : System Segment
     R    : Read-only
     RW   : Read/Write
     A    : Access */
.set    DA_32,  0x4000  /* 32-bit segment */
.set    DA_LIMIT_4K, 0x8000     /* 4K */

/* Descriptor privilege level */
.set    DA_DPL0,  0x00  /* DPL = 0 */
.set    DA_DPL1,  0x20  /* DPL = 1 */
.set    DA_DPL2,  0x40  /* DPL = 2 */
.set    DA_DPL3,  0x60  /* DPL = 3 */

/* GDT Code- and Data-Segment Types */
.set    DA_DR,   0x90   /* Read-Only */
.set    DA_DRW,  0x92   /* Read/Write */
.set    DA_DRWA, 0x93   /* Read/Write, accessed */
.set    DA_C,    0x98   /* Execute-Only */
.set    DA_CR,   0x9A   /* Execute/Read */
.set    DA_CCO,  0x9C   /* Execute-Only, conforming */
.set    DA_CCOR, 0x9E   /* Execute/Read-Only, conforming */

/* GDT System-Segment and Gate-Descriptor Types */
.set    DA_LDT,      0x82    /* LDT */
.set    DA_TaskGate, 0x85    /* Task Gate */
.set    DA_386TSS,   0x89    /* 32-bit TSS(Available) */
.set    DA_386CGate, 0x8C    /* 32-bit Call Gate */
.set    DA_386IGate, 0x8E    /* 32-bit Interrupt Gate */
.set    DA_386TGate, 0x8F    /* 32-bit Trap Gate */

/* Selector Attributes */
.set    SA_RPL0, 0
.set    SA_RPL1, 1
.set    SA_RPL2, 2
.set    SA_RPL3, 3
.set    SA_TIG,  0
.set    SA_TIL,  4

/* Page Attributes */
.set    PG_P,    1
.set    PG_RWR,  0
.set    PG_RWW,  2
.set    PG_USS,  0
.set    PG_USU,  4

/* MACROS */

/* Segment Descriptor data structure.
   Usage: Descriptor Base, Limit, Attr
    Base:  4byte 
    Limit: 4byte (low 20 bits available)
    Attr:  2byte (lower 4 bits of higher byte are always 0) */
.macro Descriptor Base, Limit, Attr
    .2byte  \Limit & 0xFFFF
    .2byte  \Base & 0xFFFF
    .byte   (\Base >> 16) & 0xFF
    .2byte  ((\Limit >> 8) & 0xF00) | (\Attr & 0xF0FF)
    .byte   (\Base >> 24) & 0xFF
.endm

/* Gate Descriptor data structure.
   Usage: Gate Selector, Offset, PCount, Attr
    Selector:  2byte
    Offset:    4byte
    PCount:    byte
    Attr:      byte */
.macro Gate  Selector, Offset, PCount, Attr
    .2byte     (\Offset & 0xFFFF)
    .2byte     \Selector
    .2byte     (\PCount & 0x1F) | ((\Attr << 8) & 0xFF00)
    .2byte     ((\Offset >> 16) & 0xFFFF)
.endm
