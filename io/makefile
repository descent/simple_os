ifdef COMSPEC
# in windows
include ..\rule.mk
else
include ../rule.mk
endif

k_stdio.o: k_stdio.c
	gcc $(CFLAGS) -c $<
