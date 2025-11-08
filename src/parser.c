#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Parser{
    Lexer *lexer;
    Token current;
    char *error_msg;
};

static void advance(Parser *p);
static AST *parse_expr(Parser *p);
static AST *parse_primary(Parser *p);
static OpType get_op_from_ident(const char *ident);

Parser *parser_create(const char *input){
    Parser *p = malloc(sizeof(Parser));

    if(p){
        p->lexer = lexer_create(input);
        p->error_msg = NULL;
        p->current.type = TOK_ERROR;
        p->current.lexeme = NULL;
        p->current.pos = 0;
        advance(p); //Load the first token
    }

    return p;
}

AST *parser_parse(Parser *p){
    AST *ast = parse_expr(p);

    if(p->current.type != TOK_EOF){
        if(!p->error_msg){
            p->error_msg = strdup("Unexpected token after expression");
        }

        ast_free(ast);
        return NULL;
    }

    return ast;
}

const char *parser_error(Parser *p){
    return p->error_msg;
}

void parser_destroy(Parser *p){
    lexer_destroy(p->lexer);
    if(p->error_msg){
        free(p->error_msg);
    }

    free(p);
}

static void advance(Parser *p){
    token_free(&p->current);
    p->current = lexer_next(p->lexer);
}

static AST *parse_expr(Parser *p){
    return parse_primary(p);
}

static AST *parse_primary(Parser *p){
    if(p->current.type == TOK_NUMBER){
        AST *num = ast_make_number(p->current.lexeme);
        advance(p);
        return num;
    }
    else if(p->current.type == TOK_IDENT){
        char *ident = strdup(p->current.lexeme);
        advance(p);

        if(p->current.type != TOK_LPAREN){
            if(!p->error_msg){
                p->error_msg = strdup("Expected '(' after identifier");
            }

            free(ident);
            return NULL;
        }

        advance(p); //Consume '('

        AST *args[3] = {NULL, NULL, NULL};
        int arg_count = 0;

        while(p->current.type != TOK_RPAREN && arg_count < 3){
            AST *arg = parse_expr(p);

            if(!arg){
                free(ident);
                for(int i = 0; i < arg_count; i++){
                    ast_free(args[i]);
                }
                
                return NULL;
            }

            args[arg_count++] = arg;

            if(p->current.type == TOK_COMMA){
                advance(p);
            }
            else if(p->current.type != TOK_RPAREN){
                if(!p->error_msg){
                    p->error_msg = strdup("Expected ',' or ')'");
                }

                free(ident);
                
                for(int i = 0; i < arg_count; i++){
                    ast_free(args[i]);
                }

                return NULL;
            }
        }

        if(p->current.type != TOK_RPAREN){
            if(!p->error_msg){
                p->error_msg = strdup("Expected ')'");
            }

            free(ident);

            for(int i = 0; i < arg_count; i++){
                ast_free(args[i]);
            }

            return NULL;
        }

        advance(p); //Consume ')'

        OpType op = get_op_from_ident(ident);
        free(ident);
        
        if(op == OP_TERN){
            if(arg_count != 3){
                if(!p->error_msg){
                    p->error_msg = strdup("Ternaty operator requires 3 arguments");
                }

                for(int i = 0; i < arg_count; i++){
                    ast_free(args[i]);
                }

                return NULL;
            }

            return ast_make_ternary(args[0], args[1], args[2]);
        }
        else if(op != (OpType)-1){
            if(arg_count != 2){
                if(!p->error_msg){
                    p->error_msg = strdup("Binary opertor requres 2 arguments");
                }

                for(int i = 0; i < arg_count; i++){
                    ast_free(args[i]);
                }

                return NULL;
            }

            return ast_make_binary(op, args[0], args[1]);
        }
        else{
            if(!p->error_msg){
                p->error_msg = strdup("Unknown function");
            }

            for(int i = 0; i < arg_count; i++){
                ast_free(args[i]);
            }

            return NULL;
        }
    }
    else{
        if(!p->error_msg){
            p->error_msg = strdup("Expected number or function call");
        }

        return NULL;
    }
}

static OpType get_op_from_ident(const char *ident){
    if(strcmp(ident, "add") == 0){
        return OP_ADD;
    }

    if(strcmp(ident, "sub") == 0){
        return OP_SUB;
    }

    if(strcmp(ident, "mul") == 0){
        return OP_MUL;
    }

    if(strcmp(ident, "div") == 0){
        return OP_DIV;
    }

    if(strcmp(ident, "mod") == 0){
        return OP_MOD;
    }

    if(strcmp(ident, "pow") == 0){
        return OP_POW;
    }

    if(strcmp(ident, "tern") == 0){
        return OP_TERN;
    }

    return -1;
}