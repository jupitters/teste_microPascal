#include "include/parser.h"
#include "include/tipos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->token = lexer_prox_token(lexer);

    if (!parser) {
        fprintf(stderr, "[Erro]: Falha ao alocar memória para parser.\n");
        exit(1);
    }

    if (!parser->token) {
        fprintf(stderr, "[Parser]: Erro ao obter primeiro token.\n");
        free(parser);
        exit(1);
    }

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
    return parser_parse_composicao(parser);
}

AST_T* parser_parse_pr(parser_T* parser)
{
    if (strcmp(parser->token->valor, "var") == 0)
    {   
        parser_consome(parser, PAL_RES);
        AST_T* var = parser_parse_id(parser);
        return var;
    }
    
    if (strcmp(parser->token->valor, "begin") == 0)
    {
        AST_T* lista = parser_parse_lista(parser);
        
        return lista;
    }

    if (strcmp(parser->token->valor, "if") == 0)
    {
        AST_T* lista = parser_parse_lista(parser);
        
        return lista;
    }

    if (strcmp(parser->token->valor, "then") == 0)
    {
        parser_consome(parser, PAL_RES);
    }
    if (strcmp(parser->token->valor, "else") == 0)
    {
        parser_consome(parser, PAL_RES);
    }


    if (strcmp(parser->token->valor, "program") == 0)
    {
        parser_consome(parser, PAL_RES);
        
        if (parser->token->tipo != T_ID)
        {
            printf("[Parser]: Esperado um identificador como nome do programa.\n");
            exit(1);
        }
        else
        {
            char* valor = calloc(strlen(parser->token->valor) + 1, sizeof(char));
            strcpy(valor, parser->token->valor);
            parser_consome(parser, T_ID);

            AST_T* ast = init_ast(AST_COMPOSICAO);
            ast->nome = valor;
            parser_consome(parser, SMB_SEM);

            return ast;
        }
    }
    else
    {
        printf("[Parser]: Esperado 'program' no inicio do arquivo.\n%s\n", parser->token->valor);
        //exit(1);
    }
      
}

AST_T* parser_parse_id(parser_T* parser)
{
    char* valor = calloc(strlen(parser->token->valor) + 1, sizeof(char));
    strcpy(valor, parser->token->valor);
    parser_consome(parser, T_ID);

    AST_T* ast = init_ast(AST_VARIAVEL);
    ast->nome = valor;

    if (parser->token->tipo == OP_ASS)
    {
        parser_consome(parser, OP_ASS);

        while (parser->token->tipo != SMB_SEM)
        {
            ast->tipo_dado += tipo_p_int(parser->token->valor);

            if (parser->token->tipo == OP_AD)
            {
                parser_consome(parser, OP_AD);
            }

            if (parser->token->tipo == OP_MUL)
            {
                parser_consome(parser, OP_MUL);
            }

            if (parser->token->tipo == OP_MIN)
            {
                parser_consome(parser, OP_MIN);
            }

            if (parser->token->tipo == OP_DIV)
            {
                parser_consome(parser, OP_DIV);
            }
                
            if (parser->token->tipo == NUM_INT)
            {
                parser_consome(parser, NUM_INT);
            }

            if (parser->token->tipo == NUM_FLT)
            {
                parser_consome(parser, NUM_FLT);
            }

            if (parser->token->tipo == T_ID)
            {
                parser_consome(parser, T_ID);
            }
        }
        
        parser_consome(parser, SMB_SEM);
    }

    if (parser->token->tipo == OP_DEC)
    {
        parser_consome(parser, OP_DEC);
        ast->tipo_dado = tipo_p_int(parser->token->valor);
        
        parser_consome(parser, PAL_RES);
        parser_consome(parser, SMB_SEM);
    }

    if (parser->token->tipo == SMB_COM)
    {
        parser_consome(parser, SMB_COM);
        parser_parse_id(parser);
    }

    return ast;
}

AST_T* parser_parse_num(parser_T* parser)
{
    char* valor = calloc(strlen(parser->token->valor) + 1, sizeof(char));
    strcpy(valor, parser->token->valor);
    parser_consome(parser, NUM_INT);

    AST_T* ast = init_ast(AST_ASS);
    ast->nome = valor;
    return ast;
}

AST_T* parser_parse_lista(parser_T* parser)
{
    parser_consome(parser, PAL_RES);
    AST_T* ast = init_ast(AST_COMPOSICAO);
    lista_coloca(ast->children, parser_parse_expr(parser));
        
    while (parser->token->tipo != SMB_EOP)
    {

        if (parser->token->tipo == SMB_SEM)
        {
            parser_consome(parser, SMB_SEM);
        }

        if (parser->token->tipo == OP_GT)
        {
            parser_consome(parser, OP_GT);
        }

        lista_coloca(ast->children, parser_parse_expr(parser));
    }

    parser_consome(parser, SMB_EOP);
    
    return ast;
}

AST_T* parser_parse_expr(parser_T* parser)
{
    switch (parser->token->tipo)
    {
        case PAL_RES: return parser_parse_pr(parser);
        case T_ID: return parser_parse_id(parser);
        case NUM_INT: return parser_parse_num(parser);
        default: { printf("[Parser]: Token nao esperado '%s'\n", token_to_str(parser->token));
        exit(1); };
    }
}

AST_T* parser_parse_composicao(parser_T* parser)
{
    AST_T* composicao = init_ast(AST_COMPOSICAO);

    while (parser->token->tipo != T_EOF)
    {
        lista_coloca(composicao->children, parser_parse_expr(parser));
    }

    return composicao;
}

