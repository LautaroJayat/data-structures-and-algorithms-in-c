# Checking if parentheses are balanced in a string

**Table of contents:**

- [The strategy](#the-strategy)
- [The Implementation](#the-implementation)
- [Performing some tests](#performing-some-tests)
- [Link to the source](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/04_check_balanced_braces)

This is an interesting example of how a stack can be used to solve a real problem. If you have programming experience or familiarity with a programming language, you likely have an intuition about what balanced parentheses are. We say that a string has balanced parentheses when each opening parenthesis has a corresponding closing parenthesis in the right position. For example, the following strings are balanced:

```sh
(hey)(how)are(you?)

(((hey))) how are (you)?

((hey))(how)(are(you)?)
```

But, the following strings are unbalanced:

```sh
# it has the correct number of pairs but in bad order
)(hey) how are you(

# it has one closing parentheses with no opening counterpart
(hey) how are (you)) ?

# it is the lamest string of all
(((( hey how are you?
```

we might be interested in performing these kinds of checks to ensure that homologous syntactic rules are correct. For example, we may want to verify if a string has been closed correctly or if our Lisp program will work properly. These checks are essential for maintaining the integrity and correctness of our code and programs.

## The strategy

The strategy we will employ is quite straightforward.
For each parenthesis encountered, we will perform the following steps:

1. If it is an opening parenthesis, we will push it onto the stack.
2. If it is a closing parenthesis, we will peek at the element on the top of the stack:

- If the top element is an opening parenthesis, we will pop it, discarding both the popped element and the current element.
- If the top element is a closing parenthesis, we will push the current element onto the stack.

In the end, we need to check if the stack is empty. If it is empty, it means that each parenthesis had its counterpart in the right place. However, if there are remaining elements in the stack, it implies that the string was not properly balanced.

To illustrate this example:

```sh
string = "((hey))"
stack = []

# we start checking character by character in the string
# the first one is "(" si we push it
Push(stack, "(") #=> stack contains ["("]

# then we encounter another opening parentheses so we push it
Push(stack, "(") #=> stack contains ["(", "("]

# then the next 3 elements are discarded as they
# are meaningless

# but the 4th one is ")"
# so it cancels the last element we added
Pop(stack,...)  #=> stack contains ["("]

# we repeat the operation and out stack is now empty
Pop(stack,...)  #=> stack contains ["("]

if (stack->length > 0){
    return unbalanced
}

return balanced
```

## The implementation

For the implementation we will be using the same stack we implemented in [chapter 01](../01_stack_array_implementation/readme.md).
The only difference will be the type of data to be stored in the underlying array:

```c
typedef struct {
// we use "int" to represent our characters
    int* collection;
    uint32_t capacity;
    uint32_t size;
} Stack;
```

Again, we will be using almost the same operations shown in the example of chapter 01:

```c
// the signature of our functions
Stack* CreateNewStack(uint32_t capacity);
void DestroyStack(Stack** stackp);
bool Is_Empty(Stack* stack);
bool Push(Stack* stack, int item);
bool Pop(Stack* stack, int* poppedItem);
bool Peek(Stack* stack, int* peekedItem);
```

First, we will define certain constants to prevent code errors caused by mistyping characters. Subsequently, in the code, we will reference their values using the names assigned to these variables:

```c
const int OPEN_BRACE = '(';
const int CLOSING_BRACE = ')';
const int END_OF_STRING = 0;
```

The actual operations we will perform are as follows:

1. Accept a pointer to a string.
2. Count the characters to determine the required space for the stack.

- If the string is empty, we return early.
- If not, we continue.

3. Create a new stack.
4. Loop over each character and:

- If the character is an opening parentheses, store it in the stack.
- If the character is a closing parentheses, check if the element at the top is an opening parentheses. If it is, pop the opening one and discard both. If it is not, push the closing one.

5. After checking all characters, exit the loop and assign the size to a variable.
6. Destroy the stack.
7. If the size is 0, return true; otherwise, return false.

The implementation can be seen in the following snippet:

```c
// We receive the string
bool IsABalancedString(char* input) {
    int stringLength = 0;
    // we loop over the string until the end and count each iteration
    while (input[stringLength] != END_OF_STRING) {
        stringLength++;
    }

    // if the counter is 0, there was no character in the string
    if (stringLength == 0) {
        printf("empty string provided");
        return true;
    }

    // We create the stack
    Stack* stack = CreateNewStack(stringLength);
    int i = 0;

    // And loop over all the string
    for (i = 0; i < stringLength; i++) {

        // we store the open parentheses
        if (input[i] == OPEN_BRACE) {
            Push(stack, input[i]);
            continue;
        }

        if (input[i] == CLOSING_BRACE) {
            int item = 0;
            // we check what the last element was
            Peek(stack, &item);

            if (item == OPEN_BRACE) {
                // pop if we can cancel it out
                Pop(stack, &item);
                continue;
            }
            // push it if we cant cancel them out
            Push(stack, input[i]);
        }
    }

    // we store the size because we are going to free the stack
    int finalSize = stack->size;
    DestroyStack(&stack);

    // If all elements were cancelled, it was balanced
    if (finalSize == 0) {
        printf("the string was balanced\n");
        return true;
    }

    // at this point, we know it wasn't balanced at all
    printf("the string was unbalanced\n");
    return false;
}
```

## Performing some tests

As we have [tested the stack in chapter 01](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/blob/main/01_stack_array_implementation/test.c), we can now focus on testing this checker function.

The simplest way is to come up with some correct strings, and other bad strings.
And then just assert whether they are balanced or not:

```c
int main() {
    char* balancedStrings[5];
    balancedStrings[1] = "hey";
    balancedStrings[2] = "(hey there)";
    balancedStrings[3] = "(hey (there) pal)";
    balancedStrings[4] = "(((dude (((how are (((you?)))))))))";
    balancedStrings[5] = "()()()()()()()()()()()";

    int i;
    for (i = 0; i < 5; i++) {
        bool result = IsABalancedString(balancedStrings[i]);
        assert(result == true);
    }

    char* unbalancedStrings[5];
    unbalancedStrings[0] = "(hey";
    unbalancedStrings[1] = "(hey )there)";
    unbalancedStrings[2] = "(hey (there) pal))";
    unbalancedStrings[3] = "(((dude (((how are ()((you?)))))))))";
    unbalancedStrings[4] = "()()()()()()()()()())()";

    for (i = 0; i < 5; i++) {
        bool result = IsABalancedString(unbalancedStrings[i]);
        assert(result == false);
    }
    return 0;
}
```
