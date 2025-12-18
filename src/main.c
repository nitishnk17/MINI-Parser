#include <stdio.h>
#include "y.tab.h" /* Import definitions from Bison */

int main() {
    /* yyparse() returns 0 if parsing was successful, 1 otherwise */
    return yyparse(); 
}