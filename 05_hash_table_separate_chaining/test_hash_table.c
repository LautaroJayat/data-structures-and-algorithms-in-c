#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash_table.h"

void _testNewNode() {
    char* s;
    char* goodString = "hey how are you?";
    int i;
    char badString[MAX_KEY_LEN + 3000];
    for (i = 0; i < MAX_KEY_LEN + 20; i++) {
        badString[i] = *"!";
    }

    Node* newNode = CreateNode(goodString, goodString);
    s = newNode->key;
    assert(*s == *goodString);
    s = newNode->value;
    assert(*s == *goodString);
    free(newNode);
    printf("Testing with good string: PASS\n");


    Node* newNode2 = CreateNode(NULL, goodString);
    assert(newNode2 == NULL);
    free(newNode2);
    printf("Testing with null string pointer: PASS\n");

    Node* newNode3 = CreateNode((char*)badString, goodString);
    assert(newNode3 == NULL);
    free(newNode3);
    printf("Testing with bad string: PASS\n");


}

void _testClearList() {
    char goodString[] = "to delete";
    unsigned int expectedNodes = 10;
    unsigned int i;
    Node* head = CreateNode(goodString, goodString);
    for (i = 1; i < expectedNodes; i++) {
        Node* tmp = head;
        head = CreateNode(goodString, goodString);
        head->next = tmp;
    };
    unsigned int clearedNodes = ClearList(&head);
    assert(clearedNodes == expectedNodes);
}

void _testRemoveNode() {
    char notToDelete[] = "do not delete me";
    char toDelete[] = "to delete";

    int i;
    printf("Testing deletion of the last node\n");
    Node* head = CreateNode(toDelete, toDelete);
    for (i = 1; i < 4; i++) {
        Node* tmp = head;
        head = CreateNode(notToDelete, notToDelete);
        head->next = tmp;
    };
    bool success = RemoveNode(&head, toDelete);
    assert(success == true);

    printf("Testing deletion of node in the middle\n");
    Node* deleteMeNode = CreateNode(toDelete, toDelete);
    deleteMeNode->next = head;
    head = deleteMeNode;
    for (i = 1; i < 4; i++) {
        Node* tmp = head;
        head = CreateNode(notToDelete, notToDelete);
        head->next = tmp;
    };
    success = RemoveNode(&head, toDelete);
    assert(success == true);

    printf("Testing deletion of the head node\n");
    deleteMeNode = CreateNode(toDelete, toDelete);
    deleteMeNode->next = head;
    head = deleteMeNode;
    success = RemoveNode(&head, toDelete);
    assert(success == true);

    printf("Testing early return when for empty lists\n");
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

void _testGetNodeValue() {
    char aKey[] = "meh";
    char aValue[] = "meh";
    char targetKey[] = "target key";
    char targetValue[] = "you found me";
    char missingKey[] = "im not there";
    int i;

    printf("Testing get value by key\n");
    Node* head = CreateNode(targetKey, targetValue);
    for (i = 1; i < 4; i++) {
        Node* tmp = head;
        head = CreateNode(aKey, aValue);
        head->next = tmp;
    };

    char* result = GetNodeValue(head, targetKey);
    assert(result != NULL);
    assert(strcasecmp(targetValue, result) == 0);
    free(result);

    result = GetNodeValue(head, missingKey);
    assert(result == NULL);
    free(result);

    while (head != NULL) {
        Node* tmp = head->next;
        free(head);
        head = tmp;
    }
}

void _testCreateHashTable() {
    HashTable* hashTable = CreateHashTable(3);
    assert(hashTable->capacity == 10);
    free(hashTable->collection);
    free(hashTable);
    hashTable = CreateHashTable(15);
    assert(hashTable->capacity == 15);
    unsigned int i;
    for (i = 0; i < 15; i++) {
        assert(hashTable->collection[i] == NULL);

    }
    free(hashTable->collection);
    free(hashTable);
}

void _testCreateHashConsistency() {
    char* string = "hey, how are you";
    int i, j;
    for (j = 10; j < 1000; j++) {
        unsigned int hash = _computeHash(string, j);
        for (i = 0; i < 100; i++) {
            assert(hash == _computeHash(string, j));
        }
    }
}

void _testStoreGetAndRemove() {
    char* keys[] = { "key1","key2","key3","key4","key5","key6" };
    HashTable* hashTable = CreateHashTable(15);
    int i;
    for (i = 0; i < 6; i++) {
        bool success = Store(hashTable, keys[i], keys[i]);
        assert(success == true);
    }
    for (i = 0; i < 6; i++) {
        char* value = Get(hashTable, keys[i]);
        assert(strcmp(value, keys[i]) == 0);
        free(value);
    }
    char* defaultValue = "default";
    for (i = 0; i < 6; i++) {
        bool success = Store(hashTable, keys[i], defaultValue);
        assert(success == true);
    }
    for (i = 0; i < 6; i++) {
        char* value = Get(hashTable, keys[i]);
        assert(strcmp(value, defaultValue) == 0);
        free(value);
    }
    assert(Get(hashTable, "missingKey") == NULL);
    bool success = Remove(hashTable, keys[1]);
    assert(success == true);
    assert(Get(hashTable, keys[1]) == NULL);
    for (i = 0; i < hashTable->capacity; i++) {
        Node* n = hashTable->collection[i];
        ClearList(&n);
    }
    free(hashTable->collection);
    free(hashTable);

}

int main(void) {
    _testNewNode();
    _testClearList();
    _testRemoveNode();
    _testGetNodeValue();
    _testCreateHashTable();
    _testCreateHashConsistency();
    _testStoreGetAndRemove();
    return 0;
}