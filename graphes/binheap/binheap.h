#define ALLOC 50

typedef struct binary_heap {
	int (*compare)(void*, void*);
	void** array_content;
	int size;
} binary_heap;

void insert_heap(binary_heap* binH, void* val);

void* remove_heap(binary_heap* binH);
