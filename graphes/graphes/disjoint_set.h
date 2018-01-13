typedef struct disjoint_set_e {
	int rank;
	struct disjoint_set_e* parent;
	void* content;
} disjoint_set_e;

typedef struct disjoint_set {
	disjoint_set_e** elements;	
} disjoint_set;

disjoint_set_e* dse_create(void* content);

disjoint_set_e* dse_find(disjoint_set_e* from);

void dse_union(disjoint_set_e* x, disjoint_set_e* y);
