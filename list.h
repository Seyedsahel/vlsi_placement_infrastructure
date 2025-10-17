#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct list {
    size_t elsize;
    size_t size;
    int index;
    void* data;
} list;


list* initial_list(size_t lsize, size_t elsize);
void add_element(list* l, void* element);
void* get_element(list* l, int elidx);
bool set_element(list* l, int elidx, void* value);
void swap(list* l, int idx1, int idx2);
void delete_element(list* l, int idx);
void* copy_element(list* l, int elidx);
list* copy_list(list* l);

#endif 
