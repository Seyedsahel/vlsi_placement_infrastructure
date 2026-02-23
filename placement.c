
// placement.c
#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    int x;
    int y;
} TL;  

typedef struct node node;
typedef struct net net;

typedef struct net{
   int id;
   char* name;
   list *nodes;
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
    list* nlist; // stores node* entries
    list* tlist; // stores net* entries
} place;

// trim helpers: trim leading and trailing spaces in-place
static void trim_inplace(char *s) {
    if (!s) return;
    // trim leading
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);

    // trim trailing
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

// ------------ place_init ------------
place* place_init(int rownum, size_t list_size){
    place* p = malloc(sizeof(place));
    if (!p) {
        fprintf(stderr, "Memory allocation failed for place!\n");
        return NULL;
    }

    p->row_num = rownum;
    p->nlist = initial_list(list_size, sizeof(node*));
    if (!p->nlist) {
        fprintf(stderr, "initial_list failed for nlist\n");
        free(p);
        return NULL;
    }
    p->tlist = initial_list(list_size, sizeof(net*));
    if (!p->tlist) {
        fprintf(stderr, "initial_list failed for tlist\n");
        // cleanup
        free(p->nlist->data);
        free(p->nlist);
        free(p);
        return NULL;
    }

    return p;
}

// ------------ reader ------------
void reader(place* p, const char* filename) {
    if (!p) {
        fprintf(stderr, "reader: place pointer is NULL\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return;
    }

    char line[512];
    int net_id = 0;
    int node_id = 0;
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

        trim_inplace(net_name);
        char *net_name_dup = strdup(net_name);
        if (!net_name_dup) {
            fprintf(stderr, "strdup failed for net_name\n");
            continue;
        }

        net* nt = malloc(sizeof(net));
        if (!nt) {
            fprintf(stderr, "malloc failed for net\n");
            free(net_name_dup);
            continue;
        }
        nt->id = net_id++;
        nt->name = net_name_dup;
        nt->source_terminal = NULL;
        nt->nodes = initial_list(10, sizeof(node*));
        if (!nt->nodes) {
            fprintf(stderr, "initial_list failed for nt->nodes\n");
            free(nt->name);
            free(nt);
            continue;
        }

        char* rest = strtok(NULL, ":");
        if (!rest) {
            // add net to tlist anyway (empty net)
            add_element(p->tlist, &nt);
            continue;
        }

        // tokenize the rest by ',' (tokens are node references)
        char* token = strtok(rest, ",");
        while (token) {
            // trim leading/trailing spaces
            trim_inplace(token);
            if (token[0] == '\0') {
                token = strtok(NULL, ",");
                continue;
            }

            char nodename[64];
            char pinname[64] = "";
            if (strchr(token, '.')) {
                // format like NAME.PIN
                if (sscanf(token, "%63[^.].%63s", nodename, pinname) != 1) {
                    // if sscanf failed to parse nodename, skip
                    token = strtok(NULL, ",");
                    continue;
                }
                trim_inplace(nodename);
            } else {
                // no dot -> whole token is nodename
                strncpy(nodename, token, sizeof(nodename) - 1);
                nodename[sizeof(nodename) - 1] = '\0';
                trim_inplace(nodename);
            }

            node* found = NULL;
            // search existing nodes in p->nlist
            for (int i = 0; i < p->nlist->index; i++) {
                void* elem = get_element(p->nlist, i);
                if (!elem) continue;
                node* stored_ptr = *(node**)elem; // deref to get node*
                if (!stored_ptr || !stored_ptr->node_name) continue;
                if (strcmp(stored_ptr->node_name, nodename) == 0) {
                    found = stored_ptr;
                    break;
                }
            }

            if (!found) {
                node* newn = malloc(sizeof(node));
                if (!newn) {
                    fprintf(stderr, "malloc failed for node\n");
                    token = strtok(NULL, ",");
                    continue;
                }
                newn->id = node_id++;
                newn->node_name = strdup(nodename);
                newn->cell_name = strdup(nodename);
                newn->net = nt;
                newn->row_num = 0;
                newn->point.x = newn->point.y = 0;
                newn->width = newn->height = 0;

                // add pointer to nlist (list stores node*)
                add_element(p->nlist, &newn);

                found = newn;
            }

            // add pointer to nt->nodes (list stores node*)
            add_element(nt->nodes, &found);

            token = strtok(NULL, ",");
        }

        // add pointer to tlist (list stores net*)
        add_element(p->tlist, &nt);
    }

    fclose(file);
}

//---------main--------------
int main(void){
    place* p = place_init(4, 100);
    if (!p) {
        fprintf(stderr, "place_init failed\n");
        return 1;
    }

    printf("test\n");
    reader(p, "netlist.txt");

    printf("== Nets ==\n");
    for (int i = 0; i < p->tlist->index; i++) {
        void* elt = get_element(p->tlist, i);
        if (!elt) {
            printf("tlist element %d is NULL\n", i);
            continue;
        }
        net* nt = *(net**) elt;
        if (!nt) {
            printf("tlist[%d] net pointer NULL\n", i);
            continue;
        }
        printf("%s: ", nt->name ? nt->name : "<noname>");
        for (int j = 0; j < nt->nodes->index; j++) {
            void* nelt = get_element(nt->nodes, j);
            if (!nelt) continue;
            node* np = *(node**) nelt;
            if (!np || !np->node_name) continue;
            printf("%s ", np->node_name);
        }
        printf("\n");
    }

    printf("== Nodes ==\n");
    for (int i = 0; i < p->nlist->index; i++) {
        void* elt = get_element(p->nlist, i);
        if (!elt) {
            printf("nlist element %d is NULL\n", i);
            continue;
        }
        node* nd = *(node**) elt;
        if (!nd) {
            printf("nlist[%d] node pointer NULL\n", i);
            continue;
        }
        printf("%s\n", nd->node_name ? nd->node_name : "<noname>");
    }

    // NOTE: for brevity I don't free all allocated memory here.
    // In a production program you'd free nets, nodes, their strings and lists.

    return 0;
}