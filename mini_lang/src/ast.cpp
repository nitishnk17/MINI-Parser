// This file contains the EXECUTION logic for our Abstract Syntax Tree. While ast.hpp DEFINES what nodes look like (structure), ast.cpp IMPLEMENTS what nodes DO (behavior).

#include<iostream>
#include "ast.hpp"
#include<map>
#include<string>
#include<stdexcept>

void printSymbolTable();

static std::map<std::string, int> symbolTable;

int evalExpr(Expr* expr){
    //integer literals
    /*
     * HOW IT WORKS:
     *   1. Try to cast expr to IntExpr*
     *   2. If successful (intExpr is not nullptr):
     *      a. We have an integer literal
     *      b. Return its stored value
     *   3. If unsuccessful: intExpr is nullptr, skip this if-block
    */
    if(auto intExpr= dynamic_cast<IntExpr*>(expr)){
        return intExpr->value;
    }



    //variable refernce
    /*
     *  PROCESS:
     *   1. Extract variable name from the node
     *   2. Search for it in the symbol table
     *   3. If found: return its value
     *   4. If not found: throw an error (undefined variable)
    */
    if(auto varExpr=dynamic_cast<VarExpr*>(expr)){
        auto it=symbolTable.find(varExpr->name);
        if(it==symbolTable.end()){
            throw std::runtime_error("Undefined variable: " + varExpr->name);
        }
        return it->second;
    }

    //Binary expression
    /*
     * PROCESS:
     *   1. Recursively evaluate the left operand → get a number
     *   2. Recursively evaluate the right operand → get a number
     *   3. Apply the operator to these two numbers
     *   4. Return the result
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


//Statement executor
/*
* HOW IT WORKS:
*   Similar to evalExpr, but instead of returning a value, we perform actions:
*   - VarDeclStmt: Add variable to symbol table with value 0
*   - VarDeclInitStmt: Add variable with initial value
*   - AssignStmt: Change variable's value
*   - IfStmt: Execute then or else branch based on condition
*   - WhileStmt: Repeatedly execute body while condition is true
*   - BlockStmt: Execute each statement in the block in order

* SIDE EFFECTS:
*   This function CHANGES THE WORLD:
*   - Modifies symbol table (variables get created/changed)
*   - May trigger loops and conditional execution
*   - May recursively execute other statements


* EXAMPLES:
*   execStmt(VarDeclStmt("x"))          → symbolTable["x"] = 0
*   execStmt(AssignStmt("x", 10))       → symbolTable["x"] = 10
*   execStmt(IfStmt(x>5, print("big"))) → if x>5, execute print
*/

void execStmt(Stmt* stmt){ //take a statement AST node and execute it (perform its action)
    // variable Declaration (create variable in symbol table with value 0)
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

    //Assignment
    /*
     * Check if variable was declared with 'var'
     * Evaluate the right hand side expression
     * update the variable's value in symbol table
     * 
     *  Code: x = x + 1;
     *   Action:
     *     1. Check if "x" exists in symbol table
     *     2. If not: throw error "Cannot assign to undeclared variable: x"
     *     3. If yes: Evaluate x + 1:
     *        a. Look up x → suppose it's 10
     *        b. Compute 10 + 1 → 11
     *     4. symbolTable["x"] = 11
     *   After: x now has value 11
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
     * Evaluate the condition expression
     * If result is non-zero (true): execute then-branch
     * If result is zero (false): execute else-branch (if it exists)
     * EXAMPLE (with else):
     *   Code: if (x > 5) y = 10; else y = 20;
     *   If x=7:
     *     1. Evaluate x > 5 → 7 > 5 → 1 (true)
     *     2. 1 != 0, so execute then-branch
     *     3. Execute y = 10
     *   After: y is 10
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
     * Evaluate condition
     * If true (non-zero): execute body, go back to step 1
     * If false (zero): exit loop, continue after while
     *
    */

    if(auto whileStmt=dynamic_cast<WhileStmt*>(stmt)){

        while(evalExpr(whileStmt -> condition) !=0){
            execStmt(whileStmt->body);
        }
        return;
    }

    //Block Statement
    /*
     * ACTION: Execute each statement in the block, in order
     *
     * WHY BLOCKS EXIST?:
     *   Many constructs expect ONE statement:
     *     if (condition) STATEMENT
     *     while (condition) STATEMENT
     *
     *   But we often want multiple actions. Solution: Group them in a block!
     *     if (x > 5) {      // The { } creates ONE block statement
     *         y = 10;       // containing multiple statements
     *         z = 20;
     *     }
     * 
     * Execution:
     *     1. Execute: var x = 10; → x is created with value 10
     *     2. Execute: var y = 20; → y is created with value 20
     *     3. Execute: var sum = x + y;
     *        a. Evaluate x + y → 10 + 20 → 30
     *        b. Create sum with value 30
     *   After: x=10, y=20, sum=30
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






/*
* WHAT IS dynamic_cast?:
*   A C++ operator that safely converts pointers between related types
*   - Checks if 'expr' actually points to an IntExpr object
*   - If YES: Returns pointer to IntExpr
*   - If NO: Returns nullptr
*/