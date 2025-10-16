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
void set_element(list* l, int elidx, void* value){
      if (elidx < 0 || (size_t)elidx >= l->index) {
        fprintf(stderr, "Index out of range!\n");
        return;
    }

    void* dest = (char*)l->data + (elidx * l->elsize);

    memcpy(dest, value, l->elsize);
}

void swap(list* l, int idx1, int idx2){
    if(idx1 < 0 || (size_t)idx1 >= l->index || idx2 < 0 || (size_t)idx2 >= l->index){
        fprintf(stderr, "Index out of range!\n");
        return;
    }
    void* ptr1 = l->data + idx1 * l->elsize;
    void* ptr2 = l->data + idx2 * l->elsize;
    void* tmp = malloc(l->elsize);
    memcpy(tmp,ptr1,l->elsize);
    memcpy(ptr1,ptr2,l->elsize);
    memcpy(ptr2,tmp,l->elsize);
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
    Person p4 = {"sahel", 21};
    set_element(&people,1,&p4);
    Person* p = (Person*)get_element(&people, 1);
    printf("set res %d: %s (%d)\n", 1, p->name, p->age);


    swap(&people,0,1);
     for (int i = 0; i < people.index; i++) {
        Person* p = (Person*)get_element(&people, i);
        if (p) {
            printf("Person %d: %s (%d)\n", i, p->name, p->age);
            free(p);
        }
    }
    delete_element(&people,1);
      for (int i = 0; i < people.index; i++) {
        Person* p = (Person*)get_element(&people, i);
        if (p) {
            printf("Person %d: %s (%d)\n", i, p->name, p->age);
            free(p);
        }
    }

    free(people.data);
    free(copy_people.data);
    return 0;
}
