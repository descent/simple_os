echo.bin: echo
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
echo: echo.o start.o
	ld -m elf_i386 -Ttext 0x1000 -o echo start.o echo.o
echo.o: echo.c
	gcc -m32 -c echo.c 
start.o: start.S
	gcc -m32 -c start.S 

clean:
	rm -rf *.o echo
