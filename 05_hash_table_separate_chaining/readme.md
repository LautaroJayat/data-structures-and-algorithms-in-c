# Hash table + separate chaining for collision resolution

**Table of contents**

- [What is a hash table?](#what-is-a-hash-table)
- [A collision resolution strategy](#a-collision-resolution-strategy)
- [Implementing the hash table](#implementing-the-hash-table)
- [Linked list related operations](#linked-list-related-operations)
  - [Defining interfaces](#defining-interfaces)
  - [Creating a node](#creating-a-node)
    - [Testing node creation](#testing-node-creation)
  - [Clearing the list](#clearing-the-list)
    - [Testing list deletion](#testing-list-deletion)
  - [Removing a node from the list](#removing-a-node-from-the-list)
    - [Testing node removal](#testing-node-removal)
- [Hash table related operations](#hash-table-related-operations)
  - [Creating a hash table](#creating-a-hash-table)
    - [Testing hash table creation](#testing-hash-table-creation)
  - [Storing a key-value pair in our hash table](#storing-a-key-value-pair-in-our-hash-table)
  - [Implementing a simple hash function](#implementing-a-simple-hash-function)
    - [Testing our hashing function](#testing-our-hashing-function)
  - [Checking if we need to resize](#checking-if-we-need-to-resize)
  - [Doing the actual resizing](#doing-the-actual-resizing)
    - [Testing the resizing](#testing-the-resizing)
  - [Getting a value from the hash table](#getting-a-value-from-the-hash-table)
  - [Removing a key-value pair from the hash table](#remving-a-key-value-pair-from-the-hash-table)
  - [Testing a complete flow](#testing-a-complete-flow)
  - [Link to source code](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/05_hash_table_separate_chaining)

## What is a hash table?

A hash table can be described as a collection of key-value pairs where:

- Each key appears at most once.
- Each key maps to only one value.
- A function is used to consistently map the potentially infinite space of possible keys to finite domain of "hash codes" that we can handle.

In respect to the third bullet point mentioned above, the underlying problem the hash function solves is as follows:

If we have a limited number of keys, and time is not a problem, we could store key-value pairs in a linked list or an array. Then, if we want to look for a value, we just traverse the whole collection, comparing our input key with each key we have stored.

Although the approach is simple, it implies that the number of lookups needed to find a value might grow as the number of stored keys also grows. To avoid this, a function is used to convert a key into something that gives us a hint about where the key is located in memory, without needing to traverse the entire collection.

The functions that make this work are called "hashing functions."

A useful hashing function should be fast to compute without wasting too many resources. If it's not, each lookup may have a consistent time but could be slower than the entire traversing and comparing approach mentioned above.

It should also minimize the probability of outputting the same value for two different inputs, which is commonly called a "collision."

As we just mentioned, we are talking about "minimizing" collisions rather than trying to avoid them completely. This is because we are mapping an infinite set of possible keys to a finite set of possible outputs. Collisions will be inevitable at some point.

## A collision resolution strategy

Collisions can be minimized, but they cannot be avoided. This means we need a way to handle these collisions.

For this article, we have chosen to start with what is called "separate chaining", which consists on using linked lists to store all key-value pairs where different key maps to the same output after being passed to our hash function. This way, every time we want to store a key-value pair, we first compute the hash, then we look for the head node corresponding to that hash and traverse the list, comparing each stored key with the input key.

The approach still implies a linear time complexity for the step where we traverse the entire linked list in search of the provided key, but now the universe of possible keys is reduced to a fraction of the original.

Furthermore, in the design, we can decide how many elements can be stored in a linked list or the limit for the ratio between available linked lists and overall elements stored, and resize the entire data structure accordingly.

<br>
<br>

# Implementing the hash table

To present this data structure, we will first introduce the interfaces we are going to use. Then we will move on to the linked list-related operations. Each of them will have comments to guide the reader through the code, along with some quick tests to understand how they behave.

Finally, we will proceed to the hash table-related functions, providing implementation details and additional tests. The intention is to progress from bottom to top, from the lower level to the integration level.

## Defining interfaces

Our hash table will consist of a struct containing:

- the number of elements already stored
- an array of pointers that leads to the head of a linked list
- a number representing the size of the array above

```c
typedef struct {
    Node** collection;
    unsigned int capacity;
    unsigned int storedElements;
} HashTable;
```

For the collection, we are using a pointer-to-pointer semantic just to simplify the process of changing the head of the linked list.

Then, for the nodes, we will use structs containing the following:

- a pointer to where we will store the key
- a pointer to where we will store the value
- a pointer to the next element in the list

```c
typedef struct Node_T {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct Node_T* next;
} Node;
```

Apart from that, we will define certain numbers to constrain the behavior of the linked list:

```c
#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 256
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2
```

For the API of our hash table, we will support the following operations:

- creating a hash table
- storing a key-value pair
- removing a key-value pair
- looking up a value for a given key.

The function signatures for these operations will be as follows:

```c
HashTable* CreateHashTable(unsigned int capacity);
bool Store(HashTable** hashTable, char* key, char* value);
char* Get(HashTable* hashTable, char* key);
bool Remove(HashTable* hashTable, char* key);
```

For convenience, we will create a separate set of functions that handle linked list operations such as creating a node, removing a node, getting the value of a node, and clearing a whole list.
The function signatures for these operations will be as follows:

```c
Node* CreateNode(char* key, char* value);
bool RemoveNode(Node** head, char* key);
unsigned int ClearList(Node** headNode);
char* GetNodeValue(Node* head, char* key);
```

Lastly, there are a couple of additional "internal" functions that will be needed for determining when to resize the hash table, computing the hash, and performing the actual resizing:

```c
unsigned int _computeHash(char* key, unsigned int capacity);
bool _needsToResize(HashTable* hashTable);
HashTable* _resize(HashTable* hashTable);
```

<br>
<br>

## Linked list related operations

We won't be providing a comprehensive explanation of linked lists here, as we have covered this topic in a previous chapter.
If you have never heard about this data structure, please follow [this link](../02_linked_list/readme.md).

Here, we will only cover the necessary operations to integrate linked lists within our hash table.

### Creating a node

When creating a node, we want to check that both the key and the value are valid pointers to a string, and also that their lengths are within the bounds we want to handle.
Then, we will allocate memory for the struct and copy the key and value to the corresponding fields in the struct.
At this point, we don't care if the keys already exist in a node of our linked list. That will be handled at a higher scope.

```c
// We accept the pointers to the strings and return a pointer to a node
Node* CreateNode(char* key, char* value) {
    // are the pointers valid?
    if (key == NULL || value == NULL) {
        printf("error: key and value must be string\n");
        return NULL;
    }

    // are the lengths of the strings valid?
    if (strlen(key) > MAX_KEY_LEN || strlen(value) > MAX_VALUE_LEN) {
        printf("error: key and value should not exceed max lengths\n");
        return NULL;
    }

    // At this point we can start asking for memory
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("error: could not allocate memory for new node\n");
        return NULL;
    }
    // by default we set the next pointer to null
    // the calling function will take care of setting the correct value
    newNode->next = NULL;

    // then we copy the key and the value to their buffers in the struct
    strcpy(newNode->key, key);
    strcpy(newNode->value, value);

    // And we return the pointer to the new node
    return newNode;
};
```

Notice that we are using functions declared in `string.h` for string manipulation. Here we are using `strlen` to get the length of a string. It accepts a pointer to a string and returns a number representing the length. Then we are using `strcpy` to copy a string into a buffer.

### Testing node creation

To test the function described above we will do the following:

```c
void _testNewNode() {
    // we will use this as a temp variable to store a string
    char* s;
    // this will be used as a good input
    char* goodString = "hey how are you?";
    // here we will create a string that exceeds the limits for
    // our string length
    int i;
    char badString[MAX_KEY_LEN + 3000];
    for (i = 0; i < MAX_KEY_LEN + 20; i++) {
        badString[i] = *"!";
    }

    // We create a new node with correct inputs
    // and check everything was stored correctly
    Node* newNode = CreateNode(goodString, goodString);
    s = newNode->key;
    assert(*s == *goodString);
    s = newNode->value;
    assert(*s == *goodString);
    free(newNode);
    printf("Testing with good string: PASS\n");

    // Now we provide an invalid poiner and expect
    // the function to return NULL
    Node* newNode2 = CreateNode(NULL, goodString);
    assert(newNode2 == NULL);
    free(newNode2);
    printf("Testing with null string pointer: PASS\n");

    // Now we provide a long string and expect a NULL pointer
    Node* newNode3 = CreateNode((char*)badString, goodString);
    assert(newNode3 == NULL);
    free(newNode3);
    printf("Testing with bad string: PASS\n");
}
```

### Clearing the list

To clear the list, we will take a pointer to a head node and then iterate over all its members. Each node will be freed, and then the head will be set to NULL.
To provide feedback, we will return the number of nodes that were removed.

```c
// we receive the pointer of the head node
// and return an integer with the number of nodes that were deleted.
unsigned int ClearList(Node** headNode) {
    // we access the head node
    Node* head = *headNode;
    // and set a temporary node that will help us in the iteration
    Node* tmp;
    // we create a counter
    unsigned int deletedNodes = 0;
    // and start a loop that will end when the current node is NULL
    while (head != NULL) {
        // we store the current node ina  tmp variable
        tmp = head
        // we assign the next node as the current node
        head = tmp->next;;
        // free the current node
        free(head);
        // and update the counter
        deletedNodes++;
    }

    // finaly we update where the caller pointer is pointing to
    *headNode = NULL;
    printf("deleted %d nodes, linked list is now empty\n", deletedNodes);
    // and we return the counter
    return deletedNodes;
};
```

### Testing list deletion

A simple test of the happy path can be done by creating a list with an arbitrary number of nodes and then passing the list into our function.
After that, we can assert if the returned value corresponds to the number of nodes we created.

```c
void _testClearList() {
    // we create a valid stirng for input and output
    char goodString[] = "to delete";
    // we set the number of values we want our list to have
    unsigned int expectedNodes = 10;
    // start a counter
    unsigned int i;
    // we create the first node
    Node* head = CreateNode(goodString, goodString);
    // and we add loop to generate the rest of the nodes
    for (i = 1; i < expectedNodes; i++) {
        // we store the current head
        Node* tmp = head;
        // assign which should be the new head
        head = CreateNode(goodString, goodString);
        // and update the next node with the old head
        head->next = tmp;
    };
    // we clear the list and store the result
    unsigned int clearedNodes = ClearList(&head);
    // the result should be equal to the nodes we created
    assert(clearedNodes == expectedNodes);
}
```

### Removing a node from the list

To remove a node from the list, we need to access the list using the head node. Then, we traverse the list until we find the key we are looking for.
When we find the key, we free that node and make the previous node point to the node that comes after the deleted one.
If the target node is the head node, we simply delete the head and assign the next node as the new head.
In our implementation, we will return a boolean value to indicate whether the process succeeded or not.

```c
// we accept a pointer to a pointer to the head node
// and a pointer that holds a reference to the key we
// are loking for
bool RemoveNode(Node** headP, char* key) {
    // first we check if the pointers are valid
    if (headP == NULL || *headP == NULL) {
        printf("could not remove node, the list is empty\n");
        return false;
    }
    // then we dereference to access the actual head
    Node* currentNode = *headP;

    // we check if the head node is the one we are looking for
    if (strcmp(currentNode->key, key) == 0) {
        // in case it is, we assign the next node as the new head
        *headP = currentNode->next;
        // we free the one we want to remove
        free(currentNode);
        // and return a success
        return true;
    }
    // at this point we know we need to traverse the list
    // so we store a reference to the next node
    Node* nextNode = currentNode->next;
    // and start looping and checking until we reach the end
    while (nextNode != NULL) {
        // if the next node is the node we are looking for
        if (strcmp(nextNode->key, key) == 0) {
            // make the previous (currentNode) point to
            // the next node of the one we are going to remove
            currentNode->next = nextNode->next;
            // then we remove the one we found
            free(nextNode);
            // and return
            return true;
        }
        // if not, we just move one step
        // and update the pointers for the next iteration
        currentNode = nextNode;
        nextNode = nextNode->next;
    }
    // if we reach this point, that means the key was not in any
    // of the stored pointers
    printf("node of key %s could not be found\n", key);
    return false;
};
```

### Testing node removal

To test if the node removal is working, we will assert for success or failure by using the boolean output of our function.

```c
void _testRemoveNode() {
    // We will use this string for key-values we dont want to delete
    char notToDelete[] = "do not delete me";
    // we will use this to mark a node to delete
    char toDelete[] = "to delete";

    int i;
    printf("Testing deletion of the last node\n");
    // We create the head node, this will end up at the tail
    Node* head = CreateNode(toDelete, toDelete);
    // we prepend 3 more nodes that we dont want to delete
    for (i = 1; i < 4; i++) {
        Node* tmp = head;
        head = CreateNode(notToDelete, notToDelete);
        head->next = tmp;
    };
    // we decide to remove the last node
    bool success = RemoveNode(&head, toDelete);
    // and assert
    assert(success == true);

    printf("Testing deletion of node in the middle\n");
    // now we will delete a node in the middle
    Node* deleteMeNode = CreateNode(toDelete, toDelete);
    deleteMeNode->next = head;
    head = deleteMeNode;
    // and make a sandwich prepending 3 more nodes
    for (i = 1; i < 4; i++) {
        Node* tmp = head;
        head = CreateNode(notToDelete, notToDelete);
        head->next = tmp;
    };
    // we remove the node
    success = RemoveNode(&head, toDelete);
    // and everything went OK
    assert(success == true);

    printf("Testing deletion of the head node\n");
    // now we prepend one node and delete it
    deleteMeNode = CreateNode(toDelete, toDelete);
    deleteMeNode->next = head;
    head = deleteMeNode;
    // we try to remove it
    success = RemoveNode(&head, toDelete);
    // and everything
    assert(success == true);

    printf("Testing early return when providing empty lists\n");
    Node* badPointer = NULL;
    success = RemoveNode(NULL, toDelete);
    assert(success == false);
    success = RemoveNode(&badPointer, toDelete);
    assert(success == false);
    while (head != NULL) {
        Node* tmp = head->next;
        free(head);
        head = tmp;
    }
}
```

If desired, we could also traverse the list to assert that there is no node with the key we wanted to remove.

### Getting a node value

To traverse the linked list and find a value by a key, we will accept a pointer to the head node and a key. We will validate that those pointers are not null pointers. If everything is fine, we will loop over each node, comparing the input string with each key of each node.
If we find a match, we will allocate memory for that string and return a pointer to that buffer.
If no match is found, we will return null.
It will be the responsibility of a higher scope to free that allocated memory.

```c
// We receive a pointer to to the head
// a pointer to a string
// and we return a pointer to a copy of the string
char* GetNodeValue(Node* head, char* key) {
    // we check if the pointers are valid
    if (head == NULL) {
        printf("could not find node because the list was empty\n");
        return NULL;
    }

    if (key == NULL) {
        printf("cant check for node value due to nil key\n");
        return NULL;
    }
    Node* tmp = head;

    // if so, we start iterating over each node
    // we will stop if the key the same as our input
    // or if we reach the last
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }

    // at this point, if tmp is null is because we exited the loop
    // after checking the last node and we didn't find anything
    if (tmp == NULL) {
        // so we return
        return NULL;
    }

    // if we reach this line, this mean we found something
    // so we ask for a block of memory of the size we need
    char* buff = malloc(strlen(tmp->value) * sizeof(char*));
    // but we exit if we could not allocate that memory
    if (buff == NULL) {
        return NULL;
    }
    // in the best case scenario we copy the key to our buffer
    strcpy(buff, tmp->value);
    // and return the buffer
    return buff;
}
```

<br>
<br>

## Hash table related operations

To simplify things, we can say that our hash table is essentially a dynamic array of pointers to the head of a linked list.
To determine in which linked list a key-value pair should be stored, we pass the key to a hash function that outputs a number between 0 and the number of slots in our array.
Here's a conceptual example:

```sh
# each key maps to a given index
hash(k1) -> 0
hash(k2) -> 2
hash(k3) -> 3

[   0   ,   1   ,   2   ,   3   ,   4   ]
    |       |       |       |       |
  k1-v1    NULL   k2-v2   k3-v3    NULL
```

Then, if a new key maps to an index where the linked list already has a value, we link the new node to it:

```sh
# a new value maps to an index where
# the linked list already has a value
hash(k4) -> 0

# so we bind the new node
[   0   ,   1   ,   2   ,   3   ,   4   ]
    |       |       |       |       |
  k4-v4    NULL   k2-v2   k3-v3    NULL
    |
  k1-v1
```

### Creating a hash table

As we presented above in the section on [defining interfaces](#defining-interfaces), our hash table is implemented as a struct that contains an array of pointers to linked lists, along with two numbers: one to keep track of the elements stored and the other to indicate the array capacity.

Creating an instance of this struct is straightforward. We provide a number representing the desired capacity, attempt to allocate the necessary memory, and set the defaults as shown in the following example:

```c
// we return a pointer to the hash table
// and accept an int for the capacity
HashTable* CreateHashTable(unsigned int capacity) {
    // if capacity is less than an arbitrary number
    // we set a default capacity
    if (capacity < 4) {
        capacity = 10;
    }

    // we try to allocate an array of pointers to
    // linked lists head pointers
    Node** collection = calloc(capacity, sizeof(Node*));
    // And return if the operation failed
    if (collection == NULL) {
        printf("error: could not initialize underlying collection\n");
        return NULL;
    }
    // At this point we can try to allocate the memory for the hash table.
    HashTable* hashTable = malloc(sizeof(HashTable));

    // If we fail we free the array previously allocated
    if (hashTable == NULL) {
        printf("error: could not initialize hash table\n");
        free(collection);
        return NULL;
    }

    // now we set all the fields and return
    hashTable->collection = collection;
    hashTable->capacity = capacity;
    hashTable->storedElements = 0;
    return hashTable;
}
```

### Testing hash table creation

We will include some simple assertions for the previous function. We want to verify if the capacity and defaults are correctly set.

```c
void _testCreateHashTable() {
    // first we create a hash table with a capacity
    // that is lower than our min accepted value
    HashTable* hashTable = CreateHashTable(3);
    // so we expect it to be the default value
    assert(hashTable->capacity == 10);

    // we free everything
    free(hashTable->collection);
    free(hashTable);
    // and now we create one with an OK capacity
    hashTable = CreateHashTable(15);

    // and we assert it is using our input
    // to set this field
    assert(hashTable->capacity == 15);

    // as a final step we can iterate and see that
    // all slots are NULL pointers
    unsigned int i;
    for (i = 0; i < 15; i++) {
        assert(hashTable->collection[i] == NULL);

    }
    // and we clean everything up
    free(hashTable->collection);
    free(hashTable);
}
```

### Storing a key-value pair in our hash table

Now we come to the part where we start performing useful operations with our hash table!
Please note that there are three steps that will be somewhat abstract for us at this point. These steps involve checking if we need to resize our hash table, the actual resizing process, and the operation where we compute the hash. These "private" functions will be presented after this section.

**Note:** Just for presentation purposes, we will include tests for storing, getting, and removing values at the end of the article.

To store a value, we need to follow the following flow:

1. check that our provided inputs are valid
2. check if we need to resize the hash table, and if so, perform the resizing
3. Compute the hash for the key to determine which linked list should store our value
4. Traverse the list and check if a node with the same key exists. If a matching node is found, update its value; otherwise, append a new node to the list.

Please find some guidance in the comments:

```c
// we will return boolean to signal if the process succeeded
// and receive a pointer to a hash table, a key and a value
bool Store(HashTable** hashTableP, char* key, char* value) {
    // if some of the inputs is a NULL pointer, we return
    if (hashTableP == NULL || *hashTableP || key == NULL || value == NULL) {
        printf("error: bad values provided\n");
        return false;
    }
    // get our actual pointer in a handy variable
    HashTable* hashTable = *hashTableP;

    // we check if the hash table needs to be resized
    // and we do it if so
    if (_needsToResize(hashTable)) {
        printf("needs to resize\n");
        // try to resize
        HashTable* newHashtable = _resize(&hashTable);
        if (newHashTable != NULL) {
            // we replace everything if succeeded
            *hashTableP = newHashTable;
            hashTable = newHashTable;
        } // or print and error if not
        else printf("error: could not resize hash table, we will try on next Store operation\n");
    }

    // and now we compute the hash using the capacity of the array
    // used to store the number of buckets for our linked lists
    unsigned int position = _computeHash(key, hashTable->capacity);

    // and we get the head node of our hash table
    Node* head = hashTable->collection[position];

    // we traverse it and check our node key
    // with the provided key
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
            // in case we find a node with the
            // same key, we exit the loop
            break;
        }
        // if not, we go to the next node
        head = head->next;
    }

    // at this point we migth have a node stored in
    // the "head" variable.
    if (head != NULL) {
        // if so, we copy our value to the "value"
        // field in the found node
        strcpy(head->value, value);
        return true;
    }

    // if we didn't find it, we create a new node
    Node* newNode = CreateNode(key, value);

    // if the node couldn't be created, we return
    if (newNode == NULL) {
        printf("error: could not alocate memory for node\n");
        return false;
    }
    // if everything went ok, we prepend the new node
    newNode->next = hashTable->collection[position];
    hashTable->collection[position] = newNode;
    return true;
}
```

### Implementing a simple hash function

NNow, let's focus on the function that takes a key and produces a number that will serve as an index for the underlying array used to store references to the heads of our linked lists. As the array grows, we need to be able to update the range within which this number should fall.
For our implementation we will be using a simplified version of a [rolling hash strategy](https://en.wikipedia.org/wiki/Rolling_hash) In this strategy, we iterate over each character of the key, producing a hash, and then use that hash to compute the next one. In each iteration, we will take the modulo operation to ensure that our results fall within the desired bounds.

```c
// we accept a pointer to where our key lies and
// a number for the upper limit four our hash
unsigned int _computeHash(char* key, unsigned int capacity) {
    // our hash starts with 0
    unsigned int hash = 0;
    // as our counter
    unsigned int counter = 0;
    // and we select a multiplier
    unsigned int primeNumber = 31;
    //then we loop over the string
    while (key[counter] != '\0') {
        // we multiply our hash with and add the curren character
        hash = (hash * primeNumber) + key[counter];
        // and update the hash
        hash = hash % capacity;
        counter++;
    }
    // finally we return a number that will be between 0
    // and our upper limit (which happens to be our array capacity)
    return hash;
}
```

### Testing our hashing function

Now, our goal is to test whether our hash function consistently produces the same output for a given key. We'll conduct a simple test to gauge the consistency of the results when we manipulate the upper limit of the hash value and repeat the operation multiple times with same key.

However, it's important to note that this test is not an absolute proof of consistency or a demonstration of homogeneous distribution of the hash values. To provide such proof, we would need a formal, logical, and mathematical demonstration, which exceeds the scope of this article.

```c
void _testCreateHashConsistency() {
    // we will use a simple string
    char* string = "hey, how are you";
    // and two counters
    int i, j;
    // we will compute a hash for 990 different capacities
    for (j = 10; j < 1000; j++) {
        unsigned int hash = _computeHash(string, j);
        // and then generate the hash 100 times
        for (i = 0; i < 100; i++) {
            // and check those 100 times we received the same hash
            assert(hash == _computeHash(string, j));
        }
    }
}
```

### Checking if we need to resize

Determining when to resize our hash table is somewhat arbitrary. A common approach is to calculate a ratio between the number of buckets or linked lists available and the number of elements stored in our data structure.

To implement this operation, we will accept a hash table as input and calculate whether adding one more element would cause the ratio to exceed our predetermined limit.

```c
// We return a boolean and accept a hash table
bool _needsToResize(HashTable* hashTable) {
    // we calculate the ratio between the actual stored elements plus one
    // and the current capacity (the number of buckets for our linked lists)
    // as our numbers were int, we need to cast them to float first
    float ratio = (float)(hashTable->storedElements + 1) / (float)hashTable->capacity;
    // if the ratio is greater than 1.5 we know we need to resize
    if (ratio > (float)1.5) {
        return true;
    }
    // if less, we can continue adding nodes
    return false;
};
```

### Doing the actual resizing

To resize our hash table, we follow a process of creating a new hash table with a larger capacity, which means more buckets for storing linked lists. We then traverse all the existing linked lists and attempt to store their elements in the new hash table. To simplify this process, we can use the same `Store` method that we implemented for our hash tables.

Once all elements have been copied to the new hash table, we can free the memory occupied by the old hash table and return a pointer to the new hash table. If we encounter any issues during the allocation process, we need to clean up all allocated memory and inform the upper scope to handle the failure appropriately.

```c
// we accept a pointer to a hash table and return a pointer to a hash table
HashTable* _resize(HashTable* hashTable) {
    // We store the pointer to avoid mutating parameters
    HashTable* oldHashTable = hashTable;
    // we try to allocate memory for the new hash table
    HashTable* newHashTable = CreateHashTable(oldHashTable->capacity * GROWTH_FACTOR);
    // And return if we failed
    if (newhashtable == NULL){
        return NULL
    }

    unsigned int i;
    bool success = true;
    // then we will loop over all linked lists
    for (i = 0; i < oldHashTable->capacity; i++) {
        Node* currentNode = oldHashTable->collection[i];
        // And for each linked list, we will loop for all its nodes
        while (currentNode != NULL) {
            // we will try to store the node in the new linked lists
            // while keeping track if we succeeded
            success = Store(&newHashTable, currentNode->key, currentNode->value);
            // we return if there was a problem
            if (!success) break;
            // and continue with the next node if everything went ok
            currentNode = currentNode->next;
        }
        // if we exited the while loop with an error, we stop the outer loop too
        if (!success) {
            printf("error: could not complete sesizing\n");
            break;
        }
    }

    // if we succeeded we can clear the old hash table
    if (success) {
        int i;
        for (i = 0; i < oldHashTable->capacity; i++) {
            ClearList(&hashTable->collection[i]);
        }
        free(oldHashTable->collection);
        free(oldHashTable);
        printf("success!!\n");
        return newHashTable;
    }
    printf("cleaning up after resizing attempt\n");
    // at this point we know we failed and we need to clean the new hash table
    for (i = 0; i < newHashTable->capacity; i++) {
        Node* currentNode = newHashTable->collection[i];
        while (currentNode != NULL) {
            Node* tmp = currentNode->next;
            free(currentNode);
            currentNode = tmp;
        }
    }
    free(newHashTable->collection);
    free(newHashTable);
    return NULL;
};
```

### Testing the resizing

To verify if the resizing operation is being performed correctly, we can do a simple check by attempting to store fifty key-value pairs in the hash table and then checking if the capacity has changed. This check can help us confirm if the resizing process is triggered and if the capacity of the hash table is updated accordingly.

```c
void _testResizing() {
    // we create a buffer for our strings
    char input[256];
    // initialize some ints
    int i, INITIAL_CAP;
    INITIAL_CAP = 5;
    // create a little hash table
    HashTable* hashTable = CreateHashTable(INITIAL_CAP);
    // and assert the initial conditions
    assert(hashTable->capacity == INITIAL_CAP);
    // then we procedurally create some key-values
    // and try to store them in the hash table
    for (i = 0; i < 50; i++) {
        sprintf(input, "string_%d", i);
        bool success = Store(&hashTable, input, input);
        assert(success == true);
    }
    // at this point the capacity should be pretty higher
    // than te initial one
    assert(hashTable->capacity > INITIAL_CAP);
    // finally we clean everything up
    for (i = 0; i < hashTable->capacity; i++) {
        ClearList(&hashTable->collection[i]);
    }
    free(hashTable->collection);
    free(hashTable);
}
```

### Getting a value from the hash table

To retrieve a value by its key from the hash table, we need to perform the following steps:

1. Validate that both the hash table and the key are valid inputs.
2. Calculate the hash of the key using our hash function. This will give us the index of the linked list where the value may be stored.
3. If the slot for that linked list is empty, it means the key is not present in our hash table. We return null to indicate the absence of a value.
4. If we find a linked list in that slot, we need to search for the key within the linked list. We delegate this search operation to our GetNodeValue function.
5. The GetNodeValue function will traverse the linked list, comparing each node's key with the input key. If a matching key is found, we return the corresponding value.
6. If we reach the end of the linked list without finding a matching key, we return null to indicate that the key is not present in our hash table.

```c
//we get a pointer to a hash table, and a pointer to a string
char* Get(HashTable* hashTable, char* key) {
    // if the pointers are NULL, or the string is empty, we consider that an error
    if (hashTable == NULL || key == NULL || strlen(key) == 0) {
        printf("error: bad values were provided:\n%p\n%s\n%ld\n", hashTable, key, strlen(key));
        return NULL;
    }
    // at this point we can compute the hash
    unsigned int position = _computeHash(key, hashTable->capacity);
    // and get the correct linked list
    Node* head = hashTable->collection[position];
    // then we check if we need to return
    if (head == NULL) return NULL;
    // If not, we delegate the rest of the flow
    char* value = GetNodeValue(head, key);
    // and return what the GetNodeValue give us
    return value;
};
```

### Remving a key-value pair from the hash table

To remove a key-value pair from the hash table, we need to follow these steps:

1. Validate the inputs to ensure they are valid (hash table and key).
2. Calculate the hash of the key using our hash function to determine the linked list where the pair may be stored.
3. If there is no linked list associated with the calculated hash, it means the key is not present in the hash table. We can return from the function at this point.
4. If a linked list is found at the calculated hash, we pass a pointer to the head pointer of the linked list to our RemoveNode function.
5. The RemoveNode function will traverse the linked list, searching for the node with a matching key. If found, it will remove the node from the linked list and update the head pointer if necessary.

```c
//we get a pointer to a hash table, and a pointer to a string
bool Remove(HashTable* hashTable, char* key) {
    // if the pointers are NULL, or the string is empty, we consider that an error
    if (hashTable == NULL || key == NULL || strlen(key) == 0) {
        printf("error: bad values were provided:\n%p\n%s\n%ld\n", hashTable, key, strlen(key));
        return false;
    }
    // at this point we can compute the hash
    unsigned int position = _computeHash(key, hashTable->capacity);
    // if there is no linked list, we can return
    if (hashTable->collection[position] == NULL) return true;
    // if we find a linked list, we delegate the node removal
    bool success = RemoveNode(&hashTable->collection[position], key);
    // and output the result
    return success;
};
```

### Testing a complete flow

Now we get to test a complete flow!
In it, we will try to:

1. create a hash table
2. store some key-value pairs
3. change their values
4. try to get a value for a non present key
5. remove a key and hen check it is not present in the hash table

```c
void _testStoreGetAndRemove() {
    // we create some keys
    char* keys[] = { "key1","key2","key3","key4","key5","key6" };
    // we initialize a hash table
    HashTable* hashTable = CreateHashTable(5);
    int i;
    // we store all the keys
    for (i = 0; i < 6; i++) {
        bool success = Store(&hashTable, keys[i], keys[i]);
        // and check everything went ok
        assert(success == true);
    }
    // then we loop over all the keys and check they have a correct value
    for (i = 0; i < 6; i++) {
        char* value = Get(hashTable, keys[i]);
        assert(strcmp(value, keys[i]) == 0);
        free(value);
    }
    // we create a new expected string for all the values
    char* defaultValue = "default";
    // and change the values for all the keys
    for (i = 0; i < 6; i++) {
        bool success = Store(&hashTable, keys[i], defaultValue);
        assert(success == true);
    }
    // Then we get all the keys and assert their new values
    for (i = 0; i < 6; i++) {
        char* value = Get(hashTable, keys[i]);
        assert(strcmp(value, defaultValue) == 0);
        free(value);
    }
    // we try to get a value for a non present key
    assert(Get(hashTable, "missingKey") == NULL);
    // we remove one key
    bool success = Remove(hashTable, keys[1]);
    assert(success == true);
    // and assert there is no value for it now
    assert(Get(hashTable, keys[1]) == NULL);
    // and now we clean everything up
    for (i = 0; i < hashTable->capacity; i++) {
        Node* n = hashTable->collection[i];
        ClearList(&n);
    }
    free(hashTable->collection);
    free(hashTable);
}
```
