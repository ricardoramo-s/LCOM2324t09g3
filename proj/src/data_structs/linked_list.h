#pragma once

#include <stdio.h>
#include <stdlib.h>

// Definition of a node in the linked list
typedef struct Node {
    void *data;
    struct Node* next;
} Node;

Node* Node_create(void *data);

void Node_append(Node** head, void *data);

void Node_remove(Node** head, void *data);

void List_print(Node* head, void (*print)(const void*));
