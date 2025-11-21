#ifndef ast_h
#define ast_h

#include <stdint.h>
#include "lexer.h"
#include "sds.h"

typedef enum {
    NODE_PROGRAM,
    NODE_LET_STATEMENT,
    NODE_RETURN_STATEMENT,
    NODE_EXPRESSION_STATEMENT,
    NODE_IDENTIFIER,
    NODE_INTEGER_LITERAL,
} NodeType;

typedef struct Node Node;
typedef struct Program Program;
typedef struct LetStatement LetStatement;
typedef struct ReturnStatement ReturnStatement;
typedef struct ExpressionStatement ExpressionStatement;
typedef struct Identifier Identifier;
typedef struct IntegerLiteral IntegerLiteral;

struct Identifier {
    Token token;
};

struct LetStatement {
    Token token;
    Identifier* name;
    Node* value;
};

struct ReturnStatement {
    Token token;
    Node* return_value;
};

struct ExpressionStatement {
    Token token;
    Node* expression;
};

struct IntegerLiteral {
    Token token;
    uint64_t value;
};

struct Program {
    Node** statements;
    int statement_count;
    int statement_capacity;
};

struct Node {
    NodeType type;
    union {
        Program program;
        LetStatement let_statement;
        ReturnStatement return_statement;
        ExpressionStatement expression_statement;
        Identifier identifier;
        IntegerLiteral integer_literal;
    } as;
};

#define IS_PROGRAM(node) ((node)->type == NODE_PROGRAM)
#define IS_LET_STATEMENT(node) ((node)->type == NODE_LET_STATEMENT)
#define IS_RETURN_STATEMENT(node) ((node)->type == NODE_RETURN_STATEMENT)
#define IS_IDENTIFIER(node) ((node)->type == NODE_IDENTIFIER)
#define IS_EXPRESSION_STATEMENT(node) ((node)->type == NODE_EXPRESSION_STATEMENT)
#define IS_INTEGER_LITERAL(node) ((node)->type == NODE_INTEGER_LITERAL)

#define AS_PROGRAM(node) (&(node)->as.program)
#define AS_LET_STATEMENT(node) (&(node)->as.let_statement)
#define AS_RETURN_STATEMENT(node) (&(node)->as.return_statement)
#define AS_EXPRESSION_STATEMENT(node) (&(node)->as.expression_statement)
#define AS_IDENTIFIER(node) (&(node)->as.identifier)
#define AS_INTEGER_LITERAL(node) (&(node)->as.integer_literal)

Node* new_program_node();
Node* new_let_statement_node(Token token);
Node* new_return_statement_node(Token token);
Node* new_identifier_node(Token token);
Node* new_expression_node(Token token, Node *node);
Node* new_integer_literal(Token token, uint64_t value);
sds node_to_string(Node *node);
const char *token_type_to_string(TokenType type);
void add_statement(Program *program, Node* statement);

#endif
