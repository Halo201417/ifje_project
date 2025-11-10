#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * Internal structure of the lexer
 */
struct Lexer{
    const char *input;
    size_t pos;
};


/**
 * Creates a new lexer for the `input` string
 * The function does not copy the string, it just ensures that the string
 * remains valid while the lexer is being used
 * 
 * @param input: string to be tokenized
 * @return a pointer to a new lexer structure or null if memory allocation fails
 */
Lexer *lexer_create(const char *input){
    Lexer *l = malloc(sizeof(Lexer));

    if(l){
        l->input = input;
        l->pos = 0;
    }

    return l;
}


/**
 * Increases the lexer position while there are spaces
 */
static void skip_whitespace(Lexer *l){
    while(isspace((unsigned char)l->input[l->pos])){
        l->pos++;
    }
}


/**
 * Skip any C-style comment block
 * If "/*" is found the position advances to "* /" or to the end of the input
 * If the comment block does not close, it leaves `pos` in the end of the string
 */
static void skip_comment(Lexer *l){
    if(l->input[l->pos] == '/' && l->input[l->pos + 1] == '*'){
        l->pos += 2;    //Skips "/*"

        //Advances to "*/" or until '\0'
        while(l->input[l->pos] && !(l->input[l->pos] == '*' && l->input[l->pos + 1] == '/')){
            l->pos++;
        }

        //If "*/" is foudn it skips these two characters
        if(l->input[l->pos]){
            l->pos += 2;
        }
    }
}


/**
 * Returns the next token from the input
 * 
 * Process:
 * 1. Skip spaces and comments
 * 2. Determine the token type
 * 3. For tokens tat require a lexeme a substring is assigned and copied
 * 
 * @param l: a pointer to the lexer
 * @return the next token or TOK_EOF when the end is reached
 */
Token lexer_next(Lexer *l){
    Token t = {.type = TOK_ERROR, .lexeme = NULL, .pos = l->pos};

    while(1){
        skip_whitespace(l);
        if(l->input[l->pos] == '\0'){
            t.type = TOK_EOF;
            return t;
        }

        skip_comment(l);
        if(l->input[l->pos] == '\0'){
            t.type = TOK_EOF;
            return t;
        }

        if(!isspace((unsigned char)l->input[l->pos]) && !(l->input[l->pos] == '/' && l->input[l->pos + 1] == '*')){
            break;
        }
    }

    t.pos = l->pos;

    //Identifier
    if(isalpha((unsigned char)l->input[l->pos])){
        size_t start = l->pos;
        
        while(isalpha((unsigned char)l->input[l->pos])){
            l->pos++;
        }

        size_t len = l->pos - start;
        t.lexeme = malloc(len + 1);

        if(t.lexeme){
            strncpy(t.lexeme, l->input + start, len);
            t.lexeme[len] = '\0';
            t.type = TOK_IDENT;
        }

        return t;
    }

    //Number
    if(isdigit((unsigned char)l->input[l->pos]) || l->input[l->pos] == '+' || l->input[l->pos] == '-' || l->input[l->pos] == '.'){
        size_t start = l->pos;

        if(l->input[l->pos] == '+' || l->input[l->pos] == '-'){
            l->pos++;
        }

        //Digit sequence
        int has_digits = 0;
        
        while(isdigit((unsigned char)l->input[l->pos])){
            l->pos++;
            has_digits = 1;
        }

        //Fraction
        if(l->input[l->pos] == '.'){
            l->pos++;

            while((unsigned char)isdigit(l->input[l->pos])){
                l->pos++;
                has_digits = 1;
            }
        }

        //Exponent
        if(tolower((unsigned char)l->input[l->pos]) == 'e'){
            l->pos++;
            
            if(l->input[l->pos] == '+' || l->input[l->pos] == '-'){
                l->pos++;
            }

            int exp_digits = 0;

            while(isdigit((unsigned char)l->input[l->pos])){
                l->pos++;
                exp_digits = 1;
            }

            if(!exp_digits){
                t.type = TOK_ERROR;
                return t;
            }
        }

        if(!has_digits){
            t.type = TOK_ERROR;
            return t;
        }

        size_t len = l->pos - start;
        t.lexeme = malloc(len + 1);

        if(t.lexeme){
            strncpy(t.lexeme, l->input + start, len);
            t.lexeme[len] = '\0';
            t.type = TOK_NUMBER;
        }
        else{
            t.type = TOK_ERROR;
        }

        return t;
    }

    //Single char tokens
    switch (l->input[l->pos]){
        case '(':
            t.type = TOK_LPAREN;
            l->pos++;
            break;
        case ')':
            t.type = TOK_RPAREN;
            l->pos++;
            break;
        case ',':
            t.type = TOK_COMMA;
            l->pos++;
            break;
        default:
            t.type = TOK_ERROR;
            l->pos++;
            break;
    }

    return t;
}


/**
 * Frees the memory associated with the token's lexeme
 * This must be called every time we stop using a token that contains a lexeme
 */
void token_free(Token *t){
    if(t->lexeme){
        free(t->lexeme);
        t->lexeme = NULL;
    }
}


/**
 * Frees the lexer
 */
void lexer_destroy(Lexer *l){
    free(l);
}