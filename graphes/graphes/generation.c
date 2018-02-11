#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "time.h"

void shuffle(edge *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            edge t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void print_grid(graph* g, int width, int height, size_t *preds) {
    assoc_list ass = to_assoc(g);
    int print_width = (width*2 + 1);
    int print_height = (height * 2 + 1);
    char to_print[ print_width * print_height ];
    for(int y = 0; y<print_height; y++) {
        for(int x = 0; x<print_width; x++) {
            to_print[x + y * print_width] = 0;
        }
    }

    for(int x=0; x<width; x++) {
        for(int y=0; y<height; y++) {
            to_print[(x*2+1) + (y*2+1)*print_width] = 1;
            for(size_t i=0; i<ass.lst[x+y*width].nb_neighbors; i++) {
                size_t id_neighbor = ass.lst[x+y*width].neighbors[i];
                if(g->nodes[id_neighbor].x - g->nodes[x+y*width].x == 0) {
                    if(g->nodes[id_neighbor].y < g->nodes[x+y*width].y) {//UP
                        to_print[(x*2+1) + (y*2)*print_width] = 1;
                    }
                }
                if(g->nodes[id_neighbor].y - g->nodes[x+y*width].y == 0) {
                    if(g->nodes[id_neighbor].x < g->nodes[x+y*width].x) {//LEFT
                        to_print[(x*2) + (y*2+1)*print_width] = 1;
                    }
                }
            }
        }
    }
    size_t path = g->nodeslen-1;
    while(path != -1) {
        to_print[g->nodes[path].x*2+1 + (g->nodes[path].y*2+1)*print_width] = 2;
        path = preds[path];
    }

    for(int y = 0; y<print_height; y++) {
        for(int x = 0; x<print_width; x++) {
            switch (to_print[x + y*print_width]) {
                case 0:
                    printf("\u2588");
                    break;
                case 1:
                    printf(" ");
                    break;
                case 2:
                    printf("*");
                    break;
            };
        }
        printf("\n");
    }

    free_assoc_list(ass);
}

int main(int argc, char const* argv[])
{
    if(argc != 3) {
        printf("Usage : generation width height\n");
        return 1;
    }

    srand(time(NULL));

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    size_t nodeslen = width*height;
    size_t edgeslen = width*(height + 1) + height*(width + 1);
    node* nodes = malloc(nodeslen*sizeof(node));
    edge* edges = malloc(edgeslen*sizeof(edge));
    graph g;
    g.edges = edges;
    g.nodes = nodes;
    g.nodeslen = nodeslen;
    g.edgeslen = edgeslen;
    int i_edges = 0;

    for(int x=0; x<width; x++) {
        for(int y=0; y<height; y++) {
            nodes[x+y*width].x = x;
            nodes[x+y*width].y = y;
            if(x!=0) {
                edges[i_edges].node1 = x-1+y*width;
                edges[i_edges].node2 = x+y*width;
                i_edges++;
            }
            if(y!=0) {
                edges[i_edges].node1 = x+(y-1)*width;
                edges[i_edges].node2 = x+y*width;
                i_edges++;
            }
        }
    }
    g.edgeslen = i_edges;

    shuffle(g.edges, g.edgeslen);
    edge* new_edges = malloc(g.edgeslen*sizeof(edge));

    size_t sz = kruskal(&g, new_edges);
    g.edgeslen = sz;
    g.edges = new_edges;
    free(edges);
    
    size_t* preds = djikstra(&g, 0);

    printf("width : %d, height : %d, nb_node : %d, nb_edges : %d, edges : %d\n", width, height, width*height, (width*(height + 1) + height*(width + 1) ), g.edgeslen);
    print_grid(&g, width, height, preds);

    free(g.nodes);
    free(g.edges);
    free(preds);

    return 0;
}
