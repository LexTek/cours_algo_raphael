#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "disjoint_set.h"
#include "binheap.h"

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
	for(size_t i=0; i<edgeslen; i++) {
		if(edges[i].node1 >= nodeslen || edges[i].node2 >= nodeslen) {
			free(ret);
			return NULL;
		}
	}
	return ret;
}

typedef struct assoc_elem {
	size_t* neighbors;
	double* lengths;
	size_t nb_neighbors;
} assoc_elem;

double distance_n(node* n1, node* n2) {
	return sqrt((n1->x - n2->x)*(n1->x - n2->x) + (n1->y - n2->y)*(n1->y - n2->y));
}

double distance_e(graph* g, edge* e) {
	return distance_n(g->nodes + e->node1, g->nodes + e->node2);
}

assoc_elem* to_assoc(graph* g) {
	assoc_elem* assoc = malloc(g->nodeslen * sizeof(assoc_elem));
	for(size_t i = 0; i<g->nodeslen; i++) {
		assoc[i].nb_neighbors = 0; 
	}
	
	for(size_t i = 0; i<g->edgeslen; i++) {
		size_t node1 = g->edges[i].node1;
		size_t node2 = g->edges[i].node2;
		assoc[node1].neighbors = realloc(assoc[node1].neighbors, (assoc[node1].nb_neighbors + 1) * sizeof(size_t));
		assoc[node1].lengths = realloc(assoc[node1].lengths, (assoc[node1].nb_neighbors + 1) * sizeof(size_t));
		assoc[node1].neighbors[assoc[node1].nb_neighbors] = node2;
		assoc[node1].lengths[assoc[node1].nb_neighbors] = distance_e(g, g->edges + i);
		assoc[node1].nb_neighbors++;
		
		assoc[node2].neighbors = realloc(assoc[node2].neighbors, (assoc[node2].nb_neighbors + 1) * sizeof(size_t));
		assoc[node2].lengths = realloc(assoc[node2].lengths, (assoc[node2].nb_neighbors + 1) * sizeof(size_t));
		assoc[node2].neighbors[assoc[node2].nb_neighbors] = node1;
		assoc[node2].lengths[assoc[node2].nb_neighbors] = distance_e(g, g->edges + i);
		assoc[node2].nb_neighbors++;
	}
	return assoc;
}

//-------------------------------------------------------------------//

edge* kruskal(graph* g) {
	disjoint_set ds;
	ds.elements = malloc(sizeof(disjoint_set_e)*g->nodeslen);

	edge* ret = malloc(sizeof(edge)*g->edgeslen);

	int cpt = 0;

	for(size_t i=0; i<g->nodeslen; i++) {
		disjoint_set_e* ds_node_i = malloc(sizeof(disjoint_set_e));
		ds_node_i->rank = 0;
		ds_node_i->parent = ds_node_i;
		ds_node_i->content = g->nodes + i;
		ds.elements[i] = ds_node_i;
	}

	for(size_t i=0; i<g->edgeslen; i++) {
		if(dse_find(ds.elements[g->edges[i].node1])
			!= dse_find(ds.elements[g->edges[i].node2]))
		{
			dse_union(ds.elements[g->edges[i].node1],
					ds.elements[g->edges[i].node2]);
			ret[cpt] = g->edges[i];
			//cpt; WHYYYYYYYYYYY ????
		}
	}
	return ret;
}

//-------------------------------------------------------------------//
typedef struct djikstra_node {
	node* u;
	size_t id;
	double dist;
} djikstra_node;

int compare_ds_node(djikstra_node* n1, djikstra_node* n2) {
	if(n1->dist < n2->dist) return -1;
	else if(n2->dist < n1->dist) return 1;
	else return 0;
}

double djikstra(graph* g, size_t dst, size_t start) {
	djikstra_node* dists = malloc(g->nodeslen * sizeof(djikstra_node));
	for(size_t i=0; i<g->nodeslen; i++) {
		dists[i].u = &(g->nodes[i]);
		dists[i].id = i;
		if(i == start) dists[i].dist = 0;
		else dists[i].dist = DBL_MAX; // BAD_C0DE
	}

	djikstra_node** array_nodes = malloc(2*sizeof(djikstra_node*));
	array_nodes[1] = &(dists[start]);

	binary_heap min_dist;

	min_dist.size = 1;
	min_dist.array_content = (void **) array_nodes;
	min_dist.compare = (int (*)(void*, void*)) compare_ds_node;

	assoc_elem* assoc_list = to_assoc(g);

	while(min_dist.size > 0) {
		djikstra_node* u = remove_heap(&min_dist);
		for(size_t i=0; i<assoc_list[u->id].nb_neighbors; i++) {
			djikstra_node* neighbor = dists + assoc_list[u->id].neighbors[i];
			double edge_length = assoc_list[u->id].lengths[i];
			if(u->dist + edge_length < neighbor->dist) {
				neighbor->dist = u->dist + edge_length;
				insert_heap(&min_dist, neighbor);
			}
		}
	}

	return dists[dst].dist;
}

int main(int argc, char const* argv[])
{
	node nodes[6];
	int xs[6] = {0, 1, 2, 3, 4, 3};
	int ys[6] = {0, 3, 2, 1, 2, 3};
	for(int i=0; i<6; i++) {
		nodes[i].x = xs[i];
		nodes[i].y = ys[i];
	}
	edge edges[9];
	edges[0].node1 = 0;
	edges[0].node2 = 1;
	
	edges[1].node1 = 1;
	edges[1].node2 = 2;
	
	edges[2].node1 = 0;
	edges[2].node2 = 2;
	
	edges[3].node1 = 0;
	edges[3].node2 = 3;
	
	edges[4].node1 = 2;
	edges[4].node2 = 3;
	
	edges[5].node1 = 2;
	edges[5].node2 = 5;
	
	edges[6].node1 = 3;
	edges[6].node2 = 5;
	
	edges[7].node1 = 3;
	edges[7].node2 = 4;
	
	edges[8].node1 = 5;
	edges[8].node2 = 4;

	graph g;
	g.edges = edges;
	g.edgeslen = 9;
	g.nodes = nodes;
	g.nodeslen = 6;

	double min_d = djikstra(&g, 4, 0);
	printf("%lf", min_d);
	return 0;
}
