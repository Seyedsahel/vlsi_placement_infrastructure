#include <stdio.h>
#include <string.h>


typedef struct {
    int x;
    int y;
} TL;  

typedef struct
{
   int id;
   char* name;
   // conected nodes that is a list of pointers to nodes
   Node* source_terminal;

} Net;

typedef struct {
    int id;
    char* node_name ;
    char* cell_name ;
    TL point;
    float width;
    float height;
    int row_num;
    Net* net;

} Node;

typedef struct 
{
    //metodes:
    //WL_cal for calculating wire length with hpwl algo
    //placer for placing algorithm
    // sa for simmulated anealing
    // reader with input: netlist and output: nlist(list of nodes) and tlist(list of nets)
    //parameters:
//int row_num;
// nlist: list of nodes
// tlist: list of nets
} Place;
