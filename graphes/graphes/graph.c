#include <stdlib.h>
#include <math.h>
#include "disjoint_set.h"

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

graph* init_graph(size_t nodeslen, size_t edgeslen, node* nodes, edge* edges) {
	graph* ret = malloc(sizeof(graph));
	ret->nodes = nodes;
	ret->edges = edges;
	for(int i=0; i<edgeslen; i++) {
		if(edges[i].node1 >= nodeslen || edges[i].node2 >= nodeslen) {
			free(ret);
			return NULL;
		}
	}
	return ret;
}

double distance_n(node* n1, node* n2) {
	return sqrt((n1->x - n2->x)*(n1->x - n2->x) + (n1->y - n2->y)*(n1->y - n2->y));
}

double distance_e(graph* g, edge* e) {
	return distance_n(g->nodes + e->node1, g->nodes + e->node2);
}

edge* kruskal(graph* g) {
	disjoint_set ds;
	ds.elements = malloc(sizeof(disjoint_set_e)*g->nodeslen);

	edge* ret = malloc(sizeof(edge)*g->edgeslen);

	int cpt = 0;

	for(int i=0; i<g->nodeslen; i++) {
		disjoint_set_e* ds_node_i = malloc(sizeof(disjoint_set_e));
		ds_node_i->rank = 0;
		ds_node_i->parent = ds_node_i;
		ds_node_i->content = g->nodes + i;
		ds.elements[i] = ds_node_i;
	}

	for(int i=0; i<g->edgeslen; i++) {
		if(dse_find(ds.elements[g->edges[i].node1])
			!= dse_find(ds.elements[g->edges[i].node2]))
		{
			dse_union(ds.elements[g->edges[i].node1],
					ds.elements[g->edges[i].node2]);
			ret[cpt] = g->edges[i];
			cpt;
		}
	}
	return ret;
}
