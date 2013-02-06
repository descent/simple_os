//kl meads kernel loader
#ifndef KL_GLOBAL_H
#define KL_GLOBAL_H

#define KERNEL_ES 0x0050
#define KERNEL_INFO (KERNEL_ES << 4 | LOAD_KERNEL_OFFSET)
#define IMAGE_SIZE  8192
#define BLOCK_SIZE  512
#define READ_FAT_ADDR (0x3000) // original is 0x2000, but will overwrite bss (variable bpb), so change to 0x3000
#define IMAGE_LMA   (0x4000)
#define LOAD_KERNEL_OFFSET (0x0)
//#define IMAGE_LMA   0x8000
#define IMAGE_ENTRY 0x800c
#define buf_addr_val (*(u8 volatile*(IMAGE_LMA)))


#endif

