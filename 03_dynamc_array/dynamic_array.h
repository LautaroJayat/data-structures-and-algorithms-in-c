#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int32_t* collection;
    u_int32_t capacity;
    u_int32_t size;
} D_array;

D_array* CreateDynamicArray(uint32_t capacity);
bool DestroyDynamicArray(D_array**);
bool Push(D_array* array, int32_t val);
bool Pop(D_array* array, int32_t* returnValue);
bool IsEmpty(D_array* array);

