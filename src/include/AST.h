#ifndef MP_AST_H
#define MP_AST_H
typedef struct AST_STRUCT
{
    enum
    {
        AST_COMPOSICAO,
        AST_DEF_FUNC,
        AST_DEF_TIPO,
        AST_VARIAVEL,
        AST_DECLARACAO,
        AST_NOOP,
    } tipo;
} AST_T;

AST_T* init_ast(int tipo);
#endif