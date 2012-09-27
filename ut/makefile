ut: ut.o ../io/k_stdio_k.o  ../io/k_stdio.o
	$(CC) -std=c99 -m32 -o $@ $^
ut.o: ut.c
	$(CC) -std=c99 -m32  -fno-builtin -g -Iinclude -I../include -c $<

.PHONE: clean distclean kloaderp.bin

clean:
	rm -rf *.o ut
