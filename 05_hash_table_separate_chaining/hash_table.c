#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"

Node* CreateNode(char* key, char* value) {
    if (key == NULL || value == NULL) {
        printf("error: key and value must be string\n");
        return NULL;
    }

    if (strlen(key) > MAX_KEY_LEN || strlen(value) > MAX_VALUE_LEN) {
        printf("error: key and value should not exceed max lengths\n");
        return NULL;
    }

    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("error: could not allocate memory for new node\n");
        return NULL;
    }
    newNode->next = NULL;
    strcpy(newNode->key, key);
    strcpy(newNode->value, value);
    return newNode;
};

unsigned int ClearList(Node** headNode) {
    Node* head = *headNode;
    Node* tmp;
    unsigned int deletedNodes = 0;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
        deletedNodes++;
    }
    *headNode = NULL;
    printf("deleted %d nodes, linked list is now empty\n", deletedNodes);
    return deletedNodes;
};

bool RemoveNode(Node** headP, char* key) {
    if (headP == NULL || *headP == NULL) {
        printf("could not remove node, the list is empty\n");
        return false;
    }
    Node* currentNode = *headP;

    if (strcmp(currentNode->key, key) == 0) {
        *headP = currentNode->next;
        free(currentNode);
        return true;
    }

    Node* nextNode = currentNode->next;

    while (nextNode != NULL) {
        if (strcmp(nextNode->key, key) == 0) {
            currentNode->next = nextNode->next;
            free(nextNode);
            return true;
        }
        currentNode = nextNode;
        nextNode = nextNode->next;
    }
    printf("node of key %s could not be found\n", key);
    return false;
};

char* GetNodeValue(Node* head, char* key) {
    if (head == NULL) {
        printf("could not find node because the list was empty\n");
        return NULL;
    }

    if (key == NULL) {
        printf("cant check for node value due to nil key\n");
        return NULL;
    }
    Node* tmp = head;
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        return NULL;
    }

    char* buff = malloc(strlen(tmp->value) * sizeof(char*));

    if (buff == NULL) {
        return NULL;
    }
    strcpy(buff, tmp->value);
    return buff;
}

HashTable* CreateHashTable(unsigned int capacity) {
    if (capacity < 4) {
        capacity = 10;
    }

    Node** collection = calloc(capacity, sizeof(Node*));
    if (collection == NULL) {
        printf("error: could not initialize underlying collection\n");
        return NULL;
    }
    HashTable* hashTable = malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        printf("error: could not initialize hash table\n");
        free(collection);
        return NULL;
    }
    hashTable->collection = collection;
    hashTable->capacity = capacity;
    hashTable->storedElements = 0;
    return hashTable;
}

unsigned int _computeHash(char* key, unsigned int capacity) {
    unsigned int hash = 0;
    unsigned int counter = 0;
    unsigned int primeNumber = 31;
    while (key[counter] != '\0') {
        hash = (hash * primeNumber) + key[counter];
        hash = hash % capacity;
        counter++;
    }
    return hash;
}

bool _needsToResize(HashTable* hashTable) {
    float ratio = (float)(hashTable->storedElements + 1) / (float)hashTable->capacity;
    if (ratio > (float)1.5) {
        return true;
    }
    return false;
};

HashTable* _resize(HashTable* hashTable) {
    HashTable* oldHashTable = hashTable;
    HashTable* newHashTable = CreateHashTable(oldHashTable->capacity * GROWTH_FACTOR);

    unsigned int i;
    bool success = true;
    for (i = 0; i < oldHashTable->capacity; i++) {
        Node* currentNode = oldHashTable->collection[i];

        while (currentNode != NULL) {
            success = Store(&newHashTable, currentNode->key, currentNode->value);
            if (!success) break;
            currentNode = currentNode->next;
        }

        if (!success) {
            printf("error: could not complete sesizing\n");
            break;
        }
    }

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

bool Store(HashTable** hashTableP, char* key, char* value) {

    if (hashTableP == NULL || *hashTableP == NULL || key == NULL || value == NULL) {
        printf("error: bad values provided\n");
        return false;
    }
    HashTable* hashTable = *hashTableP;
    if (_needsToResize(hashTable)) {
        printf("needs to resize\n");
        HashTable* newHashTable = _resize(hashTable);
        if (newHashTable != NULL) {
            *hashTableP = newHashTable;
            hashTable = newHashTable;
        }
        else printf("error: could not resize hash table, we will try on next Store operation\n");

    }

    unsigned int position = _computeHash(key, hashTable->capacity);

    Node* head = hashTable->collection[position];

    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
            break;
        }
        head = head->next;
    }

    if (head != NULL) {
        strcpy(head->value, value);
        return true;
    }

    Node* newNode = CreateNode(key, value);

    if (newNode == NULL) {
        printf("error: could not alocate memory for node\n");
        return false;
    }
    newNode->next = hashTable->collection[position];
    hashTable->collection[position] = newNode;
    hashTable->storedElements += 1;
    return true;
}

char* Get(HashTable* hashTable, char* key) {
    if (hashTable == NULL || key == NULL || strlen(key) == 0) {
        printf("error: bad values were provided:\n%p\n%s\n%ld\n", hashTable, key, strlen(key));
        return NULL;
    }
    unsigned int position = _computeHash(key, hashTable->capacity);
    Node* head = hashTable->collection[position];
    char* value = GetNodeValue(head, key);
    return value;
};

bool Remove(HashTable* hashTable, char* key) {
    if (hashTable == NULL || key == NULL || strlen(key) == 0) {
        printf("error: bad values were provided:\n%p\n%s\n%ld\n", hashTable, key, strlen(key));
        return false;
    }
    unsigned int position = _computeHash(key, hashTable->capacity);
    bool success = RemoveNode(&hashTable->collection[position], key);
    return success;
};