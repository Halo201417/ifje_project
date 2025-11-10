#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"


/**
 * Opaque structure that represents the parser state
 * 
 * The parser is responsible for converting a sequence of tokens
 * into an AST validating the syntactic structure of the expression
 */
typedef struct Parser Parser;

Parser *parser_create(const char *input);   //We create a parser from the input with an internal lexer
AST *parser_parse(Parser *p);   //NULL in any case of error
const char *parser_error(Parser *p);
void parser_destroy(Parser *p);

#endif