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

    const char *op_str = NULL;
    int spaces = 1; //put spaces around the operator

    switch (a->op){
        case OP_ADD:
            op_str = "+";
            spaces = 1;
            break;
        case OP_SUB:
            op_str = "-";
            spaces = 1;
            break;
        case OP_MUL:
            op_str = "*";
            spaces = 1;
            break;
        case OP_DIV:
            op_str = "/";
            spaces = 1;
            break;
        case OP_MOD:
            op_str = "%";
            spaces = 1;
            break;
        case OP_POW:
            op_str = "^";
            spaces = 0;
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

    if(a->op == OP_TERN){
        char *L = ast_to_string_rec(a->left, my_prec, 0);
        char *M = ast_to_string_rec(a->middle, my_prec, 0);
        char *R = ast_to_string_rec(a->right, my_prec, 1);

        size_t len = strlen(L) + strlen(M) + strlen(R) + 2 + (needs_parens ? 2 : 0);    // '?' and ':' plus parentheses
        char *res = malloc(len + 1);

        if(!res){
            free(L);
            free(M);
            free(R);

            return strdup("");
        }

        if(needs_parens){
            snprintf(res, len + 1, "(%s?%s:%s)", L, M, R);
        }
        else{
            snprintf(res, len + 1, "%s?%s:%s", L, M, R);
        }

        free(L);
        free(M);
        free(R);

        return res;
    }
    else{
        char *L = ast_to_string_rec(a->left, my_prec, 0);
        char *R = ast_to_string_rec(a->right, my_prec, 1);

        size_t len;
        char *res = NULL;

        if(spaces){
            len = strlen(L) + strlen(R) + strlen(op_str) + 2 + (needs_parens ? 2 : 0);  //Spaces around op, parenthese if needed
            res = malloc(len + 1);

            if(!res){
                free(L);
                free(R);

                return strdup("");
            }

            if(needs_parens){
                snprintf(res, len + 1, "(%s %s %s)", L, op_str, R);
            }
            else{
                snprintf(res, len + 1, "%s %s %s", L, op_str, R);
            }
        }
        else{   //no spaces
            len = strlen(L) + strlen(R) + strlen(op_str) + (needs_parens ? 2 : 0);
            res = malloc(len + 1);

            if(!res){
                free(L);
                free(R);

                return strdup("");
            }

            if(needs_parens){
                snprintf(res, len + 1, "(%s^%s)", L, R);
            }
            else{
                snprintf(res, len + 1, "%s^%s", L, R);
            }
        }

        free(L);
        free(R);

        return res;
    }
}

