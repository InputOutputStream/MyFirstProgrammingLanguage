#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct AST_STRUCT
{
    enum{
        AST_FUNCTION_CALL,
        AST_FUNCTION_DEFINITION,
        AST_VARIABLE_DEFINITION,
        AST_VARIABLE,     
        AST_STRING,
        AST_COMPOUND, // List of multiple statements[nodes]
        AST_NOOP,
    }type;

    /**Scope  */
    struct SCOPE_STRUCT *scope;

    /**Ast Variable definition */
    char *variable_definition_variable_name;
    struct AST_STRUCT* variable_definition_value;

    /**AST function definition */
    char *funtion_definition_name;
    struct AST_STRUCT *function_definition_body;
    struct AST_STRUCT **function_definition_args;
    size_t function_definition_args_size;
    
    /**Ast variable */
    char *variable_name;

    /**Funtion Call */
    char *funtion_call_name;
    struct  AST_STRUCT** funtion_call_arguments;
    size_t funtion_call_arguments_size;

    /**Ast String */
    char *string_value;

    /**AST Compound*/
    struct AST_STRUCT **compound_value;
    size_t compound_size;

}ast_t;


ast_t *init_ast(int type);

#endif
