#ifndef AST_HPP
#define AST_HPP

#include<memory>
#include<string>
#include<vector>

struct  ASTNode     //base class for all the nodes
{
    virtual ~ASTNode()=default; //when deleting a node, delete it properly (without this: child objects may not be destroyed correctly, memory problems can happen)
};


// -------expressions (code that calculates and return a value) ----------

struct Expr: ASTNode{};     //base class for all the expressions


//integer literal: 10
//": expr" means intexpr is a type of expr ie inheritance 
struct IntExpr: Expr{   //store the number that apppears directly in the code var x=44 (ast -> nodetype: intexpr, value= 44)
    int value;
    explicit IntExpr(int v):value(v) {} //explicit - prevents accidental conversions. (without it, c++ might automatically converts int to intexpr in unexpected ways. we want to be explicit means i am creating an intexpr)
};

// variable refernce : x

//ast repr- varexpr("x") : node type: varexpr, name stored: "x"

struct VarExpr : Expr {     //stores the name of a variable when it's used in an expression
    std::string name;
    explicit VarExpr(const std::string& n): name(n) {}
};

// binary expression: a+b
struct BinaryExpr : Expr{       //stores an operation between two expressions
    char op;        // operation to perform(+,-,*,/....)
    Expr* left;     // expr* because it can be int,var,expression
    Expr* right;

    BinaryExpr(char oper, Expr* l, Expr* r): op(oper), left(l), right(r) {}

    ~BinaryExpr() {     //without this memory leak, the child nodes would stay in memmory forever
        delete left;
        delete right;
    }
};

// ------------- statements( does not return values) ----------------
// statements means code that perfoems an action or controls flows 
//like var x=10, x=10, if(x>3){...}, while(i<2){....}

struct Stmt : ASTNode{};


//var x;
// vardecstmt("x") -> type: vardeclstmt, name: "x" default value is 0
struct VarDeclStmt : Stmt{
    std::string name;
    explicit VarDeclStmt(const std::string& n) : name(n) {}
};


// x=expr (x=5+3)
// execution: when this node is executed:
// evaluate the expression on right (e.g, 5 + 3 = 8)
// look up variable name in symbol table
// update its value to the result (x becomes 8)
struct AssignStmt : Stmt {
    std::string name;
    Expr* expr;

    AssignStmt(const std::string& n, Expr* e) : name(n), expr(e) {}

    ~AssignStmt() {
        delete expr;
    }
};

//var x=expr;
// why separate from vardeclstmt and assignstmt?:
// grammar has separate rules for:
// var x;        (declaration only)
// var x = 10;   (declaration with init)
// this is one statement that does both in user's code
struct VarDeclInitStmt : Stmt{
    std::string name;

    Expr* expr;

    VarDeclInitStmt(const std:: string& n, Expr* e) : name(n), expr(e) {}

    ~VarDeclInitStmt(){
        delete expr;
    }
};


// if (condition) thenstmt else elsestmt
/*
 *   if (x > 5)              // only condition and then-branch
 *       y = 10;
 *
 *   if (x > 5)              // condition, then-branch, and else-branch
 *       y = 10;
 *   else
 *       y = 20;

 why three pointers?:
   - condition: expression to test (must evaluate to number)
   - thenstmt: statement to execute if condition is true
   - elsestmt: statement to execute if condition is false (optional)
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

//while
/*
 *   while (x < 10)          // simple while
 *       x = x + 1;
 *
 *   while (i < 5) {         // while with block
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

// block statement: {stmt1;stmt2; stmt3; ... }
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
why shared_ptr?

in an ast:

nodes are connected
multiple parts may refer to same node
manual delete is error-prone

shared_ptr:

automatic memory management
reference counting
no memory leaks

*/