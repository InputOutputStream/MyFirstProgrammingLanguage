#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "visitor.h"
#include "io.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Error: No Input File!\n");
        fprintf(stderr, "Usage: ./edu <file.edu>\n");
        exit(EXIT_FAILURE);
    }

    lexer_t *lexer = init_lexer(
  		get_file(argv[1])
    );

    parser_t *parser = init_parser(lexer);

    ast_t *root = parser_parse(parser);
    visitor_t *visitor = init_visitor();

    visitor_visit(visitor, root);
    return 0;
}
