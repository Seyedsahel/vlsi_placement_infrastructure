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

