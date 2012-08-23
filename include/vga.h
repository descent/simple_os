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

void draw_box();
void draw_box_1(int draw_x, int draw_y, int c);
void draw_str();

#endif
