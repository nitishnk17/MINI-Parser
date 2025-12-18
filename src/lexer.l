%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h" /* Tokens are defined in parser.y */
%}

%option noyywrap
%option yylineno

%%

    /* 1. Integers */
[0-9]+      { yylval.ival = atoi(yytext); return INTEGER; }

    /* 2. Arithmetic Operators */
"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }

    /* 3. Parentheses */
"("         { return '('; }
")"         { return ')'; }

    /* 4. Ignore Whitespace (spaces, tabs, newlines) */
[ \t\n]     { /* Do nothing */ }

    /* 5. Catch-all for errors (Temporary: stops on unknown chars) */
.           { fprintf(stderr, "Lexer Error: Unknown character '%s' at line %d\n", yytext, yylineno); exit(1); }

%%