#ifndef AST_HPP
#define AST_HPP

#include<memory>
#include<string>
#include<vector>

struct  ASTNode
{
    virtual ~ASTNode()=default; //When deleting a node, delete it properly (Without this: Child objects may not be destroyed correctly, Memory problems can happen)
};

// using ASTNodePtr=std::shared_ptr<ASTNode>;  // create a type alias typedef std::shared_ptr<ASTNode> ASTNodePtr;

// -------Expressions ----------

struct Expr: ASTNode{};

// using ExprPtr=std::shared_ptr<Expr>;

//Integer literal: 10

struct IntExpr: Expr{
    int value;
    explicit IntExpr(int v):value(v) {}
};

// variable refernce : x
struct VarExpr : Expr {
    std::string name;
    explicit VarExpr(const std::string& n): name(n) {}
};

// Binary expression: a+b
struct BinaryExpr : Expr{
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