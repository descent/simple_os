CC=gcc-4.7
dump_bpb: dump_bpb.o
	$(CC) -m32 -std=c99 -o $@ $<
dump_bpb.o: dump_bpb.c
	$(CC) -m32 -std=c99 -c $<
genromfs: genromfs.o
	gcc -o $@ $<
genromfs.o: genromfs.c
	gcc -o $@ -c $<

genromfs_2.o: genromfs_2.c  
	gcc -o $@ -c $<

clean:
	rm -rf *.o
