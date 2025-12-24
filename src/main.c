#include <stdio.h>
#include <stdlib.h> // Required for atoi
#include "y.tab.h"
#include "ast.h"


extern struct ASTNode* root; 



int main() {
    // yyparse returns 0 if parsing was successful
    if (yyparse() == 0) {
        int result = evaluate(root);
        printf("Result: %d\n", result);
        return 0;
    } else {
        return 1;
    }
}