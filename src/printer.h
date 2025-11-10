#ifndef PRINTER_H
#define PRINTER_H

#include "ast.h"


/**
 * @file printer.h
 * @brief Defines the function to convert an AST back into a string representation 
 */


char *ast_to_string(const AST *a);

#endif