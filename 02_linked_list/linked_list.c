#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "linked_list.h"

Node* CreateNewNode() {
    return (Node*)malloc(sizeof(Node));
}

void InsertToHead(Node** pointerToHead, uint32_t number)
{
    Node* newNode = CreateNewNode();
    newNode->data = number;
    newNode->next = *pointerToHead;
    *pointerToHead = newNode;
};

int InsertAtNthPosition(Node** head, int32_t number, uint32_t position) {
    if (head == NULL) {
        return 1;
    }
    if (position == 0) {
        InsertToHead(head, number);
        return 0;
    }

    int32_t i;
    Node* prevNode = *head;
    for (i = 1; i < position; i++) {
        prevNode = prevNode->next;
    }
    Node* newNode = CreateNewNode();
    newNode->data = number;
    newNode->next = prevNode->next;
    prevNode->next = newNode;
    return 0;
}

int RemoveFromNthPosition(Node** head, uint32_t position) {
    if (*head == NULL) {
        return 1;
    }

    Node* toDelete;
    if (position == 0) {
        toDelete = *head;
        *head = (*head)->next;
        free(toDelete);
        return 0;
    }

    Node* prevNode = *head;

    int32_t i;
    for (i = 0; i < position - 1; i++) {
        if (prevNode->next == NULL) {
            return 1;
        }
        prevNode = prevNode->next;
    }

    toDelete = prevNode->next;
    prevNode->next = toDelete->next;
    free(toDelete);
    return 0;
}

void PrintAll(Node* head) {
    Node* currentNode = head;
    printf("The current values of the linked list are: [ ");
    while (currentNode != NULL) {
        printf("%d ", currentNode->data);
        currentNode = currentNode->next;
    };
    printf("]\n");
}