#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Stack_T {
    int* collection;
    uint32_t capacity;
    uint32_t size;
} Stack;


Stack* CreateNewStack(uint32_t capacity) {
    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        return stack;
    }

    stack->collection = malloc(sizeof(int) * capacity);
    if (stack->collection == NULL) {
        free(stack);
        return NULL;
    }
    stack->capacity = capacity;
    stack->size = 0;
    return stack;
}

void DestroyStack(Stack** stackp) {
    Stack* stack = *stackp;
    if (stack == NULL) {
        return;
    }
    if (stack->collection != NULL) {
        free(stack->collection);
    }
    free(stack);
    *stackp = NULL;
}

bool Is_Empty(Stack* stack) {
    return  stack->size == 0;
}

bool Is_Full(Stack* stack) {
    return stack->capacity == stack->size;
}

bool Push(Stack* stack, int item) {
    if (stack == NULL) return false;
    if (Is_Full(stack)) return false;
    stack->collection[stack->size] = item;
    stack->size += 1;
    return true;
}

bool Pop(Stack* stack, int* poppedItem) {
    if (stack == NULL) return false;
    if (Is_Empty(stack)) return false;
    stack->size -= 1;
    *poppedItem = stack->collection[stack->size];
    return true;
}

bool Peek(Stack* stack, int* peekedItem) {
    if (stack == NULL) return false;
    if (Is_Empty(stack)) return false;
    *peekedItem = stack->collection[stack->size - 1];
    return true;
}