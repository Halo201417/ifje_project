#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>   // ssize_t
#include "parser.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // Trim trailing newline
    if(read > 0 && input[read - 1] == '\n'){
        input[read - 1] = '\0';
    }

    Parser *parser = parser_create(input);
    AST *ast = parser_parse(parser);

    if(!ast){
        const char *err = parser_error(parser);
        fprintf(stderr, "Error: %s\n", err ? err : "Unknown error");
        parser_destroy(parser);
        free(input);
        return 1;
    }

    char *output = ast_to_string(ast);
    printf("%s\n", output);

    free(output);
    ast_free(ast);
    parser_destroy(parser);
    free(input);

    return 0;
}