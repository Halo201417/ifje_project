#ifndef AST_H
#define AST_H

/**
 * @file ast.h
 * @brief Definition of the Abstract Syntax Tree (AST) structure and its associated functions to
 *        construct, free and manage expressions
 * 
 * The AST represents a hierarchical structure of a mathematical expression
 * Every node can be a number (leaf) or an operation (internal node)
 */


/**
 * @enum NodeType
 * @brief Indicates the node type in the tree 
 */
typedef enum{
    NODE_NUMBER,
    NODE_OP
} NodeType;


/**
 * @enum OpType
 * @brief Operation types supported by the tree
 */
typedef enum{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_POW,
    OP_TERN
} OpType;


/**
 * @struct AST
 * @brief Represents a node in the AST
 * 
 * Structure contents depend on the `type`
 */
typedef struct AST{
    NodeType type;

    // If type is NODE_NUMBER
    char *num_text;
    double num_value;   //Parsed with strtod

    //If type is NODE_OP
    OpType op;
    struct AST *left;
    struct AST *middle; //Null for binary operations
    struct AST *right;
} AST;

AST *ast_make_number(const char *text);
AST *ast_make_binary(OpType op, AST *left, AST *right);
AST *ast_make_ternary(AST *left, AST *middle, AST *right);
void ast_free(AST *a);

int ast_prec(const AST *a);
int ast_is_right_assoc(OpType op);

#endif