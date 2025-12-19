#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUM,
    NODE_OP
} NodeType;

typedef struct ASTNode {
    struct ASTNode *left;
    struct ASTNode *right;
    NodeType type;
    int value;
    char op;
} ASTNode;

/* Function Prototypes */
struct ASTNode* createNumberNode(int val);
struct ASTNode* createOperatorNode(char op, struct ASTNode *left, struct ASTNode *right);

int evaluate(struct ASTNode* node);

#endif