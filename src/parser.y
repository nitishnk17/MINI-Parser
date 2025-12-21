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

%type <node> expression term factor assignment program statement_list statement block variable_decl if_statement

/* Precedence (Lowest to Highest) */
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/'


%%

program:
    statement_list      { root = $1; }
    ;

statement_list:
    statement                   { $$ = $1; }
    | statement statement_list  { $$ = createSequenceNode($1, $2); }
    ;

statement:
    variable_decl       { $$ = $1; }
    | assignment ';'    { $$ = $1; }
    | if_statement      { $$ = $1; }
    | block             { $$ = $1; }
    | expression ';'    { $$ = $1; } 
    ;

block:
    '{' statement_list '}'  { $$ = $2; }
    ;

variable_decl:
    VAR IDENTIFIER ';'                  { $$ = createVarDeclNode($2, NULL); }
    | VAR IDENTIFIER '=' expression ';' { $$ = createVarDeclNode($2, $4); }
    ;

assignment:
    IDENTIFIER '=' expression   { $$ = createAssignmentNode($1, $3); }
    ;

if_statement:
    IF '(' expression ')' statement                 { $$ = createIfNode($3, $5, NULL); }
    | IF '(' expression ')' statement ELSE statement { $$ = createIfNode($3, $5, $7); }
    ;

expression:
    expression '+' expression { $$ = createOperatorNode('+', $1, $3); }
    | expression '-' expression { $$ = createOperatorNode('-', $1, $3); }
    | expression '*' expression { $$ = createOperatorNode('*', $1, $3); }
    | expression '/' expression { $$ = createOperatorNode('/', $1, $3); }
    | expression '<' expression { $$ = createOperatorNode('<', $1, $3); }
    | expression '>' expression { $$ = createOperatorNode('>', $1, $3); }
    | expression EQ expression  { $$ = createOperatorNode(EQ, $1, $3); }
    | expression NE expression  { $$ = createOperatorNode(NE, $1, $3); }
    | expression LE expression  { $$ = createOperatorNode(LE, $1, $3); }
    | expression GE expression  { $$ = createOperatorNode(GE, $1, $3); }
    | '(' expression ')'        { $$ = $2; }
    | term                      { $$ = $1; }
    ;

term:
    factor              { $$ = $1; }
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