#include "linked_list.h"

Node* Node_create(void *data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void Node_append(Node** head, void *data) {
    Node* newNode = Node_create(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void Node_remove(Node** head, void *data) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* temp = *head;
    Node* prev = NULL;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->data == data) {
        *head = temp->next; // Changed head
        free(temp); // Free old head
        return;
    }

    // Search for the key to be deleted
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) {
        printf("Element not found in the list\n");
        return;
    }

    // Unlink the node from linked list
    prev->next = temp->next;
    free(temp); // Free memory
}

void List_print(Node* head, void (*print)(const void*)) {
    Node* temp = head;
    while (temp != NULL) {
        print(temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}
