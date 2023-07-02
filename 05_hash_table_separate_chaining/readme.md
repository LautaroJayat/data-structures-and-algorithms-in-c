**[WORK IN PROGRESS]**

**TODO**

- check grammar
- check style
- check if it has sense
- check links
- rebase onto web branch
- re order topics if needed

# Hash table + spread chaining for collision resolution

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

## What is a hash table?

A hash table can be described as a collection of key value-pairs where:

- each key appears once at most,
- each key maps to only one value and
- where a function is used to consistently map the maybe infinite space of possible keys to finite domain of "hash codes" that we can handle.

In respect to the third bullet mentioned above, the underlying problem the hash function solves is the following:

If have a limited number of keys, and time is not a problem, we could store key-value pairs in a linked list or an array. Then, if we want to look for a value we just traverse the whole colection comparing our input key with each key we have stored.

Although the approach is simple, it implies that the number of lookups needed to find a value migth grow as the number of keys stored also grow. To avoid this, a function is used so we can convert a key into something that give us a hint on where the key is located in memory without needing to traverse all the collection.

The function that makes this work are called "hashing functions".

A useful hashing function should be fast to compute, without wasting too many resources. If not, each lookup may have a consist time but it could be slower than make the whole traversing and comparing approach mentioned above.

Also it should minimize the probability of outputing the same value for two different inputs, what is commonly called "collision".

As we just mentioned, we are talking about "minimizing" the collisions and not trying to find a way to avoid them completly. This is because we are mapping an "infinite" set of possible keys to a finte set of possible outputs. Collisions will be inevitable at some point.

## A collision resolution strategy

Collisions can be minimized but they can not be avoided. This mean we need a way in which we hanlde this collisions.

For this article we have choose to start with what is called "separate chaining", which consist in using a linked list to store all key value pairs where the key maps to the same output after being passed to our hash function. This way, every time we want to store a key value pair, we fisrt compute the hash, then we look for the head node corresponding to that hash and traverse the list comparing each stored key with the inputed key.

The approach still implies a linear time complexity for the step where we traverse the whole linked list in search for the provided key, but now the universe of possible keys is reduced to a fraction of the original.

Even more, in the design, we can decide how many elements can be stored in a linked list or the limit for the ratio between available linked lists and overall elements stored, and resize the whole data structure acordingly.

<br>
<br>

# Implementing the hash table

To present this data structure we will first introduce the interfaces we are going to use. Then we will move to the linked list related operations, each of them will have some comments to guide the reader through the code, with also some quick tests to understand how they behave.
Finally we will move to the hash table related functions, again, with implementation details and some tests.
The idea is to go from bottom to top, from the lower level to the integration level.

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

For the collection, we are using this pointer to poinert semantic just to ease the case where we want to change the head of linked list.

Then, for the nodes we will use structs containing the following with the following:

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

Apart from that, we will define some numbers to constraint the behaviour of the lined list:

```c
#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 256
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2
```

For the API of our hash table we will suport creating a hash table, storing a key-value pair, removing a key-value pair, and looking a value for a given key.
The signatures for the functions will be the following ones:

```c
HashTable* CreateHashTable(unsigned int capacity);
bool Store(HashTable** hashTable, char* key, char* value);
char* Get(HashTable* hashTable, char* key);
bool Remove(HashTable* hashTable, char* key);
```

For convinence we will create a separate set of functions that handles linked list operations such as creating a node, removing a node, getting the value of a node and clearing a whole list.
The signatures for those functions will be:

```c
Node* CreateNode(char* key, char* value);
bool RemoveNode(Node** head, char* key);
unsigned int ClearList(Node** headNode);
char* GetNodeValue(Node* head, char* key);
```

Lastly, there are a couple of more "internal" functions that will also be needed for knowing when to resize the hash table, compute the hash, and doing the actual resizing:

```c
unsigned int _computeHash(char* key, unsigned int capacity);
bool _needsToResize(HashTable* hashTable);
HashTable* _resize(HashTable* hashTable);
```

<br>
<br>

## Linked list related operations

We won't be making a compehensive explaination of linked lists, as we have covered this topic in a previoud chapter.
If you have never heard about this data structure, please follow [this link](../02_linked_list/readme.md).
Here we will cover just the necessary operations in order to integrate linked lists with in our hash table.

### Creating a node

When creating a node we want to check that both the key and the value are a valid pointer to a string, and also that their lengths are withing the bounds we want to handle.
Then we will allocate memory for the struct, and copy the key and value to the correpsonding fields in the struct.
At this point, we don't care if the keys already exists in a node of our linked list. That will be handled in a higher scope.

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

Notice that we are using functions declared in `string.h` for string manipulation. Here we are using `strlen` to get the length of a string, it accepts a pointer to a string and returns a number representing the length. Then we are using `strcpy` to copy a string into a buffer.

### Testing node creation

To test the function above we will do the following:

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

For clearing the list we will take a pointer to a head node and then iterate over all it's members. Each node will be freed and then the head will be set to NULL.
To provide feedbak we will return the number of nodes that were removed.

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

A simple test of the happy path can be done by creating a list of an arbitrary number of nodes and then passing the list into our function.
After that we can assert if the returned value corresponds with the number of nodes we created.

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

To remove a node from the list we need to access the list by the head node, then we traverse until we reach the key we are looking for.
When we find it, we free that node and make the previous one point to the node that came after the deleted one.
In case the target node is the head node, we just delete the head and assign the next node as the new head.
In our implementation, we will return a boolean to signal if the process succeeded or not.

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

To test if the node removal is working, we will assert for success or failure using the boolean output of our function.

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

If we really want, we could also traverse the list to assert there is no node with the key we wanted to remove.

### Getting a node value

To traverse the linked list and find a value by a key, we will accept a pointer to the head node, and a key. We will validate that those pointers are not null pointers and, if everything went ok, we will loop over each node comparing the input string with each key of each node.
If we find it, we will create allocate memory for that string and return a pointer to that bufer.
If not, we will return null.
A higher scope will be responisble for freeing that memory.

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

To simplify things we can say that our hash table is just a dynamic aray of pointers to the head of a linked list.
To determine in which linked list a key-value pair should be stored, we pass the key to a hash function that outputs a number between 0 and the number of slots in our array like in the following conceptual example:

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

As we [presented above](#defining-interfaces), our hash table is just a struct contianing an array of pointers to linked lists, and two numbers: one to keep track uf the elements stored and the other to know the array capacity.

Making an instance of this struct is pretty simple, we just provide a number representing a the desired capacity, attempt to allocate all memory needed, and set the defaults as in the following example:

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

We will make some simple asserts for the previous function.
What we want to know if is the capacity and the defaults are correctly set:

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

Now come the part where we start doing nice things with our hash table!
Keep in mind there are three steps that will be kind of abstract for us now. Those are the steps where we check if we need to resize our hash table, the actual resizing process and the operation where we compute the hash. Those "private" functions will be presented after this part.
**Node:** Just for presentation propouses, we will test storing, getting and removing values at the end of the article.

For storing a value we need to perform the following flow:

1. check that our provided inputs are valid
2. check if we need to resize and if so
3. compute the hash for the key, so we know which linked list should store our value
4. traverse the list and see if a node with the same key exists, so we know if we need to update it or append a new node

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

Now that we already have the backbones of our hash table, we need a function that receives a key and outputs a number between that could serve us to index the underlying array used to store the references to heads of our linked lists. As we grow the array, we need to be able to update the ranges in which this number should go.
For our implementation we will be using a simplified version of a [rolling hash strategy](https://en.wikipedia.org/wiki/Rolling_hash), where we iterate over each character, producing a hash, and then using that hash to compute the next one. In each iteration we will be taking the _modulo_ to constrain the bounds where our results can exist.

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

What we want to test now is if, at least, our function outputs the same values for a given key. We will do a simple test to give us an idea of how consistent the results are when playing with the upper limit for the result, and by reproducing the same hash several times.
We will make a simple test, this is not a real proof of consistency, neither a proof for homogeneous distribution of the output. Doing that would require a formal (logical and mathemathical) demostration.

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

The moment where we decide to resize our hashing table is somewhat arbitrary. A common practice is to calculate a ratio between the buckets or linked lists available and the number of elements we have stored in our data structure.

To perform this operation we will accept a hash table and then calculate if one more element would cause the ratio to overpass our selected limit.

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



