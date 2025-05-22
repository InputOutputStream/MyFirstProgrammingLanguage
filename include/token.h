#ifndef TOKEN_H
#define TOKEN_H


typedef struct TOKEN_STRUCT
{
    enum{
         TOKEN_ID,
         TOKEN_EQUALS,
         TOKEN_STRING,
         TOKEN_SEMI,
         TOKEN_COMMA,
         TOKEN_LPAREN,
         TOKEN_RPAREN,
         TOKEN_LSBRACE,
         TOKEN_RSBRACE,
         TOKEN_EOF,
    }type;
    
    char *value;
}token_t;


token_t *init_token(int type, char *value);



#endif // TOKEN
