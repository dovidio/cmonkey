#ifndef lexer_h
#define lexer_h

#import "sds.h"

typedef enum {
    TOKEN_ILLEGAL,
    TOKEN_EOF,
    
    TOKEN_IDENT,
    TOKEN_INT,
    TOKEN_STRING,
    
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_BANG,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    
    TOKEN_LT,
    TOKEN_GT,
    
    TOKEN_EQ,
    TOKEN_NOT_EQ,
    
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    
    TOKEN_FUNCTION,
    TOKEN_LET,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
} TokenType;

typedef struct {
    TokenType type;
    sds literal;
    int line;
} Token;

typedef struct {
    const char* input;
    int position;
    int read_position;
    char ch;
    int line;
} Lexer;

void init_lexer(const char* input);
Token next_token();
Token make_token(TokenType type, const char *start, int length, int line);

#endif
