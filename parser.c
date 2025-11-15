#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Parser parser;

static Node *parse_statement();
static Node *parse_let_statement();
static Node *parse_return_statement();

void init_parser(const char *input) {
  init_lexer(input);

  parser.errors = ALLOCATE(ParseError, 8);
  parser.error_count = 0;
  parser.error_capacity = 8;

  next_token_parser();
  next_token_parser();
}

void next_token_parser() {
  parser.current_token = parser.peek_token;
  parser.peek_token = next_token();
}

int expect_peek(TokenType type) {
  if (parser.peek_token.type == type) {
    next_token_parser();
    return 1;
  } else {
    peek_error(type);
    return 0;
  }
}

void peek_error(TokenType type) {
  if (parser.error_count >= parser.error_capacity) {
    int old_capacity = parser.error_capacity;
    parser.error_capacity = GROW_CAPACITY(old_capacity);
    parser.errors = GROW_ARRAY(ParseError, parser.errors, old_capacity,
                               parser.error_capacity);
  }

  ParseError *error = &parser.errors[parser.error_count];
  error->message = ALLOCATE(char, 256);

  snprintf(error->message, 256, "expected next token to be %s, got %s instead",
           token_type_to_string(type),
           token_type_to_string(parser.peek_token.type));

  parser.error_count++;
}

ParseError *get_errors(int *count) {
  *count = parser.error_count;
  return parser.errors;
}

Node *parse_program() {
  Node *program_node = new_program_node();

  while (parser.current_token.type != TOKEN_EOF) {
    Node *statement = parse_statement();
    if (statement) {
      add_statement(program_node, statement);
    }
    next_token_parser();
  }

  return program_node;
}

static Node *parse_statement() {
  switch (parser.current_token.type) {
  case TOKEN_LET:
    return parse_let_statement();
  case TOKEN_RETURN:
    return parse_return_statement();
  default:
    return NULL;
  }
}

static Node *parse_let_statement() {
  Node *stmt_node = new_let_statement_node(parser.current_token);
  if (!expect_peek(TOKEN_IDENT)) {
    FREE(Node, stmt_node);
    return NULL;
  }

  Node *ident_node = new_identifier_node(parser.current_token);

  LetStatement *stmt = AS_LET_STATEMENT(stmt_node);
  stmt->name = AS_IDENTIFIER(ident_node);

  if (!expect_peek(TOKEN_ASSIGN)) {
    FREE(Node, ident_node);
    FREE(Node, stmt_node);
    return NULL;
  }

  while (parser.current_token.type != TOKEN_SEMICOLON &&
         parser.current_token.type != TOKEN_EOF) {
    next_token_parser();
  }

  return stmt_node;
}

static Node *parse_return_statement() {
  Node *return_statement = new_return_statement_node(parser.current_token);

  while (parser.current_token.type != TOKEN_SEMICOLON &&
         parser.current_token.type != TOKEN_EOF) {
    next_token_parser();
  }

  return return_statement;
}
