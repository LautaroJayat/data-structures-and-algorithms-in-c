#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 256
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2


typedef struct Node_T {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct Node_T* next;
} Node;

typedef struct {
    Node** collection;
    unsigned int capacity;
    unsigned int storedElements;
} HashTable;

Node* CreateNode(char* key, char* value);
bool RemoveNode(Node** head, char* key);
unsigned int ClearList(Node** headNode);
char* GetNodeValue(Node* head, char* key);


HashTable* CreateHashTable(unsigned int capacity);
bool Store(HashTable** hashTable, char* key, char* value);
char* Get(HashTable* hashTable, char* key);
bool Remove(HashTable* hashTable, char* key);

unsigned int _computeHash(char* key, unsigned int capacity);
bool _needsToResize(HashTable* hashTable);
HashTable* _resize(HashTable* hashTable);
