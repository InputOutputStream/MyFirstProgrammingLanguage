#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "visitor.h"

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
    visitor->variable_definitions = (void *)0;
    visitor->variable_definitions_size = 0;

    return visitor;
}

ast_t *visitor_visit(visitor_t *visitor, ast_t *node){

    switch (node->type)
    {
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
       exit(EXIT_FAILURE);
    }

    if(strcmp(node->funtion_call_name, "print")==0)
    {
        return _builtin_print(visitor, node->funtion_call_arguments, node->funtion_call_arguments_size);
    }

    fprintf(stderr, "Undefined method: %s\n", node->funtion_call_name);
    exit(1);
}

ast_t *visitor_visit_variable_definition(visitor_t *visitor, ast_t *node){
    if (node == NULL)
    {
       perror("NULL AST NODE ENCOUNTERED BY VARIABLE DEFINITION VISITOR");
       exit(EXIT_FAILURE);
    }

    if(visitor->variable_definitions == (void *)0)
    {
        visitor->variable_definitions = calloc(1, sizeof(struct AST_STRUC*));
        visitor->variable_definitions[0] = node;
        visitor->variable_definitions_size++;
    }   
    else
    {
        visitor->variable_definitions_size++;
        visitor->variable_definitions = realloc(visitor->variable_definitions, 
            visitor->variable_definitions_size * sizeof(struct AST_STRUCT*));
        if(!visitor->variable_definitions)
        {
            fprintf(stderr, "Realloc failed on viditor variable definitions\n");
            for (size_t i = 0; i < visitor->variable_definitions_size; i++)
            {
                free(visitor->variable_definitions[i]);
            }
            free(visitor);
            exit(EXIT_FAILURE);
        }
        visitor->variable_definitions[visitor->variable_definitions_size-1] = node;
    }
    
    return node;
}

ast_t *visitor_visit_string(visitor_t *visitor, ast_t *node){
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
    for (size_t i = 0; i < visitor->variable_definitions_size; i++)
    {
        ast_t *var_def = visitor->variable_definitions[i];
        if(strcmp(var_def->variable_definition_variable_name, node->variable_name)==0)
        {
            return visitor_visit(visitor, var_def->variable_definition_value);
        }
    }
    
    fprintf(stderr,"Undefinied variable %s\n", node->variable_name);
    exit(EXIT_FAILURE);
    return node;
}
