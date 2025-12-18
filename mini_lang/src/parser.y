// Contains: Token Declarations, Grammar rules , Semantic hooks
// Bison reads it and generates parser.tab.c (or .cpp) and parser.tab.h

%{
    #include<cstdio>
    #include<cstdlib>

    int yylex();
    void yyerror(const char *s);
%}

%token VAR IF ELSE WHILE
%token IDENTIFIER INTEGER
%token PLUS MINUS MUL DIV
%token ASSIGN
%token SEMICOLON
%token LPAREN RPAREN
%token LBRACE RBRACE
%token EQ NEQ LT GT LE GE

%%
program:

    ;


%%

void yyerror(const char *s){
    printf("Syntax error: %s\n",s);
}

int main(){
    printf("Parsing started.......\n");
    yyparse();
    printf("Parsing finished.\n");
    return 0;
}

