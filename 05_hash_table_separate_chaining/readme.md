**[WORK IN PROGRESS]**

# Hash Table with spread chaining for colition resolution

**Table of contents**

1. [What is a hash table?](#what-is-a-hash-table)
2. [A collision resolution strategy](#a-collision-resolution-strategy)
3. [Implementing the hash table](#implementing-the-hash-table)
   - [Defining interfaces](#defining-interfaces)
   - [Creating a node](#creating-a-node)
     - [Testing node creation](#testing-node-creation)

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

##### Testing node creation

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
