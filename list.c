#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

list* initial_list(size_t lsize, size_t elsize) {
    list* l = malloc(sizeof(list));
    if (!l) {
        fprintf(stderr, "Memory allocation failed for list!\n");
        return NULL;
    }

    l->elsize = elsize;
    l->size = lsize;
    l->index = 0;
    l->data = malloc(l->size * l->elsize);
    if (!l->data) {
        fprintf(stderr, "Memory allocation failed for data!\n");
        free(l);
        return NULL;
    }

    return l;
}

void* get_element(list* l, int elidx) {
    if (elidx < 0 || (size_t)elidx >= l->index) {
        fprintf(stderr, "Index out of range!\n");
        return NULL;
    }

    return (char*)l->data + (elidx * l->elsize);
}

void add_element(list* l, void* element) {
    if (l->index == l->size) {
        size_t new_size = (l->size == 0) ? 1 : l->size * 2;
        void* new_data = realloc(l->data, new_size * l->elsize);
        if (!new_data) {
            fprintf(stderr, "Memory allocation failed!\n");
            return;
        }
        l->data = new_data;
        l->size = new_size;
    }

    memcpy((char*)l->data + (l->index * l->elsize), element, l->elsize);
    l->index++;
}

bool set_element(list* l, int elidx, void* value) {
    if (elidx < 0 || (size_t)elidx >= l->index) {
        fprintf(stderr, "Index out of range!\n");
        return false;
    }

    void* dest = (char*)l->data + (elidx * l->elsize);
    return memcpy(dest, value, l->elsize) != NULL;
}

void swap(list* l, int idx1, int idx2) {
    if (idx1 < 0 || (size_t)idx1 >= l->index || idx2 < 0 || (size_t)idx2 >= l->index) {
        fprintf(stderr, "Index out of range!\n");
        return;
    }

    void* ptr1 = (char*)l->data + idx1 * l->elsize;
    void* ptr2 = (char*)l->data + idx2 * l->elsize;
    void* tmp = malloc(l->elsize);

    if (!tmp) {
        fprintf(stderr, "Memory allocation failed for swap!\n");
        return;
    }

    memcpy(tmp, ptr1, l->elsize);
    memcpy(ptr1, ptr2, l->elsize);
    memcpy(ptr2, tmp, l->elsize);
    free(tmp);
}

void delete_element(list* l, int idx) {
    if (idx < 0 || (size_t)idx >= l->index) {
        fprintf(stderr, "Index out of range!\n");
        return;
    }

    char* ptr = (char*)l->data + idx * l->elsize;
    size_t bytes_to_move = (l->index - idx - 1) * l->elsize;

    if (bytes_to_move > 0) {
        memmove(ptr, ptr + l->elsize, bytes_to_move);
    }

    l->index--;
}

void* copy_element(list* l, int elidx) {
    if (elidx < 0 || (size_t)elidx >= l->index) {
        fprintf(stderr, "Index out of range!\n");
        return NULL;
    }

    void* sour = (char*)l->data + (elidx * l->elsize);
    void* dest = malloc(l->elsize);
    if (!dest) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    memcpy(dest, sour, l->elsize);
    return dest;
}

list* copy_list(list* l) {
    list* new_list = initial_list(l->size, l->elsize);
    if (!new_list) return NULL;

    new_list->index = l->index;
    memcpy(new_list->data, l->data, l->size * l->elsize);
    return new_list;
}
