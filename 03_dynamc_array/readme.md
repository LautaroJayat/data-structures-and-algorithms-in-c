# Implementing a dynamic array

Dynamic arrays are similar to static arrays in the sense that they allow easy reading, insertion, or modification of elements by referring to them using their index within the array. However, unlike static arrays, dynamic arrays implement a mechanism to dynamically increase or decrease their memory as needed.

**Table of contents**

- [Basic Operations](#basic-operations)
  1. [Creating a dynamic array](#creating-a-dynamic-array)
  2. [Destroying a dynamic array](#destroying-a-dynamic-array)
  3. [Pushing to a dynamic array](#pushing-to-a-dynamic-array)
  4. [Popping an element from a dynamic array](#popping-an-element-from-a-dynamic-array)
- [Source code of this example](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/03_dynamc_array)

## Basic operations

To implement a basic dynamic array, we need to maintain information about the maximum reserved space, the number of elements already stored, and a resizing strategy for the underlying data structure.

A struct that includes a pointer to an array, along with two integers representing the capacity and number of elements, is sufficient to start. The following snippet shows the aforementioned structure and the function signatures for the essential operations we need to support.

```c
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

```

In this example, we will focus on supporting the **Push** and **Pop**. operations for the dynamic array. Directly accessing data using `array->collection[i]`.
will be allowed, although it is considered unsafe. It's worth noting that more advanced implementations of dynamic arrays may include additional functions such as `getSlice`, `filter`, `map`, `getElement`. However, these functionalities are beyond the scope of this article.

### Creating a dynamic array

To create a dynamic array, we will utilize a constructor function that encapsulates the underlying implementation, ensuring safety and consistency across instantiations.

The constructor function will receive the size of the array as a parameter. It will attempt to create an array and check if the creation is successful. Subsequently, the appropriate size will be allocated for the remaining struct components. Finally, we will set up the struct to ensure it is ready for use. The implementation is as follows:

```c
// The capacity will be given by the caller context
D_array* CreateDynamicArray(uint32_t capacity) {

    // We will start trying to allocate thearray first
    int32_t* collection = (int32_t*)calloc(capacity, sizeof(int32_t));

    // If the step above couldn't be done, we will not procceed
    if (collection == NULL) {
        return NULL;
    }

    // If the array was allocated, we continue with the struct
    D_array* array = (D_array*)malloc(sizeof(D_array));

    // Same as the array, we check if everything went ok
    if (array == NULL) {
        // in case it didn't we free the collection and return
        free(collection);
        return NULL;
    }

    // If everything went ok, we continue assigning each field
    array->collection = collection;
    array->size = 0;
    array->capacity = capacity;
    return array;
}
```

### Destroying a dynamic array

In order to destroy the array, we need to free both the underlying collection and the struct. To maintain a clean and tidy process, we will begin by freeing the space that was reserved for the underlying collection. If we were to free the struct first, there is a possibility of losing the reference to the array and causing a memory leak.

In the following snippet, you can find a possible implementation:

```c
// We pass a pointer to a pointer to the dynamic array
bool DestroyDynamicArray(D_array** array) {
    // as we want to free the original pointer, we dereference it
    D_array* arr = *array;

    // if it was null, we can return
    if (arr == NULL) {
        return false;
    }

    // if the underlying collection exists we just free it
    if (arr->collection != NULL) {
        free(arr->collection);
    }

    // finally we free the array
    free(arr);
    return true;
}
```

Since arguments are passed by value, we don't accept just a pointer to a dynamic array; instead, we accept a pointer to a pointer to that array. If we were to only receive a pointer to a dynamic array, the operation `free(arr)` would not affect the original pointer itself. By accepting a pointer to a pointer, we ensure that we can modify the original pointer and free the memory correctly.

### Pushing to a dynamic array

To push to a dynamic array, we first confirm that the struct and the collection exist. Next, we check if a resize is necessary, perform the resize if needed, and finally update the array and counters accordingly. Here's an example implementation:

```c
// we receive the array and the value we want to store
bool Push(D_array* array, int32_t val) {
    // we check if the struct and the underlying collecition exists
    if (array == NULL || array->collection == NULL) {
        return false;
    }
    // Check if we need to resize
    if (_needsToResize(array)) {
        // resize if needed
        bool ok = _resize(array);
        if (!ok) return false;
    }
    // At this point we can update the underlying collection
    array->collection[array->size] = val;
    // and update the counter
    array->size++;
    return true;
}
```

The function that determines if a resize is needed for our array is quite simple. It checks if the dynamic array is not a null pointer and compares the size to the capacity. In our case, we want to resize the array when we reach half of the capacity.

```c
// we pass the pointer to the struct
bool _needsToResize(D_array* array) {
    // we check if it is a null pointer
    if (array == NULL) {
        return false;
    }
    // check if the double of the size + 1 is still less than the capacity
    return  (array->size + 1) * 2 < array->capacity;
}
```

If we need to resize the underlying array, we can utilize the `realloc` function, which takes two arguments: the original memory space and the desired final memory size. With `realloc`, a new memory space will be reserved, the old values will be copied into the new section, and the original space will be freed.

To determine the growth factor, we will simply double the space each time we reach half of the capacity.

```c
// We accept a pointer to the array
bool _resize(D_array* array) {
    // we check everything is in place
    if (array == NULL || array->collection == NULL) {
        return false;
    }

    // We calculate the new capacity
    uint32_t newCapacity = 2 * array->capacity;

    // and then reasign which our collection is
    array->collection = (int32_t*)realloc(array->collection, sizeof(int32_t) * newCapacity);

    // after this, we check everything went ok
    if (array->collection == NULL) {
        return false;
    }

    // and we update the new capacity
    array->capacity = newCapacity;

    return true;
}
```

### Popping an element from a dynamic array

To pop an element from our dynamic array, we will accept a pointer to the array and a pointer to an integer. If the operation is successful, we will update the value that the integer pointer points to. Then, we will update the size field and return a boolean indicating the success of the operation.

```c

// we accept a pointer to an int
bool Pop(D_array* array, int32_t* returnValue) {
    // check if we can continue
    if (array == NULL || array->collection == NULL) {
        return true;
    }
    // we decrease out poinert
    array->size--;
    // And we assign the value so the caller context can make use of it
    *returnValue = array->collection[array->size];;
    return true;
}
```
