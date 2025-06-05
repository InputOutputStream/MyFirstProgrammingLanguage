#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

typedef struct VISITOR_STRUCT
{

}visitor_t;


visitor_t *init_visitor(void);

ast_t *visitor_visit(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_function_call(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_function_definition(visitor_t *visitor, ast_t* node);
ast_t *visitor_visit_variable_definition(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_string(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_compound(visitor_t *visitor, ast_t *node);
ast_t *visitor_visit_variable(visitor_t *visitor, ast_t *node);

#endif // !VISITOR_H