#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    int x;
    int y;
} TL;  

typedef struct node node;
typedef struct net net;

typedef struct net{
   int id;
   char* name;
   list nodes;
   node* source_terminal;
} net;

typedef struct node{
    int id;
    char* node_name;
    char* cell_name;
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

// ------------ place_init ------------
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

// ------------ reader ------------
void reader(place* p, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return;
    }

    char line[512];
    int net_id = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0)
            continue;

        char* start = strchr(line, '(');
        char* end = strchr(line, ')');
        if (!start || !end)
            continue;
        *end = '\0';
        start++;

        char* net_name = strtok(start, ":");
        if (!net_name) continue;

        net_name = strdup(net_name);
        net* nt = malloc(sizeof(net));
        nt->id = net_id++;
        nt->name = net_name;
        nt->source_terminal = NULL;
        nt->nodes = *initial_list(10, sizeof(node*)); 

        char* rest = strtok(NULL, ":");
        if (!rest) continue;

        char* token = strtok(rest, ",");
        while (token) {
            while (*token == ' ') token++; 

            char nodename[64];
            char pinname[64] = "";
            if (strchr(token, '.'))
                sscanf(token, "%63[^.].%63s", nodename, pinname);
            else
                strcpy(nodename, token);

            node* found = NULL;
            for (int i = 0; i < p->nlist->index; i++) {
                node* n = get_element(p->nlist, i);
                if (strcmp(n->node_name, nodename) == 0) {
                    found = n;
                    break;
                }
            }

            if (!found) {
                node n;
                n.id = p->nlist->index;
                n.node_name = strdup(nodename);
                n.cell_name = strdup(pinname);
                n.net = nt;
                n.row_num = 0;
                n.point.x = n.point.y = 0;
                n.width = n.height = 0;
                add_element(p->nlist, &n);

                found = get_element(p->nlist, p->nlist->index - 1);
            }

            add_element(&nt->nodes, &found);
            token = strtok(NULL, ",");
        }

        add_element(p->tlist, nt);
    }

    fclose(file);
}

//---------main--------------

int main(){
    
place* p = place_init(4, 10);
printf("test\n");
reader(p, "netlist.txt");

printf("== Nets ==\n");
for (int i = 0; i < p->tlist->index; i++) {
    net* nt = get_element(p->tlist, i);
    printf("%s: ", nt->name);
    for (int j = 0; j < nt->nodes.index; j++) {
        node** np = get_element(&nt->nodes, j);
        printf("%s ", (*np)->node_name);
    }
    printf("\n");
}
printf("== Nodes ==\n");
for (int i = 0; i < p->nlist->index; i++) {
    net* nd = get_element(p->nlist, i);
    printf("%s", nd->name);
    for (int j = 0; j < nd->nodes.index; j++) {
        node** np = get_element(&nd->nodes, j);
        printf("%s ", (*np)->node_name);
    }
    printf("\n");
}
    
    return 0;
}