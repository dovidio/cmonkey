#include "ast.h"
#include "lexer.h"
#include "sds.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <cmocka.h>

static void test_string(void **state) {
  (void)state;
  Node *program_node = new_program_node();
  Program *program = AS_PROGRAM(program_node);

  Token myVar_token = make_token(TOKEN_IDENT, "myVar", 5, 0);
  Node *myVar_node = new_identifier_node(myVar_token);

  Token anotherVar_token = make_token(TOKEN_IDENT, "anotherVar", 10, 0);
  Node *anotherVar_node = new_identifier_node(anotherVar_token);

  Token let_token = make_token(TOKEN_LET, "let", 3, 0);
  Node *let_node = new_let_statement_node(let_token);
  LetStatement *let_statement = AS_LET_STATEMENT(let_node);
  let_statement->name = AS_IDENTIFIER(myVar_node);
  let_statement->value = anotherVar_node;

  add_statement(program, let_node);

  sds actual = node_to_string(program_node);
  sds expected = sdsnew("let myVar = anotherVar;");
  if (sdscmp(actual, expected)) {
    fail_msg("expected %s, found %s", expected, actual);
  }
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_string),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
