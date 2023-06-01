#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "dynamic_array.h"

void _cleanup(D_array* array) {
    //DestroyDynamicArray(&array);
    //assert(array == NULL);
}

void _testHappyPath(uint32_t capacity) {
    D_array* array = CreateDynamicArray(capacity);
    int32_t i = 0;
    while ((array->size + 1) * 2 < array->capacity) {
        Push(array, i);
        i++;
    }
    assert(array->capacity == capacity);
    Push(array, i);
    assert(array->capacity == capacity * 2);
    int32_t* returnValue = (int32_t*)malloc(sizeof(int32_t));

    while (!IsEmpty(array)) {
        Pop(array, returnValue);
        assert(*returnValue == (int32_t)array->size);
    }
    assert(IsEmpty(array) == true);
    _cleanup(array);
}


void TestHappyPath() {
    uint32_t testCases[4] = { 10,100,1000,10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testHappyPath(testCases[i]);
    }
}

int main(void) {
    TestHappyPath();
    return 0;
}