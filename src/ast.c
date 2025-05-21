#include <stdlib.h>
#include <stdio.h>


#include "ast.h"
#include "lexer.h"


ast_t *init_ast(int type){
    ast_t* ast = calloc(1, sizeof(struct AST_STRUCT));
    if(ast == NULL)
    {
        fprintf(stderr, "Error: AST creation failed\n");
        exit(EXIT_FAILURE);
    }
    ast->type = type;

    /**Ast Variable definition */
    ast->variable_definition_variable_name = NULL;
    ast->variable_definition_value = NULL;

    /**Ast variable */
    ast->variable_name = NULL;

    /**Funtion Call */
    ast->funtion_call_name = NULL;
    ast->funtion_call_arguments_size = 0;
    ast->funtion_call_arguments=NULL;

    /**Ast String */
    ast->string_value = NULL;

    /**AST Compound*/
    ast->compound_value = NULL;
    ast->compound_size = 0;
    return ast;
}

 