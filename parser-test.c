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

  printf("Parser has %d %s:\n", error_count,
         error_count == 1 ? "error" : "errors");

  for (int i = 0; i < error_count; i++) {
    printf("\t%s\n", errors[i].message);
  }

  fail();
}

static void test_let_statements(void **state) {
  (void)state;

  const char *input = "let x = 5;\n"  // Missing = sign
                      "let y = 10;\n" // Missing identifier
                      "let foobar = 838383;";

  init_parser(input);
  Node *program_node = parse_program();

  printf("Statement count: %d\n", AS_PROGRAM(program_node)->statement_count);

  checkParseErrors();
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_let_statements),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
