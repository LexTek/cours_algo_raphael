#include <stdlib.h>
#include "disjoint_set.h"

disjoint_set_e* dse_create(void* content) {
    disjoint_set_e* ret = malloc(sizeof(disjoint_set_e));
    ret->rank = 0;
    ret->parent = ret;
    ret->content = content;
    return ret;
}

disjoint_set_e* dse_find(disjoint_set_e* from) {
    if(from->parent != from)
        from->parent = dse_find(from->parent);
    return from->parent;
}

void dse_union(disjoint_set_e* x, disjoint_set_e* y) {
    disjoint_set_e* rootx = dse_find(x);
    disjoint_set_e* rooty = dse_find(y);

    if(rootx == rooty)
        return;

    if(rootx->rank < rooty->rank) {
        rootx->parent = rooty;
    } else if (rooty->rank < rootx->rank) {
        rooty->parent = rootx;
    } else {
        rooty->parent = rootx;
        rootx->rank++;
    }
}
