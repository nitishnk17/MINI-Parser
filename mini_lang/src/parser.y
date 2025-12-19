// Contains: Token Declarations, Grammar rules , Semantic hooks
// Bison reads it and generates parser.tab.c (or .cpp) and parser.tab.h


%{
    #include<cstdio>
    #include<cstdlib>
    #include "src/ast.hpp"

    int yylex();
    void yyerror(const char *s);

    // Forward declarations for union
    struct Expr;
    struct Stmt;
%}

%union {
    int ival;
    char* sval;
    struct Expr* expr;
    struct Stmt* stmt;
}


%token VAR IF ELSE WHILE
%token PLUS MINUS MUL DIV
%token ASSIGN
%token SEMICOLON
%token LPAREN RPAREN
%token LBRACE RBRACE
%token EQ NEQ LT GT LE GE

%token <ival> INTEGER
%token <sval> IDENTIFIER

%type <stmt> statement variable_decl assignment
%type <expr> expression term factor

%%
program:
    statement_list
    ;

statement_list:
    statement_list statement;
    |
    ;

statement:
    variable_decl
    | assignment 
    ;

variable_decl:
    VAR IDENTIFIER SEMICOLON {
        $$ = new VarDeclStmt($2);
    }
    ;

assignment:
    IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = new AssignStmt($1, $3);
    }
    ;

expression:
        expression PLUS term {
            $$ = new BinaryExpr('+', $1, $3);
        }
    |   expression MINUS term {
            $$ = new BinaryExpr('-',$1,$3);
        }
    |   term {
            $$ = $1;
        }
    ;

term:
        term MUL factor {
            $$ = new BinaryExpr('*', $1, $3);
        }
    |   term DIV factor {
            $$ = new BinaryExpr('/',$1, $3);
        } 
    |   factor {
            $$ =$1;
        }

    ;

factor:
        INTEGER {
            $$ = new IntExpr($1);
        }
    |   IDENTIFIER {
            $$ = new VarExpr($1);
        }
    |   LPAREN expression RPAREN {
            $$ = $2;
        }

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

