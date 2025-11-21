#ifndef parser_h
#define parser_h

#include "lexer.h"
#include "ast.h"

typedef struct {
    char* message;
} ParseError;

typedef struct {
    Token current_token;
    Token peek_token;
    ParseError* errors;
    int error_count;
    int error_capacity;
} Parser;

typedef enum {
    LOWEST,
    EQUALS,
    LESS_GREATER,
    SUM,
    PRODUCT,
    PREFIX,
    CALL
} Precedence;

void init_parser(const char* input);
Node* parse_program();
void parse_next_token();
int expect_peek(TokenType type);
void peek_error(TokenType type);
ParseError* get_errors(int* count);

#endif
