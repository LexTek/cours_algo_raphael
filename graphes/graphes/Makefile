CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=gnu99
LDFLAGS=-lm
EXEC=graph

all: $(EXEC)

graph: graph.o binheap.o disjoint_set.o binheap.h disjoint_set.h
		$(CC) -o graph graph.o binheap.o disjoint_set.o $(LDFLAGS)

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
