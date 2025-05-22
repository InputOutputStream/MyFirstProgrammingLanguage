
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"


lexer_t *init_lexer(char *contents){
    if(contents == NULL)
        return NULL;

    lexer_t *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    if(lexer == NULL)
    {
        fprintf(stderr, "Error: Lexer creation failed\n");
        exit(EXIT_FAILURE);
    }
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_t *lexer){
    if(lexer == NULL)
        return;

    if(lexer->i < strlen(lexer->contents) - 1)
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
    else
    {
        lexer->c = '\0';
    }
}


void lexer_skip_white_space(lexer_t *lexer){
    if(lexer == NULL)
        return;

    while(lexer->c == ' ' || lexer->c == 10)
    {
        lexer_advance(lexer);
        if(lexer->c == '\0')
            break;
    } 
}



token_t *lexer_get_next_token(lexer_t *lexer)
{
    if(lexer == NULL)
        return NULL;

    if(lexer->c == '\0')
        return NULL;

    while(lexer->c not '\0' and lexer->i < strlen(lexer->contents))
    {
        if(lexer->c == ' ' or lexer->c == 10)
            lexer_skip_white_space(lexer);
 
        if(isalnum(lexer->c))
            return lexer_collect_id(lexer);

        if(lexer->c == '"')
            return lexer_collect_string(lexer);
       

        switch(lexer->c)
        {
            case '=':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
                break;
            case ',':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
                break;
            case ';':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
                break;
            case '(':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
                break;
            case ')':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
                break;
            case '[':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_LSBRACE, lexer_get_current_char_as_string(lexer)));
                break;
            case ']':
                return _lexer_advance_with_token(lexer, init_token(TOKEN_RSBRACE, lexer_get_current_char_as_string(lexer)));
                break;
        }
    }

    return init_token(TOKEN_EOF, "\0");
}


token_t *_lexer_advance_with_token(lexer_t *lexer, token_t *token)
{
    if(lexer == NULL)
        return NULL;

    lexer_advance(lexer);
    return token;
}

token_t *lexer_collect_string(lexer_t *lexer){
    if(lexer == NULL)
        return NULL;

    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    if(value == NULL)
    {
        fprintf(stderr, "Error: Lexer Advance Allocation object creation failed\n");
        exit(EXIT_FAILURE);
    }
    value[0] = '\0';
    while(lexer->c != '"' && lexer->c != '\0')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advance(lexer);
    }
    
    if(lexer->c != '"')
    {
        // Error: Unterminated string literal
        free(value);
        return NULL;
    }
    
    lexer_advance(lexer);

    return init_token(TOKEN_STRING, value);
}


token_t *lexer_collect_id(lexer_t *lexer)
{
    if(lexer == NULL)
        return NULL;

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while(isalnum(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advance(lexer);
    }

    return init_token(TOKEN_ID, value);
}


char *lexer_get_current_char_as_string(lexer_t *lexer){
    if(lexer == NULL)
        return NULL;

    static char str[2];
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}

