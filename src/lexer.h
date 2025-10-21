#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum{
    TOK_EOF,
    TOK_NUMBER,
    TOK_IDENT,
    TOK_LPAREN, //We divided the parenthesis (
    TOK_RPAREN, // )
    TOK_COMMA,
    TOK_ERROR
} TokenType;

typedef struct{
    TokenType type;
    char *lexeme;   //NULL for EOF
    size_t pos;
} Token;

typedef struct Lexer Lexer;

Lexer *lexer_create(const char *input);
Token lexer_next(Lexer *l); //Next token
void token_free(Token *t);   //Free lexeme
void lexer_destroy(Lexer *l);

#endif