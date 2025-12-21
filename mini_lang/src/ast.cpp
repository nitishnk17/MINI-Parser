#include<iostream>
#include "ast.hpp"
#include<map>
#include<string>
#include<stdexcept>

void printSymbolTable();

static std::map<std::string, int> symbolTable;

int evalExpr(Expr* expr){
    //integer literals
    if(auto intExpr= dynamic_cast<IntExpr*>(expr)){
        return intExpr->value;
    }

    //variable refernce
    if(auto varExpr=dynamic_cast<VarExpr*>(expr)){
        auto it=symbolTable.find(varExpr->name);
        if(it==symbolTable.end()){
            throw std::runtime_error("Undefined variable: " + varExpr->name);
        }
        return it->second;
    }

    //Binary expression

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

        }
    }

    throw std::runtime_error("Unknown expression type");

}


void execStmt(Stmt* stmt){
    // variable Declaration
    if(auto decl=dynamic_cast<VarDeclStmt*>(stmt)){
        symbolTable[decl->name]=0;
        return;
    }

    //Assignment
    if(auto assign = dynamic_cast<AssignStmt*>(stmt)){
        int value=evalExpr(assign->expr);
        symbolTable[assign->name]=value;
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