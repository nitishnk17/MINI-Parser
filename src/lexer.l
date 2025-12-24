%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h" /* Tokens are defined in parser.y */
#include <string.h>
%}

%option noyywrap
%option yylineno
%option noinput nounput


%%

[0-9]+      { yylval.ival = atoi(yytext); return INTEGER; }

"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"="         { return '='; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }

";"         { return ';'; }

"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"<"                 { return '<'; }
">"                 { return '>'; }

"var"               { return VAR; }   
"if"                { return IF; }    
"else"              { return ELSE; }  
"while"             { return WHILE; } 

    /*  Ignore Whitespace (spaces, tabs, newlines) */
[ \t\n\r]   { /* Do nothing */ }

    /* Variable name matching */
[a-zA-Z][a-zA-Z0-9]* { yylval.stri = strdup(yytext); return IDENTIFIER; } 

    /* Catch-all for errors */
.                   { fprintf(stderr, "Unknown character %c\n", *yytext); }

"//".* { /* Ignore single-line comments */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignore multi-line comments */ }

%%

