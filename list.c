#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list{
    size_t elsize; //size of each element
    size_t size; // size of list
    int index; // where in list we are?
    void* data; // where is list in mem?
} list;

void* get_element(list* l,int elidx) {
    if (elidx < 0 || (size_t)elidx >= l->size) {
        fprintf(stderr, "Index out of range!\n");
        return NULL;
    }

    void* sour = l->data + (elidx * l->elsize);
    void* dest = malloc(l->elsize);
    if (!dest) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    memcpy(dest, sour, l->elsize);
    return dest;
}
int main(){

    list l;
    l.elsize = sizeof(int);
    l.size = 4;
    l.index = 0;
    l.data = malloc(l.size * l.elsize);


    int nums[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        memcpy(l.data + (l.index * l.elsize), &nums[i], l.elsize);
        l.index++;
    }

      // test get_element
    int *value = (int*)get_element(&l, 2);
    if (value) {
        printf("Element at index 2 = %d\n", *value);
        free(value);
    }

    free(l.data);

    return 0;
}