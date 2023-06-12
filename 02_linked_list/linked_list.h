#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Node_T
{
    int32_t data;
    struct Node_T* next;
} Node;

Node* CreateNewNode();

void InsertToHead(Node** head, uint32_t number);

int InsertAtNthPosition(Node** head, int32_t number, uint32_t position);

int RemoveFromNthPosition(Node** head, uint32_t position);

void PrintAll(Node* head);

