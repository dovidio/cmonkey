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

  sds actual_name = let_stmt->name->token.literal;
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

static void test_identifier_expression(void **state) {
  (void)state;

  const char *input = "foobar";
  init_parser(input);
  Node *program_node = parse_program();

  checkParseErrors();

  assert_non_null(program_node);

  if (!IS_PROGRAM(program_node)) {
    fail_msg("ParseProgram() did not return a program node");
  }

  Program *program = AS_PROGRAM(program_node);

  if (program->statement_count != 1) {
    fail_msg("program.Statements does not contain 1 statement. got=%d",
             program->statement_count);
  }

  Node *stmt = program->statements[0];

  if (!IS_EXPRESSION_STATEMENT(stmt)) {
    fail_msg("Statement is not an expression statement");
  }

  ExpressionStatement *expr_stmt = AS_EXPRESSION_STATEMENT(stmt);
  Node *ident_stmt = expr_stmt->expression;
  if (!IS_IDENTIFIER(ident_stmt)) {
    fail_msg("Expression is not an identifier");
  }

  Identifier *ident = AS_IDENTIFIER(ident_stmt);
  if (strcmp(ident->token.literal, "foobar") != 0) {
    fail_msg("Ident value not %s, got %s", "foobar", ident->token.literal);
  }
}

static void test_integer_literal(void **state) {
  (void)state;

  const char *input = "5;";
  init_parser(input);
  Node *program_node = parse_program();

  checkParseErrors();

  assert_non_null(program_node);

  if (!IS_PROGRAM(program_node)) {
    fail_msg("ParseProgram() did not return a program node");
  }

  Program *program = AS_PROGRAM(program_node);

  if (program->statement_count != 1) {
    fail_msg("program.Statements does not contain 1 statement. got=%d",
             program->statement_count);
  }

  Node *stmt = program->statements[0];

  if (!IS_EXPRESSION_STATEMENT(stmt)) {
    fail_msg("Statement is not an expression statement");
  }

  ExpressionStatement *expr_stmt = AS_EXPRESSION_STATEMENT(stmt);
  Node *integer_stmt = expr_stmt->expression;
  if (!IS_INTEGER_LITERAL(integer_stmt)) {
    fail_msg("Expression is not an integer literal");
  }

  IntegerLiteral *integer_literal = AS_INTEGER_LITERAL(integer_stmt);
  if (strcmp(integer_literal->token.literal, "5") != 0) {
    fail_msg("Integer value not %s, got %s", "5",
             integer_literal->token.literal);
  }

  if (integer_literal->value != 5) {
    fail_msg("Integer value not %d, got %d", 5, integer_literal->value);
  }
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_let_statements),
      cmocka_unit_test(test_return_statements),
      cmocka_unit_test(test_identifier_expression),
      cmocka_unit_test(test_integer_literal),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
