#include "ast.h"
#include "memory.h"
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

Node *new_identifier_node(Token token) {
  Node *node = ALLOCATE(Node, 1);

  node->type = NODE_IDENTIFIER;
  Identifier *ident = AS_IDENTIFIER(node);
  ident->token = token;
  ident->start = token.start;
  ident->length = token.length;

  return node;
}

void add_statement(Node *program_node, Node *statement) {
  if (!IS_PROGRAM(program_node))
    return;

  Program *program = AS_PROGRAM(program_node);

  if (program->statement_count >= program->statement_capacity) {
    int old_capacity = program->statement_capacity;
    program->statement_capacity = GROW_CAPACITY(old_capacity);
    program->statements = GROW_ARRAY(Node *, program->statements, old_capacity,
                                     program->statement_capacity);
  }

  program->statements[program->statement_count] = statement;
  program->statement_count++;
}
