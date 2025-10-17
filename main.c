#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
    char name[20];
    int age;
} Person;

int main() {
    list* people = initial_list(4, sizeof(Person));

    Person p1 = {"Alice", 25};
    Person p2 = {"Bob", 30};
    Person p3 = {"Charlie", 28};

    add_element(people, &p1);
    add_element(people, &p2);
    add_element(people, &p3);

    for (int i = 0; i < people->index; i++) {
        Person* p = get_element(people, i);
        printf("Person %d: %s (%d)\n", i, p->name, p->age);
    }

    list* copy_people = copy_list(people);

    for (int i = 0; i < copy_people->index; i++) {
        Person* p = get_element(copy_people, i);
        printf("copy Person %d: %s (%d)\n", i, p->name, p->age);
    }

    Person p4 = {"Sahel", 21};
    set_element(people, 1, &p4);

    for (int i = 0; i < people->index; i++) {
        Person* p = get_element(people, i);
        printf("Person after set %d: %s (%d)\n", i, p->name, p->age);
    }
    swap(people, 0, 1);

    for (int i = 0; i < people->index; i++) {
        Person* p = get_element(people, i);
        printf("Person after swap %d: %s (%d)\n", i, p->name, p->age);
    }
    delete_element(people, 1);

    for (int i = 0; i < people->index; i++) {
        Person* p = get_element(people, i);
        printf("Person after delete %d: %s (%d)\n", i, p->name, p->age);
    }

    free(people->data);
    free(people);
    free(copy_people->data);
    free(copy_people);

    return 0;
}
