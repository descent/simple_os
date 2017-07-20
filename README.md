simple os:
on the x86/32 bit mode platform.
* using x86 protected mode.
* process switch.
* implement fork/exec/wait.
* support vga graphic mode, in protected mode switch to graphic mode and switch back to text mode.
* kernel loader can load more than 1M size kernel (using Unreal mode).
* ipc.
* ramdisk.

demo video:

[![IMAGE ALT TEXT](https://img.youtube.com/vi/wtTc2bLOnFY/0.jpg)](https://www.youtube.com/watch?v=wtTc2bLOnFY)

boot loader loads the kernel loader (kernel_loader/kloaderp.bin) to 0x9000:0x0100.

in kernel loader, init c runtime, then use c code (16 bit) read elf kerenl
(in Unreal mode) to memory address 0x300000+0x28*16 (by bios 0x13),
then enter protected mode.

in protected mode, copy the the p_kernel.elf (c code, in the state, only can use stack variable)
follow the elf header information (Program Headers:).
then jump to the kernel address (elf entry point address).

kloaderp.bin load p_kernel.elf to 0x300000+0x28*16

than copy p_kernel.elf by elf information to absolute address 0x100000.
kernel load address depends k.ld (. = 0x100000), current is 0x100000.
p_kernel.elf entry pointer is 0x100000.

ramdisk filesystem is romfs.
it is megered to p_kernel.elf.

tty design:
0, 1, 2 text mode alt+f1, alt+f2, alt+f3
The 3rd tty is graphic mode (alt+f6)
now can switch graphic/text mode tty.

the url show some pictures and a vidoe demo (in real pc).
(http://descent-incoming.blogspot.tw/2013/02/x86-1m-kernel.html)

make test floppy image
boot.bin is boot loader
c-bootloader/mkimg.sh -b boot.bin -d boot.img

copy kernel loader and kernel to floppy image
./script/cp2img.sh 

modify REAL_PC macro
kernel_loader/protected_code.c
if only in emulator, comment #define REAL_PC

compile/test:
qemu:
  make

real pc:
  make REAL_PC=1

qemu cmd:
  qemu-system-i386 -fda boot.img --nographic # ctrl+a x to exit
  qemu-system-i386 -fda boot.img --display sdl
  qemu-system-i386 -fda boot.img --display curses # esc + 1, esc + 2 q enter to exit ref: http://stackoverflow.com/questions/14165158/how-to-switch-to-qemu-monitor-console-when-running-with-curses
  qemu-system-i386 -fda boot.img

bochs cmd:
  bochs 1.6.8 configure file
  x86.bxrc for bochs debugger
  x86-gdb.bxrc for gdb
  bochs -f x86.bxrc

