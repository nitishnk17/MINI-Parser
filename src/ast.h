#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUM,
    NODE_OP,
    NODE_VAR,
    NODE_ASSIGN,
    NODE_SEQ
} NodeType;

typedef struct ASTNode {
    struct ASTNode *left;
    struct ASTNode *right;
    NodeType type;
    int value;
    char op;
    char *varID;
} ASTNode;

/* Function Prototypes */
struct ASTNode* createNumberNode(int val);
struct ASTNode* createOperatorNode(char op, struct ASTNode *left, struct ASTNode *right);

struct ASTNode* createVariableNode(char* name);
struct ASTNode* createAssignmentNode(char* name, struct ASTNode* right);

struct ASTNode* createSequenceNode(struct ASTNode* left, struct ASTNode* right);

int evaluate(struct ASTNode* node);

#endif