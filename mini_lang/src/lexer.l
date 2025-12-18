%option noyywrap

%{
#include<cstdio>
#include<cstdlib>
#include "parser.tab.h"
%}

%% // starting rules
[ \t\n]+    ;

"var"           { return VAR; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }


[0-9]+          { return INTEGER; }

[a-zA-Z][a-zA-Z0-9]*    { return IDENTIFIER; }

"=="            { return EQ; }
"!="            { return NEQ; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }

"="             { return ASSIGN; }
";"             { return SEMICOLON; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }

"//".*          ;

.               { printf("Unknown token: %s\n", yytext); }

%%    // ending rule
