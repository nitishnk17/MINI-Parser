%{
#include <stdio.h>
#include <stdlib.h>

/* We need to declare these so the parser knows they exist */
int yylex(void);
void yyerror(const char *s);
%}

/* This union holds the actual values of our tokens */
%union {
    int ival;
}

/* Tokens coming from the lexer */
%token <ival> INTEGER

/* Non-terminals (the logic blocks we define below) */
%type <ival> expression term factor

%%

/* The starting point of our program */
program:
    expression          { printf("Result: %d\n", $1); }
    ;

/* Level 1: Addition and Subtraction (Lowest Priority) */
expression:
    term                { $$ = $1; }
    | expression '+' term { $$ = $1 + $3; }
    | expression '-' term { $$ = $1 - $3; }
    ;

/* Level 2: Multiplication and Division (Higher Priority) */
term:
    factor              { $$ = $1; }
    | term '*' factor   { $$ = $1 * $3; }
    | term '/' factor   {
                          if($3 == 0) {
                              yyerror("Cannot divide by zero!");
                              exit(1);
                          }
                          $$ = $1 / $3;
                        }
    ;

/* Level 3: The Base Elements (Highest Priority) */
factor:
    INTEGER             { $$ = $1; }
    | '(' expression ')' { $$ = $2; }
    ;

%%

/* Error handling function */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}