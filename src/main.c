/**
 * This macro at the top of the main file defines the version of the POSIX standard that the compiler
 * needs to support
 */
#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>   // ssize_t
#include "parser.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Main function of the program
 * Reads an expression from standard input, analyzes it, and transforms it into infix notation
 */
int main(){
    char *input = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&input, &len, stdin);
    if(read == -1){
        fprintf(stderr, "No input or read error\n");
        free(input);
        return 1;
    }

    //Removes the newline character if it exists
    if(read > 0 && input[read - 1] == '\n'){
        input[read - 1] = '\0';
    }

    //Creates a parser with the read text
    Parser *parser = parser_create(input);
    AST *ast = parser_parse(parser);

    //Sends an error if the syntax analysis fails
    if(!ast){
        const char *err = parser_error(parser);
        fprintf(stderr, "Error: %s\n", err ? err : "Unknown error");
        parser_destroy(parser);
        free(input);
        return 1;
    }

    //Transforms the AST into an output string
    char *output = ast_to_string(ast);
    printf("%s\n", output);

    //Free all the allocated resources
    free(output);
    ast_free(ast);
    parser_destroy(parser);
    free(input);

    return 0;
}