#include <stdint.h>
#include <stdbool.h>
#include "stack.h"

Stack* CreateNewStack(uint32_t capacity) {
    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        return stack;
    }

    stack->collection = malloc(sizeof(int32_t) * capacity);
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

bool Push(Stack* stack, int32_t item) {
    if (stack == NULL) return false;
    if (Is_Full(stack)) return false;
    stack->collection[stack->size] = item;
    stack->size += 1;
    return true;
}

bool Pop(Stack* stack, int32_t* poppedItem) {
    if (stack == NULL) return false;
    if (Is_Empty(stack)) return false;
    stack->size -= 1;
    *poppedItem = stack->collection[stack->size];
    return true;
}

bool Peek(Stack* stack, int32_t* peekedItem) {
    if (stack == NULL) return false;
    if (Is_Empty(stack)) return false;
    *peekedItem = stack->collection[stack->size - 1];
    return true;
}