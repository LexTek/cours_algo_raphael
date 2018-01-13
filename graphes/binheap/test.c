#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binheap.h"

#define NBVALS 1000000
#define RANGE 100000

int compare_int(int* a, int* b) {
	return *a - *b;
}

int main() {
	int** array_int;
	array_int = malloc(1*sizeof(int*));
	binary_heap heap;
	heap.compare = (int (*)(void*, void*)) compare_int;
	heap.array_content = (void**) array_int;
	heap.size = 0;

	struct timespec tstart={0,0}, tend={0,0};

	srand(time(NULL));
	int vals[NBVALS];
	int sortedvals[NBVALS];

	clock_gettime(CLOCK_MONOTONIC, &tstart);
	for(int i=0; i<NBVALS; i++) {
		vals[i] = rand()%RANGE;
		insert_heap(&heap, vals+i);
	}
	clock_gettime(CLOCK_MONOTONIC, &tend);
	printf("inserted %d in %.5f seconds\n", NBVALS, ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec)-((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	
	printf("extracting ...\n");
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	for(int i=0; i<NBVALS; i++) {
		int v = *(int*)remove_heap(&heap);
		sortedvals[i] = v;
	}
	clock_gettime(CLOCK_MONOTONIC, &tend);
	printf("removed %d in %.5f seconds\n", NBVALS, ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec)-((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	
	int precval = -1;
	for(int i=0; i<NBVALS; i++) {
		if(sortedvals[i] < precval) exit(1);
		precval = sortedvals[i];
	}
}
