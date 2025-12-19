#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct ASTNode* createNumberNode(int val) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_NUM;
    newNode->value = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct ASTNode* createOperatorNode(char op, struct ASTNode *left, struct ASTNode *right) {
    ASTNode *newNode = malloc(sizeof(ASTNode));
    newNode->type = NODE_OP;
    newNode->op = op;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

int evaluate(struct ASTNode* node) {
    if (!node->left && !node->right) {
        return node->value;
    }

    int leftVal = evaluate(node->left);
    int rightVal = evaluate(node->right);

    switch (node->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return leftVal / rightVal;
        default:  return 0;
    }
}