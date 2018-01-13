#include <stdlib.h>
#include "binheap.h"

void sift_up(binary_heap* binH, int id) {
	if(id == 1) return;

	int idParent = id/2;
	if(binH->compare(binH->array_content[id], binH->array_content[idParent]) < 0) {
		void* temp = binH->array_content[id];
		binH->array_content[id] = binH->array_content[idParent];
		binH->array_content[idParent] = temp;
		sift_up(binH, idParent);
	}
}

void insert_heap(binary_heap* binH, void* val) {
	binH->size++;
	void** p = realloc(binH->array_content, ((binH->size)+1) * sizeof(void*));
	if(p == NULL) {
		exit(1);
	} else {
		binH->array_content = p;
	}
	binH->array_content[binH->size] = val;
	sift_up(binH, binH->size);
}

void sift_down(binary_heap* binH, int id) {
	if(id*2 > binH->size || id*2+1 > binH->size)
		return;
	int compareRL = binH->compare(binH->array_content[id*2], binH->array_content[id*2+1]);
	int idSon;
	if(compareRL < 0)
		idSon = id*2;
	else
		idSon = id*2+1;
	int compareSon = binH->compare(binH->array_content[id], binH->array_content[idSon]);
	if(compareSon > 0) {
		void* temp = binH->array_content[id];
		binH->array_content[id] = binH->array_content[idSon];
		binH->array_content[idSon] = temp;
		sift_down(binH, idSon);
	}
}

void* remove_heap(binary_heap* binH) {
	void* ret = binH->array_content[1];
	binH->array_content[1] = binH->array_content[binH->size];
	void** p = realloc(binH->array_content, binH->size * sizeof(void*));
	if(p == NULL) {
		exit(1);
	} else {
		binH->array_content = p;
	}
	binH->size--;
	sift_down(binH, 1);
	return ret;
}

