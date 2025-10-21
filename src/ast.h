#ifndef AST_H
#define AST_H

typedef enum{
    NODE_NUMBER,
    NODE_OP
} NodeType;

typedef enum{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_POW,
    OP_TERN
} OpType;

typedef struct AST{
    NodeType type;

    // If type is NODE_NUMBER
    char *num_text;
    double num_value;   //Parse with strtod

    //If type is NODE_OP
    OpType op;
    struct AST *left;
    struct AST *middle; //If binary == NULL
    struct AST *right;
} AST;

AST *ast_make_number(const char *text);
AST *ast_make_binary(OpType op, AST *left, AST *right);
AST *ast_make_ternary(AST *left, AST *middle, AST *right);
void ast_free(AST *a);

int ast_prec(const AST *a);
int ast_is_right_assoc(OpType op);

#endif