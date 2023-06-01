#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct Stack_T {
    int* collection;
    uint32_t capacity;
    uint32_t size;
} Stack;

Stack* CreateNewStack(uint32_t capacity);
void DestroyStack(Stack** stackp);
bool Is_Empty(Stack* stack);
bool Is_Full(Stack* stack);
bool Push(Stack* stack, int item);
bool Pop(Stack* stack, int* poppedItem);
bool Peek(Stack* stack, int* peekedItem);

