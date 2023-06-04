# Implementing a stack using a simple array

Table of contents

- [The stack as an abstract data structure](#the-stack-as-an-abstract-data-structure)
- [Now yes, on implementation details](#now-yes-on-implementation-details)
  1. [Create a stack](#1-create-a-stack)
  2. [Destroy a stack](#2-destroy-the-stack)
  3. [Check if the stack is full](#3-check-if-the-stack-is-full)
  4. [Put an element at the top of the stack](#4-put-an-element-at-the-top-of-the-stack)
  5. [Check if the stack is empty](#5-check-if-the-stack-is-empty)
  6. [remove the element at the top of the stack](#6-remove-the-element-at-the-top-of-the-stack)
  7. [Peek the element at the top of the stack but without removing it](#7-peek-the-element-at-the-top-of-the-stack-but-without-removing-it)
- [Creating some tests](#creating-some-tests)

## The stack as an abstract data structure

Thinking abstract, it acts as a container that preserves the order of elements in the way they were added.

To remove an element, one can only take the last element that was placed there, which is known as the element at the top of the stack.

The fundamental operations are typically referred to as **Push**, which adds an element to the stack, and **Pop**, which retrieves the last element.

The order in which elements are placed into or removed from the stack is commonly known as "**last in, first out**" or **LIFO**.

To illustrate the idea:

```sh
# 1. the stack is empty
[]

# 1. I want to put X in the stack

[ X ]

# 2. I want to put Y in the stack

[ X , Y ]

# 3. I want to put Z in the stack

[ X , Y , Z ]

# 4. Now I want to remove an element

[ X , Y ] # --> and you got Z because it was the latest element placed in there.
```

Apart from the fundamental operations, there are other operations that can be performed on a stack, commonly referred to as non-essential operations.

One such operation is called **Peek**, which allows us to observe the last element added to the stack without removing it. Although this behavior can be achieved by temporarily removing the element and subsequently adding it back to the top of the stack, it is not considered essential.

Similarly, as other examples of non essential operations, we can also check whether the stack is empty or full to prevent executing operations that would be meaningless in an abstract sense or potentially cause system crashes in a real implementation.

## Now yes, on implementation details

In this example, we will implement the stack using a simple array.

Note that resizing our underlying array at runtime will be beyond the scope of the current example.

For our implementation, we will support the following operations:

1. Create a stack
2. Destroy the stack
3. Put an element at the top of the stack
4. Remove the element at the top of the stack
5. Peek the element at the top of the stack but without removing it
6. Check if the stack is full
7. Check if the stack is empty

To start, let's create a _headers_ file that will serve as our interface:

```c
// stack.h
#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    int32_t* collection;
    uint32_t capacity;
    uint32_t size;
} Stack;

Stack* CreateNewStack(uint32_t capacity);
void DestroyStack(Stack** stackp);
bool Is_Empty(Stack* stack);
bool Is_Full(Stack* stack);
bool Push(Stack* stack, int32_t item);
bool Pop(Stack* stack, int32_t* poppedItem);
```

At the top, we include all the necessary libraries that we will need.

Then we are defining the struct that will act as our stack. Inside this struct, we have three components:

- `collection`, which is a pointer to a place in memory that will be allocated for the underlying array.
- `capacity`, which will store a unsigned integer that indicates the maximum number of elements that our array can hold.
- `size`, which will keep track of the actual number of elements that the stack contains.

Finally, at the bottom, we present the function signatures for the operations that will enable us to interact with the stack.

### 1. Create a stack

By having a function that takes care of the whole process of creating the stack, we can isolate the implementation details of the instantiation process.

This practice can help us debug some problems because the whole process is always done the same, and the code is in always in one place. Also, this may prevent ourselves of "manually" allocating memory, casting pointers, and doing the whole set up for each Stack, which adds more cognitive load to the code, and more places where we can introduce errors.

Lets see how we are implementing it, please find a little guidance on the comments:

```c
// We will receive an integer representing the number of elements we want to store
// The function will output a pointer where our Stack was placed in the heap
Stack* CreateNewStack(uint32_t capacity) {

    // We use the "malloc" function as our allocator and we ask
    // to reserve a place in memory with the size of a Stack
    Stack* stack = malloc(sizeof(Stack));

    // We check if the stack is a null pointer
    // this can occur when malloc fails to allocate
    if (stack == NULL) {
        // I this is the case, we shouldn't continue with the process
        return stack;
    }

    // Now we are asking for a place in memory to store the size of our
    // array type, but multiplied by our capacity
    // and we store the pointer in our "collection" field
    stack->collection = malloc(sizeof(int32_t) * capacity);

    // Again, if we fail we shouldn't continue
    if (stack->collection == NULL) {
        // so we free the previously allocated memory for the stack
        free(stack);
        // And we return just a null pointer that the caller should check
        return NULL;
    }

    // If everything turned out OK,
    // we keep track of the capacity of our stack
    stack->capacity = capacity;

    // And, because there is nothing stored yet
    // we set the size as 0
    stack->size = 0;

    // lastly, we return this pointer to the stack
    return stack;
}
```

The key here is to check if the memory was allocated correctly. This is what will might change the flow of our program and allow us to clean anything we might need.

### 2. Destroy the stack

Destroying the stack is to free al memory allocated for this stack and, depending on the chosen semantics, returning a null pointer so the caller can replace what is inside the pointer or delegating our function to make that process.

In our example, we will opt for the last approach.

Lets see the code, again, please find some guidance in the comments:

```c
// we will receive a pointer to the actual pointer that holds the address
// of the already allocated stack
void DestroyStack(Stack** stackp) {

    // to ease some things, we will de-reference the pointer
    // so we can start working with the stack.
    Stack* stack = *stackp;

    // If it happens that the caller sent us a reference
    // to a null pointer, we should return
    if (stack == NULL) {
        return;
    }

    // Now, we check if the pointer to the underlying array
    // actually points to something
    if (stack->collection != NULL) {
        // If so, we free that memory
        free(stack->collection);
    }
    // Then we free the memory reserved for the struct
    free(stack);

    // and lastly, we say
    // "that reference to a struct is now a reference to a null pointer"
    *stackp = NULL;
}
```

Is worth to notice that we are freeing the memory in the inverse order it was allocated:

1. first the underlying array
2. then the stack

If we did it in the same order it was allocated we would free the stack and loose the reference to where the array was placed.

### 3. Check if the stack is full

Before putting an element in the array, is a good idea to know if we are out of space. If we are not checking that, we might corrupt memory that may be used by other part of the program. And that wouldn't be nice.

The code is trivial, but it assume we are keeping track of the number of elements in the stack. The operation just checks if the size is less than the capacity, and return a boolean to the caller context.

```c
bool Is_Full(Stack* stack) {
    return stack->capacity == stack->size;
}
```

### 4. Put an element at the top of the stack

To put an element at the top of the stack is very straightforward.
For this example, we are assuming that the position of the last element is in the position `size - 1`.
Then, the rest is simple as the following snippet:

```c
// We receive a pointer to the stack we will be working with
// and also the element we want to store.
// The return value will be a boolean indicating if
// the operation succeeded or not
bool Push(Stack* stack, int32_t item) {
    // We check if there is something stored there
    if (stack == NULL) return false;
    // we check if there is some place to store the element
    if (Is_Full(stack)) return false;

    // then we assign the item on top the last element
    stack->collection[stack->size] = item;
    // And we update the current size of the stack
    stack->size += 1;

    // And we tell the caller "everything went ok :)"
    return true;
}
```

### 5. Check if the stack is empty

Similar to the Push operation, before popping an element, is a good practice to check if there is something to pop.

The implementation is pretty simple, we just need to check if the size is zero.

```c
bool Is_Empty(Stack* stack) {
    return  stack->size == 0;
}
```

### 6. Remove the element at the top of the stack

Popping an element is pretty similar to pushing one. For our implementation we made two decisions:

First, we wont be returning the element, we will return if the operation was successful or not. The value of the popped element will be stored in a pointer to an integer that the caller is passing us as the second argument in our function.

Second, we wont be "cleaning" or setting a "zero value" in the position of the element we have stored. We could do it, but, as we have used `malloc`, none of the original elements are zeros.
If we were to sore some sensitive data, we could do it, tho.

```c
// They give us the pointer to the stack and a place
// to reference the returned value
// we send back a boolean indicating if everything went ok
bool Pop(Stack* stack, int32_t* poppedItem) {
    // We check if the pointer is valid
    if (stack == NULL) return false;
    // we check if there is something to return
    if (Is_Empty(stack)) return false;
    // we decrease the size
    stack->size -= 1;
    // and we assign the value to the place in memory
    // that pointer is pointing to
    *poppedItem = stack->collection[stack->size];
    // and we are ready to go
    return true;
}
```

Remember: we are decreasing the size of the array before getting the item because, at this point, the size is one number bigger than the position of the last element.

### 7. Peek the element at the top of the stack but without removing it

Peeking is the same as popping, but we are not decreasing, nor cleaning anything

```c
// All the same operations as in the example above
bool Peek(Stack* stack, int32_t* peekedItem) {
    if (stack == NULL) return false;
    if (Is_Empty(stack)) return false;
    // But we subtract one in place
    *peekedItem = stack->collection[stack->size - 1];
    return true;
}
```

## Creating some tests

To test the happy path we will do the following:

1. we create a stack
2. we will try to Push all elements we can until it is at it's maximum capacity and check if, indeed, it full.
3. then we will pop all the elements while checking if the operation succeeded, and if the elements are in the right order.
4. we check if the stack is empty
5. we clean everything

```c
void _testHappyPath(uint32_t capacity) {
    Stack* stack = CreateNewStack(capacity);

    // we could also assert(stack != NULL) here

    int32_t i;
    // we loop and assign the index to each position
    for (i = 0; i < capacity; i++) {
        Push(stack, i);
    }
    // we check if it is full
    assert(Is_Full(stack) == true);

    int32_t j;
    bool result;

    // we loop again while popping each element
    for (i = 0; i < capacity; i++) {
        result = Pop(stack, &j);

        // we check if the value is the the expected one
        // using capacity - 1 - i, because we are going from
        // the biggest element to the lowest element
        assert(j == capacity - 1 - i);

        // and we also check if each operation succeeded
        assert(result == true);
    }
    // finally, we verify if the stack is empty
    assert(Is_Empty(stack) == true);

    // then se clean everything up
    _cleanup(stack);
}
```

Our cleanup function also can give us some hints on whether some auxiliary operations are OK

```c
void _cleanup(Stack* stack) {
    // we pass a reference to our stack pointer
    DestroyStack(&stack);

    // by the semantics we are using, we now can
    // check if the original pointer is pointing to NULL
    assert(stack == NULL);
}
```
