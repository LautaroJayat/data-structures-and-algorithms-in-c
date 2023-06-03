# Implementing a stack using a simple array

## The stack as an abstract data structure

Thinking abstract, it acts as a container that preserves the order of elements in the way they were added.

To remove an element, one can only take the last element that was placed there, which is known as the element at the top of the stack.

The fundamental operations are typically referred to as **Push**, which adds an element to the stack, and **Pop**, which retrieves the last element.

The order in which elements are placed into or removed from the stack is commonly known as "**last in, first out**" or **LIFO**.

To ilustrate the idea:

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

Similarly, as other examples of non essential operantios, we can also check whether the stack is empty or full to prevent executing operations that would be meaningless in an abstract sense or potentially cause system crashes in a real implementation.

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

- `collection`, which is a pointer to a place in memmory that will be alocated for the underlying array.
- `capacity`, which will store a unsigned integer that indicates the maximum number of elements that our array can hold.
- `size`, which will keep track of the actual number of elements that the stack contains.

Finally, at the bottom, we present the function signatures for the operations that will enable us to interact with the stack.

### 1. Create a stack

### 2. Destroy the stack

### 3. Put an element at the top of the stack

### 4. Remove the element at the top of the stack

### 5. Peek the element at the top of the stack but without removing it

### 6. Check if the stack is full

### 7. Check if the stack is empty

## Creating some tests
