#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

ast_t *visitor_visit(ast_t *node);
ast_t *visitor_visit_function_call(ast_t *node);
ast_t *visitor_visit_variable_definition(ast_t *node);
ast_t *visitor_visit_string(ast_t *node);
ast_t *visitor_visit_compound(ast_t *node);
ast_t *visitor_visit_variable(ast_t *node);

#endif // !VISITOR_H