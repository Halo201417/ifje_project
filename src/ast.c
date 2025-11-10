#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


/**
 * Create an AST node (Abstract Syntax Tree) of type int
 * @param text: original text of the number
 * @return a pointer to the new AST node or null in case the memory allocation fails
 */
AST *ast_make_number(const char *text){
    AST *a = malloc(sizeof(AST));

    if(a){
        a->type = NODE_NUMBER;
        a->num_text = strdup(text); //Copy of the literal number text
        a->num_value = strtod(text, NULL);  //Converts the text to a numerical value
    }

    return a;
}


/**
 * Creates a binary node for the AST
 * @param op: type of operation
 * @param left: left subtree
 * @param right: right subtree
 * @return a pointer to the new AST node or null in case the memory allocation fails
 */
AST *ast_make_binary(OpType op, AST *left, AST *right){
    AST *a = malloc(sizeof(AST));

    if(a){
        a->type = NODE_OP;
        a->op = op;
        a->left = left;
        a->middle = NULL;   //Not used in binary operations
        a->right = right;
    }

    return a;
}


/**
 * Creates a ternary node for the AST
 * @param left: conditional expression
 * @param middle: expression if true
 * @param right: expression if false
 * @return a pointer to the new AST node or null in case the memory allocation fails
 */
AST *ast_make_ternary(AST *left, AST *middle, AST *right){
    AST *a = malloc(sizeof(AST));

    if(a){
        a->type = NODE_OP;
        a->op = OP_TERN;
        a->left = left;
        a->middle = middle;
        a->right = right;
    }

    return a;
}


/**
 * Recursively frees the memory used by the AST
 * @param a: the tree's root
 */
void ast_free(AST *a){
    if(!a){ //Empty tree
        return;
    }

    if(a->type == NODE_NUMBER){
        free(a->num_text);
    }
    else if(a->type == NODE_OP){
        ast_free(a->left);
        ast_free(a->middle);
        ast_free(a->right);
    }

    free(a);
}


/**
 * Returns the precedence of the node.
 * Used to decide when to add a parentheses during printing
 * @param a: AST node
 * @return the precedence level
 */
int ast_prec(const AST *a){
    if(a->type != NODE_OP){
        return 5;   //Numbers have a higher precedence than any operator
    }

    switch (a->op){
        case OP_TERN:
            return 1;
        case OP_ADD:
        case OP_SUB:
            return 2;
        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
            return 3;
        case OP_POW:
            return 4;
    }

    return 0;
}


/**
 * Indicates if an operator is right-associative
 * @param op: operator
 * @return 1 if it is right-associative, 0 otherwise
 */
int ast_is_right_assoc(OpType op){
    return op == OP_POW || op == OP_TERN;
}