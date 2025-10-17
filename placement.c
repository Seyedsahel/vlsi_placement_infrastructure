#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdlib.h>



typedef struct {
    int x;
    int y;
} TL;  

typedef struct net{
   int id;
   char* name;
   list nodes;
   node* source_terminal;

} net;

typedef struct node{
    int id;
    char* node_name ;
    char* cell_name ;
    TL point;
    float width;
    float height;
    int row_num;
    net* net;

} node;

typedef struct place{
    
int row_num;
list* nlist;
list* tlist;
} place;

//metodes:
    //place_init 
    //WL_cal for calculating wire length with hpwl algo
    //placer for placing algorithm
    // sa for simmulated anealing
    // reader with input: netlist and output: nlist(list of nodes) and tlist(list of nets)

place* place_init(int rownum, size_t list_size){
    
    place* p = malloc(sizeof(place));
    if (!p) {
        fprintf(stderr, "Memory allocation failed for place!\n");
        return NULL;
    }

    p->row_num = rownum;
    p->nlist = initial_list(list_size,sizeof(node));
    p->tlist = initial_list(list_size,sizeof(net));

    return p;

}

