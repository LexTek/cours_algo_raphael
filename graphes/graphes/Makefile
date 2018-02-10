CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=gnu99
LDFLAGS=-lm
EXEC=generation

all: $(EXEC)

generation: generation.o graph.o binheap.o disjoint_set.o graph.h binheap.h disjoint_set.h
		$(CC) -o generation generation.o graph.o binheap.o disjoint_set.o $(LDFLAGS)

generation.o: generation.c
		$(CC) -c generation.c $(CFLAGS)

graph.o: graph.c
		$(CC) -c graph.c $(CFLAGS)

binheap.o: binheap.c
		$(CC) -c binheap.c $(CFLAGS)

disjoint_set.o: disjoint_set.c
	    $(CC) -c disjoint_set.c $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf hello
