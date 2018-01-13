#ifndef DISJOINTSET_H
#define DISJOINTSET_H

struct disjoint_set_e {
	int rank;
	struct disjoint_set_e *parent;
	void* array_content;
};

typedef struct disjoint_set_e disjoint_set_e;

disjoint_set_e* dse_create(void* array_content); 

disjoint_set_e* dse_find(disjoint_set_e* from); 

void dse_union(disjoint_set_e* x, disjoint_set_e* y);

#endif
