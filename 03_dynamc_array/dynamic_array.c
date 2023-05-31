#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "dynamic_array.h"

D_array* CreateDynamicArray(uint32_t capacity) {
    int32_t* collection = (int32_t*)calloc(capacity, sizeof(int32_t));
    if (collection == NULL) {
        return NULL;
    }

    D_array* array = (D_array*)malloc(sizeof(D_array));
    if (array == NULL) {
        return NULL;
    }
    array->collection = collection;
    array->size = 0;
    array->capacity = capacity;
    return array;
}

bool DestroyDynamicArray(D_array** array) {
    D_array* arr = *array;
    if (arr == NULL) {
        return false;
    }
    if (arr->collection != NULL) {
        free(arr->collection);
    }
    free(arr);
    return true;
}

bool _needsToResize(D_array* array) {
    if (array == NULL) {
        return false;
    }
    return  (array->size + 1) * 2 >= array->capacity;
}

bool _resize(D_array* array) {
    if (array == NULL || array->collection == NULL) {
        return false;
    }
    uint32_t newCapacity = 2 * array->capacity;

    array->collection = (int32_t*)realloc(array->collection, sizeof(int32_t) * newCapacity);

    if (array->collection == NULL) {
        return false;
    }

    array->capacity = newCapacity;

    return true;
}

bool Push(D_array* array, int32_t val) {
    if (array == NULL || array->collection == NULL) {
        return false;
    }
    if (_needsToResize(array)) {
        bool ok = _resize(array);
        if (!ok) return false;
    }
    array->collection[array->size] = val;
    array->size++;
    return true;
}

bool IsEmpty(D_array* array) {
    if (array == NULL || array->collection == NULL) {
        return true;
    }

    return array->size == 0;
}

bool Pop(D_array* array, int32_t* returnValue) {
    if (IsEmpty(array)) {
        return false;
    }
    array->size--;
    int32_t x;
    *returnValue = array->collection[array->size];;
    return true;
}