#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// int main(int argc, char **argv)
int main()
{
    lexer_t *lexer = init_lexer(
		"int c = \"john banga\"; print(c);\n");

    parser_t *parser = init_parser(lexer);
    ast_t *root = parser_parse(parser);

    fprintf(stdout, "Root Type = %d\n", root->type);
    fprintf(stdout, "Compound Size = %d", root->compound_size);
    
    fprintf(stdout, "\n\nTerminated/..............\n");
    return 0;
}
