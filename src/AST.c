#include "include/AST.h"
#include <stdlib.h>

AST_T* init_ast(int tipo)
{
    AST_T* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->tipo = tipo;

    return ast;
}