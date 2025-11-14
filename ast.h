#ifndef ast_h
#define ast_h

#include "lexer.h"

typedef enum {
    NODE_PROGRAM,
    NODE_LET_STATEMENT,
    NODE_RETURN_STATEMENT,
    NODE_IDENTIFIER
} NodeType;

typedef struct Node Node;
typedef struct Program Program;
typedef struct LetStatement LetStatement;
typedef struct ReturnStatement ReturnStatement;
typedef struct Identifier Identifier;

struct Identifier {
    Token token;
    const char* start;
    int length;
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
        Identifier identifier;
    } as;
};

#define IS_PROGRAM(node) ((node)->type == NODE_PROGRAM)
#define IS_LET_STATEMENT(node) ((node)->type == NODE_LET_STATEMENT)
#define IS_RETURN_STATEMENT(node) ((node)->type == NODE_RETURN_STATEMENT)
#define IS_IDENTIFIER(node) ((node)->type == NODE_IDENTIFIER)

#define AS_PROGRAM(node) (&(node)->as.program)
#define AS_LET_STATEMENT(node) (&(node)->as.let_statement)
#define AS_RETURN_STATEMENT(node) (&(node)->as.return_statement)
#define AS_IDENTIFIER(node) (&(node)->as.identifier)

Node* new_program_node();
Node* new_let_statement_node(Token token);
Node* new_return_statement_node(Token token);
Node* new_identifier_node(Token token);
void add_statement(Node* program_node, Node* statement);

#endif