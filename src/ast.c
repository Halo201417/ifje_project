#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

AST *ast_make_number(const char *text){
    AST *a = malloc(sizeof(AST));

    if(a){
        a->type = NODE_NUMBER;
        a->num_text = strdup(text);
        a->num_value = strtod(text, NULL);
    }

    return a;
}

AST *ast_make_binary(OpType op, AST *left, AST *right){
    AST *a = malloc(sizeof(AST));

    if(a){
        a->type = NODE_OP;
        a->op = op;
        a->left = left;
        a->middle = NULL;
        a->right = right;
    }

    return a;
}

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

void ast_free(AST *a){
    if(!a){
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

int ast_prec(const AST *a){
    if(a->type != NODE_OP){
        return 5;   //Numbers higher than any op
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

int ast_is_right_assoc(OpType op){
    return op == OP_POW || op == OP_TERN;
}