echo.bin: echo
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
echo: start.o echo.o
#../syscall.o 
#../asm_func.o ../clock.o ../k_stdlib.o ../process.o ../storage.o  ../systask.o ../asm_syscall.o  ../k_assert.o  ../p_kernel.o  ../start.o 
	ld -m elf_i386 -Ttext 0x1000 -o echo $^
echo.o: echo.c
	gcc -g -m32 -c echo.c 
start.o: start.S
	gcc -g -m32 -c start.S 

clean:
	rm -rf *.o echo
