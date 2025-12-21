#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUM,
    NODE_OP,
    NODE_VAR,
    NODE_ASSIGN,
    NODE_SEQ,
    NODE_IF_STMT,
    NODE_VAR_DCLRT
} NodeType;

typedef struct ASTNode {
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *elsePtr;
    NodeType type;
    int value;
    int op;
    char *varID;
} ASTNode;

/* Function Prototypes */
struct ASTNode* createNumberNode(int val);
struct ASTNode* createOperatorNode(int op, struct ASTNode *left, struct ASTNode *right);

struct ASTNode* createVariableNode(char* name);
struct ASTNode* createAssignmentNode(char* name, struct ASTNode* right);

struct ASTNode* createSequenceNode(struct ASTNode* left, struct ASTNode* right);

struct ASTNode* createVarDeclNode(char* name, struct ASTNode* initExpr);
struct ASTNode* createIfNode(struct ASTNode* cond, struct ASTNode* ifBody, struct ASTNode* elseBody);
void declareVariable(char* name);


int evaluate(struct ASTNode* node);

#endif