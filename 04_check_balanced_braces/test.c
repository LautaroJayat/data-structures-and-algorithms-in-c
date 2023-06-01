#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "checker.h"



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