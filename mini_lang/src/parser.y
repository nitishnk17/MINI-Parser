// contains: token declarations, grammar rules , semantic hooks
// bison reads it and generates parser.tab.c (or .cpp) and parser.tab.h


%{
    #include<cstdio>
    #include<cstdlib>
    #include "ast.hpp"
    #include <vector>

    std::vector<Stmt*> programStatements;
    void execStmt(Stmt* stmt);
    void printSymbolTable();
    void printStmt(Stmt* stmt, int indent);

    int yylex();
    void yyerror(const char *s);

    // forward declarations for union
    struct Expr;
    struct Stmt;
%}

%union {
    int ival;
    char* sval;
    struct Expr* expr;
    struct Stmt* stmt;

    std::vector<Stmt*>* stmt_list;
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

%type <stmt> statement variable_decl assignment if_statement while_statement block
%type <stmt_list> block_statements 
%type <expr> expression equality comparision term factor primary unary

%%
program:
    statement_list
    ;

statement_list:
    statement_list statement{
        programStatements.push_back($2);
    }
    |
    ;

statement:
    variable_decl{
            $$ =$1;
        }
    | assignment {
            $$=$1;
        }
    | if_statement {
            $$ = $1;
        }
    | while_statement {
            $$ = $1;
        }
    | block {
            $$ = $1;
        }
    ;

variable_decl:
    VAR IDENTIFIER SEMICOLON {
        $$ = new VarDeclStmt($2);
        free($2);
    }
    | VAR IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = new VarDeclInitStmt($2,$4);
        free($2);
    }
    ;

assignment:
    IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = new AssignStmt($1, $3);
        free($1);
    }
    ;

if_statement:
    IF LPAREN expression RPAREN statement {
        $$ = new IfStmt($3,$5);
    }
    | IF LPAREN expression RPAREN statement ELSE statement {
        $$ = new IfStmt($3,$5,$7);
    }
    ;

while_statement:
    WHILE LPAREN expression RPAREN statement{
        $$ = new WhileStmt($3, $5);
    }
    ;

block:
    LBRACE block_statements RBRACE {
        $$ = new BlockStmt(*$2);

        delete $2;
    }
    ;

block_statements:
    block_statements statement {
        $$ = $1;
        $$->push_back($2);
    }
    |{
        $$ = new std::vector<Stmt*>();
    }
    ;

expression:
    equality {
        $$ = $1;
    }
    ;

equality:
    equality EQ comparision {
        $$ = new BinaryExpr('E',$1,$3);
    }
    | equality NEQ comparision {
        $$ = new BinaryExpr('N',$1,$3);
    }
    | comparision {
        $$ = $1;
    }
    ;

comparision:
    comparision LT term{
        $$ = new BinaryExpr('<',$1,$3);
    }
    | comparision GT term {
        $$ = new BinaryExpr('>',$1,$3);
    }
    | comparision LE term {
        $$ = new BinaryExpr('L', $1, $3);

    }
    | comparision GE term {
        $$ = new BinaryExpr('G',$1,$3);
    }
    | term {
        $$ = $1;
    }
    ;


term:
        term PLUS factor {
            $$ = new BinaryExpr('+', $1, $3);
        }
    |   term MINUS factor {
            $$ = new BinaryExpr('-',$1, $3);
        }
    |   factor {
            $$ =$1;
        }

    ;


factor:
        factor MUL unary {
            $$ = new BinaryExpr('*', $1, $3);
        }
    |   factor DIV unary {
            $$ = new BinaryExpr('/',$1, $3);
        }
    |   unary {
            $$ =$1;
        }

    ;


unary:
    PLUS unary{
        $$ = $2;
    }
    | MINUS unary {
        $$ = new BinaryExpr('n', new IntExpr(0), $2);
    }
    | primary {
        $$ = $1;
    }
    ;

primary:
    INTEGER{
        $$ = new IntExpr($1);
    }
    | IDENTIFIER {
        $$ = new VarExpr($1);
        free($1);
    }
    | LPAREN expression RPAREN {
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

    // print ast (syntax tree)
    printf("\n==== Abstract Syntax Tree ====\n");
    for(Stmt* s:programStatements){
        printStmt(s, 0);
    }

    //execute program
    for(Stmt* s:programStatements){
        execStmt(s);
    }

    printSymbolTable();

    // cleanup: delete all ast nodes
    for(Stmt* s:programStatements){
        delete s;
    }
    programStatements.clear();

    return 0;
}

