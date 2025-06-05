#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>

#include "parser.h"
#include "ast.h"
#include "scope.h"


parser_t *init_parser(lexer_t *lexer){
    if (lexer == NULL){
        fprintf(stderr, "ERROR: NULL Lexer Encounted\n");
        exit(EXIT_FAILURE);
    }

    parser_t *parser = calloc(1, sizeof(struct PARSER_STRUCT));
    if(parser == NULL)
    {
        fprintf(stderr, "Error: Parser creation failed\n");
        exit(EXIT_FAILURE);
    }
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->previous_token = parser->current_token;

    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_t *parser, int token_type)
{
    if ((int)parser->current_token->type == token_type)
    {
        parser->previous_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else{
        fprintf(stderr, "Unexpected token %s with type %d \n", parser->current_token->value, parser->current_token->type );
        exit(EXIT_FAILURE);
    }
}

ast_t *parser_parse(parser_t *parser, scope_t *scope){
    
    if(parser == NULL)
    {
        fprintf(stderr, "Error: NULL Parser encountered\n");
        exit(EXIT_FAILURE);
    }

    return parser_parse_statements(parser, scope);
}

ast_t *parser_parse_statement(parser_t *parser, scope_t *scope){

    if(parser == NULL)
    {
        fprintf(stderr, "Error: NULL Parser encountered\n");
        exit(EXIT_FAILURE);
    }

    switch (parser->current_token->type)
    {
        case TOKEN_ID:
            return parser_parse_id(parser, scope);           
            break;
    }
    
    return init_ast(AST_NOOP);
}

ast_t *parser_parse_statements(parser_t *parser, scope_t *scope){

    if(parser == NULL || parser->current_token == NULL)
    {
        fprintf(stderr, "Error: NULL TOKEN ENCOUNTERED!\n");
        exit(EXIT_FAILURE);
    }

    ast_t *compound = init_ast(AST_COMPOUND);
    compound->scope = scope;
    ast_t *ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;

    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT));
    compound->compound_value[0] = ast_statement;
    compound->compound_size++;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);

        ast_t *ast_statement = parser_parse_statement(parser, scope);
        
        if(ast_statement){
            compound->compound_size++;
            compound->compound_value = realloc(
                compound->compound_value, 
                compound->compound_size*sizeof(struct AST_STRUCT*)
                );
        
                compound->compound_value[compound->compound_size-1] = ast_statement;
        }

    }

    return compound;   
}

ast_t *parser_parse_expression(parser_t *parser, scope_t *scope){
    if(parser == NULL)
        {
            fprintf(stderr, "Error: NULL Parser encountered\n");
            exit(EXIT_FAILURE);
        }

    switch (parser->current_token->type)
    {
        case TOKEN_STRING:
            return parser_parse_string(parser, scope);
            break;
        
        case TOKEN_ID:
            return parser_parse_id(parser, scope);           
            break;
    }

    return init_ast(AST_NOOP);
}

ast_t *parser_parse_function_call(parser_t *parser, scope_t *scope){
    if(parser == NULL)
    {
        fprintf(stderr, "Error: NULL Parser encountered\n");
        exit(EXIT_FAILURE);
    }

    ast_t * function_call = init_ast(AST_FUNCTION_CALL);
    function_call->funtion_call_name = parser->previous_token->value;
    parser_eat(parser, TOKEN_LPAREN);
    function_call->funtion_call_arguments = calloc(1, sizeof(struct AST_STRUCT*));
    
    ast_t *ast_expression = parser_parse_expression(parser, scope);
    function_call->funtion_call_arguments[0] = ast_expression;
    function_call->funtion_call_arguments_size++;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);
        ast_t *ast_expression = parser_parse_expression(parser, scope);
        function_call->funtion_call_arguments_size++;
        function_call->funtion_call_arguments = realloc(
            function_call->funtion_call_arguments, 
            function_call->funtion_call_arguments_size*sizeof(struct AST_STRUCT*)
            );

        function_call->funtion_call_arguments[function_call->funtion_call_arguments_size-1] = ast_expression;
    }
    parser_eat(parser, TOKEN_RPAREN);
    function_call->scope = scope;
     
    return function_call;
}

ast_t *parser_parse_variable(parser_t *parser, scope_t *scope){

    if(parser == NULL)
    {
        fprintf(stderr, "Error: NULL Parser encountered\n");
        exit(EXIT_FAILURE);
    }

    char *token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // varile name of function call

    if(parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_function_call(parser, scope);

    ast_t *ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    ast_variable->scope = scope;

    return ast_variable;
}

ast_t *parser_parse_variable_definition(parser_t *parser, scope_t *scope){
    parser_eat(parser, TOKEN_ID); // let, int, float, bool
    char *var_definition_var_name  = parser->current_token->value;

    parser_eat(parser, TOKEN_ID); // Variable name
    parser_eat(parser, TOKEN_EQUALS); 
    

    ast_t *var_definition_value = parser_parse_expression(parser, scope);
    
    
    ast_t *var_definition = init_ast(AST_VARIABLE_DEFINITION);

    var_definition->variable_definition_variable_name = var_definition_var_name;
    var_definition->variable_definition_value = var_definition_value;

    var_definition->scope=scope;
    return var_definition;
    
}

ast_t *parser_parse_function_definition(parser_t *parser, scope_t *scope)
{
    ast_t *ast = init_ast(AST_FUNCTION_DEFINITION);
    parser_eat(parser, TOKEN_ID); // function keyword

    char *function_name = parser->current_token->value;
    ast->funtion_definition_name = calloc(strlen(function_name)+1, sizeof(char));
    strcpy(ast->funtion_definition_name, function_name);

    parser_eat(parser, TOKEN_ID); //function name

    parser_eat(parser, TOKEN_LPAREN);
    //Function arguments
        ast ->function_definition_args = calloc(1, sizeof(struct AST_STRUCT*));
        
        ast_t *function_args = parser_parse_variable(parser, scope);
        ast->function_definition_args_size++;
        ast->function_definition_args[ast->function_definition_args_size-1] = function_args;

        while(parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);
            ast->function_definition_args_size++;
            ast ->function_definition_args = realloc(ast->function_definition_args, sizeof(struct AST_STRUCT*) * ast->function_definition_args_size);
            ast_t *function_args = parser_parse_variable(parser, scope);
            ast->function_definition_args[ast->function_definition_args_size-1] = function_args;
        }
    //End
    parser_eat(parser, TOKEN_RPAREN);
    
    parser_eat(parser, TOKEN_LBRACE);

    // Function definition
    ast -> function_definition_body = parser_parse_statements(parser, scope);

    parser_eat(parser, TOKEN_RBRACE);   

    ast->scope = scope;
     
    return ast; 
}

ast_t *parser_parse_string(parser_t *parser, scope_t *scope){
    ast_t *ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;
    parser_eat(parser, TOKEN_STRING);

    ast_string->scope = scope;
    return ast_string;
}


ast_t *parser_parse_id(parser_t *parser, scope_t *scope)
{
    if(strcmp(parser->current_token->value, "let")==0) // Generally vars can be declared with let but we should make a typed language
    {
        return parser_parse_variable_definition(parser, scope);
    }
    else if(strcmp(parser->current_token->value, "var")==0) // Generally vars can be declared with let but we should make a typed language
    {
        return parser_parse_variable_definition(parser, scope);
    }
    else if(strcmp(parser->current_token->value, "function")==0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    else{
        return parser_parse_variable(parser, scope);
    }


    /** The following will be added but for now lets stick to these few
     * Dict
     * Tuple
     * List
     * Map
     * Set
     */ 
}