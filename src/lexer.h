#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>


/**
 * Enumeration that defines the different types of tokens that the lexer can handle
 * 
 * TOK_EOF -> End of File
 * TOK_NUMBER -> A number
 * TOK_IDENT -> Identifier
 * TOK_LPAREN -> Left parenthesis '('
 * TOK_RPAREN -> Right parenthesis ')'
 * TOK_COMMA -> Comma ','
 * TOK_ERROR -> Invalid token or invalid sequence
 */
typedef enum{
    TOK_EOF,
    TOK_NUMBER,
    TOK_IDENT,
    TOK_LPAREN, //We divided the parentheses '('
    TOK_RPAREN, // ')' into two separate tokens
    TOK_COMMA,
    TOK_ERROR
} TokenType;


/**
 * Structure that represents an individual token
 */
typedef struct{
    TokenType type;
    char *lexeme;   //NULL for EOF and single-character tokens
    size_t pos;
} Token;


/**
 * Opaque structure that represents the state of the lexical analyzer
 */
typedef struct Lexer Lexer;

Lexer *lexer_create(const char *input);
Token lexer_next(Lexer *l); //Gets the next token
void token_free(Token *t);   //Frees the lexeme
void lexer_destroy(Lexer *l);

#endif