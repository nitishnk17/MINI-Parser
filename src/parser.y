%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

int yylex(void);
void yyerror(const char *s);
struct ASTNode *root = NULL;
%}

%union {
    int ival;
    struct ASTNode *node;
}

%token <ival> INTEGER

%type <node> expression term factor

%%

program:
    expression          { root = $1; }
    ;

expression:
    term                { $$ = $1; }
    | expression '+' term { $$ = createOperatorNode('+', $1, $3); }
    | expression '-' term { $$ = createOperatorNode('-', $1, $3); }
    ;

term:
    factor              { $$ = $1; }
    | term '*' factor   { $$ = createOperatorNode('*', $1, $3); }
    | term '/' factor   { $$ = createOperatorNode('/', $1, $3); }
    ;

factor:
    INTEGER             { $$ = createNumberNode($1); } /* Convert raw int to AST Node */
    | '(' expression ')' { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}