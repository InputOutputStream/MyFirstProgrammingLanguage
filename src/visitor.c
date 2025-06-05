#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "visitor.h"
#include "scope.h"

static ast_t* _builtin_print(visitor_t *visitor, struct AST_STRUCT** args, size_t args_size)
{
    for (size_t i = 0; i < args_size; i++)
    {
        ast_t *visited_ast = visitor_visit(visitor,  args[i]);
        switch (visited_ast->type)
        {
            case AST_STRING:
                fprintf(stderr, "%s\n", visited_ast->string_value);
                break;
            default:
                printf("%p\n", (void *)visited_ast);
                break;
        } 

    }

    return init_ast(AST_NOOP);
}

visitor_t *init_visitor(void)
{
    visitor_t *visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

    return visitor;
}

ast_t *visitor_visit(visitor_t *visitor, ast_t *node){

    switch (node->type)
    {
        case AST_FUNCTION_DEFINITION:
            return visitor_visit_function_definition(visitor, node); 
            break;
        case AST_VARIABLE_DEFINITION:
            return visitor_visit_variable_definition(visitor,node);
            break;
        case AST_VARIABLE:
            return visitor_visit_variable(visitor,node);
            break;
        case AST_FUNCTION_CALL:
            return visitor_visit_function_call(visitor,node);
            break;
        case AST_STRING:
            return visitor_visit_string(visitor,node);
            break;
        case AST_COMPOUND:
            return visitor_visit_compound(visitor,node);
            break;
        case AST_NOOP:
            return node;
            break;
    }

    printf("Uncaught statetement of type %d\n", node->type);
    exit(EXIT_FAILURE);

    return init_ast(AST_NOOP);
}

ast_t *visitor_visit_function_call(visitor_t *visitor, ast_t *node){
    if (node == NULL)
    {
       perror("NULL AST NODE ENCOUNTERED FUNCTION CALL VSISITOR");
       exit(EXIT_FAILURE );
    }

    if(strcmp(node->funtion_call_name, "print")==0)
    {
        return _builtin_print(visitor, node->funtion_call_arguments, node->funtion_call_arguments_size);
    }

    ast_t *function_def = scope_get_function_definition(node->scope, node->funtion_call_name);

    if(function_def == (void *)0){
        fprintf(stderr, "Undefined method: %s\n", node->funtion_call_name);
        exit(1);
    }

    for(size_t i = 0; i<function_def->function_definition_args_size; i++)
    {
        ast_t *function_def_arg = function_def->function_definition_args[i];
        ast_t *ast_function_call_args_value = node->funtion_call_arguments[i];

        //Create variable definition
        ast_t *variable_def = init_ast(AST_VARIABLE_DEFINITION);

        //Copy to variable definition the corresponding function definition parameter name
        variable_def->variable_definition_variable_name = calloc(strlen(function_def_arg->variable_name)+1, sizeof(char));
        strcpy(variable_def->variable_definition_variable_name, function_def_arg->variable_name);

        //Assign to function definition variable the function call value given
        variable_def->variable_definition_value = ast_function_call_args_value;

        //Add the created variable to function scope
        scope_add_variable_definition(function_def->function_definition_body->scope,
                                        variable_def);
    }

    return visitor_visit(visitor, function_def->function_definition_body);

}

ast_t *visitor_visit_function_definition(visitor_t __attribute_maybe_unused__ *visitor, ast_t* node)
{
    scope_add_function_definition(node->scope, node);
    return node;
}


ast_t *visitor_visit_variable_definition(visitor_t __attribute_maybe_unused__ *visitor, ast_t *node){
    scope_add_variable_definition(node->scope, node);
    return node;
}


ast_t *visitor_visit_string(visitor_t __attribute_maybe_unused__ *visitor, ast_t *node){
    if (node == NULL)
    {
       perror("NULL AST NODE ENCOUNTERED BY STRING");
       exit(EXIT_FAILURE);
    }

    return node;
}

ast_t *visitor_visit_compound(visitor_t *visitor, ast_t *node){
    for(size_t i=0; i<node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);     
}

ast_t *visitor_visit_variable(visitor_t *visitor, ast_t *node){
    if (node == NULL)
    {
       perror("NULL AST NODE ENCOUNTERED BY VARIABLE VISITOR");
       exit(EXIT_FAILURE);
    }
    if (visitor == NULL)
    {
       perror("NULL AST VISITOR ENCOUNTERED");
       exit(EXIT_FAILURE);
    }

   ast_t *variable_definition = scope_get_variable_definition(node->scope, node->variable_name);


    if(variable_definition == (void *)0)
    {
        fprintf(stderr,"Undefinied variable `%s`\n", node->variable_name);
        exit(EXIT_FAILURE);
    }

    return visitor_visit(visitor, variable_definition->variable_definition_value);

}
