#include <stdlib.h>

typedef struct node {
    int x;
    int y;
} node;

typedef struct edge {
    size_t node1;
    size_t node2;
} edge;

typedef struct graph {
    node* nodes;
    size_t nodeslen;
    edge* edges;
    size_t edgeslen;
} graph;

typedef struct assoc_elem {
    size_t* neighbors;
    double* lengths;
    size_t nb_neighbors;
} assoc_elem;

typedef struct assoc_list {
    assoc_elem* lst;
    size_t size;
} assoc_list;

graph* init_graph(size_t nodeslen, size_t edgeslen, node* nodes, edge* edges);

assoc_list to_assoc(graph* g);

size_t kruskal(graph* g, edge* ret);

size_t* djikstra(graph* g, size_t start);

void free_assoc_content(assoc_elem* assoc);

void free_assoc_list(assoc_list lst);
