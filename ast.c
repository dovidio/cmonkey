#include "ast.h"
#include "memory.h"
#include "sds.h"
#include <stdlib.h>
#include <string.h>

Node *new_program_node() {
  Node *node = ALLOCATE(Node, 1);

  node->type = NODE_PROGRAM;
  Program *program = AS_PROGRAM(node);
  program->statements = ALLOCATE(Node *, 8);
  program->statement_count = 0;
  program->statement_capacity = 8;

  return node;
}

Node *new_let_statement_node(Token token) {
  Node *node = ALLOCATE(Node, 1);

  node->type = NODE_LET_STATEMENT;
  LetStatement *stmt = AS_LET_STATEMENT(node);
  stmt->token = token;
  stmt->name = NULL;
  stmt->value = NULL;

  return node;
}

Node *new_return_statement_node(Token token) {
  Node *node = ALLOCATE(Node, 1);

  node->type = NODE_RETURN_STATEMENT;
  ReturnStatement *stmt = AS_RETURN_STATEMENT(node);
  stmt->token = token;
  stmt->return_value = NULL;

  return node;
}

Node *new_identifier_node(Token token) {
  Node *node = ALLOCATE(Node, 1);

  node->type = NODE_IDENTIFIER;
  Identifier *ident = AS_IDENTIFIER(node);
  ident->token = token;

  return node;
}

Node *new_expression_node(Token token, Node *expression) {
  Node *node = ALLOCATE(Node, 1);

  node->type = NODE_EXPRESSION_STATEMENT;
  ExpressionStatement *statement = AS_EXPRESSION_STATEMENT(node);
  statement->token = token;
  statement->expression = expression;

  return node;
}

const char *token_type_to_string(TokenType type) {
  switch (type) {
  case TOKEN_ILLEGAL:
    return "ILLEGAL";
  case TOKEN_EOF:
    return "EOF";
  case TOKEN_IDENT:
    return "IDENT";
  case TOKEN_INT:
    return "INT";
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_ASSIGN:
    return "=";
  case TOKEN_PLUS:
    return "+";
  case TOKEN_MINUS:
    return "-";
  case TOKEN_BANG:
    return "!";
  case TOKEN_ASTERISK:
    return "*";
  case TOKEN_SLASH:
    return "/";
  case TOKEN_LT:
    return "<";
  case TOKEN_GT:
    return ">";
  case TOKEN_EQ:
    return "==";
  case TOKEN_NOT_EQ:
    return "!=";
  case TOKEN_COMMA:
    return ",";
  case TOKEN_SEMICOLON:
    return ";";
  case TOKEN_LPAREN:
    return "(";
  case TOKEN_RPAREN:
    return ")";
  case TOKEN_LBRACE:
    return "{";
  case TOKEN_RBRACE:
    return "}";
  case TOKEN_LBRACKET:
    return "[";
  case TOKEN_RBRACKET:
    return "]";
  case TOKEN_FUNCTION:
    return "FUNCTION";
  case TOKEN_LET:
    return "LET";
  case TOKEN_TRUE:
    return "TRUE";
  case TOKEN_FALSE:
    return "FALSE";
  case TOKEN_IF:
    return "IF";
  case TOKEN_ELSE:
    return "ELSE";
  case TOKEN_RETURN:
    return "RETURN";
  default:
    return "UNKNOWN";
  }
}

sds node_to_string(Node *node) {
  switch (node->type) {
  case NODE_PROGRAM: {
    Program *program = AS_PROGRAM(node);
    sds s = sdsnew("");
    for (int i = 0; i < program->statement_count; i++) {
      Node *node = program->statements[i];
      s = sdscat(s, node_to_string(node));
    }

    return s;
  }
  case NODE_LET_STATEMENT: {
    LetStatement *letStatement = AS_LET_STATEMENT(node);
    sds s = sdsnewlen(letStatement->token.start, letStatement->token.length);
    s = sdscat(s, " ");
    s = sdscat(s, letStatement->name->token.literal);
    s = sdscat(s, " = ");
    if (letStatement->value != NULL) {
      s = sdscat(s, node_to_string(letStatement->value));
    }
    s = sdscat(s, ";");

    return s;
  }
  case NODE_RETURN_STATEMENT: {
    ReturnStatement *returnStatement = AS_RETURN_STATEMENT(node);
    sds s = sdsnew(token_type_to_string(returnStatement->token.type));
    if (returnStatement->return_value != NULL) {
      s = sdscat(s, node_to_string(node));
    }

    return s;
  }
  case NODE_IDENTIFIER: {
    Identifier *identifier = AS_IDENTIFIER(node);
    sds s = sdsnew(identifier->token.literal);
    return s;
  }
  case NODE_EXPRESSION_STATEMENT: {
    ExpressionStatement *expressionStatement = AS_EXPRESSION_STATEMENT(node);
    if (expressionStatement->expression != NULL) {
      return node_to_string(expressionStatement->expression);
    }
    return sdsnew("");
  }
  }
}

void add_statement(Program *program, Node *statement) {
  if (program->statement_count >= program->statement_capacity) {
    int old_capacity = program->statement_capacity;
    program->statement_capacity = GROW_CAPACITY(old_capacity);
    program->statements = GROW_ARRAY(Node *, program->statements, old_capacity,
                                     program->statement_capacity);
  }

  program->statements[program->statement_count] = statement;
  program->statement_count++;
}
