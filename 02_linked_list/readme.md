# Implementing a linked list

Table of contents

- [The linked list as an abstract data structure](#the-linked-list-as-an-abstract-data-structure)
- [Implementing the data structure](#implementing-the-data-structure)
  1. [Create a linked list](#create-a-linked-list)
  2. [Insert a value to the head of the linked list](#insert-a-value-to-the-head-of-the-linked-list)
  3. [Remove a value from an arbitrary position](#remove-a-value-from-an-arbitrary-position)
  4. [Destroy a linked list](#destroy-a-linked-list)
  5. [Insert a value in an arbitrary position](#insert-a-value-in-an-arbitrary-position)
  6. [Print all the elements in the linked list](#remove-a-value-from-an-arbitrary-position)
- [Performing some tests](#performing-some-tests)
- [Source Code](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/02_linked_list)

## The linked list as an abstract data structure

A linked list is a linear collection of data where the order of the elements is not determined by their physical location in memory, unlike the array used in the [previous implementation of the stack](../01_stack_array_implementation/readme.md).

This data structure consists of units known as "nodes," which, in its simplest form, are structures containing the stored data and a reference to the next node in the list.

Since we rely on references to connect each node in a chain, adding or removing an element from the list is a relatively straightforward operation. It involves simply changing the reference of a node to point to the appropriate node.

This approach eliminates the need to determine the overall memory size of the collection in advance, dynamically resize the collection, or perform bound checks and reordering operations.

However, these benefits come at a cost. As the elements are not stored contiguously in memory, one cannot rely on sizes and offsets to directly access a specific node. Instead, to access a node, one must traverse the entire list up to the desired element. Consequently, accessing a particular element may require iterating through the list for the same number of iterations as the index of the element being searched.

## Implementing the data structure

In this example, we will implement a linked list where each node will have a reference to the next node. We will not support multiple references from one node to another.

The following operations will be supported:

1. Create a linked list
2. Insert a value to the head of the linked list
3. Remove a value from an arbitrary position
4. Destroy a linked list
5. Insert a value in an arbitrary position
6. Print all the elements in the linked list

After these examples, performing other operations such as replacing a value in a node or counting the number of elements should be straightforward.

Let's begin by writing the header file that will act as our interface.

```c
#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Node_T
{
    int32_t data;
    struct Node_T* next;
} Node;

Node* CreateNewNode();

void InsertToHead(Node** head, int32_t number);

int InsertAtNthPosition(Node** head, int32_t number, int32_t position);

int RemoveFromNthPosition(Node** head, int32_t position);

void PrintAll(Node* head);

```

As we can see, each node in the linked list is simply a struct that holds an integer and a pointer to the next element.

The collection is not stored in one contiguous place like an array. This means we need to be cautious when creating a node or rearranging the linked list to avoid any memory leaks. Losing track of a node may result in being unable to free that memory from the heap.

### Create a linked list

In this example, to create a node, we don't need to pass any arguments to our "constructor" function. The chosen semantics imply that the data stored in the first node will be set in the caller's context. The constructor will only handle error checking and memory manipulation.

The implementation can be seen in the following snippet:

```c
Node* CreateNewNode() {
    // we just call malloc and ask for a space in memory
    // of the size of our struct
    return (Node*)malloc(sizeof(Node));
}
```

To make use of the constructor we can make something like:

```c
// we create the node
Node* newNode = CreateNewNode();

// we check if the pointer is null
if (newNode == NULL){
    return 1
}
// we dereference the poiner and assign data
newNode->data = 69;
// as this is the only node, we assign NULL as the next node
newNode->next = NULL;
```

### Insert a value to the head of the linked list

To insert something at the begining of the linked list we need to perform this sequence of operations:

1. create a node
2. assign some data to it
3. make it point to the node that is currently at the head

To ilustrate this idea:

```c
// we ask for the value to store as data
// and a reference to the head (which is a pointer)
// this is important because in our function
// we will replace the head
void InsertToHead(Node** pointerToHead, int32_t number){

    //We create a new node
    Node* newNode = CreateNewNode();

    // we asign new data
    newNode->data = number;

    // we make it point to the old head
    newNode->next = *pointerToHead;

    // we replace the head with our new node
    *pointerToHead = newNode;
};
```

It might be tricky to understand this double pointer at first. But `Node** pointerToHead` is needed so the calling context can do the following operation:

```c
// the head is a pointer already
Node* head = CreateNewNode()
head->data = 69
head->next = NULL
// we pass a pointer to the head so the function can update the actual head
InsertToHead(&head)
```

If we were to pass just the pointer to the head instead of the pointer to a pointer to the first node, the `InsertToHead` function would receive a copy of the pointer. It would change where the copy points to, but the modification would be discarded when the function's stack frame is destroyed, without affecting the caller's context.

To overcome this, we can pass a reference to the pointer we want to modify as an argument to the function.

### Remove a value from an arbitrary position

To remove from an arbitrary position we will handle two cases:

1. The case where we are removing the first node
2. The case where we are removing any other node

There are other smarter ways to handle this operation, but this may be usefull to ilustrate the internals of this data structure.

The implementation can be seen in the following snippet, please find guidance in the comments:

```c
// We will be returining 0 or 1 to signal if the operation succeeded
// Again, we accept an integer for the position and a reference to the head node
int RemoveFromNthPosition(Node** head, int32_t position) {

    // if for some reason they give us a null pointer, we return with an error
    if (*head == NULL) {
        return 1;
    }

    // We initialize a variable to store the pointer we want to free
    Node* toDelete;

    // if we are freeing at the begining
    if (position == 0) {
        // we access the actual head pointer
        toDelete = *head;

        // we asign re-asign which node is now the head
        *head = (*head)->next;
        // we free the node we had stored
        free(toDelete);

        // then we return
        return 0;
    }

    // if we want to remove other node rather the first one
    // we will say that the head is our "previous" node
    Node* prevNode = *head;

    // we initialize a counter
    int32_t i;

    // and we start looping up to the position we want to remove
    for (i = 1; i < position ; i++) {

        // if the position is greater than the actual number of nodes
        // we will end up with the last node, wich points to NULL
        if (prevNode->next == NULL) {
            // and we return with an error
            return 1;
        }

        // the current node is now the previous of the next iteration
        prevNode = prevNode->next;
    }

    // At this point prevNode is the actual previous node
    // the next one will be our target
    // we assign our target to our "toDelete" variabe
    toDelete = prevNode->next;

    // and we make close the chain
    prevNode->next = toDelete->next;

    // then we clean our target node
    free(toDelete);
    return 0;
}
```

If we are removing a node other than the first one, the process is relatively straightforward. We locate the target node, remove it from the linked list, and then link the previous node to the next node.

However, removing the first node requires some attention. We need to change the actual head of the linked list, and this process depends on the strategy or semantics we have chosen for our implementation.

### Destroy a linked list

To destroy a linked list, we need to traverse the entire collection and free the memory allocated for each node.

We accomplish this by jumping from one node to the next, repeating the freeing operation until we reach the end of the linked list. This process is demonstrated in our `_cleanUp` implementation within the test suites, which can be found in [the source code of this example](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/blob/main/02_linked_list/test.c#L7).

```c
// test.c
void _cleanUp(Node** head) {
    // we initialize a variable to keep track of the result
    int shouldStop = 0;

    // we keep looping until we get a signal to stop
    while (shouldStop == 0) {
        // we use our previous implementation to remove
        // the head node
        shouldStop = RemoveFromNthPosition(head, 0);
    }
}
```

### Insert a value in an arbitrary position

Inserting a new node is quite similar to removing a node from an arbitrary position and may even be easier. The steps involved are as follows:

1. If the caller function passes a null pointer as the head pointer, we need to return.
2. If we are inserting at the head, we can use our previous implementation.
3. If we are inserting a node at any other position, we traverse from one node to another until we find the node at position n - 1, where n is the desired position for insertion. Then, we make the new node point to the node the previous one was pointing to. Finally, we make the previous node point to the new node.

The procedure is illustrated in the following snippet:

```c
int InsertAtNthPosition(Node** head, int32_t number, uint32_t position) {
    // We check if the linked list is empty
    if (head == NULL) {
        return 1;
    }
    // we check if we are adding to the head
    if (position == 0) {
        // we use the previous implementation
        InsertToHead(head, number);
        return 0;
    }

    // we start a counter
    int32_t i;
    // we write down which is the first node
    Node* prevNode = *head;

    // we start jumping from one node to the other
    for (i = 1; i < position; i++) {
        prevNode = prevNode->next;
    }
    // at this point, prevNode is the node before our target position

    // we create the node as we want
    Node* newNode = CreateNewNode();
    newNode->data = number;
    // we make it point to the old "next"
    newNode->next = prevNode->next;

    // we make the "prevNode" point to the new node
    prevNode->next = newNode;
    return 0;
}
```

### Print all the elements in the linked list

The simplest approach to print all the elements in a linked list is to iterate over it and call `printf` for each element. If desired, we can add characters to indicate the start and end of the list, as well as a separator between elements. For example, to print something like `[ 1 2 3 69 ]`, we can use the following function:

```c
void PrintAll(Node* head) {
    // we reasign that pointer to avoid messing with the arguments
    Node* currentNode = head;

    // We first print a string to inform what the user is seeing
    printf("The current values of the linked list are: [ ");

    // we loop over the whole list until the end of if
    while (currentNode != NULL) {
        // we print the data of each node and a space
        printf("%d ", currentNode->data);

        // we update which should be the current node for the next iteration
        currentNode = currentNode->next;
    };
    // And we close the string
    printf("]\n");
}
```

Please note that this is a naive approach, and we are not optimizing anything. We could calculate the buffer size of our string in advance, populate the buffer with the elements, and then make a single `printf` call, reducing the number of system calls. Such optimizations may be covered in a separate chapter.

## Performing some tests

The whole test suite can be found [this file](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/blob/main/02_linked_list/test.c).
Here, we will demonstrate a test to verify the correct order of elements in the list when inserting at the head.

The testing function is as follow

```c

// We will accept a number indicating how many nodes we want
int _testLinkedListOrdering(int32_t desiredNodeNumber) {
    // to avoid messing with the arguments we will reasign
    int32_t nodeNumbers = desiredNodeNumber;
    // and initialize a counter
    int32_t i;
    // we will initialize a new linked list
    Node* head;
    head = NULL;
    for (i = 0; i < nodeNumbers; i++) {
        // and then we will insert to head
        // the value of each node will be in decreasing order
        InsertToHead(&head, nodeNumbers - 1 - i);
    }
    // then we will loop over the list and check if the the value is correct
    Node* currentNode = head;
    for (i = 0; i < nodeNumbers; i++) {
        assert(currentNode->data == i);
        // we set which is the currentNode for the next iteration
        currentNode = currentNode->next;
    }
    // We cleanup using the function shown in the "Destroy the linked list" subtitle
    _cleanUp(&head);
    // and we assert the head is now NULL
    assert(head == NULL);
}
```

:)