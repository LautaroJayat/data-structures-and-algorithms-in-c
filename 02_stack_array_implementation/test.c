#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

void _cleanup(Stack* stack) {
    DestroyStack(&stack);
    assert(stack == NULL);
}

void _testCreateStack(uint32_t capacity) {
    Stack* stack = CreateNewStack(capacity);
    assert(stack != NULL);
    assert(stack->size == 0);
    assert(stack->capacity == capacity);
    assert(stack->collection != NULL);
    _cleanup(stack);
}


void TestCreateStack() {
    int32_t testCases[4] = { 10,100,1000,10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testCreateStack(testCases[i]);
    }
}

void _testHappyPath(uint32_t capacity) {
    Stack* stack = CreateNewStack(capacity);
    int32_t i;
    for (i = 0; i < capacity; i++) {
        Push(stack, i);
    }
    assert(Is_Full(stack) == true);
    int32_t j;
    bool result;
    for (i = 0; i < capacity; i++) {
        result = Pop(stack, &j);
        assert(j == capacity - 1 - i);
        assert(result == true);
    }
    assert(Is_Empty(stack) == true);
    _cleanup(stack);
}

void TestHappyPath() {
    int32_t testCases[4] = { 10,100,1000,10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testHappyPath(testCases[i]);
    }
}

void _testPoppingEmptyStack() {
    Stack* stack = CreateNewStack(1);
    int32_t i;
    bool result = Pop(stack, &i);
    assert(result == false);
    _cleanup(stack);
}

void _testAddingToFullStack() {
    Stack* stack = CreateNewStack(1);
    bool result = false;
    result = Push(stack, 1);
    assert(result == true);
    result = Push(stack, 1);
    assert(result == false);
    _cleanup(stack);
}


void TestEdgeCases() {
    _testAddingToFullStack();
    _testPoppingEmptyStack();
}

void main() {
    TestCreateStack();
    TestHappyPath();
    TestEdgeCases();
}