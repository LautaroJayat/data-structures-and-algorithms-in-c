#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    int32_t* collection;
    int32_t capacity;
    int32_t size;
} Stack;

Stack* CreateNewStack(uint32_t capacity);
void DestroyStack(Stack** stackp);
bool Is_Empty(Stack* stack);
bool Is_Full(Stack* stack);
bool Push(Stack* stack, int32_t item);
bool Pop(Stack* stack, int32_t* poppedItem);
