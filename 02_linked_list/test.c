#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

void _cleanUp(Node** head) {
    int shouldStop = 0;
    while (shouldStop == 0) {
        shouldStop = RemoveFromNthPosition(head, 0);
    }
}

int _testLinkedListOrdering(int32_t desiredNodeNumber) {
    int32_t nodeNumbers = desiredNodeNumber;
    int32_t i;
    Node* head;
    head = NULL;
    for (i = 0; i < nodeNumbers; i++) {
        InsertToHead(&head, nodeNumbers - 1 - i);
    }
    Node* currentNode = head;
    for (i = 0; i < nodeNumbers; i++) {
        assert(currentNode->data == i);
        currentNode = currentNode->next;
    }
    _cleanUp(&head);
    assert(head == NULL);
}

void _testRemoveFromLastPosition(int32_t desiredNodeNumber) {
    int32_t nodeNumbers = desiredNodeNumber;
    int32_t i;
    Node* head;
    head = NULL;
    for (i = 0; i < nodeNumbers; i++) {
        InsertToHead(&head, nodeNumbers - 1 - i);
    }

    Node* currentNode = head;

    for (i = 0; i < nodeNumbers; i++) {
        assert(currentNode->data == i);
        currentNode = currentNode->next;
    }

    RemoveFromNthPosition(&head, nodeNumbers - 1);

    currentNode = head;

    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }

    assert(currentNode->next == NULL);
    assert(currentNode->data + 1 == nodeNumbers - 1);

}


void _testInsertionsInSomePlace(int32_t desiredNodeNumber) {
    int32_t EXPECTED_NUMBER = 69;
    int32_t insertAtPosition = desiredNodeNumber % 69;
    int32_t nodeNumbers = desiredNodeNumber;
    int32_t i;
    Node* head;
    head = NULL;
    for (i = 0; i < nodeNumbers; i++) {
        InsertToHead(&head, nodeNumbers - 1 - i);
    }
    int err = InsertAtNthPosition(&head, EXPECTED_NUMBER, insertAtPosition);
    assert(err == 0);
    Node* currentNode = head;
    for (i = 0; i <= nodeNumbers; i++) {
        if (i < insertAtPosition) {
            assert(currentNode->data == i);
        }
        else if (i == insertAtPosition) {
            assert(currentNode->data == EXPECTED_NUMBER);
        }
        else {
            assert(currentNode->data == i - 1);
        }
        currentNode = currentNode->next;
    }
    _cleanUp(&head);
    assert(head == NULL);
}

void _testInsertAtTheBeginingAndTheEnd(int32_t desiredNodeNumber) {
    int32_t EXPECTED_NUMBER = 69;
    int32_t end = desiredNodeNumber + 1;
    int32_t start = 0;
    int32_t nodeNumbers = desiredNodeNumber;
    int32_t i;
    Node* head;
    head = NULL;
    for (i = 0; i < nodeNumbers; i++) {
        InsertToHead(&head, nodeNumbers - 1 - i);
    }
    int err = InsertAtNthPosition(&head, EXPECTED_NUMBER, start);
    assert(err == 0);
    err = InsertAtNthPosition(&head, EXPECTED_NUMBER, end);
    assert(err == 0);
    assert(head->data == EXPECTED_NUMBER);
    i = 0;
    Node* curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
        i++;
    }
    assert(curr->data == EXPECTED_NUMBER);
    assert(i == end);
    assert(curr->next == NULL);
    _cleanUp(&head);
    assert(head == NULL);
}

void TestLinkedListOrdering() {
    int32_t testCases[4] = { 10, 100, 1000, 10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testLinkedListOrdering(testCases[i]);
    }
}

void TestInsertAtSomePlace() {
    int32_t testCases[4] = { 10,100,1000,10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testInsertionsInSomePlace(testCases[i]);
    }
}

void TestInsertAtTheBeginingAndTheEnd() {
    int32_t testCases[4] = { 10,100,1000,10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testInsertAtTheBeginingAndTheEnd(testCases[i]);
    }
}

void TestRemoveFromLastPosition() {
    int32_t testCases[4] = { 10,100,1000,10000 };
    int i, len;
    len = sizeof(testCases) / sizeof(testCases[0]);
    for (i = 0; i < len; i++) {
        _testRemoveFromLastPosition(testCases[i]);
    }
}

int main(void) {
    TestLinkedListOrdering();
    TestInsertAtSomePlace();
    TestInsertAtTheBeginingAndTheEnd();
    TestRemoveFromLastPosition();
    return 0;
}
