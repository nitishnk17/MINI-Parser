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
    char* stri;
}

%token <ival> INTEGER

%token <stri> IDENTIFIER

%token VAR IF ELSE WHILE
%token EQ NE LE GE

%type <node> expression term factor assignment program statement_list statement

%%

program:
    statement_list      { root = $1; }
    ;

statement_list:
    statement                   { $$ = $1; }
    | statement statement_list  { $$ = createSequenceNode($1, $2); }
    ;

statement:
    assignment ';'      { $$ = $1; }  
    | expression ';'    { $$ = $1; }  
    ;

assignment:
    IDENTIFIER '=' expression   { $$ = createAssignmentNode($1, $3); }
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
    INTEGER             { $$ = createNumberNode($1); } 
    | IDENTIFIER        { $$ = createVariableNode($1); }
    | '(' expression ')' { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "This error occurred:  %s\n", s);
}