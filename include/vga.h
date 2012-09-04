#ifndef VGA_H
#define VGA_H

// ref: 
// http://www.stanford.edu/class/cs140/projects/pintos/specs/freevga/vga/portidx.htm
// http://www.stanford.edu/class/cs140/projects/pintos/specs/freevga/vga/crtcreg.htm

// CRT Controller Registers 
#define CRTC_ADDR 0x3D4
#define CRTC_DATA 0x3D5

#define START_ADDR_H    0xC     /* reg index of video mem start addr (MSB) */
#define START_ADDR_L    0xD     /* reg index of video mem start addr (LSB) */
#define CURSOR_H        0xE     /* reg index of cursor position (MSB) */
#define CURSOR_L        0xF     /* reg index of cursor position (LSB) */


// ref: http://www.2cto.com/kf/201111/109728.html
#define PALETTE_MASK            0x3c6   //调色板屏蔽寄存器端口，放入0xff可以通过调色板索引寄存器0x3c7和0x3c8访问你希望的寄存器 
#define PALETTE_REGISTER_RD     0x3c7   //读颜色寄存器端口 
#define PALETTE_REGISTER_WR     0x3c8   //写颜色寄存器端口 
#define PALETTE_DATA            0x3c9   //调色板数据寄存器端口 

#endif
