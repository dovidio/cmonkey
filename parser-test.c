#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <cmocka.h>

static void checkParseErrors() {
  int error_count;
  ParseError *errors = get_errors(&error_count);

  if (error_count == 0) {
    return;
  }

  fprintf(stderr, "Parser has %d %s:\n", error_count,
          error_count == 1 ? "error" : "errors");

  for (int i = 0; i < error_count; i++) {
    fprintf(stderr, "\t%s\n", errors[i].message);
  }

  fail_msg("Parser encountered errors (see stderr for details)");
}

static int testLetStatement(Node *stmt, const char *name) {
  if (!IS_LET_STATEMENT(stmt)) {
    printf("Statement is not a let statement\n");
    return 0;
  }

  LetStatement *let_stmt = AS_LET_STATEMENT(stmt);
  if (let_stmt->name == NULL) {
    printf("Let statement name is NULL\n");
    return 0;
  }

  char actual_name[256];
  snprintf(actual_name, sizeof(actual_name), "%.*s",
           let_stmt->name->token.length, let_stmt->name->token.start);

  if (strcmp(actual_name, name) != 0) {
    printf("Let statement name not '%s'. got=%s\n", name, actual_name);
    return 0;
  }

  return 1;
}

static void test_let_statements(void **state) {
  (void)state;

  const char *input = "let x = 5;\n"
                      "let y = 10;\n"
                      "let foobar = 838383;";

  init_parser(input);
  Node *program_node = parse_program();

  checkParseErrors();

  assert_non_null(program_node);

  if (!IS_PROGRAM(program_node)) {
    fail_msg("ParseProgram() did not return a program node");
  }

  Program *program = AS_PROGRAM(program_node);
  if (program->statement_count != 3) {
    fail_msg("program.Statements does not contain 3 statements. got=%d",
             program->statement_count);
  }

  const char *expected_identifiers[] = {"x", "y", "foobar"};

  for (int i = 0; i < 3; i++) {
    Node *stmt = program->statements[i];
    if (!testLetStatement(stmt, expected_identifiers[i])) {
      fail_msg("testLetStatement failed for identifier '%s'",
               expected_identifiers[i]);
    }
  }
}

static void test_return_statements(void **state) {
  (void)state;

  const char *input = "return 5;\n"
                      "return true;\n"
                      "return foobar;";

  init_parser(input);
  Node *program_node = parse_program();

  checkParseErrors();

  assert_non_null(program_node);

  if (!IS_PROGRAM(program_node)) {
    fail_msg("ParseProgram() did not return a program node");
  }

  Program *program = AS_PROGRAM(program_node);
  if (program->statement_count != 3) {
    fail_msg("program.Statements does not contain 3 statements. got=%d",
             program->statement_count);
  }

  for (int i = 0; i < 3; i++) {
    Node *stmt = program->statements[i];
    if (!IS_RETURN_STATEMENT(stmt)) {
      fail_msg("Statement %d is not a return statement", i);
    }

    ReturnStatement *return_stmt = AS_RETURN_STATEMENT(stmt);
    // We'll add return value testing later when we implement expressions
    (void)return_stmt; // Suppress unused variable warning
  }
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_let_statements),
      cmocka_unit_test(test_return_statements),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
