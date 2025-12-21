#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

struct ASTNode* createNumberNode(int val) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_NUM;
    newNode->value = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct ASTNode* createOperatorNode(int op, struct ASTNode *left, struct ASTNode *right) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_OP;
    newNode->op = op;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

struct Symbol {
    char name[32];
    int value;
};

struct Symbol symbolTable[100]; 
int symbolCount = 0;

void declareVariable(char* name) {
    for(int i=0; i<symbolCount; i++) {
        if(strcmp(symbolTable[i].name, name) == 0) {
            fprintf(stderr, "The variable '%s' has already been declared.\n", name);
            exit(1);
        }
    }
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].value = 0; // Default to 0
    symbolCount++;
}

void setVariable(char* name, int val) {
    // Check if variable already exists; if so, update it
    for(int i=0; i<symbolCount; i++) {
        if(strcmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].value = val;
            return;
        }
    }
    
    fprintf(stderr, "This variable '%s' has not been declared. Use 'var %s' first.\n", name, name);
    exit(1);
}

int getVariable(char* name) {
    // Look for the variable in the table
    for(int i=0; i<symbolCount; i++) {
        if(strcmp(symbolTable[i].name, name) == 0) {
            return symbolTable[i].value;
        }
    }
    
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(1);
}


struct ASTNode* createVariableNode(char* name) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_VAR;
    newNode->varID = strdup(name); // Make a copy of the name string
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct ASTNode* createAssignmentNode(char* name, struct ASTNode* right) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_ASSIGN;
    newNode->varID = strdup(name);
    newNode->right = right; // The expression to assign is on the right
    newNode->left = NULL;
    return newNode;
}


struct ASTNode* createSequenceNode(struct ASTNode* left, struct ASTNode* right) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_SEQ;
    newNode->left = left;   
    newNode->right = right; 
    return newNode;
}


struct ASTNode* createVarDeclNode(char* name, struct ASTNode* initExpr) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->type = NODE_VAR_DCLRT; 
    n->varID = strdup(name); 
    n->left = initExpr; // if not then NULL
    n->right = NULL; 
    n->elsePtr = NULL;
    return n;
}

struct ASTNode* createIfNode(struct ASTNode* cond, struct ASTNode* ifBody, struct ASTNode* elseBody) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->type = NODE_IF_STMT; 
    n->left = cond; 
    n->right = ifBody; 
    n->elsePtr = elseBody;
    return n;
}


int evaluate(struct ASTNode* node) {
    if (node == NULL) return 0;

    if (node->type == NODE_NUM) {
        return node->value;
    }
    
    if (node->type == NODE_VAR) {
        return getVariable(node->varID);
    }

    if (node->type == NODE_VAR_DCLRT) {
        declareVariable(node->varID);
        if (node->left) { // If there is an initialization (var x = 10)
            int val = evaluate(node->left);
            setVariable(node->varID, val);
        }
        return 0; 
    }

    if (node->type == NODE_ASSIGN) {
        int val = evaluate(node->right); 
        setVariable(node->varID, val); 
        return val; 
    }

    if (node->type == NODE_SEQ) {
        evaluate(node->left);   
        return evaluate(node->right); 
    }

    if (node->type == NODE_IF_STMT) {
        int condition = evaluate(node->left);
        if (condition) { 
            return evaluate(node->right);
        } else if (node->elsePtr != NULL) {
            return evaluate(node->elsePtr);
        }
        return 0;
    }

    int leftVal = evaluate(node->left);
    int rightVal = evaluate(node->right);

    switch (node->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': 
            if (rightVal == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            return leftVal / rightVal;
        
        case '<': return leftVal < rightVal;
        case '>': return leftVal > rightVal;
        case EQ:  return leftVal == rightVal;
        case NE:  return leftVal != rightVal;
        case LE:  return leftVal <= rightVal;
        case GE:  return leftVal >= rightVal;


        default:  return 0;
    }
}


