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

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; 

        if (strlen(line) == 0 || line[0] == '#') 
            continue; 

        if (strncmp(line, "NODE", 4) == 0) {
            node n;
            n.node_name = malloc(32);
            n.cell_name = malloc(32);
            n.net = NULL;

            if (sscanf(line, "NODE %31s %31s %f %f",
                       n.node_name, n.cell_name, &n.width, &n.height) != 4) {
                fprintf(stderr, "Invalid NODE line: %s\n", line);
                free(n.node_name);
                free(n.cell_name);
                continue;
            }

            n.id = p->nlist->index;
            n.row_num = 0;
            n.point.x = 0;
            n.point.y = 0;

            add_element(p->nlist, &n);
        }
        else if (strncmp(line, "NET", 3) == 0) {
            net t;
            t.name = malloc(32);
            t.source_terminal = NULL;

            char netname[32];
            int count;
            char* ptr = strdup(line); 
            if (!ptr) continue;

            char* token = strtok(ptr, " ");
            if (!token || strcmp(token, "NET") != 0) { free(ptr); continue; }

            token = strtok(NULL, " ");
            if (!token) { free(ptr); continue; }
            strcpy(netname, token);
            strcpy(t.name, netname);

            token = strtok(NULL, " ");
            if (!token) { free(ptr); continue; }
            count = atoi(token);

            t.id = p->tlist->index;
            t.nodes = *initial_list(count, sizeof(node*));

            int node_added = 0;
            while ((token = strtok(NULL, " ")) != NULL) {
                for (int i = 0; i < p->nlist->index; i++) {
                    node* n = get_element(p->nlist, i);
                    if (strcmp(n->node_name, token) == 0) {
                        add_element(&t.nodes, &n);
                        n->net = &t;
                        node_added++;
                        break;
                    }
                }
            }

            if (node_added != count) {
                fprintf(stderr, "Warning: not all nodes found for NET %s\n", t.name);
            }

            add_element(p->tlist, &t);
            free(ptr);
        }
    }

    fclose(file);
}

// =================== SIMULATED ANNEALING ==================

// ---Wirelength: HPWL ---
float WL_cal(place* p) {
    float total = 0.0f;

    for (int i = 0; i < p->tlist->index; i++) {
        net* nt = get_element(p->tlist, i);
        if (!nt || nt->nodes.index == 0) continue;

        float minx = 1e9, miny = 1e9, maxx = -1e9, maxy = -1e9;

        for (int j = 0; j < nt->nodes.index; j++) {
            node** ndptr = get_element(&nt->nodes, j);
            if (!ndptr || !(*ndptr)) continue;
            node* nd = *ndptr;

            if (nd->point.x < minx) minx = nd->point.x;
            if (nd->point.x > maxx) maxx = nd->point.x;
            if (nd->point.y < miny) miny = nd->point.y;
            if (nd->point.y > maxy) maxy = nd->point.y;
        }

        total += (maxx - minx) + (maxy - miny);
    }

    return total;
}

// --- تعویض مکان دو نود ---
void swap_nodes(node* a, node* b) {
    TL temp = a->point;
    a->point = b->point;
    b->point = temp;

    int tmp_row = a->row_num;
    a->row_num = b->row_num;
    b->row_num = tmp_row;
}

// --- ایجاد موقعیت اولیه تصادفی ---
void random_init(place* p, int maxX, int row_height) {
    srand(time(NULL));
    for (int i = 0; i < p->nlist->index; i++) {
        node* n = get_element(p->nlist, i);
        n->row_num = rand() % p->row_num;
        n->point.x = rand() % maxX;
        n->point.y = n->row_num * row_height;
    }
}

// --- الگوریتم اصلی Simulated Annealing ---
void sa(place* p) {
    if (!p || p->nlist->index == 0) return;

    const float T_init = 1000.0f;   // دمای اولیه
    const float T_min = 1.0f;       // حداقل دما
    const float alpha = 0.90f;      // نرخ سرد شدن
    const int moves_per_temp = 100; // تعداد حرکات در هر دما

    random_init(p, 1000, 50);       // جایگذاری اولیه‌ی تصادفی

    float T = T_init;
    float curr_cost = WL_cal(p);
    float best_cost = curr_cost;
    list* best_nlist = copy_list(p->nlist);

    printf("Initial WL = %.2f\n", curr_cost);

    while (T > T_min) {
        for (int m = 0; m < moves_per_temp; m++) {
            int a = rand() % p->nlist->index;
            int b = rand() % p->nlist->index;
            if (a == b) continue;

            node* na = get_element(p->nlist, a);
            node* nb = get_element(p->nlist, b);

            swap_nodes(na, nb);

            float new_cost = WL_cal(p);
            float delta = new_cost - curr_cost;

            if (delta < 0 || (expf(-delta / T) > ((float)rand() / RAND_MAX))) {
                curr_cost = new_cost;
                if (curr_cost < best_cost) {
                    best_cost = curr_cost;
                    free(best_nlist);
                    best_nlist = copy_list(p->nlist);
                }
            } else {
                swap_nodes(na, nb); // برگشت در صورت رد
            }
        }

        printf("T = %.2f   Current WL = %.2f   Best = %.2f\n", T, curr_cost, best_cost);
        T *= alpha;
    }

    free(p->nlist);
    p->nlist = best_nlist;

    printf("Final Best Wirelength = %.2f\n", best_cost);
}
