**[WORK IN PROGRESS]**

# Hash Table with spread chaining for colition resolution

**Table of contents**

- [What is a hash table?](#what-is-a-hash-table)
- [A collision resolution strategy](#a-collision-resolution-strategy)
- [Implementing the hash table](#implementing-the-hash-table)
  - [Defining interfaces](#defining-interfaces)
  - [Creating a node](#creating-a-node)
  - [Testing node creation](#testing-node-creation)
  - [Clearing the list](#clearing-the-list)
  - [Testing list deletion](#testing-list-deletion)
  - [Removing a node from the list](#removing-a-node-from-the-list)
  - [Testing node removal](#testing-node-removal)

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

## Implementing the hash table

### Defining interfaces

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
bool Store(HashTable* hashTable, char* key, char* value);
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
void _resize(HashTable** hashTable);
```

#### Creating a node

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

#### Testing node creation

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

#### Clearing the list

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
        // we store the next node
        tmp = head->next;
        // free the current node
        free(head);
        // and assign the next node as the current node
        head = tmp;
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

#### Testing list deletion

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

#### Removing a node from the list

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

#### Testing node removal
