#include "parser.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(){
    
    char *input = NULL;
    size_t len = 0;
    __ssize_t read;

    input = malloc(BUFFER_SIZE);
    
    if(!input){
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    len = 0;

    while((read = getline(&input, &len, stdin)) != -1){
        //getline handles the reallocation
    }

    if(input[strlen(input) - 1] == '\n'){
        input[strlen(input) - 1] = '\0';
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