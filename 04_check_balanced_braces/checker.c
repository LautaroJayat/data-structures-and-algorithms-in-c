#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "stringStack.h"

const int OPEN_BRACE = '(';
const int CLOSING_BRACE = ')';
const int END_OF_LINE = 0;

bool IsABalancedString(char* input) {
    int stringLength = 0;

    while (input[stringLength] != 0) {
        stringLength++;
    }

    if (stringLength == 0) {
        printf("empty string provided");
        return true;
    }

    Stack* stack = CreateNewStack(stringLength);

    int i = 0;

    for (i = 0; i < stringLength; i++) {
        if (input[i] == OPEN_BRACE) {
            Push(stack, input[i]);
            continue;
        }
        if (input[i] == CLOSING_BRACE) {
            int item = 0;
            Peek(stack, &item);

            if (item == OPEN_BRACE) {
                Pop(stack, &item);
                continue;
            }

            Push(stack, input[i]);
        }
    }

    int finalSize = stack->size;
    DestroyStack(&stack);

    if (finalSize == 0) {
        printf("the string was balanced\n");
        return true;
    }

    printf("the string was unbalanced\n");
    return false;

}
