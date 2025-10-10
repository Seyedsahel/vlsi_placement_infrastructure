#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* data;              
    struct Node *next;   
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    size_t size;
} List;

// func for create a list
List* list_create(void) {
    List *l = malloc(sizeof(List));
    if (!l) return NULL;
    l->head = l->tail = NULL;
    l->size = 0;
    return l;
}

// Func to create a node 
struct Node* createNode(void* data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
