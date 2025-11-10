#include "printer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/**
 * Recursive function to convert an AST subtree into a string.
 * It manages operator precedence and associativity to determine parentheses placement.
 * @param a: the current AST node
 * @param parent_prec: the precedence level of the parent operator
 * @param is_right_child: flag indicating if the current node is the right child of its parent
 * @return a dynamically allocated string representing the expression
 */
static char *ast_to_string_rec(const AST *a, int parent_prec, int is_right_child);


/**
 * Main entry function to convert the AST root into an infix string.
 * Starts the recursive process with the lowest parent precedence (0)
 * @param a: the root of the AST
 * @return a dynamically allocated string representing the full expression
 */
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
    int spaces = 1; //Flag to indicate if spaces should be place around the operator

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

    //Precedence check
    if(my_prec < parent_prec){
        needs_parens = 1;
    }
    //Associativity check
    else if(my_prec == parent_prec){
        //Right-associative operators
        if(ast_is_right_assoc(a->op)){
            if(!is_right_child){
                needs_parens = 1;
            }
        }
        //Left-associative operators
        else{
            if(is_right_child){
                needs_parens = 1;
            }
        }
    }

    //Special handling
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

