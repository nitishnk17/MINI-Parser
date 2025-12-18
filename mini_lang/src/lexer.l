%option noyywrap

%{
#include<cstdio>
#include<cstdlib>
%}

%%  
[ \t\n]+    ;

"var"           {printf("TOKEN: VAR\n");}
"if"            {printf("TOKEN: IF\n");}
"else"          {printf("TOKEN: ELSE\n");}
"while"         {printf("TOKEN: WHILE\n");}

[0-9]+          {printf("INTEGER(%s)\n",yytext);}

[a-zA-Z][a-zA-Z0-9]*    {printf("IDENTIFIER(%s)\n",yytext);}

"=="            {printf("EQ\n");}
"!="            {printf("NEQ\n");}
"<="            {printf("LE\n");}
">="            {printf("GE\n");}
"<"             {printf("LT\n");}
">"             {printf("GT\n");}

"+"             {printf("PLUS\n");}
"-"             {printf("MINUS\n");}
"*"             {printf("MUL\n");}
"/"             {printf("DIV\n");}

"="             {printf("ASSIGN\n");}
";"             {printf("SEMICOLON\n");}
"("             {printf("LPAREN\n");}
")"             {printf("RPAREN\n");}
"{"             {printf("LBRACE\n");}
"}"             {printf("RBRACE\n");}


"//".*          ;

.               {printf("UNKNOWN(%s)\n",yytext);}


%%  


int main() {
    yylex();
    return 0;
}
