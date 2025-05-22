#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// int main(int argc, char **argv)
int main()
{
    lexer_t *lexer = init_lexer(
		"int name = \"john banga\"; print(name);\n");

    parser_t *parser = init_parser(lexer);
    ast_t *root = parser_parse(parser);

     

    
    fprintf(stdout, "\n\nTerminated/..............\n");
    return 0;
}
