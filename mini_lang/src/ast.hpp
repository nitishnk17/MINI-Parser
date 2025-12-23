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

//AST Repr- VarExpr("x") : Node type: VarExpr, name stored: "x"

struct VarExpr : Expr {     //Stores the name of a variable when it's used in an expression
    std::string name;
    explicit VarExpr(const std::string& n): name(n) {}
};

// Binary expression: a+b
struct BinaryExpr : Expr{       //Stores an operation between two expressions
    char op;        // operation to perform(+,-,*,/....)
    Expr* left;     // Expr* because it can be int,var,expression
    Expr* right;

    BinaryExpr(char oper, Expr* l, Expr* r): op(oper), left(l), right(r) {}

    ~BinaryExpr() {     //without this memory leak, the child nodes would stay in memmory forever
        delete left;
        delete right;
    }
};

// ------------- Statements( does not return values) ----------------
// statements means code that perfoems an action or controls flows 
//like var x=10, x=10, if(x>3){...}, while(i<2){....}

struct Stmt : ASTNode{};


//var x;
// VarDecStmt("x") -> type: VarDeclStmt, name: "x" default value is 0
struct VarDeclStmt : Stmt{
    std::string name;
    explicit VarDeclStmt(const std::string& n) : name(n) {}
};


// x=expr (x=5+3)
// EXECUTION: When this node is executed:
// Evaluate the expression on right (e.g, 5 + 3 = 8)
// Look up variable name in symbol table
// Update its value to the result (x becomes 8)
struct AssignStmt : Stmt {
    std::string name;
    Expr* expr;

    AssignStmt(const std::string& n, Expr* e) : name(n), expr(e) {}

    ~AssignStmt() {
        delete expr;
    }
};

//var x=expr;
// WHY SEPARATE FROM VarDeclStmt AND AssignStmt?:
// Grammar has separate rules for:
// var x;        (declaration only)
// var x = 10;   (declaration with init)
// This is ONE statement that does both in user's code
struct VarDeclInitStmt : Stmt{
    std::string name;

    Expr* expr;

    VarDeclInitStmt(const std:: string& n, Expr* e) : name(n), expr(e) {}

    ~VarDeclInitStmt(){
        delete expr;
    }
};


// if (condition) thenStmt else elseStmt
/*
 *   if (x > 5)              // Only condition and then-branch
 *       y = 10;
 *
 *   if (x > 5)              // Condition, then-branch, and else-branch
 *       y = 10;
 *   else
 *       y = 20;

 WHY THREE POINTERS?:
   - condition: Expression to test (must evaluate to number)
   - thenStmt: Statement to execute if condition is true
   - elseStmt: Statement to execute if condition is false (optional)
*/
struct IfStmt : Stmt{

    Expr* condition;

    Stmt* thenStmt;

    Stmt* elseStmt;

    IfStmt(Expr* cond, Stmt* thenS, Stmt* elseS=nullptr) : condition(cond), thenStmt(thenS), elseStmt(elseS) {}

    ~IfStmt(){
        
        delete condition;
        delete thenStmt;

        if(elseStmt) delete elseStmt;
    }
};

//While
/*
 *   while (x < 10)          // Simple while
 *       x = x + 1;
 *
 *   while (i < 5) {         // While with block
 *       sum = sum + i;
 *       i = i + 1;
 *   }
 * 
 * 
*/

struct WhileStmt : Stmt {

    Expr* condition;

    Stmt* body;

    WhileStmt(Expr* cond, Stmt* b) : condition(cond), body(b) {}

    ~WhileStmt(){
        delete condition;
        delete body;
    }
};

// Block Statement: {stmt1;stmt2; stmt3; ... }
/*
*   {
 *       var x = 10;
 *       var y = 20;
 *       var sum = x + y;
 *   }
 * 
*/
struct BlockStmt : Stmt {
    
    std::vector<Stmt* > statements;

    explicit BlockStmt(const std::vector<Stmt*>& stmts) : statements(stmts) {}

    ~BlockStmt(){
        for(Stmt* s : statements){
            delete s;
        }
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