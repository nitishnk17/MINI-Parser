// this file contains the execution logic for our abstract syntax tree. while ast.hpp defines what nodes look like (structure), ast.cpp implements what nodes do (behavior).

#include<iostream>
#include "ast.hpp"
#include<map>
#include<string>
#include<stdexcept>

void printSymbolTable();
void printExpr(Expr* expr, int indent);
void printStmt(Stmt* stmt, int indent);

static std::map<std::string, int> symbolTable;

int evalExpr(Expr* expr){
    //integer literals
    /*
     * how it works:
     *   1.try to cast expr to intexpr*
     *   2.if successful (intexpr is not nullptr):
     *      a.we have an integer literal
     *      b.return its stored value
     *   3.if unsuccessful: intexpr is nullptr, skip this if-block
    */
    if(auto intExpr= dynamic_cast<IntExpr*>(expr)){
        return intExpr->value;
    }


    //variable refernce
    /*
     *  process:
     *   1.extract variable name from the node
     *   2.search for it in the symbol table
     *   3.if found: return its value else error (undefined variable)
    */
    if(auto varExpr=dynamic_cast<VarExpr*>(expr)){
        auto it=symbolTable.find(varExpr->name);
        if(it==symbolTable.end()){
            throw std::runtime_error("Undefined variable: " + varExpr->name);
        }
        return it->second;
    }

    //binary expression
    /*
     * process:
     *   1.recursively evaluate the left operand -> get a number
     *   2.recursively evaluate the right operand -> get a number
     *   3.apply the operator to these two numbers
     *   4.return the result
    */

    if(auto binExpr=dynamic_cast<BinaryExpr*>(expr)){
        int left = evalExpr(binExpr->left);
        int right= evalExpr(binExpr->right);

        switch(binExpr->op){
            case '+': return left+right;
            case '-': return left-right;
            case '*': return left*right;
            case '/':
                if(right == 0){
                    throw std::runtime_error("Division by zero");
                }
                return left/right;
            
            case 'E': return left == right ? 1 : 0;
            case 'N': return left != right ? 1 : 0;
            case '<': return left < right ? 1 : 0;
            case '>': return left > right ? 1: 0;
            case 'L': return left <= right ? 1 : 0; //<=
            case 'G': return left >= right ? 1 : 0; //>=
            case 'n': return left - right; //negation

            default: throw std::runtime_error("Unknown Binary operation");

        }
    }

    throw std::runtime_error("Unknown expression type");

}


//statement executor
/*
* how it works:
*   similar to evalexpr, but instead of returning a value, we perform actions:
*   - vardeclstmt: add variable to symbol table with value 0
*   - vardeclinitstmt: add variable with initial value
*   - assignstmt: change variable's value
*   - ifstmt: execute then or else branch based on condition
*   - whilestmt: repeatedly execute body while condition is true
*   - blockstmt: execute each statement in the block in order

* side effects:
*   this function changes the world:
*   - modifies symbol table (variables get created/changed)
*   - may trigger loops and conditional execution
*   - may recursively execute other statements


* examples:
*   execstmt(vardeclstmt("x"))  -> symboltable["x"] = 0
*   execstmt(assignstmt("x", 10))  -> symboltable["x"] = 10
*   execstmt(ifstmt(x>5, print("big"))) -> if x>5, execute print
*/

void execStmt(Stmt* stmt){ //take a statement ast node and execute it (perform its action)
    // variable declaration (create variable in symbol table with value 0)
    if(auto decl=dynamic_cast<VarDeclStmt*>(stmt)){
        symbolTable[decl->name]=0;
        return;
    }


    // variable declaration with initialization (evaluate the initialization expression and create variable in symbol table with that value)
    if(auto declInit = dynamic_cast<VarDeclInitStmt*> (stmt)){
        int value=evalExpr(declInit->expr);
        symbolTable[declInit->name]=value;
        return;
    }

    //assignment
    /*
     * check if variable was declared with 'var'
     * evaluate the right hand side expression
     * update the variable's value in symbol table
     * 
     *  code: x = x + 1;
     *   action:
     *     1.check if "x" exists in symbol table
     *     2.if not: error "cannot assign to undeclared variable: x"
     *     3.if yes: evaluate x + 1:
     *        a.look up x-> suppose it's 10
     *        b.compute 10 + 1-> 11
     *     4.symboltable["x"] = 11
     *   after:x now has value 11
    */
    if(auto assign = dynamic_cast<AssignStmt*>(stmt)){
        auto it=symbolTable.find(assign->name);
        if(it==symbolTable.end()){
            throw std::runtime_error("Cannot assign to undeclated variable: "+assign->name);
        }

        int value= evalExpr(assign->expr);

        symbolTable[assign->name]=value;
        return;
    }

    //if statement
    /*
     * evaluate the condition expression
     * if result is non-zero (true): execute then-branch
     * if result is zero (false): execute else-branch (if it exists)
     * example (with else):
     *   code: if (x > 5) y = 10; else y = 20;
     *   if x=7:
     *     1)evaluate x > 5 -> 7 > 5 -> 1 (true)
     *     2)1 != 0, so execute then-branch
     *     3)execute y = 10
     *   after:y is 10
    */

    if(auto ifStmt = dynamic_cast<IfStmt*>(stmt)){
        int condition = evalExpr(ifStmt->condition);

        if(condition != 0){
            execStmt(ifStmt->thenStmt);
        }else if(ifStmt->elseStmt !=nullptr){
            execStmt(ifStmt->elseStmt);
        }
        return;
    }

    //while loop
    /*
     * evaluate condition
     * if true (non-zero): execute body, go back to step 1
     * if false (zero): exit loop, continue after while
     *
    */

    if(auto whileStmt=dynamic_cast<WhileStmt*>(stmt)){

        while(evalExpr(whileStmt -> condition) !=0){
            execStmt(whileStmt->body);
        }
        return;
    }

    //block statement
    /*
     * action: execute each statement in the block, in order
     *
     * why blocks exist?:
     *   many constructs expect one statement:
     *     if (condition) statement
     *     while (condition) statement
     *
     *   but we often want multiple actions. solution: group them in a block!
     *     if (x > 5) {    
     *         y = 10;       
     *         z = 20;
     *     }
     * 
     * execution:
     *     1.execute: var x= 10; -> x is created with value 10
     *     2.execute: var y= 20; -> y is created with value 20
     *     3.execute: var sum= x + y;
     *        a)evaluate x + y -> 10 + 20 -> 30
     *        b)create sum with value 30
     *   after:x=10 y=20 sum=30
    */

    if(auto blockStmt = dynamic_cast<BlockStmt*> (stmt)){
        for(Stmt* s:blockStmt->statements){
            execStmt(s);
        }
        return;
    }


    throw std::runtime_error("Unknown statement type");
}

void printSymbolTable(){
    std::cout<<"\n---- Symbol Table ----\n";
    for(const auto& entry:symbolTable){
        std::cout<<entry.first<<" = "<<entry.second<<"\n";
    }
}

// for indentation
static void printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
}

// printing an expression tree
void printExpr(Expr* expr, int indent) {
    if (auto intExpr = dynamic_cast<IntExpr*>(expr)) {
        printIndent(indent);
        std::cout << "IntExpr(" << intExpr->value << ")\n";
        return;
    }

    if (auto varExpr = dynamic_cast<VarExpr*>(expr)) {
        printIndent(indent);
        std::cout << "VarExpr(\"" << varExpr->name << "\")\n";
        return;
    }

    if (auto binExpr = dynamic_cast<BinaryExpr*>(expr)) {
        printIndent(indent);
        std::string opName;
        switch (binExpr->op) {
            case '+': opName = "PLUS"; break;
            case '-': opName = "MINUS"; break;
            case '*': opName = "MUL"; break;
            case '/': opName = "DIV"; break;
            case 'E': opName = "EQ"; break;
            case 'N': opName = "NEQ"; break;
            case '<': opName = "LT"; break;
            case '>': opName = "GT"; break;
            case 'L': opName = "LE"; break;
            case 'G': opName = "GE"; break;
            case 'n': opName = "NEG"; break;
            default: opName = "UNKNOWN"; break;
        }
        std::cout << "BinaryExpr(" << opName << ")\n";
        printExpr(binExpr->left, indent + 1);
        printExpr(binExpr->right, indent + 1);
        return;
    }
}

// print a statement tree
void printStmt(Stmt* stmt, int indent) {
    if (auto decl = dynamic_cast<VarDeclStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "VarDeclStmt(\"" << decl->name << "\")\n";
        return;
    }

    if (auto declInit = dynamic_cast<VarDeclInitStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "VarDeclInitStmt(\"" << declInit->name << "\")\n";
        printExpr(declInit->expr, indent + 1);
        return;
    }

    if (auto assign = dynamic_cast<AssignStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "AssignStmt(\"" << assign->name << "\")\n";
        printExpr(assign->expr, indent + 1);
        return;
    }

    if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "IfStmt\n";
        printIndent(indent + 1);
        std::cout << "Condition:\n";
        printExpr(ifStmt->condition, indent + 2);
        printIndent(indent + 1);
        std::cout << "Then:\n";
        printStmt(ifStmt->thenStmt, indent + 2);
        if (ifStmt->elseStmt) {
            printIndent(indent + 1);
            std::cout << "Else:\n";
            printStmt(ifStmt->elseStmt, indent + 2);
        }
        return;
    }

    if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "WhileStmt\n";
        printIndent(indent + 1);
        std::cout << "Condition:\n";
        printExpr(whileStmt->condition, indent + 2);
        printIndent(indent + 1);
        std::cout << "Body:\n";
        printStmt(whileStmt->body, indent + 2);
        return;
    }

    if (auto blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
        printIndent(indent);
        std::cout << "BlockStmt\n";
        for (Stmt* s : blockStmt->statements) {
            printStmt(s, indent + 1);
        }
        return;
    }
}




/*
* what is dynamic_cast?:
*   a c++ operator that safely converts pointers between related types
*   - checks if 'expr' actually points to an intexpr object
*   - if yes: returns pointer to intexpr
*   - if no: returns nullptr
*/