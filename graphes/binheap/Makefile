CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=gnu99 -O3
LDFLAGS=-O3
EXEC=test

all: $(EXEC)

test: test.o libbinheap.a
	$(CC) -o test -static $< -L. -lbinheap $(LDFLAGS)

test.o: test.c
	$(CC) -o $@ -c $< $(LDFLAGS)

libbinheap.a: binheap.o
	ar rcs $@ $<

binheap.o: binheap.c binheap.h
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o *.a

mrproper: clean
	rm -rf $(EXEC)
