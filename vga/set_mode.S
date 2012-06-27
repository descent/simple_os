# ref: http://bos.asmhackers.net/docs/vga_without_bios/snippet_1/modsrc30/MAIN.ASM
# ref: http://bos.asmhackers.net/docs/vga_without_bios/snippet_1/modsrc30/USAGE.TXT
# ref: http://bos.asmhackers.net/docs/vga_without_bios/snippet_1/modsrc30/README.TXT
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=-   Currently supported Video Modes:    -= Mode Number =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=-  03H    -=- 320 x 200 x 256           -=     03H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=-  10H    -=- 640 x 350 x 16            -=     10H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=-  12H    -=- 640 x 480 x 16            -=     12H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=-  13H    -=- 320 x 200 x 256           -=     13H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=- Chain 4 -=- unchained 320 x 200 x 256 -=     14H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=- Mode X  -=- unchained 320 x 240 x 256 -=     15H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=- Mode A  -=- unchained 320 x 400 x 256 -=     16H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=- Mode B  -=- unchained 360 x 480 x 256 -=     17H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#=- Mode C  -=- 640 x 400 x 16            -=     18H     =-
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

.equ ATTRCON_ADDR      ,       0x03C0
.equ MISC_ADDR         ,       0x03C2
.equ VGAENABLE_ADDR    ,       0x03C3
.equ SEQ_ADDR          ,       0x03C4
.equ GRACON_ADDR       ,       0x03CE
.equ CRTC_ADDR         ,       0x03D4
.equ STATUS_ADDR       ,       0x03DA


#.code32
.code16
.text
.global begin
begin:
#MOV $MODE12H, %si
xchg %bx, %bx #bochs magic break point
mov %cs, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %ss

MOV $MODE10H, %si
CALL SETMODE
call SET_OUT
# draw screen
   pushw $12
   CALL CLEAR10H

# Set Mode 03h
#SET03H:
#  MOV $MODE03H, %si
#  CALL SETMODE
  # Must do this to ensure proper text
#  CALL SETFONT
#  JMP SET_OUT


  jmp .


CLEAR10H:
   PUSH %BP
   MOV %SP, %bp

MOV $0xA000, %ax
MOV %AX, %es
XOR %DI,%DI
MOV $7000, %cx

CLEAR_LOOP2:
   MOV $0x03CE, %dx
   MOV $0x0FF08, %ax
   OUT %AX, %dx

   MOV $0, %eax
   #XCHG DWORD PTR ES:[DI],EAX
   xchgl %EAX, %es:(%di)

   MOV $0x03C4, %dx
   #MOV AH,BYTE PTR [BP+4]
   movb 4(%BP), %ah
   MOV $2, %AL
   OUT %AX, %dx

   #MOV DWORD PTR ES:[DI],0FFFFFFFFH
   movl $0xFFFFFFFF, %es:(%di)

   MOV $0x0F02, %ax
   OUT %AX, %dx

   MOV $0x03CE, %dx
   MOV $0x0FF08, %ax
   OUT %AX, %dx

   ADD $4, %DI
   LOOP CLEAR_LOOP2

   POP %BP
   RET 




SETMODE:
   # Send MISC regs
   MOV $MISC_ADDR, %dx
   MOV (%si), %AL
   OUT %al, %DX
   call iodelay
   INC %SI

   MOV $STATUS_ADDR, %dx
   MOV (%si), %AL
   OUT %AL, %dx
   call iodelay
   INC %SI

   # Send SEQ regs
   MOV $0, %CX
REG_LOOP:
   MOV $SEQ_ADDR, %dx
   MOV %CL, %al
   OUT %AL, %dx
   call iodelay

   MOV $SEQ_ADDR, %dx
   INC %DX
   MOV (%si), %AL
   OUT %AL, %dx
   call iodelay
   INC %SI
   INC %CX
   CMP $5, %cl
   JL REG_LOOP

   # Clear Protection bits
   MOV $0x0E, %ah
   MOV 0x11, %al
   AND 0x7F, %ah
   MOV $CRTC_ADDR, %dx
   OUT %AX, %dx
   call iodelay

   # Send CRTC regs
   MOV $0, %cx
REG_LOOP2:
   MOV $CRTC_ADDR, %dx
   MOV %CL, %al
   OUT %AL, %dx
   call iodelay

   MOV $CRTC_ADDR, %dx
   INC %DX
   MOV (%si), %AL
   OUT %AL, %dx
   call iodelay

   INC %SI
   INC %CX
   CMP $25, %CL
   JL REG_LOOP2

   # Send GRAPHICS regs
   MOV $0, %CX
REG_LOOP3:
   MOV $GRACON_ADDR, %dx
   MOV %CL, %al
   OUT %AL, %dx
   call iodelay

   MOV $GRACON_ADDR, %dx
   INC %DX
   MOV (%si), %AL
   OUT %AL, %dx
   call iodelay

   INC %SI
   INC %CX
   CMP $9, %CL
   JL REG_LOOP3

   MOV $STATUS_ADDR, %dx
   IN %DX, %al
   call iodelay

   # Send ATTRCON regs
   MOV $0, %cx
REG_LOOP4:
   MOV $ATTRCON_ADDR, %dx
   IN %DX, %ax

   MOV %CL, %al
   OUT %AL, %dx
   call iodelay

   MOV (%si), %AL
   OUT %AL, %dx
   call iodelay

   INC %SI
   INC %CX
   CMP $21, %CL
   JL REG_LOOP4

   MOV $0x20, %al
   OUT %AL, %dx
   call iodelay
   RET
#SETMODE ENDP
SET_OUT:
  CALL SETPALETTE
  #POP %BP
  RET

iodelay:
  nop
  nop
  nop
  nop
  ret

SETPALETTE:
   # Address of the 6-byte array
   MOV $PALETTE, %di

   MOV $0, %ECX
PAL_LOOP:
   MOV %CX, %ax
   # Send color
   MOV $0x03C8, %dx
   OUT %AL, %dx

   # Write R value
   MOV $0x03C9, %dx
   movb (%DI), %al
   OUT %AL, %dx

   # Write G value
   MOV $0x03C9, %dx
   movb 1(%DI), %al
   OUT %AL, %dx

   # Write B value
   MOV $0x03C9, %dx
   movb 2(%DI), %al
   OUT %AL, %dx

   ADD $3, %di
   INC %CX
   CMP $16, %CX
   JLE PAL_LOOP

   RET

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#=- VGA register variables for the various videomodes                       -=
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if 0
MODE03H:
.byte 0x67,0x00,0x03,0x00,0x03,0x00,0x02
.byte 0x5F,0x4F,0x50,0x82,0x55,0x81,0x0BF,0x1F,0x00,0x4F,0x0E,0x0F,0x00,0x00,0x00,0x00
.byte 0x9C,0x0E,0x8F,0x28,0x01,0x96,0x0B9,0x0A3,0x0FF
.byte 0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0x0FF
.byte 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
.byte 0x0C,0x00,0x0F,0x08,0x00
#endif


MODE10H:
.byte 0xA3,0x00,0x03,0x01,0x0F,0x00,0x06
.byte 0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00
.byte 0x83,0x85,0x5D,0x28,0x0F,0x63,0x0BA,0x0E3,0x0FF
.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0x0FF
.byte 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
.byte 0x01,0x00,0x0F,0x00,0x00

MODE12H:
.byte 0x0E3, 0x00, 0x03, 0x01, 0x0F, 0x00, 0x06
.byte 0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x59
.byte 0xEA,0x8C,0x0DF,0x28,0x00,0x0E7,0x04,0x0E3,0x0FF
.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0x0FF
.byte 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
.byte 0x01,0x00,0x0F,0x00,0x00

PALETTE:
.byte   0,  0,  0,  0,  0, 42,  0, 42,  0,  0, 42, 42, 42,  0,  0
.byte 42,  0, 42, 42, 21,  0, 42, 42, 42, 21, 21, 21, 21, 21, 63, 21, 63, 21
.byte 21, 63, 63, 63, 21, 21, 63, 21, 63, 63, 63, 21, 63, 63, 63

