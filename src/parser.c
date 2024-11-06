#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->token = lexer_prox_token(lexer);

    return parser;
}

token_T* parser_consome(parser_T* parser, int tipo)
{
    if (parser->token->tipo != tipo)
    {
        printf("[Parser]: Token nao esperado: '%s', token esperado: '%s'\n", token_to_str(parser->token), tipo_token_to_str(tipo));
        exit(1);
    }

    parser->token = lexer_prox_token(parser->lexer);
    return parser->token;
}

AST_T* parser_parse(parser_T* parser)
{
    return init_ast(AST_NOOP);
}

AST_T* parser_parse_composicao(parser_T* parser)
{
    //AST_T* composicao = init_ast(AST_COMPOSICAO);

    while (parser->token->tipo != TOKEN_EOF)
    {
        /*AST_T* child = */parser_parse(parser);
    }

    return init_ast(AST_NOOP);
}