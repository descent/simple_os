echo.bin: echo
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
echo: start.o echo.o
#../syscall.o 
#../asm_func.o ../clock.o ../k_stdlib.o ../process.o ../storage.o  ../systask.o ../asm_syscall.o  ../k_assert.o  ../p_kernel.o  ../start.o 
	#ld -m elf_i386 -Ttext 0x1000 -o echo $^
	ld -m elf_i386 -Ttext 0x1000 -e cstart -o echo start.o echo.o ../p_kernel.o ../io/k_stdio.o ../asm_func.o ../asm_syscall.o ../vga/set_mode_p.o ../vga/draw_func.o ../vga/font_rawdata.o ../tty/tty.o ../tty/keyboard.o ../tty/console.o ../clock.o ../start.o ../process.o ../storage.o ../syscall.o ../io/k_stdio_k.o ../k_assert.o ../systask.o ../mm/mm.o ../k_stdlib.o ../fs/romfs.o ../fs/vfs.o
echo.o: echo.c
	gcc -g -I../include -m32 -c echo.c 
start.o: start.S
	gcc -g -m32 -c start.S 

clean:
	rm -rf *.o echo
