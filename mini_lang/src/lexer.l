%option noyywrap

%{
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include "ast.hpp"
#include "parser.tab.h"
%}

%% // starting rules

[ \t\n]+    ;

"var"           { return VAR; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }


[0-9]+          { yylval.ival = atoi(yytext); return INTEGER; }

[a-zA-Z][a-zA-Z0-9]*    { 
                            yylval.sval = strdup(yytext);
                            return IDENTIFIER;
                    }

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
