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

//funk to add node at first of list
void list_add_front(List *list, Node *node){
    if (list->head == NULL) {
        list->head = list->tail = node;
        node->next = NULL;
    } else {
        node->next = list->head;
        list->head = node;
    }
    list->size++;

}
//func to add node at the end of the list
void list_add_back(List *list, Node *node){

    if (list->head == NULL) {
        list->head = list->tail = node;
        node->next = NULL;
    } else {
        list->tail->next = node;
        list->tail = node;
        node->next = NULL;
        
    }
    list->size++;
}
//func to add node at the end of the list
void list_insert_at(List *list, Node *node, size_t pos) {
    if (pos > list->size) {
        printf("Position out of range!\n");
        return;
    }

    if (pos == 0) {
        list_add_front(list, node);
        return;
    }

    if (pos == list->size) {
        list_add_back(list, node);
        return;
    }

    Node *current = list->head;
    for (size_t i = 0; i < pos - 1; i++) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;

    list->size++;
}
