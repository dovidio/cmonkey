#include "lexer.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <cmocka.h>

typedef struct {
  TokenType expected_type;
  const char *expected_literal;
  int expected_line;
} ExpectedToken;

static void test_next_token(void **state) {
  (void)state;

  const char *input = "let five = 5;\n"
                      "let ten = 10;\n"
                      "\n"
                      "let add = fn(x, y) {\n"
                      "  x + y;\n"
                      "};\n"
                      "\n"
                      "let result = add(five, ten);\n"
                      "!-/*5;\n"
                      "5 < 10 > 5;\n"
                      "\n"
                      "if (5 < 10) {\n"
                      "    return true;\n"
                      "} else {\n"
                      "    return false;\n"
                      "}\n"
                      "\n"
                      "10 == 10;\n"
                      "10 != 9;\n"
                      "\"foobar\"\n"
                      "\"foo bar\"\n"
                      "[1, 2];";

  ExpectedToken tests[] = {
      {TOKEN_LET, "let", 1},         {TOKEN_IDENT, "five", 1},
      {TOKEN_ASSIGN, "=", 1},        {TOKEN_INT, "5", 1},
      {TOKEN_SEMICOLON, ";", 1},     {TOKEN_LET, "let", 2},
      {TOKEN_IDENT, "ten", 2},       {TOKEN_ASSIGN, "=", 2},
      {TOKEN_INT, "10", 2},          {TOKEN_SEMICOLON, ";", 2},
      {TOKEN_LET, "let", 4},         {TOKEN_IDENT, "add", 4},
      {TOKEN_ASSIGN, "=", 4},        {TOKEN_FUNCTION, "fn", 4},
      {TOKEN_LPAREN, "(", 4},        {TOKEN_IDENT, "x", 4},
      {TOKEN_COMMA, ",", 4},         {TOKEN_IDENT, "y", 4},
      {TOKEN_RPAREN, ")", 4},        {TOKEN_LBRACE, "{", 4},
      {TOKEN_IDENT, "x", 5},         {TOKEN_PLUS, "+", 5},
      {TOKEN_IDENT, "y", 5},         {TOKEN_SEMICOLON, ";", 5},
      {TOKEN_RBRACE, "}", 6},        {TOKEN_SEMICOLON, ";", 6},
      {TOKEN_LET, "let", 8},         {TOKEN_IDENT, "result", 8},
      {TOKEN_ASSIGN, "=", 8},        {TOKEN_IDENT, "add", 8},
      {TOKEN_LPAREN, "(", 8},        {TOKEN_IDENT, "five", 8},
      {TOKEN_COMMA, ",", 8},         {TOKEN_IDENT, "ten", 8},
      {TOKEN_RPAREN, ")", 8},        {TOKEN_SEMICOLON, ";", 8},
      {TOKEN_BANG, "!", 9},          {TOKEN_MINUS, "-", 9},
      {TOKEN_SLASH, "/", 9},         {TOKEN_ASTERISK, "*", 9},
      {TOKEN_INT, "5", 9},           {TOKEN_SEMICOLON, ";", 9},
      {TOKEN_INT, "5", 10},          {TOKEN_LT, "<", 10},
      {TOKEN_INT, "10", 10},         {TOKEN_GT, ">", 10},
      {TOKEN_INT, "5", 10},          {TOKEN_SEMICOLON, ";", 10},
      {TOKEN_IF, "if", 12},          {TOKEN_LPAREN, "(", 12},
      {TOKEN_INT, "5", 12},          {TOKEN_LT, "<", 12},
      {TOKEN_INT, "10", 12},         {TOKEN_RPAREN, ")", 12},
      {TOKEN_LBRACE, "{", 12},       {TOKEN_RETURN, "return", 13},
      {TOKEN_TRUE, "true", 13},      {TOKEN_SEMICOLON, ";", 13},
      {TOKEN_RBRACE, "}", 14},       {TOKEN_ELSE, "else", 14},
      {TOKEN_LBRACE, "{", 14},       {TOKEN_RETURN, "return", 15},
      {TOKEN_FALSE, "false", 15},    {TOKEN_SEMICOLON, ";", 15},
      {TOKEN_RBRACE, "}", 16},       {TOKEN_INT, "10", 18},
      {TOKEN_EQ, "==", 18},          {TOKEN_INT, "10", 18},
      {TOKEN_SEMICOLON, ";", 18},    {TOKEN_INT, "10", 19},
      {TOKEN_NOT_EQ, "!=", 19},      {TOKEN_INT, "9", 19},
      {TOKEN_SEMICOLON, ";", 19},    {TOKEN_STRING, "foobar", 20},
      {TOKEN_STRING, "foo bar", 21}, {TOKEN_LBRACKET, "[", 22},
      {TOKEN_INT, "1", 22},          {TOKEN_COMMA, ",", 22},
      {TOKEN_INT, "2", 22},          {TOKEN_RBRACKET, "]", 22},
      {TOKEN_SEMICOLON, ";", 22},    {TOKEN_EOF, "", 22},
  };

  init_lexer(input);

  int num_tests = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < num_tests; i++) {
    Token token = next_token();

    assert_int_equal(token.type, tests[i].expected_type);
    assert_int_equal(token.line, tests[i].expected_line);
    assert_string_equal(token.literal, tests[i].expected_literal);
  }
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_next_token),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
