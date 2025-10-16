#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list{
    size_t elsize;
    size_t size;
    int index;
    void* data;
} list;

void* get_element(list* l,int elidx) {
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

list initial_list(size_t lsize,size_t elsize){
    list l;
    l.elsize = elsize;
    l.size = lsize;
    l.index = 0;
    l.data = malloc(l.size * l.elsize);
    if (!l.data) {
            fprintf(stderr, "Memory allocation failed!\n");
            
        }

    return l;
}
// void* copy_element(list* l, int index){}

list copy_list(list* l){
    list new_list = initial_list(l->size,l->elsize);
    new_list.index=l->index;
    memcpy(new_list.data,l->data,l->size*l->elsize);
    return new_list;

}
typedef struct {
    char name[20];
    int age;
} Person;

int main() {
    list people = initial_list(4,sizeof(Person));
    Person p1 = {"Alice", 25};
    Person p2 = {"Bob", 30};
    Person p3 = {"Charlie", 28};

    add_element(&people, &p1);
    add_element(&people, &p2);
    add_element(&people, &p3); 

    for (int i = 0; i < people.index; i++) {
        Person* p = (Person*)get_element(&people, i);
        if (p) {
            printf("Person %d: %s (%d)\n", i, p->name, p->age);
            free(p);
        }
    }

    list copy_people = copy_list(&people);

    for (int i = 0; i < people.index; i++) {
    Person* p = (Person*)get_element(&copy_people, i);
        if (p) {
            printf("Person_copy %d: %s (%d)\n", i, p->name, p->age);
            free(p);
        }
    }




    free(people.data);
    return 0;
}
