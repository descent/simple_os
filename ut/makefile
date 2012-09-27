include ../rule.mk

ut: ut.o ../io/k_stdio_k.o  ../io/k_stdio.o
	$(CC) -std=c99 -m32 -o $@ $^
ut.o: ut.c
	$(CC) -std=c99 -m32  -fno-builtin -g -Iinclude -I../include -c $<

io/k_stdio.o: io
	(cd ../io; make k_stdio.o)
io/k_stdio_k.o: io
	(cd ../io; make k_stdio_k.o)

.PHONE: clean distclean kloaderp.bin

clean:
	rm -rf *.o ut
