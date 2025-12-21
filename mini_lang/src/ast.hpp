#ifndef AST_HPP
#define AST_HPP

#include<memory>
#include<string>
#include<vector>

struct  ASTNode     //base class for all the nodes
{
    virtual ~ASTNode()=default; //When deleting a node, delete it properly (Without this: Child objects may not be destroyed correctly, Memory problems can happen)
};


// -------Expressions (code that calculates and return a value) ----------

struct Expr: ASTNode{};     //Base class for all the expressions


//Integer literal: 10
//": Expr" means IntExpr is a type of Expr ie inheritance 
struct IntExpr: Expr{   //store the number that apppears directly in the code var x=44 (AST -> nodetype: IntExpr, value= 44)
    int value;
    explicit IntExpr(int v):value(v) {} //explicit - prevents accidental conversions. (Without it, c++ might automatically converts int to IntExpr in unexpected ways. We want to be explicit means I am creating an IntExpr)
};

// variable refernce : x
struct VarExpr : Expr {     //Stores the name of a variable when it's used in an expression
    std::string name;
    explicit VarExpr(const std::string& n): name(n) {}
};

// Binary expression: a+b
struct BinaryExpr : Expr{       //Stores an operation between two expressions
    char op;
    Expr* left;
    Expr* right;

    BinaryExpr(char oper, Expr* l, Expr* r): op(oper), left(l), right(r) {}

    ~BinaryExpr() {
        delete left;
        delete right;
    }
};

// ------------- Statements ----------------

struct Stmt : ASTNode{

};

// using StmtPtr = std::shared_ptr<Stmt>;

//var x;
struct VarDeclStmt : Stmt{
    std::string name;
    explicit VarDeclStmt(const std::string& n) : name(n) {}
};

// x=expr
struct AssignStmt : Stmt {
    std::string name;
    Expr* expr;

    AssignStmt(const std::string& n, Expr* e) : name(n), expr(e) {}

    ~AssignStmt() {
        delete expr;
    }
};


#endif


/*
Why shared_ptr?

In an AST:

Nodes are connected
Multiple parts may refer to same node
Manual delete is error-prone

shared_ptr:

Automatic memory management
Reference counting
No memory leaks

*/