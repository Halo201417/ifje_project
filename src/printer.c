#include "printer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *ast_to_string_rec(const AST *a, int parent_prec, int is_right_child);

char *ast_to_string(const AST *a){
    return ast_to_string_rec(a, 0, 0);
}

static char *ast_to_string_rec(const AST *a, int parent_prec, int is_right_child){
    
    if(!a){
        return strdup("");
    }

    if(a->type == NODE_NUMBER){
        return strdup(a->num_text);
    }

    char *op_str = NULL;

    switch (a->op){
        case OP_ADD:
            op_str = "+";
            break;
        case OP_SUB:
            op_str = "-";
            break;
        case OP_MUL:
            op_str = "*";
            break;
        case OP_DIV:
            op_str = "/";
            break;
        case OP_MOD:
            op_str = "%";
            break;
        case OP_POW:
            op_str = "^";
            break; 
        case OP_TERN:   //Create a special handling
            op_str = NULL;
            break;
    }

    int my_prec = ast_prec(a);
    int needs_parens = 0;

    if(my_prec < parent_prec){
        needs_parens = 1;
    }
    else if(my_prec == parent_prec){
        if(ast_is_right_assoc(a->op)){
            if(!is_right_child){
                needs_parens = 1;
            }
        }
        else{
            if(is_right_child){
                needs_parens = 1;
            }
        }
    }

    char *left_str = ast_to_string_rec(a->left, my_prec, 0);
    char *right_str = ast_to_string_rec(a->right, my_prec, 1);
    char *middle_str = a->middle ? ast_to_string_rec(a->middle, my_prec, 0) : NULL;

    char *result;
    size_t len;

    if(a->op == OP_TERN){
        len = strlen(left_str) + strlen(middle_str) + strlen(right_str) + 3 + (needs_parens ? 2 : 0);   //? : spaces
        result = malloc(len + 1);

        if(result){
            snprintf(result, len + 1, "(%s?%s:%s)", left_str, middle_str, right_str);
        }
        else{
            snprintf(result, len + 1, "%s?%s:%s", left_str, middle_str, right_str);
        }
    }
    else{
        len = strlen(left_str) + strlen(right_str) + strlen(op_str) + 2 + (needs_parens ? 2 : 0);   //Spaces around op
        result = malloc(len + 1);

        if(result){
            snprintf(result, len + 1, "(%s %s %s)", left_str, op_str, right_str);
        }
        else{
            snprintf(result, len + 1, "%s %s %s", left_str, op_str, right_str);
        }
    }

    free(left_str);
    free(right_str);
    if(middle_str){
        free(middle_str);
    }

    return result ? result : strdup("");
}

